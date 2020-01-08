#pragma once

#include "FlashBang.hpp"
#include "ISoundObject.hpp"
#include "ISoundContainer.hpp"

using namespace FlashBang;

class SoundScene
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

	
};

