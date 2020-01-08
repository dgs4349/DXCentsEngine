#pragma once

#include "FlashBang.hpp"
#include "ISoundObject.hpp"

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
	void UnLoad() override;


	float Volume(float val) override;
	float Tune(float val) override;
	float Pan(float val) override;
	int Order(int val) override;
	
protected:
	float _setVolume(float val) override;
	float _setTune(float val) override;
	float _setPan(float val) override;
	int _setOrder(int val) override;

private:
	std::unique_ptr<DirectX::SoundEffect> soundEffect;
	std::unique_ptr<DirectX::SoundEffectInstance> soundEffectInstance;
};

