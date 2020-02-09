#pragma once

#include "FlashBang.hpp"
#include "ISoundObject.hpp"
#include "ISoundContainer.hpp"
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

	virtual void Load() override;
	void Unload() override{ unload_(); }

	virtual ISoundObject* Queue(bool finish = false) override;
	virtual ISoundObject* Queue(ISoundObject* previous, bool finish = false) override;
	virtual ISoundObject* After(bool finish = false) override;
	virtual ISoundObject* After(ISoundObject* next, bool finish = false) override;
	virtual void Play() override;
	virtual void Pause() override;
	virtual void Resume() override;
	virtual void Finish() override;
	virtual void Stop() override;

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


	virtual void updateSound_(float dt) override;
	virtual void updateEffects_(float dt) override;

};
