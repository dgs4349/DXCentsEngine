#include "SoundObject.hpp"

/////////////////////// Parsing ///////////////////////

void SoundObject::from_json(const json& j, SoundObject& s) {
	if (j.size() == 1 && j[0].is_object()) {
		s.Key = j.begin().key();
		from_json(j[0], s);
	}

	for (auto i : j.items()) {
		s.parseParam_(i.key(), j);
	}
}

/*
	returns pointer to var based on SOUND_PARAM chars
*/
SoundObject::ParamSetterFunc SoundObject::GetParamSetFunc(SOUND_PARAM p, SoundObject& s)
{
	switch (p) {
	case SOUND_PARAM::VOLUME: return *s.Volume;
	case SOUND_PARAM::TUNE: return  *s.Tune;
	case SOUND_PARAM::PAN: return *s.Pan;

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

void SoundObject::parseParam_(const std::string& key, const json& j)
{
	switch ((SOUNDOBJECT_ARG)key[0])
	{
	case SOUNDOBJECT_ARG::FILE: j[key].get_to(File);  break;
	case SOUNDOBJECT_ARG::KEY:	j[key].get_to(Key); break;

	case SOUNDOBJECT_ARG::EFFECTS: parseEffects_(key, j); break;

		// move to default once int parameters implemented
	case (SOUNDOBJECT_ARG)SOUND_PARAM::LOOP: j[key].get_to(loop_); break;

	default:
		auto func = GetParamSetFunc((SOUND_PARAM)key[0], *this);
		if (func != nullptr) func(j[key].get<float>());
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

	// same deal as sound
	json currentEffect;

	for (int i = 0; i < j[key].size(); ++i) {
		try {
			currentEffect = j[key][i];

			// array effect
			if (currentEffect.is_array()) {
				Effect* effect = new Effect(
					// currentEffect[1] = "volume", currentEffect[1][0] = 'v' = SOUND_PARAM::VOLUME
					GetParamSetFunc((SOUND_PARAM)(currentEffect[1])[0], *this),
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

			Effect* effect = new Effect();

			for (auto [key, value] : currentEffect.items()) {
				switch ((EFFECT_ARG)key[0]) {
				case EFFECT_ARG::KEY:
					value.get_to(effectName);
					break;
				case EFFECT_ARG::PARAM:
					SOUND_PARAM param;
					value.get_to(param);
					effect->ValueFunctionPointer = GetParamSetFunc(param, *this);
					break;
				case EFFECT_ARG::MIN:
					value.get_to(effect->ValueMin);
					break;
				case EFFECT_ARG::MAX:
					value.get_to(effect->ValueMax);
					break;
				}
			}

			AddEffect(key, effect);
		}
		catch (std::exception effect) {
			printf(effect.what());
			throwEffectError_(i, key, j);
		}
	}
}

void SoundObject::throwEffectError_(int i, const std::string& key, const json& j) {
	const char* message = R"(
		Error parsing effect. Please double check effect schema:
			- Array: [ key "jump_vol", param "vol", float min, float max ]
			- Object: { key: "keyName", param: "vol", low: floatMin, high: floatMax }
			- Object-Alt: "key" : { param: "vol", low: floatMin, high: floatMax }
	)";

	const auto currentEffect = j[key][i];
	std::string effectIssue = std::string("\nEffect index: ") + std::to_string(i) + "\n";

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
	for (auto it = Effects.begin(); it != Effects.end(); ++it) {
		it->second->Update(dt);
	}
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

bool SoundObject::DeleteEffect(std::string const& key)
{
	Effect* effect = FindEffect(key);
	if (effect != nullptr) delete effect;
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
