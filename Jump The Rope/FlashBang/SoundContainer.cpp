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
	for(auto& el : this->items())
	{
		if (!isalpha(el.key()[0]))
		{
			// we might need to check the docs for adding a conversion
			Sound* s = *el.value();
		}
	}
}

void SoundContainer::createSound_(std::string const& key)
{
	
}
