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
class Sound
{
public:
	Sound();
	Sound(AudioEngine* audioEngineDX, const wchar_t* location);
	Sound(AudioEngine* audioEngineDX, const wchar_t* location, bool loop);
	~Sound();

	enum PARAMS{VOLUME, PITCH, PAN, PLAY};

	// todo: fix
	enum SoundEffectState { Ready = 0, DelayStart = 1, Starting = 2, Playing = 3, Completed = 4 };

	struct Connection { float* gameVar; float varMin; float varMax; };

	// TODO deprecate RTPC, RTPCParams, Bind=>Connect/Effect
	struct RTPC {
		float pmin; float pmax; float pval;
		float* control; float cmin; float cmax; float cval;
	};
	struct RTPCParams { float* volume; float* pitch; float* pan; };


	void Update(float deltaTime, float totaltime);

	void Link(Sound* linkee);
	void Link(Sound* linkee, bool loop);

	void Play();
	void Play(float volume, float pitch, float pan);
	void Play(float totalTime);
	void Play(float volume, float pitch, float pan, float totalTime);

	void PlayOnUpdate();
	void PlayOnUpdate(float volume, float pitch, float pan);

	RTPCParams* CreateRTPCParams();
	void Bind(float*& param, float* control, float pmin, float pmax, float cmin, float cmax);

	void SetLoop(bool loop);

	void Volume(float vol = 1.0f);
	void Pitch(float pitch = 0.0f);
	void Pan(float pan = 0.0f);

	Sound* Effect(PARAMS param, float min, float max, Connection connect);

	Connection Connect(float* gameVar, float varMin, float varMax);

	void Stop(bool immediate = true);
	void Set(float volume=1.0f, float pitch=0.0f, float pan=0.0f, bool setLinked=true);
	void SetRTPCs();

	// TODO replace with Effect Class to hold various Effects, including Effect.Fade for instance
	void Fade(float from, float to, float timeMillis);
	void Fade(float from, float to, float timeMillis, float startTime);

	bool IsReady();
	SoundEffectState GetPlaybackStatus() { return state; }

private:
	Sound * linked = nullptr;
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

