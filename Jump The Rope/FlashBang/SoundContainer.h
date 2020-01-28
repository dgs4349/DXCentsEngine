#pragma once

#include "FlashBang.hpp"
#include "ISoundContainer.hpp"
#include "Sound.h"

using namespace FlashBang;


class SoundContainer : public ISoundContainer
{
public:

	// todo:
	/*'
	 * This should probs be in Sound or SoundObject but nonetheless
	 *
	 * Sax parsing doesn't have a json object creator, which is stupid. But, it can be useful
	 *	construct on json: So our key-command conversions can just be a function to iterator through json objects to convert
	 *	construct on string or input: We can use a sax interface to "fix" a string and then convert a json object
	 */
	
	SoundContainer();
	~SoundContainer();
	
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
		else return nullptr;
	}

	ISoundObject* operator[] (int i)
	{
		return soundObjects_[i];
	}
	
protected:
	void setVolume_(float val) override;
	void setTune_(float val) override;
	void setPan_(float val) override;
	void setIndex_(int val) override;
	void setLoop_(int val) override;

	void unload_();


	ISoundObject* createSound_(json const& j) override;
	ISoundObject* createSoundContainer_(json const& attr) override;
	ISoundObject* createSound_(std::string const& key, json const& j) override;
	ISoundObject* createSoundContainer_(std::string const& key, json const& attr) override;
	
	int addSoundObject_(ISoundObject const& soundObject) override;
	int addSoundObject_(std::string const& key, ISoundObject const& soundObject) override;
	
private:
	
	std::vector<ISoundObject*> soundObjects_;
};
