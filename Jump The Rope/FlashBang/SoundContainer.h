#pragma once

#include "FlashBang.hpp"
#include "ISoundContainer.hpp"
#include "Sound.h"

using namespace FlashBang;


class SoundContainer : ISoundContainer
{
public:
	SoundContainer();
	~SoundContainer();
	
	void Play() override;
	void Pause() override;
	void Resume() override;
	void Stop() override;
	void Load() override;
	void Unload() override;
	
protected:
	void setVolume_(float val) override;
	void setTune_(float val) override;
	void setPan_(float val) override;
	void setIndex_(int val) override;
	void setLoop_(int val) override;

	void unload_();
	
	void createSound_(std::string const& key) override;
};

