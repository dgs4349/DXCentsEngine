#pragma once

#include <Audio.h>
#include "CentsEngine.h"
#include "CentsSoundEffect.h"
#include <vector>

using namespace DirectX;
class CentsAudioHandler
{
public:
	CentsAudioHandler();
	~CentsAudioHandler();
	
	void Update(float deltaTime, float totalTime);
	void Init();
	void Suspend();
	void Resume();

	DirectX::AudioEngine* GetAudioEngine() { return audEngine.get(); }

	void Add(CentsSoundEffect* effect);

private:
	std::unique_ptr<AudioEngine> audEngine;
	bool isSilent = false;
	std::vector<CentsSoundEffect*> effects;
};
