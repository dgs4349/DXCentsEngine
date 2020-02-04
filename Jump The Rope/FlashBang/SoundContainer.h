#pragma once

#include "FlashBang.hpp"
#include "ISoundContainer.hpp"
#include "Sound.h"

using namespace FlashBang;


class SoundContainer : public ISoundContainer
{
public:
	~SoundContainer() {}
	
	static SoundContainer* Create(json& definingJson)
	{
		SoundContainer* s = new SoundContainer();
		ISoundContainer::from_json(definingJson, *s);
		return s;
	}
	static SoundContainer* Create(std::string& definingString)
	{
		json j = definingString;
		return Create(j);
	}
	
	void Play() override;
	void Pause() override;
	void Resume() override;
	void Stop() override;
	void Load() override;
	void Unload() override;

	void AddSoundObject(ISoundObject* s) { soundObjects_.push_back(s); }

	ISoundObject* operator[] (std::string const& s)
	{
		if (find(s) != end())
		{
			const int index = ((json)*this)[s].get<int>();
			return soundObjects_[index];
		}
		return nullptr;
	}

	ISoundObject* operator[] (int i)
	{
		return soundObjects_[i];
	}
	
protected:
	void handleVolume_(float val) override;
	void handleTune_(float val) override;
	void handlePan_(float val) override;
	void handleIndex_(int val) override;
	void handleLoop_(int val) override;

	void unload_();


	ISoundObject* createSound_(json const& j) override;
	ISoundObject* createSoundContainer_(json const& attr) override;
	ISoundObject* createSound_(std::string const& key, json const& j) override;
	ISoundObject* createSoundContainer_(std::string const& key, json const& attr) override;
	
	int addSoundObject_(ISoundObject const& soundObject) override;
	int addSoundObject_(std::string const& key, ISoundObject const& soundObject) override;
	
private:
	SoundContainer() = delete;
	
	std::vector<ISoundObject*> soundObjects_;
};
