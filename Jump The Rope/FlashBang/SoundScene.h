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
	void UnLoad() override;

	int Index(int val) override;

	float GlobVolume(float val) { Volume(val); }
	float GlobTune(float val)   { Tune(val); }
	float GlobPan(float val)	{ Pan(val); }
	int   GlobLoop(int val)		{ Loop(val); }

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

