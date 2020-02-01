#pragma once

#include "FlashBang.hpp"
#include "ISoundObject.hpp"
#include "SoundEngine.h"

#include <Audio.h>

using namespace FlashBang;

class Sound : ISoundObject
{
public:

	static Sound* Create(json& definingJson)
	{
		Sound* s = new Sound();
		ISoundObject::from_json(definingJson, *s);
		return s;
	}
	static Sound* Create(std::string& definingString)
	{
		json j = definingString;
		return Create(j);
	}
	
	void Play() override;
	void Pause() override;
	void Resume() override;
	void Stop() override;
	void Load() override;
	void Unload() override{ unload_(); }


	std::unique_ptr<DirectX::SoundEffect> DirectXSoundEffect;
	std::unique_ptr<DirectX::SoundEffectInstance> DirectXSoundEffectInstance;
	
protected:
	void setVolume_(float val) override;
	void setTune_(float val) override;
	void setPan_(float val) override;
	void setIndex_(int val) override;
	void setLoop_(int val) override;

private:
	Sound();
	~Sound();

	void unload_();

};
