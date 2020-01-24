#pragma once

#include "FlashBang.hpp"
#include "ISoundContainer.hpp"
#include "Sound.h"

using namespace FlashBang;


class SoundContainer : public ISoundContainer
{
public:
	SoundContainer();
	~SoundContainer();
	
	void Play() override;
	void Pause() override;
	void Resume() override;
	void Stop() override;
	void Load() override;
	void Unload() override;

	void AddSoundObject(ISoundObject* s) { sounds_.push_back(s); }

	ISoundObject* operator[] (std::string s)
	{
		if (find(s) != end())
		{
			const int index = ((json)*this)[s].get<int>();
			return sounds_[index];
		}
		else return nullptr;
	}

	
protected:
	void setVolume_(float val) override;
	void setTune_(float val) override;
	void setPan_(float val) override;
	void setIndex_(int val) override;
	void setLoop_(int val) override;

	void unload_();
	
	void createSound_(std::string const& key) override;

private:
	std::vector<ISoundObject*> sounds_;
	std::vector<int> managedSoundObjects_;
};
