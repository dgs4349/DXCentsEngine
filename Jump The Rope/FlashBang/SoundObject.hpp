#pragma once

#include "FlashBang.hpp"
#include "Effect.hpp"

#include <nlohmann/json.hpp>

using nlohmann::json;

using namespace FlashBang;

/*
	Handles parsing
	Handles basic update logic

	Creates basic behavior for derrived classes
	is NOT in charge of playback, that is for derrived classes
*/
class SoundObject
{
public:

	/////////////////////// Operators ///////////////////////

	~SoundObject() {
		for (auto it = Effects.begin(); it != Effects.end(); it++) {
			delete it->second;
		}
	}

	virtual SoundObject& operator=(const json& j) = 0;
	virtual SoundObject& operator=(const std::string& s) = 0;
	void operator() () { Play(); }

	// if the File attributes are the same, we can probably assume they are equal, or are
	//	the same "sound" even if they are not the same instance of SoundObject, which is useful

	// != operator will be faster in this instance
	virtual bool operator!=(const SoundObject& other) { return File.compare(other.File) != 0; }
	virtual bool operator==(const SoundObject& other) { return !(*this != other); }


	/////////////////////// Lifecycle (StateChange) Hooks ////////////////////

	// all hooks basically connect to this one void pointer called on state change
	void (*OnStateChange)(SOUND_STATE state) = nullptr;
	
	// methods called statically to expand on functionality
	struct StateChangeHook {

		SOUND_STATE State;
		void (*Callback)();

		StateChangeHook(SOUND_STATE state, void(*callback)()) {
			State = state;
			Callback = callback;
		}
		void OnStateChangeHook(SOUND_STATE state) {
			if (State == state) Callback();
		}
	};

	struct StateChangeHookContainer {
		std::map<SOUND_STATE, void* ()> Hooks;

		StateChangeHookContainer(std::map<SOUND_STATE, void* ()> const& hooks) {
			Hooks = hooks;
		}
		void OnStateChangeHook(SOUND_STATE state) {
			auto it = Hooks.find(state);
			if (it != Hooks.end()) {
				*(it->second)();
			}
		}
	};

	void ConnectStateChangeHook(StateChangeHook const& hook) { 
		OnStateChange = *hook.OnStateChangeHook; 
	}

	void ConnectStateChangeHookContainer(StateChangeHookContainer const& hooks) { 
		OnStateChange = *hooks.OnStateChangeHook; 
	}


	/////////////////////// Parsing ///////////////////////

	std::string File = nullptr;
	std::map<std::string, Effect*> Effects;
	
	// this method sadly has to be snake case and I don't like snake case :c
	static void from_json(const json& j, SoundObject& s);

	// todo: handle non char keys, such as Position and the like
	typedef float (*ParamSetterFunc)(float val);
	static ParamSetterFunc GetParamSetFunc(EFFECT_PARAMETER p, SoundObject& s);

	virtual SoundObject* CopyParams(const SoundObject& s);

	virtual void SetParams() {
		Volume(volume_);
		Pan(pan_);
		Tune(tune_);
		Loop(loop_);
		duration_ = getDuration_();
	}

	/////////////////////// Sound Methods ///////////////////////

	virtual void Load() = 0;
	virtual void Unload() = 0;

	// could do a small optimization with needsUpdate_, but we'd need special SOUND_STATE logic
	void Update(float dt);

	void Play();
	void Pause() { State(SOUND_STATE::PAUSED); handlePause_(); }
	void Resume() { State(SOUND_STATE::PLAYING); handleResume_(); }
	void Finish() { State(SOUND_STATE::FINISHING); handleFinish_(); }
	void Stop() { State(SOUND_STATE::COMPLETE); handleStop_(); }

	bool Playing() { return state_ == SOUND_STATE::PLAYING || state_ == SOUND_STATE::FINISHING; }
	unsigned int CurrentLoop() { return currentLoop_; }

	SoundObject* Queue(SoundObject* next, bool finish = false);
	SoundObject* Queued() { return queued_; }

	/////////////////////// Effect Methods ///////////////////////

	Effect* FindEffect(std::string const& key);
	void AddEffect(std::string key, Effect* effect);
	void AddEffects(std::map<std::string, Effect*> const& effects);
	bool DeleteEffect(std::string const& key);

	SoundObject* ConnectEffect(std::string const& effectKey, Effect::Connection connection);
	SoundObject* ConnectEffects(std::map<std::string, Effect::Connection> const& connections);

	/////////////////////// Param Getters/Setters ///////////////////////

	// getters and setters may be evil, but setting params will need additional
	//		logic in derived classes
	// this way unfortunately makes the most intuitive sense while still
	//		preventing accidental user-caused misbehavior
	
	float Volume() { return volume_; }
	float Volume(float val) {
		volume_ = handleVolume_(val);
		return volume_;
	}

	float Tune() { return tune_; }
	float Tune(float val)
	{
	    tune_ = handleTune_(val);
		return tune_;
	}
	
	float Pan() { return pan_; }
	float Pan(float val)
	{
		pan_ = handlePan_(val);
		return pan_;
	}

	int Loop() { return loop_; }
	int Loop(int val, bool resetCurrentLoop = false)
	{
		loop_ = handleLoop_(val);
		if (resetCurrentLoop) currentLoop_ = 0;
		return loop_;
	}

	SOUND_STATE State() { return state_; }
	virtual SOUND_STATE State(SOUND_STATE val) { 
		state_ = val; 
		if (OnStateChange != nullptr) {
			OnStateChange(state_);
		}
		return state_; 
	};

	float Duration() { return duration_; }


	/////////////////////// Protected Members ///////////////////////

protected:
	SoundObject() = default;
	SoundObject(const SoundObject& s) = delete;

	float	volume_ = 0.f;
	float	tune_ = 0.f;
	float	pan_ = 0.f;
	int		loop_ = 0;
	
	int	currentLoop_ = 0;
	float duration_ = 0.f;
	float elapsedTime_ = 0.f;

	SOUND_STATE state_ = SOUND_STATE::UNLOADED;

	SoundObject* queued_ = nullptr;
	
	virtual void handlePlay_() = 0;
	virtual void handlePause_() = 0;
	virtual void handleResume_() = 0;
	virtual void handleFinish_() = 0;
	virtual void handleStop_() = 0;

	virtual float getDuration_() = 0;
	void updateEffects_(float dt);

	virtual float handleVolume_(float val) = 0;
	virtual float handleTune_(float val) = 0;
	virtual float handlePan_(float val) = 0;

	virtual int handleLoop_(int val) { return val < 0 ? INT_MAX : val; };

	// overridden in ISoundContainer
	virtual void parseParam_(std::string& key, const json& j);
	virtual void parseFile_(std::string& fileKey, const json& j);
	void parseEffects_(std::string& effectsKey, const json& j);

};

