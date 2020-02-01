#pragma once

#include "FlashBang.hpp"
#include <nlohmann/json.hpp>

using nlohmann::json;

using namespace FlashBang;

class ISoundObject
{
public:
	~ISoundObject(){}
	
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

	// this method sadly has to be snake case
	static void from_json(const json& j, ISoundObject& s) {
		std::string key;
		for (auto i : j.items()) {
			key = i.key();
			s.parseParam_(key, j);
		}
	}
	
	void operator() () { Play(); }

	ISoundObject& operator= (const ISoundObject& s)
	{
		if (this != &s)
		{
			volume_ = s.volume_;
			tune_ = s.tune_;
			pan_ = s.pan_;
			loop_ = s.loop_;
		}
		return *this;
	}

	ISoundObject(const ISoundObject& s) = delete;
	
protected:
	float	volume_ = 0.0f;
	float	tune_ = 0.0f;
	float	pan_ = 0.0f;
	int		index_ = -1;
	int		loop_ = 0;
	
	virtual void setVolume_(float val) = 0;
	virtual void setTune_(float val) = 0;
	virtual void setPan_(float val) = 0;
	virtual void setIndex_(int val) = 0;
	virtual void setLoop_(int val) = 0;

	virtual void parseParam_(std::string& key, const json& j)
	{
		if (islower(key[0]))
		{
			switch (key[0])
			{
			case 'v': j[key].get_to(volume_); break;
			case 't': j[key].get_to(tune_); break;
			case 'p': j[key].get_to(pan_); break;
			case 'l': j[key].get_to(loop_); break;

			case 'f': parseFile_(key, j);  break;

			case 'e':

				
				
				break;
				
			default: break;
			}
		}
	}

	virtual void parseFile_(std::string& fileKey, const json& j)
	{
		j[fileKey].get_to(File);
	}

private:
	ISoundObject() = delete;
};

