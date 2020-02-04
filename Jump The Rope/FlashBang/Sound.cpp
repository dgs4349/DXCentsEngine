#include "Sound.h"

using namespace FlashBang;

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

void Sound::handleVolume_(float val)
{
	DirectXSoundEffectInstance->SetVolume(val);
}

void Sound::handleTune_(float val)
{
	DirectXSoundEffectInstance->SetPan(val);
}

void Sound::handlePan_(float val)
{
	DirectXSoundEffectInstance->SetPan(val);
}

void Sound::handleLoop_(int val){}

void Sound::handleIndex_(int val){}

void Sound::unload_() 
{
	DirectXSoundEffectInstance.release();
	DirectXSoundEffect.release();
}
