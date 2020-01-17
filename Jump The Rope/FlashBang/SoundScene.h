#pragma once

#include "FlashBang.hpp"
#include "ISoundObject.hpp"
#include "ISoundContainer.hpp"

using namespace FlashBang;

class SoundScene : ISoundContainer
{
	/*
		Container to hold Sounds and SoundContainers to be updated when scene is active

	
	
	*/
public:
	SoundScene();
	~SoundScene();

	void SetListenerPosition();
	void SetListenerVelocity();

	void SetSoundPosition();
	void SetSoundVelocity();

	void AddSound();
	void AddContainer();

	void SetReverb();

	void Load() override;
	void Unload() override;

	int Index(int val) override;

	float GlobalVolume	(float val) { return Volume(val); }
	float GlobalTune	(float val) { return Tune(val); }
	float GlobalPan		(float val) { return Pan(val); }
	int   GlobalLoop	(int val)	{ return Loop(val); }

	void Play() override;
	void Pause() override;
	void Resume() override;
	void Stop() override;
	
private:
	float Volume(float val) override;
	float Tune(float val) override;
	float Pan(float val) override;
	int   Loop(int val) override;
	
protected:
	void setVolume_(float val) override;
	void setTune_(float val) override;
	void setPan_(float val) override;
	void setIndex_(int val) override;
	void setLoop_(int val) override;
};

