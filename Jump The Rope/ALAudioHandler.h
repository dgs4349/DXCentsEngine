#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include <stdio.h>
#include <iostream>
#include <vector>


#define BUFFER_SIZE 4096

#define NUM_BUFFERS 3
#define NUM_SOURCES 1
#define NUM_ENVIRONMENTS 1

enum PLATFORM { WINDOWS, MAC, LINUX };
// move to macro
const PLATFORM CURRENT_PLATFORM = WINDOWS;

class ALAudioHandler
{
public:


	ALAudioHandler();
	ALAudioHandler(PLATFORM platform);
	// NOTE may be able to move PLATFORM and constr to macros
	//		ESPECIALLY if we need predefined buffer adjustments

	~ALAudioHandler();
	
	void Init();

	void Update(float deltaTime, float totalTime);

	void Suspend();
	void Resume();


	//!TODO: move from types defined in http://www.edenwaith.com/products/pige/tutorials/openal.php to https://github.com/kcat/openal-soft/wiki/Programmer's-Guide

	//struct AL_SOURCE { ALfloat* POS, VEL; };
	struct AL_LISTENER { ALfloat POS[3], VEL[3], ORI[6]; };
	//
	//ALfloat* VectorZero() {
	//	float arr[3] = { 0.0f, 0.0f, 0.0f }; return arr;
	//};
	//ALfloat* OrientationZero() {1
	//	float arr[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }; return arr;
	//}
	//AL_SOURCE SourceZero() { return { VectorZero(), VectorZero(), OrientationZero()} }

	std::vector<AL_LISTENER> Listeners;
	int ActiveListener = 0;

	void SetListener(int index);
	void SetListener(AL_LISTENER listener);

	// creates sound effect and passes through Add and Manage
	//CentsSoundEffect* CreateSoundEffect(const wchar_t* location, bool loop = false);

	// adds sound effect to update list
	//void Add(CentsSoundEffect* effect);

	// will call destruct on sound effect on delete
	//void Manage(CentsSoundEffect* effect);

private:
	ALCdevice *Device;
	ALCcontext *Context;

	//std::unique_ptr<AudioEngine> audEngine;
	//bool isSilent = false;
	//std::vector<CentsSoundEffect*> effects;
	//std::vector<CentsSoundEffect*> managedEffects;


};

