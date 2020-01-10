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
}

void ::Sound::Pause()
{
}

void ::Sound::Resume()
{
}

void ::Sound::Stop()
{
}

void ::Sound::Load()
{
	soundEffect = std::make_unique<DirectX::OldSoundEffect>(audioEngineDX, location);
	SoundEngine
	soundEffectInstance = soundEffect->CreateInstance();
}

void Sound::setVolume_(float val)
{
}

void Sound::setTune_(float val)
{
}

void Sound::setPan_(float val)
{
}

void Sound::setIndex_(int val)
{
}

void Sound::unload_()
{
}
