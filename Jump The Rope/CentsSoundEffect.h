#pragma once

#include <Audio.h>

using namespace DirectX;

/*
	@author Davis Smith
	- Wraps SoundEffect and SoundEffectInstance from DirectXTK Audio
	- Allows chaining soundeffects to play in sequence
	- Adds playback status
*/

class CentsSoundEffect
{
public:
	CentsSoundEffect();
	CentsSoundEffect(AudioEngine* audEngine, const wchar_t* location);
	CentsSoundEffect(AudioEngine* audEngine, const wchar_t* location, bool loop);
	~CentsSoundEffect();

	enum SoundEffectState { Ready = 0, DelayStart = 1, Starting = 2, Playing = 3, Completed = 4 };

	void Update(float deltaTime, float totaltime);

	void Link(CentsSoundEffect* linkee);
	void Link(CentsSoundEffect* linkee, bool loop);

	void Play();
	void Play(float volume, float pitch, float pan);
	void Play(float totalTime);
	void Play(float volume, float pitch, float pan, float totalTime);

	void PlayOnUpdate();
	void PlayOnUpdate(float volume, float pitch, float pan);

	struct RTPC { 
		float** param; float pmin; float pmax; float pval; 
		float* control; float cmin; float cmax; float cval; 
	};
	struct AudioParams { float* volume; float* pitch; float* pan; };
	AudioParams RTPCParameters;

	void Bind(float** param, float* control, float pmin, float pmax, float cmin, float cmax);

	void SetLoop(bool loop);

	void Stop(bool immediate = true);
	void Set(float volume=1.0f, float pitch=0.0f, float pan=0.0f, bool setLinked=true);
	void Set(AudioParams params);

	bool IsReady();
	SoundEffectState GetPlaybackStatus() { return state; }

private:
	CentsSoundEffect * linked = nullptr;
	
	SoundEffectState state = Ready;

	void Start(float totalTime);

	// used in Set to prevent stack overflow
	bool paramsSetting = false;

	bool rtpcUpdate = false;

	// parameters held and updated on 
	bool bound;
	std::vector<RTPC> rtpcs;

	float length;
	bool Loop = false;
	bool SetLoopDelayed = false;

	float startTime;
	float endTime;
	std::unique_ptr<SoundEffect> soundEffect;
	std::unique_ptr<SoundEffectInstance> soundEffectInstance;
};

