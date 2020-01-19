#pragma once

#include "FlashBang.hpp"
#include <nlohmann/json.hpp>

using nlohmann::json;

using namespace FlashBang;

class ISoundObject
{
public:
	ISoundObject() = default;
	ISoundObject(json& j) { from_json(j, *this); }

	virtual ~ISoundObject(){}

	std::string File = nullptr;

	// todo fix these recent updates in other imlpementations
	virtual float Volume() { return volume_; }
	virtual float Volume(float val)
	{
		setVolume_(val);
		volume_ = val;
		return volume_;
	}

	virtual float Tune() { return tune_; }
	virtual float Tune(float val)
	{
	    setTune_(val);
	    tune_ = val;
		return tune_;
	}
	
	virtual float Pan() { return pan_; }
	virtual float Pan(float val)
	{
	    setPan_(val);
	    pan_ = val;
		return pan_;
	}

	/*
	 * _index: the index
	 */
	virtual int Index() { return index_; }
	virtual int Index(int val)
	{
	    setIndex_(val);
	    index_ = val;
		return index_;
	}

	virtual int Loop() { return loop_; }
	virtual int Loop(int val)
	{
	    setLoop_(val);
	    loop_ = val;
		return loop_;
	}

	virtual void Play() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	virtual void Stop() = 0;

	virtual void Load() = 0;
	virtual void Unload() = 0;

	static void from_json(const json& j, ISoundObject& s) {
		
		const auto end = j.end();
		
		if (j.find("v") != end) j["v"].get_to(s.volume_);
		if (j.find("t") != end) j["t"].get_to(s.tune_);
		if (j.find("p") != end) j["p"].get_to(s.pan_);
		if (j.find("l") != end) j["l"].get_to(s.loop_);
		
		if (j.find("f") != end && j["f"].is_string()) {
			j["f"].get_to(s.File);
		}
	}
	
	void operator() () { Play(); }

	ISoundObject& operator= (const ISoundObject& s)
	{
		if (this == &s) return *this;

		volume_ = s.volume_;
		tune_ = s.tune_;
		pan_ = s.pan_;
		loop_ = s.loop_;
	}


	ISoundObject(const ISoundObject& s) = delete;
	
protected:
	float	volume_ = 0.0f;
	float	tune_ = 0.0f;
	float	pan_ = 0.0f;
	int		index_ = -1;
	int		loop_ = 0;

	bool	managed_ = true;
	
	virtual void setVolume_(float val) = 0;
	virtual void setTune_(float val) = 0;
	virtual void setPan_(float val) = 0;
	virtual void setIndex_(int val) = 0;
	virtual void setLoop_(int val) = 0;

};

