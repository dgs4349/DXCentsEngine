#include "Sound.h"

using namespace FlashBang;

::Sound::~Sound()
{
	unload_();
}

ISoundObject& Sound::operator=(const json& j)
{
	// TODO: insert return statement here
}

ISoundObject& Sound::operator=(const std::string& s)
{
	// TODO: insert return statement here
}

void ::Sound::Play()
{
	DirectXSoundEffectInstance->Play();
}

void ::Sound::Pause()
{
	DirectXSoundEffectInstance->Pause();
}

void ::Sound::Resume()
{
	DirectXSoundEffectInstance->Resume();
}

void Sound::Finish()
{
}

void ::Sound::Stop()
{
	DirectXSoundEffectInstance->Stop();
}

void ::Sound::Load()
{
	// should we check for '|' characters?
	
	// should we keep File as a wstring?
	const wchar_t* file_str = std::wstring(File.begin(), File.end()).c_str();
	DirectXSoundEffect = SoundEngine::LoadSoundDX(file_str);

	// note: we can add flags here such as NoPitch or ReverbUseFilters
	DirectXSoundEffectInstance = DirectXSoundEffect->CreateInstance();

	// initiate all params on dx sound effects
	handleVolume_(volume_);
	handleTune_(tune_);
	handlePan_(pan_);
	handleLoop_(loop_);
}

float Sound::handleVolume_(float val)
{
	DirectXSoundEffectInstance->SetVolume(val);
	return val;
}

float Sound::handleTune_(float val)
{
	DirectXSoundEffectInstance->SetPan(val);
}

float Sound::handlePan_(float val)
{
	DirectXSoundEffectInstance->SetPan(val);
}

int Sound::handleLoop_(int val){
	return val;
}

SOUND_STATE Sound::handleState_(SOUND_STATE state)
{
	return state;
}

void Sound::updateSound_(float dt)
{
}

void Sound::updateEffects_(float dt)
{
	for (auto it = Effects.begin(); it != Effects.end(); ++it) it->second->Update(dt);
}

ISoundObject* Sound::Queue(bool finish = false)
{
	if (queuer_ == nullptr) {
		throw std::exception("Cannot implicitly call Queue(): no 'previous' ISoundObject provided to queue this Sound");
	}
	else queuer_->After(this, finish);
	return queuer_;
}

ISoundObject* Sound::Queue(ISoundObject* previous, bool finish = false)
{
	queuer_ = previous;
	queuer_->After(this, finish);
	return queuer_;
}

ISoundObject* Sound::After(bool finish = false)
{
	if (afteree_ == nullptr) {
		throw std::exception("Cannot implicitly call After(): no 'next' ISoundObject provided to call.");
	}
	else if (finish) State(SOUND_STATE::FINISH);
	return afteree_;
}

ISoundObject* Sound::After(ISoundObject* next, bool finish = false)
{
	afteree_ = next;
	return After(finish);
}

void Sound::unload_() 
{
	DirectXSoundEffectInstance.release();
	DirectXSoundEffect.release();
}
