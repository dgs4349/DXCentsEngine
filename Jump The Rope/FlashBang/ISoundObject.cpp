#include "ISoundObject.hpp"

/////////////////////// Parsing ///////////////////////

void ISoundObject::from_json(const json& j, ISoundObject& s) {
	std::string key;
	for (auto i : j.items()) {
		key = i.key();
		s.parseParam_(key, j);
	}
}


ISoundObject::ParamSetterFunc ISoundObject::GetParamSetFunc(EFFECT_PARAMETER p, ISoundObject& s)
{
	switch (p) {
	case EFFECT_PARAMETER::VOLUME: return *s.Volume;
	case EFFECT_PARAMETER::TUNE: return  *s.Tune;
	case EFFECT_PARAMETER::PAN: return *s.Pan;

	default: return nullptr;
	}
}
ISoundObject* ISoundObject::CopyParams(const ISoundObject& s)
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

void ISoundObject::parseParam_(std::string& key, const json& j)
{
	if (islower(key[0]))
	{
		switch (key[0])
		{
		// move to default once int parameters implemented
		case 'l': j[key].get_to(loop_); break;

		case 'f': parseFile_(key, j);  break;
		case 'e': parseEffects_(key, j);break;

		default: 
			auto func = GetParamSetFunc((EFFECT_PARAMETER)key[0], *this);
			if (func != nullptr) func(j[key].get<float>());
			break;
		}
	}
}

void ISoundObject::parseFile_(std::string& fileKey, const json& j)
{
	j[fileKey].get_to(File);
}

void ISoundObject::parseEffects_(std::string& effectsKey, const json& j)
{
	// effect json syntax: "name": { "vol": { float min, float max } }
	for (auto effect : j[effectsKey].items()) {

		std::string EffectName = effect.key();

		auto effectArgs = effect.value().items()[0]; // "vol": { float min, float max }

		ParamSetterFunc func = GetParamSetFunc(effectArgs.key()[0], *this);

		AddEffect(
			EffectName,
			new Effect(
				func, effectArgs.value()[0], effectArgs.value()[1]
			));
	}
}

/////////////////////// Sound Methods ///////////////////////

void ISoundObject::Update(float dt) {
	//enum class SOUND_STATE { UNLOADED, IDLE, PAUSED, QUEUED, READY, PLAYING, FINISHING };

	// IIRC, soundEffect/Instance doesn't always catch playing/stopped/pausing accurately
	//  so it's important to track this ourselves to keep audio as seemless as possible
	//  but this can be tested and verified later during unit testing

	if (state_ < SOUND_STATE::READY) return;

	switch (state_) {

	case SOUND_STATE::READY:

		State(SOUND_STATE::PLAYING);

		Play();

		// fall through

	case SOUND_STATE::PLAYING:

		elapsedTime_ += dt;

		if (elapsedTime_ > duration_) {

			if (currentLoop_ < loop_) {

				++currentLoop_;
			}
			else if (currentLoop_ == loop_) {

				DirectXSoundEffectInstance->Stop(false);
				State(SOUND_STATE::FINISHING);
			}

			// reset elapsed time 0 + overlap
			elapsedTime_ = elapsedTime_ - duration_;
		}
		break;

	case SOUND_STATE::FINISHING:

		elapsedTime_ += dt;

		if (elapsedTime_ > duration_) {

			State(SOUND_STATE::IDLE);
			// auto unload here?

			if (afteree_ != nullptr) {
				afteree_->CopyParams(*this);

				// won't there be a potential jitter if this also has to wait a frame
				afteree_->Play();
			}
		}

	}

}

/////////////////////// Effect Methods ///////////////////////

Effect* ISoundObject::FindEffect(std::string const& key)
{
	auto it = Effects.find(key);
	if (it != Effects.end()) {
		return it->second;
	}
	return nullptr;
}

void ISoundObject::AddEffect(std::string key, Effect* effect)
{
	Effects.insert({ key, effect });
}

void ISoundObject::AddEffects(std::map<std::string, Effect*> const& effects)
{
	for (auto it = effects.begin(); it != effects.end(); ++it) {
		AddEffect(it->first, it->second);
	}
}

bool ISoundObject::DeleteEffect(std::string const& key)
{
	Effect* e = FindEffect(key);
	if (e != nullptr) delete e;
	Effects.erase(key);
}

ISoundObject* ISoundObject::ConnectEffect(std::string const& effectKey, Effect::Connection connection)
{
	auto effect = FindEffect(effectKey);
	if (effect != nullptr) effect->Connect(connection);
	return this;
}

ISoundObject* ISoundObject::ConnectEffects(std::map<std::string, Effect::Connection> const& connections)
{
	for (auto it = connections.begin(); it != connections.end(); it++) {
		auto effect = FindEffect(it->first);
		if (effect != nullptr) effect->Connect(it->second);
	}
	return this;
}
