#pragma once

#include <Audio.h>
#include "CentsEngine.h"

class AudioHandler
{
public:
	AudioHandler();
	~AudioHandler();
	
	void Update();
	void Init();
	void Suspend();
	void Resume();

	void PlayOnce(DirectX::SoundEffect* sfx, float volume);
	void PlayLoop(DirectX::SoundEffect* sfx, float volume);

private:
	std::unique_ptr<DirectX::AudioEngine> audEngine;
	bool isSilent = false;
};
