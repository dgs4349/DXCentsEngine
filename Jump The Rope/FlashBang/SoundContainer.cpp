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

	auto sound = new Sound();

	/*
	 * okay we need to figure out files now
	 *	- createSound_() with tailing json
	 *	- createSound_() without, where files are created during load
	 *	- createSound_() wih just file such as during a list
	 *		- or maybe do we need to add some other kind of
	 *			collection/specifier for when we do and do not
	 *			collect files and whatnot
	 *	- no, files will be parsed here and passed in one way or another
	 *	- some will have it in json, but others we will have to figure out
	 * 
	 */
	
}
