#include "SoundContainer.h"

using namespace FlashBang;


SoundContainer::SoundContainer()
{
}


SoundContainer::~SoundContainer()
{
	unload_();
}

void SoundContainer::Play()
{
}

void SoundContainer::Pause()
{
}

void SoundContainer::Resume()
{
}

void SoundContainer::Stop()
{
}

void SoundContainer::Load()
{
}

void SoundContainer::Unload()
{
	unload_();
}

void SoundContainer::setVolume_(float val)
{
}

void SoundContainer::setTune_(float val)
{
}

void SoundContainer::setPan_(float val)
{
}

void SoundContainer::setIndex_(int val)
{
}

void SoundContainer::setLoop_(int val)
{
}

void SoundContainer::unload_()
{
	for (auto el : soundObjects_) delete el;
}

ISoundObject* SoundContainer::createSound_(json const& j)
{
}

ISoundObject* SoundContainer::createSoundContainer_(json const& attr)
{
}

ISoundObject* SoundContainer::createSound_(std::string const& key, json const& j)
{
}

ISoundObject* SoundContainer::createSoundContainer_(std::string const& key, json const& attr)
{
}

int SoundContainer::addSoundObject_(ISoundObject const& soundObject)
{
}

int SoundContainer::addSoundObject_(std::string const& key, ISoundObject const& soundObject)
{
}
