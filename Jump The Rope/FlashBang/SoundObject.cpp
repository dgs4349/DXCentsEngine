#include "SoundObject.hpp"

/////////////////////// Parsing ///////////////////////

void SoundObject::from_json(const json& j, SoundObject& s) {
	std::string key;
	for (auto i : j.items()) {
		key = i.key();
		s.parseParam_(key, j);
	}
}


SoundObject::ParamSetterFunc SoundObject::GetParamSetFunc(EFFECT_PARAMETER p, SoundObject& s)
{
	switch (p) {
	case EFFECT_PARAMETER::VOLUME: return *s.Volume;
	case EFFECT_PARAMETER::TUNE: return  *s.Tune;
	case EFFECT_PARAMETER::PAN: return *s.Pan;

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

void SoundObject::parseFile_(std::string& fileKey, const json& j)
{
	j[fileKey].get_to(File);
}

void SoundObject::parseEffects_(std::string& effectsKey, const json& j)
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
		// if we have a sound effect queued after (queuer), wait a frame to sync up this is to prevent 
		//		a delay longer than a single frame (which might be noticable) when playing a following sound
		// Sound will be set ready, the update loop will automatically set state to playing and therefore
		//		start playing the sound on the synced frame
		State(SOUND_STATE::READY);
	}
}

SoundObject* SoundObject::After(bool finish)
{
	if (afteree_ == nullptr) {
		throw std::exception("Cannot implicitly call After(): no 'previous' ISoundObject provided to queue this Sound");
	}
	else afteree_->Queue(this, finish);
	return afteree_;
}

SoundObject* SoundObject::After(SoundObject* previous, bool finish)
{
	afteree_ = previous;
	afteree_->Queue(this, finish);
	return afteree_;
}

SoundObject* SoundObject::Queue(bool finish)
{
	if (queued_ == nullptr) {
		throw std::exception("Cannot implicitly call Queue(): no 'next' ISoundObject provided to call.");
	}
	else if (finish) State(SOUND_STATE::FINISHING);
	return queued_;
}

SoundObject* SoundObject::Queue(SoundObject* next, bool finish)
{
	queued_ = next;
	return Queue(finish);
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

void SoundObject::AddEffect(std::string key, Effect* effect)
{
	Effects.insert({ key, effect });
}

void SoundObject::AddEffects(std::map<std::string, Effect*> const& effects)
{
	for (auto it = effects.begin(); it != effects.end(); ++it) {
		AddEffect(it->first, it->second);
	}
}

bool SoundObject::DeleteEffect(std::string const& key)
{
	Effect* e = FindEffect(key);
	if (e != nullptr) delete e;
	Effects.erase(key);
}

SoundObject* SoundObject::ConnectEffect(std::string const& effectKey, Effect::Connection connection)
{
	auto effect = FindEffect(effectKey);
	if (effect != nullptr) effect->Connect(connection);
	return this;
}

SoundObject* SoundObject::ConnectEffects(std::map<std::string, Effect::Connection> const& connections)
{
	for (auto it = connections.begin(); it != connections.end(); it++) {
		auto effect = FindEffect(it->first);
		if (effect != nullptr) effect->Connect(it->second);
	}
	return this;
}
