#include "Sound.h"

using namespace FlashBang;

::Sound::Sound()
{
}

::Sound::~Sound()
{
	unload_();
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

void ::Sound::Stop()
{
	DirectXSoundEffectInstance->Stop();
}

void ::Sound::Load()
{
	// should we keep File as a wstring?
	const wchar_t* file_str = std::wstring(File.begin(), File.end()).c_str();
	DirectXSoundEffect = SoundEngine::LoadSoundDX(file_str);

	// note: we can add flags here such as NoPitch or ReverbUseFilters
	DirectXSoundEffectInstance = DirectXSoundEffect->CreateInstance();

	// initiate all params on dx sound effects
	setVolume_(volume_);
	setTune_(tune_);
	setPan_(pan_);
	setLoop_(loop_);
}

void Sound::setVolume_(float val)
{
	DirectXSoundEffectInstance->SetVolume(val);
}

void Sound::setTune_(float val)
{
	DirectXSoundEffectInstance->SetPan(val);
}

void Sound::setPan_(float val)
{
	DirectXSoundEffectInstance->SetPan(val);
}

void Sound::setLoop_(int val){}

void Sound::setIndex_(int val){}

void Sound::unload_()
{
	DirectXSoundEffectInstance.release();
	DirectXSoundEffect.release();
}
