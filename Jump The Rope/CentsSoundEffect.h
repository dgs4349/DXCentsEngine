#pragma once

#include <Audio.h>

using namespace DirectX;

/*
	@author Davis Smith
	- Wraps SoundEffect and SoundEffectInstance from DirectXTK Audio
	- Allows chaining soundeffects to play in sequence
	- Adds playback status

	- Link: chains this sound effect to another
	- Bind: binds a pointer from a RTPCParam object to a reference of a game variable
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
		float pmin; float pmax; float pval; 
		float* control; float cmin; float cmax; float cval; 
	};
	struct RTPCParams { float* volume; float* pitch; float* pan; };
	
	RTPCParams* CreateRTPCParams();
	void Bind(float*& param, float* control, float pmin, float pmax, float cmin, float cmax);

	void SetLoop(bool loop);

	void Stop(bool immediate = true);
	void Set(float volume=1.0f, float pitch=0.0f, float pan=0.0f, bool setLinked=true);
	void SetRTPCs();

	void Fade(float from, float to, float timeMillis);
	void Fade(float from, float to, float timeMillis, float startTime);

	bool IsReady();
	SoundEffectState GetPlaybackStatus() { return state; }

private:
	CentsSoundEffect * linked = nullptr;
	bool isLinked = false;
	
	SoundEffectState state = Ready;

	void Start(float totalTime);

	// used in Set to prevent stack overflow
	bool paramsSetting = false;
	bool rtpcUpdate = false;

	// parameters held and updated on 
	bool bound = false;
	std::vector<RTPC> rtpcs;
	std::vector<RTPCParams> rtpcParams;

	float length;
	bool Loop = false;
	bool SetLoopDelayed = false;

	float fadeStartTime = -1.0f;
	float fadeTo;
	float fadeFrom;
	float fadeEndTime;

	float map(float in, float inmin, float inmax, float outmin, float outmax);

	float startTime;
	float endTime;
	std::unique_ptr<SoundEffect> soundEffect;
	std::unique_ptr<SoundEffectInstance> soundEffectInstance;
};

