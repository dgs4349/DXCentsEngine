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
	ISoundObject() = default;

	~ISoundObject() {
		for (auto it = Effects.begin(); it != Effects.end(); it++) {
			delete it->second;
		}
	}

	void operator() () { Play(); }

	ISoundObject(const ISoundObject& s) = delete;
	
	virtual ISoundObject& operator=(const json& j) = 0;
	virtual ISoundObject& operator=(const std::string& s) = 0;

	std::string File = nullptr;
	std::map<std::string, Effect*> Effects;
	
	// this method sadly has to be snake case
	static void from_json(const json& j, ISoundObject& s);
	
	typedef float (*ParamSetterFunc)(float val);

	// todo: handle non char keys, such as Position and the like
	static ParamSetterFunc GetParamSetFunc(const char& k, ISoundObject& s) {
		switch (k) {
			case 'v': return *s.Volume;
			case 't': return  *s.Tune;
			case 'p': return *s.Pan;

			default: return nullptr;
		}
	}

	virtual ISoundObject* CopyParams(const ISoundObject& s);
	
	virtual void Load() = 0;
	virtual void Unload() = 0;

	// could do a small optimization with needsUpdate_, but we'd need special SOUND_STATE logic
	void Update(float dt) {
		updateSound_;
		updateEffects_;
	}

	virtual void Play() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	virtual void Finish() = 0;
	virtual void Stop() = 0;

	virtual ISoundObject* Queue(bool finish = false) = 0;
	virtual ISoundObject* Queue(ISoundObject* previous, bool finish = false) = 0;
	virtual ISoundObject* After(bool finish = false) = 0;
	virtual ISoundObject* After(ISoundObject* next, bool finish = false) = 0;

	bool Playing() { return state_ == SOUND_STATE::PLAYING || state_ == SOUND_STATE::FINISHING; }
	unsigned int CurrentLoop() { return currentLoop_; }

	Effect* FindEffect(std::string const& key) {
		auto it = Effects.find(key);
		if (it != Effects.end()) {
			return it->second;
		}
		return nullptr;
	}

	void AddEffect(std::string key, Effect* effect) {
		Effects.insert({ key, effect });
	}

	void AddEffects(std::map<std::string, Effect*> const& effects) {
		for (auto it = effects.begin(); it != effects.end(); ++it) {
			AddEffect(it->first, it->second);
		}
	}

	bool DeleteEffect(std::string const& key) {
		Effect* e = FindEffect(key);
		if (e != nullptr) delete e;
		Effects.erase(key);
	}

	ISoundObject* ConnectEffect(std::string const& effectKey, Effect::Connection connection) {
		auto effect = FindEffect(effectKey);
		if (effect != nullptr) effect->Connect(connection);
		return this;
	}

	ISoundObject* ConnectEffects(std::map<std::string, Effect::Connection> const& connections) {
		for (auto it = connections.begin(); it != connections.end(); it++) {
			auto effect = FindEffect(it->first);
			if (effect != nullptr) effect->Connect(it->second);
		}
		return this;
	}

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

	int Index() { return index_; }
protected:
	int Index(int val)
	{
		// index should only be set by containers, but will not have any additional logic
		index_ = val;
		return index_;
	}
public:

	ISoundObject* Queuer() { return queuer_; }
protected:
	ISoundObject* Queuer(ISoundObject* val) {
		queuer_ = Queue(val);
		return queuer_;
	}
public:

	ISoundObject* Afteree() { return afteree_; }
protected:
	ISoundObject* Afteree(ISoundObject* val) {
		afteree_ = After(val);
		return afteree_;
	}
	
	int Loop() { return loop_; }
	int Loop(int val, bool resetCurrentLoop = false)
	{
	    loop_ = handleLoop_(val);
		if (resetCurrentLoop) currentLoop_ = 0;
		return loop_;
	}

	SOUND_STATE State() { return state_; }
	SOUND_STATE State(SOUND_STATE val)
	{
		state_ = handleState_(val);
		return state_;
	}
	
protected:
	float	volume_ = 0.f;
	float	tune_ = 0.f;
	float	pan_ = 0.f;
	int		index_ = -1;
	int loop_ = 0;

	ISoundObject* queuer_ = nullptr;
	ISoundObject* afteree_ = nullptr;

	int currentLoop_ = 0;

	SOUND_STATE state_ = SOUND_STATE::UNLOADED;
	
	virtual float handleVolume_(float val) = 0;
	virtual float handleTune_(float val) = 0;
	virtual float handlePan_(float val) = 0;
	virtual int handleLoop_(int val) = 0;
	virtual SOUND_STATE handleState_(SOUND_STATE state) = 0;



	virtual void updateSound_(float dt) = 0;
	virtual void updateEffects_(float dt) = 0;

	// overridden in ISoundContainer
	void parseParam_(std::string& key, const json& j);
	void parseFile_(std::string& fileKey, const json& j);
	void parseEffects_(std::string& effectsKey, const json& j);

};

