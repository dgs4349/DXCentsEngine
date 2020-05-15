#pragma once

#include <Audio.h>

using namespace DirectX;

/*
	@author Davis Smith
	- Wraps OldSoundEffect and OldSoundEffectInstance from DirectXTK Audio
	- Allows chaining soundeffects to play in sequence
	- Adds playback status

	- Link: chains this sound effect to another
	- Bind: binds a pointer from a RTPCParam object to a reference of a game variable
*/

// speed?
enum SOUNDPARAMS { VOLUME, PITCH, PAN, ORDER };

class OldSound
{
public:
	OldSound();
	OldSound(AudioEngine* audioEngineDX, const wchar_t* location);
	OldSound(AudioEngine* audioEngineDX, const wchar_t* location, bool loop);
	~OldSound();

	enum PARAMS { VOLUME, PITCH, PAN, PLAY };

	// todo: fix
	enum OldSoundEffectState { Stopped = 0, Ready = 1, Playing = 2, Completed = 3 };

	/*
		Connect(&ref, min max)

		To add:
		Time(start, end) //total time in audio handler to effect
		Lifetime(start, end) // lifetime of audio as it's playing

	*/
	struct Connection { float* gameVar; float varMin; float varMax; float prevValue; };

	// TODO deprecate RTPC, RTPCParams, Bind=>Connect/Effect
	//struct RTPC {
	//	float pmin; float pmax; float pval;
	//	float* control; float cmin; float cmax; float cval;
	//};
	//struct RTPCParams { float* volume; float* pitch; float* pan; };

	void Update(float deltaTime, float totaltime);

	void Next(OldSound* linkee);
	void Next(OldSound* linkee, bool loop);

	void Play();
	void Play(float volume, float pitch, float pan);
	void Play(float totalTime);
	void Play(float volume, float pitch, float pan, float totalTime);

	void Ready();

	//RTPCParams* CreateRTPCParams();
	//void Bind(float*& param, float* control, float pmin, float pmax, float cmin, float cmax);

	void SetLoop(bool loop);

	void Volume(float vol = 1.f);
	void Pitch(float pitch = 0.f);
	void Pan(float pan = 0.f);

	OldSound* Effect(PARAMS param, float min, float max, Connection connect);

	Connection Connect(float* gameVar, float varMin, float varMax);

	void Stop(bool immediate = true);
	void Set(float volume = 1.f, float pitch = 0.f, float pan = 0.f, bool setLinked = true);
	void SetRTPCs();

	// TODO replace with Effect Class to hold various Effects, including Effect.Fade for instance
	void Fade(float from, float to, float timeMillis);
	void Fade(float from, float to, float timeMillis, float startTime);

	bool IsReady();
	OldSoundEffectState GetPlaybackStatus() { return state; }

private:
	OldSound* linked = nullptr;
	bool isLinked = false;

	OldSoundEffectState state = Ready;

	void Start(float totalTime);

	// used in Set to prevent stack overflow
	bool paramsSetting = false;
	bool rtpcUpdate = false;

	// parameters held and updated on
	bool bound = false;
	std::vector<RTPC> rtpcs;
	std::vector<RTPCParams> rtpcParams;

	std::vector<Connection> connections;

	float length;
	bool Loop = false;
	bool SetLoopDelayed = false;

	float fadeStartTime = -1.f;
	float fadeTo;
	float fadeFrom;
	float fadeEndTime;

	float map(float in, float inmin, float inmax, float outmin, float outmax);

	float startTime;
	float endTime;
	std::unique_ptr<SoundEffect> soundEffect;
	std::unique_ptr<SoundEffectInstance> soundEffectInstance;
};
