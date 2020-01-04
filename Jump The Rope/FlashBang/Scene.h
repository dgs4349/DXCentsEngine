#pragma once

#include "SoundEngine.h"
using namespace FlashBang;

class Scene
{
	/*
		Container to hold Sounds and SoundContainers to be updated when scene is active

	
	
	*/
public:
	Scene();
	~Scene();

	void SetListenerPosition();
	void SetListenerVelocity();

	void SetSoundPosition();
	void SetSoundVelocity();

	void AddSound();
	void AddContainer();

	void SetReverb();
};

