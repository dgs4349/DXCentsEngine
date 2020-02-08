#pragma once

#include "FlashBang.hpp"
#include "Effect.hpp"

#include <nlohmann/json.hpp>

using nlohmann::json;

using namespace FlashBang;

class ISoundObject
{
public:
	ISoundObject() = delete;

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
	virtual ISoundObject* CopyParams(const ISoundObject& s);

	
	virtual void Load() = 0;
	virtual void Unload() = 0;

	// could do a small optimization with needsUpdate_, but we'd need special SOUND_STATE logic
	virtual void Update(float dt) = 0;

	virtual ISoundObject* Queue(bool finish = false) = 0;
	virtual ISoundObject* Queue(ISoundObject* previous, bool finish = false) = 0;
	virtual ISoundObject* After(bool finish = false) = 0;
	virtual ISoundObject* After(ISoundObject* next, bool finish = false) = 0;

	virtual void Play() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	virtual void Finish() = 0;
	virtual void Stop() = 0;

	// getters and setters may be evil, but setting params will need additional
	//		logic in derived classes
	// this way unfortunately makes the most intuitive sense while still
	//		preventing accidental user-caused misbehavior
	
	virtual float Volume() { return volume_; }
	virtual float Volume(float val) {
		volume_ = handleVolume_(val);
		return volume_;
	}

	virtual float Tune() { return tune_; }
	virtual float Tune(float val)
	{
	    tune_ = handleTune_(val);
		return tune_;
	}
	
	virtual float Pan() { return pan_; }
	virtual float Pan(float val)
	{
		pan_ = handlePan_(val);
		return pan_;
	}

	virtual int Index() { return index_; }
protected:
	virtual int Index(int val)
	{
		// index should only be set by containers, but will not have any additional logic
		index_ = val;
		return index_;
	}
public:

	virtual ISoundObject* Queuer() { return queuer_; }
protected:
	virtual ISoundObject* Queuer(ISoundObject* val) {
		queuer_ = val;
		return queuer_;
	}
public:

	virtual ISoundObject* Afteree() { return afteree_; }
protected:
	virtual ISoundObject* Afteree(ISoundObject* val) {
		afteree_ = val;
		return afteree_;
	}
	
	virtual int Loop() { return loop_; }
	virtual int Loop(int val)
	{
	    loop_ = handleLoop_(val);
		return loop_;
	}

	virtual SOUND_STATE State() { return state_; }
	virtual SOUND_STATE State(SOUND_STATE val)
	{
		state_ = handleState_(val);
		return state_;
	}
	
protected:
	float	volume_ = 0.0f;
	float	tune_ = 0.0f;
	float	pan_ = 0.0f;
	int		index_ = -1;
	int		loop_ = 0;

	ISoundObject* queuer_ = nullptr;
	ISoundObject* afteree_ = nullptr;

	unsigned int currentLoop_ = 0;

	SOUND_STATE state_ = SOUND_STATE::UNLOADED;
	
	virtual float handleVolume_(float val) = 0;
	virtual float handleTune_(float val) = 0;
	virtual float handlePan_(float val) = 0;
	virtual int handleLoop_(int val) = 0;
	virtual SOUND_STATE handleState_(SOUND_STATE state) = 0;

	// overridden in ISoundContainer
	virtual void parseParam_(std::string& key, const json& j);
	virtual void parseFile_(std::string& fileKey, const json& j);
};

