#pragma once

#include "FlashBang.hpp"
#include "ISoundObject.hpp"
#include "SoundEngine.h"

#include <Audio.h>

using namespace FlashBang;

class Sound : ISoundObject
{
public:
	Sound() = delete;
	~Sound();
	
	ISoundObject& operator=(const json& j) override;
	ISoundObject& operator=(const std::string& s) override;
	void Load() override;
	void Queue() override;
	void Play() override;
	void Pause() override;
	void Resume() override;
	void Finish() override;
	void Stop() override;
	
	void Unload() override{ unload_(); }


	std::unique_ptr<DirectX::SoundEffect> DirectXSoundEffect;
	std::unique_ptr<DirectX::SoundEffectInstance> DirectXSoundEffectInstance;
	
private:

	void unload_();

protected:
	float handleVolume_(float val) override;
	float handleTune_(float val) override;
	float handlePan_(float val) override;
	int handleLoop_(int val) override;
	SOUND_STATE handleState_(SOUND_STATE state) override;
};
