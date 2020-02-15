#pragma once

#include "FlashBang.hpp"
#include "Effect.hpp"

#include <nlohmann/json.hpp>

using nlohmann::json;

using namespace FlashBang;

/*
	Basic interface for declaration
		Handles all parsing and parameter logic
		Forces abstract calls to handle audio and state logic
*/
class ISoundObject
{
public:

	/////////////////////// Operators ///////////////////////

	ISoundObject() = default;
	ISoundObject(const ISoundObject & s) = delete;

	~ISoundObject() {
		for (auto it = Effects.begin(); it != Effects.end(); it++) {
			delete it->second;
		}
	}

	virtual ISoundObject& operator=(const json& j) = 0;
	virtual ISoundObject& operator=(const std::string& s) = 0;
	void operator() () { Play(); }



	/////////////////////// Parsing ///////////////////////

	std::string File = nullptr;
	std::map<std::string, Effect*> Effects;
	
	// this method sadly has to be snake case
	static void from_json(const json& j, ISoundObject& s);
	
	// todo: handle non char keys, such as Position and the like
	typedef float (*ParamSetterFunc)(float val);
	static ParamSetterFunc GetParamSetFunc(EFFECT_PARAMETER p, ISoundObject& s);

	virtual ISoundObject* CopyParams(const ISoundObject& s);


	/////////////////////// Sound Methods ///////////////////////

	virtual void Load() = 0;
	virtual void Unload() = 0;

	// could do a small optimization with needsUpdate_, but we'd need special SOUND_STATE logic
	void Update(float dt);

	virtual void Play() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	virtual void Finish() = 0;
	virtual void Stop() = 0;

	bool Playing() { return state_ == SOUND_STATE::PLAYING || state_ == SOUND_STATE::FINISHING; }
	unsigned int CurrentLoop() { return currentLoop_; }

	virtual ISoundObject* Queue(bool finish = false) = 0;
	virtual ISoundObject* Queue(ISoundObject* previous, bool finish = false) = 0;
	virtual ISoundObject* After(bool finish = false) = 0;
	virtual ISoundObject* After(ISoundObject* next, bool finish = false) = 0;



	/////////////////////// Effect Methods ///////////////////////

	Effect* FindEffect(std::string const& key);
	void AddEffect(std::string key, Effect* effect);
	void AddEffects(std::map<std::string, Effect*> const& effects);
	bool DeleteEffect(std::string const& key);

	ISoundObject* ConnectEffect(std::string const& effectKey, Effect::Connection connection);
	ISoundObject* ConnectEffects(std::map<std::string, Effect::Connection> const& connections);



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


	ISoundObject* Queuer() { return queuer_; }
	ISoundObject* Queuer(ISoundObject* val) {
		queuer_ = Queue(val);
		return queuer_;
	}

	ISoundObject* Afteree() { return afteree_; }
	ISoundObject* Afteree(ISoundObject* val) {
		afteree_ = After(val);
		return afteree_;
	}

	SOUND_STATE State() { return state_; }
	SOUND_STATE State(SOUND_STATE val)
	{
		state_ = handleState_(val);
		return state_;
	}


	/////////////////////// Protected Members ///////////////////////

protected:
	float	volume_ = 0.f;
	float	tune_ = 0.f;
	float	pan_ = 0.f;
	int		loop_ = 0;
	int		currentLoop_ = 0;

	SOUND_STATE state_ = SOUND_STATE::UNLOADED;

	ISoundObject* queuer_ = nullptr;
	ISoundObject* afteree_ = nullptr;
	
	virtual void handlePlay_() = 0;
	virtual void handlePause() = 0;
	virtual void handleResume() = 0;
	virtual void handleFinish() = 0;
	virtual void handleStop() = 0;

	virtual float handleVolume_(float val) = 0;
	virtual float handleTune_(float val) = 0;
	virtual float handlePan_(float val) = 0;
	virtual int handleLoop_(int val) = 0;
	virtual SOUND_STATE handleState_(SOUND_STATE state) = 0;

	virtual void updateEffects_(float dt) = 0;

	// overridden in ISoundContainer
	virtual void parseParam_(std::string& key, const json& j);
	virtual void parseFile_(std::string& fileKey, const json& j);
	void parseEffects_(std::string& effectsKey, const json& j);

};

