#pragma once

#include <Audio.h>
#include "CentsEngine.h"
#include "Sound.h"
#include <vector>

using namespace DirectX;

/*
	SoundEngine
	@author Davis Smith

	- Handles DirectXTK Audio creation and management
	- Creates and manages Sounds
*/

class SoundEngine
{
public:
	SoundEngine();
	~SoundEngine();
	
	void Update(float deltaTime, float totalTime);
	void Init();

	void Suspend();
	void Resume();

	// creates sound effect and passes through Add and Manage
	Sound* CreateSound(const wchar_t* location, bool loop=false);

	DirectX::AudioEngine* GetAudioEngine() { return audioEngineDX.get(); }

	// adds sound effect to update list
	void Add(Sound* effect);

	// will call destruct on sound effect on delete
	void Manage(Sound* effect);

private:
	std::unique_ptr<AudioEngine> audioEngineDX;
	bool isSilent = false;
	std::vector<Sound*> effects;
	std::vector<Sound*> managedEffects;
};
