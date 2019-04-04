#pragma once

#include <Audio.h>
#include "CentsEngine.h"
#include "CentsSoundEffect.h"
#include <vector>

using namespace DirectX;

/*
	CentsAudioHandler
	@author Davis Smith

	- Handles DirectXTK Audio creation and management
	- Creates and manages CentsSoundEffects
*/

class CentsAudioHandler
{
public:
	CentsAudioHandler();
	~CentsAudioHandler();
	
	void Update(float deltaTime, float totalTime);
	void Init();

	void Suspend();
	void Resume();

	// creates sound effect and passes through Add and Manage
	CentsSoundEffect* CreateSoundEffect(wchar_t location);

	DirectX::AudioEngine* GetAudioEngine() { return audEngine.get(); }

	// adds sound effect to update list
	void Add(CentsSoundEffect* effect);

	// will call destruct on sound effect on delete
	void Manage(CentsSoundEffect* effect);

private:
	std::unique_ptr<AudioEngine> audEngine;
	bool isSilent = false;
	std::vector<CentsSoundEffect*> effects;
	std::vector<CentsSoundEffect*> managedEffects;
};
