#pragma once

#include "FlashBang.hpp"
#include "ISoundObject.hpp"
#include "SoundEngine.h"

#include <Audio.h>

using namespace FlashBang;

class Sound : ISoundObject
{
public:
	Sound();
	~Sound();

	void Play() override;
	void Pause() override;
	void Resume() override;
	void Stop() override;
	void Load() override;
	void Unload() override{ unload_(); }
	
protected:
	void setVolume_(float val) override;
	void setTune_(float val) override;
	void setPan_(float val) override;
	void setIndex_(int val) override;

private:
	std::unique_ptr<DirectX::SoundEffect> soundEffect;
	std::unique_ptr<DirectX::SoundEffectInstance> soundEffectInstance;

	void unload_();
};

