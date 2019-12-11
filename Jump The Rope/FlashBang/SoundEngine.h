#pragma once

#include <Audio.h>
#include "CentsEngine.h"
#include "Sound.h"
#include "Scene.h"
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
	static SoundEngine* Get();
	static void Release();
	
	void Update(float deltaTime, float totalTime);
	void Init();

	void Suspend();
	void Resume();

	// creates sound effect and passes through Add and Manage
	Sound* CreateSound(const wchar_t* location, bool loop=false);

	DirectX::AudioEngine* GetAudioEngine() { return audioEngineDX.get(); }

	/*
		or do we want Scenes to be set active or inactive in Scene itself?
	*/
	void AddScene(Scene scene);

	void SetActiveScene();
	void AddActiveScene();

	// adds sound effect to update list
	void Add(Sound* effect);

	// will call destruct on sound effect on delete
	void Manage(Sound* effect);

private:
	SoundEngine();
	~SoundEngine();
	
	static SoundEngine* instance;
	void operator=(SoundEngine const&);

	static int refs;
	
	static void addRef() { ++refs; }
	static void releaseRef() { --refs;}

	std::unique_ptr<AudioEngine> audioEngineDX;
	bool isSilent = false;
	std::vector<Sound*> effects;
	std::vector<Sound*> managedEffects;

public:
	SoundEngine(SoundEngine const&) = delete;
	void operator=(SoundEngine const*) = delete;
};
