#include "SoundObject.hpp"
#include <boost/stacktrace.hpp>
#include <iostream>
#include <string>

/////////////////////// Parsing ///////////////////////

void SoundObject::from_json(json const& j, SoundObject& s) {
	
	std::cout << "test cout SO" << std::endl;
	printf("test printf SO");

	auto [key, value] = j.items().begin();

	if (j.size() == 1 && value.is_object()) {
		s.Key = key;
		from_json(value, s);
	}

	for (auto [k, v] : j.items()) {
		try {
			// move to non const for casting, we can probs do better
			std::string str = k;
			s.parseParam_(str, j);
		}
		catch (std::exception e) { handleParseError_(s.Key, k, e); };
	}

}


void SoundObject::handleParseError_(
	std::string const& soundObjectKey, 
	std::string const& itemKey, 
	std::exception const& e)
{

	std::cout << boost::stacktrace::stacktrace();
	std::cout << e.what();
	if (!soundObjectKey.empty()) {
		std::cout << "Error parsing SoundObject: " << soundObjectKey << std::endl;
	}
	else {
		std::cout << "Error parsing (unkown) SoundObject." << std::endl;
	}
	std::cout << "Parse errored on itemKey: " << itemKey << std::endl;
}

/*
	returns pointer to var based on SOUND_PARAM chars
*/
ParameterCallablePtr SoundObject::GetParameterCallable(SOUND_PARAM p, SoundObject& s)
{
	switch (p) {
	case SOUND_PARAM::VOLUME: return s.VolumeCall;
	case SOUND_PARAM::TUNE: return  s.TuneCall;
	case SOUND_PARAM::PAN: return s.PanCall;

	default: return nullptr;
	}
}

SoundObject* SoundObject::CopyParams(const SoundObject& s)
{
	if (this != &s)
	{
		volume_ = s.volume_;
		tune_ = s.tune_;
		pan_ = s.pan_;
		loop_ = s.loop_;
	}
	return this;
}

void SoundObject::parseParam_(std::string& key, const json& j)
{
	switch (static_cast<SOUNDOBJECT_ARG>(key[0]))
	{
		case SOUNDOBJECT_ARG::FILE: j[key].get_to<std::string>(File);  break;
		case SOUNDOBJECT_ARG::KEY:	j[key].get_to<std::string>(Key); break;

		case SOUNDOBJECT_ARG::EFFECTS: parseEffects_(j[key]); break;

			// move to default once int parameters implemented
		case (SOUNDOBJECT_ARG)SOUND_PARAM::LOOP: j[key].get_to<int>(loop_); break;

		default:
			ParameterCallablePtr funcPtr = GetParameterCallable((SOUND_PARAM)key[0], *this);
			if (funcPtr != nullptr)
			{
				float value;
				j[key].get_to<float>(value);
				(*funcPtr)(value);
			}
			break;
	}
}

void SoundObject::parseEffects_(const json& effectsJsonObj) {
	/*
		PARAMETER key MUST BE "param"

		"effectsJsonObj":
		{
			"effect1Key":
			{
				"param": "vol",
				"min": 0.f,
				"max": 1.f
			}
			"effect2Key:"
			{
				...
			}
			...
		}
	*/

	for (auto [effectKey, effectObj] : effectsJsonObj.items()) {


		try
		{
			auto* effect = new Effect();
			// "param": "vol" => obj["param"][0] = 'v'
			std::string param;
			effectObj["param"].get_to<std::string>(param);
			effect->ParameterCallPtr = GetParameterCallable(static_cast<SOUND_PARAM>(param[0]), *this);
			effectObj["min"].get_to<float>(effect->ParameterMin);
			effectObj["max"].get_to<float>(effect->ParameterMax);
			AddEffect(effectKey, effect);
		}
		catch (std::exception e)
		{
			const char* message = R"(
				Error parsing effect. Please correct syntax.
				Correct Syntax:
				
				"effects": 
				{
					"effect1Key": { "param": "vol", "min": 0.0f, "max": 1.0f },
					"effect2Key" : { ... }
				}	

				Parameter Key must be "param", Min and Max value keys must be "min" and "max"
			)";

			printf(e.what() + '\n');

			throw std::exception(message + '\n');
		}
	}
}

/////////////////////// Sound Methods ///////////////////////

void SoundObject::Play()
{
	updateEffects_(0.f); // do a quick rtpc update before play logic

	// if state was set to playing, or there is no following sound, play immediately
	if (state_ == SOUND_STATE::PLAYING || queued_ == nullptr) {
		handlePlay_();
		currentLoop_ = 0;
		elapsedTime_ = 0;
		State(SOUND_STATE::PLAYING);
	}
	else {
		// if we have a sound currentEffect queued after (queuer), wait a frame to sync up this is to prevent
		//		a delay longer than a single frame (which might be noticable) when playing a following sound
		// Sound will be set ready, the update loop will automatically set state to playing and therefore
		//		start playing the sound on the synced frame
		State(SOUND_STATE::READY);
	}
}

SoundObject* SoundObject::Queue(SoundObject* next, bool finish)
{
	queued_ = next;
	if (finish) Finish();
	return queued_;
}

void SoundObject::updateEffects_(float dt)
{
	for (auto [key, effect] : Effects) effect->Update(dt);
}

void SoundObject::Update(float dt) {
	//enum class SOUND_STATE { UNLOADED, IDLE, PAUSED, QUEUED, READY, PLAYING, FINISHING };

	// IIRC, soundEffect/Instance doesn't always catch playing/stopped/pausing accurately
	//  so it's important to track this ourselves to keep audio as seemless as possible
	//  but this can be tested and verified later during unit testing

	if (state_ < SOUND_STATE::READY) return;

	updateEffects_(dt);

	switch (state_) {
		case SOUND_STATE::READY:
			State(SOUND_STATE::PLAYING);
			Play();
			// fall through

		case SOUND_STATE::PLAYING:

			elapsedTime_ += dt;

			if (elapsedTime_ > duration_) {
				if (currentLoop_ < loop_) ++currentLoop_;
				else if (currentLoop_ == loop_) Finish();

				// reset elapsed time 0 + overlap
				elapsedTime_ = elapsedTime_ - duration_;
			}

			break;

		case SOUND_STATE::FINISHING:

			elapsedTime_ += dt;

			if (elapsedTime_ > duration_) {
				// one from of COMPLETE state for cleanup logic
				State(SOUND_STATE::COMPLETE);

				if (queued_ != nullptr) {
					queued_->CopyParams(*this);

					// won't there be a potential jitter if this also has to wait a frame
					queued_->Play();
				}
			}
			break;

			// if complete on previous frame reset to IDLE
		case SOUND_STATE::COMPLETE:
			State(SOUND_STATE::IDLE);
			break;
		default: break;
	}
}

/////////////////////// Effect Methods ///////////////////////

Effect* SoundObject::FindEffect(std::string const& key)
{
	auto it = Effects.find(key);
	if (it != Effects.end()) {
		return it->second;
	}
	return nullptr;
}

void SoundObject::AddEffect(std::string key, Effect* currentEffect)
{
	Effects.insert({ key, currentEffect });
}

void SoundObject::AddEffects(std::map<std::string, Effect*> const& effects)
{
	for (auto it = effects.begin(); it != effects.end(); ++it) {
		AddEffect(it->first, it->second);
	}
}

void SoundObject::DeleteEffect(std::string const& key)
{
	Effect* effect = FindEffect(key);
	delete effect;
	Effects.erase(key);
}

SoundObject* SoundObject::ConnectEffect(std::string const& effectKey, Effect::Connection connection)
{
	auto currentEffect = FindEffect(effectKey);
	if (currentEffect != nullptr) currentEffect->Connect(connection);
	return this;
}

SoundObject* SoundObject::ConnectEffects(std::map<std::string, Effect::Connection> const& connections)
{
	for (auto it = connections.begin(); it != connections.end(); it++) {
		auto currentEffect = FindEffect(it->first);
		if (currentEffect != nullptr) currentEffect->Connect(it->second);
	}
	return this;
}
