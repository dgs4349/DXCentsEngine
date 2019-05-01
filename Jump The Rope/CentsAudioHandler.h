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
	CentsSoundEffect* CreateSoundEffect(const wchar_t* location, bool loop=false);

	DirectX::AudioEngine* GetAudioEngine() { return audEngine.get(); }

	// adds sound effect to update list
	void Add(CentsSoundEffect* effect);

	struct Fade { 
		int effectIndex; 
		int fadeIndex; 
		float fromVolume; 
		float toVolume; 
		float timeMillis; 
		float startTime;
		bool active = false;
	};
	Fade CreateFade(CentsSoundEffect* effect, float fromVolume, float toVolume, float timeMillis);
	Fade CreateStartFade(CentsSoundEffect* effect, float fromVoume, float toVolume, float timeMillis);
	Fade StartFade(Fade fade);
	Fade ReplaceFade(Fade oldFade, Fade newFade);

	// will call destruct on sound effect on delete
	void Manage(CentsSoundEffect* effect);

private:
	std::unique_ptr<AudioEngine> audEngine;
	bool isSilent = false;
	std::vector<CentsSoundEffect*> effects;
	std::vector<CentsSoundEffect*> managedEffects;
	std::vector<Fade> fades;
};
