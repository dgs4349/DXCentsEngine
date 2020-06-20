#include "SoundObject.hpp"

/////////////////////// Parsing ///////////////////////

void SoundObject::from_json(json const& j, SoundObject& s) {

	auto [key, value] = j.items().begin();

	if (j.size() == 1 && value.is_object()) {
		s.Key = key;
		from_json(value, s);
	}

	for (auto [k, v] : j.items()) {
		// move to non const for casting, we can probs do better
		std::string str = k;
		s.parseParam_(str, j);
	}
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
	case SOUNDOBJECT_ARG::FILE: j[key].get_to(File);  break;
	case SOUNDOBJECT_ARG::KEY:	j[key].get_to(Key); break;

	case SOUNDOBJECT_ARG::EFFECTS: parseEffects_(key, j); break;

		// move to default once int parameters implemented
	case (SOUNDOBJECT_ARG)SOUND_PARAM::LOOP: j[key].get_to(loop_); break;

	default:
		ParameterCallablePtr funcPtr = GetParameterCallable((SOUND_PARAM)key[0], *this);
		if (funcPtr != nullptr)
		{
			float value;
			j[key].get_to(value);
			(*funcPtr)(value);
		}
		break;
	}
}

void SoundObject::parseEffects_(const std::string& key, const json& j)
{
	// currentEffect array syntax: [ key "jump_vol", param "vol", float min, float max ]
	/*
		"name" : { param: "vol", low: min, high: max }
		or { key: "name", param ... }

	*/


	////////// Start here, what is going on

	// same deal as sound
	json currentEffect = json(NULL);

	// range based loop to keep reference to index used in exception handling
	for (unsigned int i = 0; i < j[key].size(); ++i) {
		try {
			currentEffect = j[key][i];

			// array effect
			if (currentEffect.is_array()) {
				auto* effect = new Effect(
					// currentEffect[1] = "volume", currentEffect[1][0] = 'v' = SOUND_PARAM::VOLUME
					GetParameterCallable((SOUND_PARAM)(currentEffect[1])[0], *this),
					currentEffect[2],
					currentEffect[3]
				);
				AddEffect(currentEffect[0], effect);
				continue;
			}

			//object effect
			std::string effectName;

			// name-first object, store name then continue to body
			if (currentEffect.size() == 1) {
				effectName = currentEffect.begin().key();
				currentEffect = currentEffect[effectName];
			}

			auto * effect = new Effect();

			for (auto [key, value] : currentEffect.items()) {
				switch (static_cast<EFFECT_ARG>(key[0])) {
				case EFFECT_ARG::KEY:
					value.get_to(effectName);
					break;
				case EFFECT_ARG::PARAM:
					SOUND_PARAM param;
					value.get_to(param);
					effect->ParameterCallPtr = GetParameterCallable(param, *this);
					break;
				case EFFECT_ARG::MIN:
					value.get_to(effect->ParameterMin);
					break;
				case EFFECT_ARG::MAX:
					value.get_to(effect->ParameterMax);
					break;
				}
			}

			AddEffect(key, effect);
		}
		catch (std::exception& effect) {
			printf(effect.what());
			throwEffectError_(i, key, j);
		}
	}
}

void SoundObject::throwEffectError_(int i, const std::string& key, const json& j) const {
	const char* message = R"(
		Error parsing effect. Please double check effect schema:
			- Array: [ key "jump_vol", param "vol", float min, float max ]
			- Object: { key: "keyName", param: "vol", low: floatMin, high: floatMax }
			- Object-Alt: "key" : { param: "vol", low: floatMin, high: floatMax }
	)";

	const auto currentEffect = j[key][i];
	auto effectIssue = std::string("\nEffect index: ") + std::to_string(i) + "\n";

	if (currentEffect.size() < 4) {
		effectIssue += "Effect is not the right length!"
			+ std::string("\nEffect length: ") + std::to_string(currentEffect.size());
	}
	// other issues involve complex currentEffect parsing which i don't feel like doing right now

	throw std::exception((message + effectIssue).c_str());
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
