#pragma once

#include "FlashBang_Fwd.hpp"
#include "Effect.hpp"

#include <nlohmann/json.hpp>

using nlohmann::json;

using namespace FlashBang;

static const int LOOP_CAP = 1000000000;

/*
	Handles parsing
	Handles basic update logic

	Creates basic behavior for derived classes
	is NOT in charge of playback, that is for derived classes

	TODO: make const SoundObject for better memory overhead w/o complex setters and effects
*/
class SoundObject
{
public:

	/////////////////////// Operators ///////////////////////

	virtual ~SoundObject() {
		for (auto [key, val] : Effects) delete val;
		delete VolumeCall;
		delete TuneCall;
		delete PanCall;
	}

	virtual SoundObject& operator=(const json& j) = 0;
	virtual SoundObject& operator=(const std::string& s) = 0;
	void operator() () { Play(); }

	// if the File attributes are the same, we can probably assume they are equal, or are
	//	the same "sound" even if they are not the same instance of SoundObject, which is useful

	// != operator will be faster in this instance
	virtual bool operator!=(const SoundObject& other) { return File != other.File; }
	virtual bool operator==(const SoundObject& other) { return !(*this != other); }

	/////////////////////// Lifecycle (StateChange) Hooks ////////////////////
	// TODO: investigate complexity and size on these, maybe divide to another object

	struct StateChangeHook
	{
		SOUND_STATE State; CallablePtr Callback;
		StateChangeHook(SOUND_STATE state, CallablePtr callback)
			{ State = state; Callback = callback; }
		StateChangeHook() = default;
	};
	
	std::map<SOUND_STATE, std::vector<CallablePtr>> StateChangeHooks;
	std::map<SOUND_STATE, std::vector<CallablePtr>> FromStateChangeHooks;

	void OnStateChange(SOUND_STATE state, CallablePtr callback)
	{
		StateChangeHooks[state].push_back(callback);
	}

	void OnStateChange(const StateChangeHook* hook)
	{
		StateChangeHooks[hook->State].push_back(hook->Callback);
	}

	void FromStateChange(SOUND_STATE state, CallablePtr callback)
	{
		FromStateChangeHooks[state].push_back(callback);
	}

	void FromStateChange(const StateChangeHook* hook)
	{
		FromStateChangeHooks[hook->State].push_back(hook->Callback);
	}
	
	/////////////////////// Parsing ///////////////////////
	std::string Key = nullptr;
	std::string File = nullptr;
	std::map<std::string, Effect*> Effects;

	// this method sadly has to be snake case and I don't like snake case :c
	static void from_json(const json& j, SoundObject& s);

	// todo: handle non char keys, such as Position and the like
	static ParameterCallablePtr GetParameterCallable(SOUND_PARAM p, SoundObject& s);

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

	bool Playing() const { return state_ == SOUND_STATE::PLAYING || state_ == SOUND_STATE::FINISHING; }
	unsigned int CurrentLoop() const { return currentLoop_; }

	SoundObject* Queue(SoundObject* next, bool finish = false);
	SoundObject* Queued() const { return queued_; }

	/////////////////////// Effect Methods ///////////////////////

	Effect* FindEffect(std::string const& key);
	void AddEffect(std::string key, Effect* effect);
	void AddEffects(std::map<std::string, Effect*> const& effects);
	void DeleteEffect(std::string const& key);

	SoundObject* ConnectEffect(std::string const& effectKey, Effect::Connection connection);
	SoundObject* ConnectEffects(std::map<std::string, Effect::Connection> const& connections);

	/////////////////////// Param Getters/Setters ///////////////////////

	// getters and setters may be evil, but setting params will need additional
	//		logic in derived classes
	// this way unfortunately makes the most intuitive sense while still
	//		preventing accidental user-caused misbehavior
	
	float Volume() const { return volume_; }
	float Volume(const float val) {
		volume_ = handleVolume_(val);
		return volume_;
	}

	float Tune() const { return tune_; }
	float Tune(const float val)
	{
		tune_ = handleTune_(val);
		return tune_;
	}

	float Pan() const { return pan_; }
	float Pan(const float val)
	{
		pan_ = handlePan_(val);
		return pan_;
	}

	int Loop() const { return loop_; }
	int Loop(const int val, const bool resetCurrentLoop = false)
	{
		loop_ = handleLoop_(val);
		if (resetCurrentLoop) currentLoop_ = 0;
		return loop_;
	}

	SOUND_STATE State() const { return state_; }
	virtual SOUND_STATE State(const SOUND_STATE val) {
		if(state_ != val)
		{
			if (!FromStateChangeHooks[state_].empty())
			{
				for (auto *const funcPtr : FromStateChangeHooks[state_]) (*funcPtr)();
			}

			state_ = val;

			if (!StateChangeHooks[state_].empty())
			{
				for (auto* const funcPtr : StateChangeHooks[state_]) (*funcPtr)();
			}

		}
		
		return state_;
	};

	float Duration() { return (duration_ >= 0.f) ? duration_ : getDuration_(); }

	// slight vulnerability to float overflow if duration_ is somehow max float
	float GetFullDuration() { return Duration() * static_cast<float> (loop_); }

	/////////////////////// Parameter Callables (Functionoids) ///////////////////////

	class VolumeCallable : public IParameterCallable
	{
		SoundObject* s_;
	public:
		VolumeCallable(SoundObject* s) { s_ = s; }
		float operator()() override { return s_->volume_; }
		float operator()(float val) override { return s_->Volume(val); }
	};

	class TuneCallable : public IParameterCallable
	{
		SoundObject* s_;
	public:
		TuneCallable(SoundObject* s) { s_ = s; }
		float operator()() override { return s_->tune_; }
		float operator()(float val) override { return s_->Tune(val); }
	};

	class PanCallable : public IParameterCallable
	{
		SoundObject* s_;
	public:
		PanCallable(SoundObject* s) { s_ = s; }
		float operator()() override { return s_->pan_; }
		float operator()(float val) override { return s_->Pan(val); }
	};

	VolumeCallable* VolumeCall = new VolumeCallable(this);
	TuneCallable* TuneCall = new TuneCallable(this);
	PanCallable* PanCall = new PanCallable(this);

	
	/////////////////////// Protected Members ///////////////////////

	SoundObject(const SoundObject & s) = delete;
	
protected:
	SoundObject() = default;

	float	volume_ = 0.f;
	float	tune_ = 0.f;
	float	pan_ = 0.f;
	int		loop_ = 0;

	int	currentLoop_ = 0;
	float duration_ = -1.f;
	float elapsedTime_ = 0.f;

	SOUND_STATE state_ = SOUND_STATE::UNLOADED;

	SoundObject* queued_ = nullptr;

	virtual void handlePlay_() = 0;
	virtual void handlePause_() = 0;
	virtual void handleResume_() = 0;
	virtual void handleFinish_() = 0;
	virtual void handleStop_() = 0;

	virtual float getDuration_(float tune) = 0;
	virtual float getDuration_() { return getDuration_(tune_); }

	void updateEffects_(float dt);

	virtual float handleVolume_(float val) = 0;
	virtual float handleTune_(float val) = 0;
	virtual float handlePan_(float val) = 0;

	virtual int handleLoop_(int val) {
		if (currentLoop_ == loop_) Finish();
		return val < 0 ? LOOP_CAP : val;
	};

	// overridden in ISoundContainer
	virtual void parseParam_(const std::string& key, const json& j);

	void parseEffects_(const std::string& key, const json& j);
	void throwEffectError_(int i, const std::string& key, const json& j) const;
};