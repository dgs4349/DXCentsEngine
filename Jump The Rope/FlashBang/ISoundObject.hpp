#pragma once

#include "FlashBang.hpp"
#include <nlohmann/json.hpp>

using nlohmann::json;

using namespace FlashBang;


class ISoundObject
{
public:
	ISoundObject() = default;
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
		j.at("v").get_to(s.volume_);
		j.at("t").get_to(s.tune_);
		j.at("p").get_to(s.pan_);
		j.at("l").get_to(s.loop_);

		j.at("f").get_to(s.File);
	}
	
protected:
	float	volume_ = 0.0f;
	float	tune_ = 0.0f;
	float	pan_ = 0.0f;
	int		index_ = 0;
	int		loop_ = 0;

	virtual void setVolume_(float val) = 0;
	virtual void setTune_(float val) = 0;
	virtual void setPan_(float val) = 0;
	virtual void setIndex_(int val) = 0;
	virtual void setLoop_(int val) = 0;
};

