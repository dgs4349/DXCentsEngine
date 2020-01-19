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
	for (int i : managedSoundObjects_) delete sounds_[i];
}

void SoundContainer::createSound_(std::string const& key)
{
	// key will be some non-numerical character followed by the key
	//	we want a key both with and without that key
	int count = sounds_.size();
	Sound* sound = new Sound();
	
	Sound* sound = new Sound();
	auto cont = new SoundContainer();
	json test;
	(*cont)[key] = 5;
	test[key] = 5;

	auto result = cont->find(key);
	cont->at(result);
}
