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

	struct Fade { int index; float fromVolume; float toVolume; float timeMillis; float startTime; };
	Fade CreateFade(CentsSoundEffect* effect, float fromVolume, float toVolume, float timeMillis);
	Fade AddCreateFade(CentsSoundEffect* effect, float fromVoume, float toVolume, float timeMillis);
	void AddFade(Fade fade);
	Fade SetFade(Fade oldFade, Fade newFade);

	// will call destruct on sound effect on delete
	void Manage(CentsSoundEffect* effect);

private:
	std::unique_ptr<AudioEngine> audEngine;
	bool isSilent = false;
	std::vector<CentsSoundEffect*> effects;
	std::vector<CentsSoundEffect*> managedEffects;
	std::vector<Fade> fades;
};
