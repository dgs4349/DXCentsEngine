#pragma once

#include "FlashBang.hpp"
#include <nlohmann/json.hpp>

using nlohmann::json;

using namespace FlashBang;


class ISoundObject
{
public:
	ISoundObject(){}
	~ISoundObject(){}

	std::string File = nullptr;

	float Volume() { return _volume; }
	virtual float Volume(float val) { _volume = val; return _volume; }

	float Tune() { return _tune; }
	virtual float Tune(float val) { _tune = val; return _tune; }

	float Pan() { return _pan; }
	virtual float Pan(float val) { _pan = val; return _pan; }

	int Order() { return _order; }
	virtual int Order(int val) { _order = val; return _order; }

	int Loop() { return _loop; }
	virtual int Loop(int val) { _loop = val; return _loop; }

	virtual void Play();
	virtual void Pause();
	virtual void Resume();
	virtual void Stop();

	virtual void Load();
	virtual void UnLoad();
	
	void from_json(const json& j, ISoundObject& s) {
		j.at("v").get_to(s._volume);
		j.at("t").get_to(s._tune);
		j.at("p").get_to(s._pan);
		j.at("o").get_to(s._order);
		j.at("l").get_to(s._loop);

		j.at("f").get_to(s.File);
	}
	
protected:
	float	_volume = 0.0f;
	float	_tune = 0.0f;
	float	_pan = 0.0f;
	int		_order = 0;
	int		_loop = 0;

	virtual float _setVolume(float val);
	virtual float _setTune(float val);
	virtual float _setPan(float val);
	virtual int _setOrder(int val);
	virtual int _setLoop(int val);
};

