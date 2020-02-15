#pragma once

#include "FlashBang.hpp"
#include "ISoundContainer.hpp"
#include "Sound.h"

using namespace FlashBang;


class SoundContainer : public ISoundContainer
{
public:
	SoundContainer() = delete;
	SoundContainer(const json& j);
	SoundContainer(const std::string& s);

	~SoundContainer() {
		for (auto el : soundObjects_) {
			delete el;
		}
		ISoundObject::~ISoundObject();
	}

	virtual ISoundContainer& operator=(const json& j) override;
	virtual ISoundContainer& operator=(const std::string& s) override;
	
	void Play() override;
	void Pause() override;
	void Resume() override;
	void Stop() override;
	void Load() override;
	void Unload() override;

	virtual void Play(int index) override;
	virtual void PlayNext() override;
	virtual void Queue(int index) override;
	virtual void QueueNext() override;

	virtual ISoundObject* Queue(bool finish = false) override;
	virtual ISoundObject* Queue(ISoundObject* previous, bool finish = false) override;
	virtual ISoundObject* After(bool finish = false) override;
	virtual ISoundObject* After(ISoundObject* next, bool finish = false) override;
	virtual void Finish() override;

	virtual int Next() override;
	virtual int Current() override;

	virtual ISoundObject* CurrentlyPlaying() override;

	virtual int SwapIndex(int oldIndex, int newIndex) override;
	virtual int ShiftIndex(int oldIndex, int newIndex) override;

	// should we watch who's being played?
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
	
	virtual int AddSoundObject(ISoundObject& soundObject) override;
	virtual int AddSoundObject(std::string const& key, ISoundObject& soundObject) override;
	virtual void AddSoundObjects(std::vector<ISoundObject*> const& soundObjects) override;
	virtual void AddSoundObjects(std::map<std::string, ISoundObject*> const& keysoundObjects) override;

protected:
	float handleVolume_(float val) override;
	float handleTune_(float val) override;
	float handlePan_(float val) override;
	int handleLoop_(int val) override;
	virtual SOUND_STATE handleState_(SOUND_STATE state) override;
	virtual SOUNDCONTAINER_PLAYBACK handlePlayback_(SOUNDCONTAINER_PLAYBACK val) override;

	void unload_();

	ISoundObject* createSound_(json const& j) override;
	ISoundObject* createSoundContainer_(json const& attr) override;
	ISoundObject* createSound_(std::string const& key, json const& j) override;
	ISoundObject* createSoundContainer_(std::string const& key, json const& attr) override;

	
private:

	std::vector<ISoundObject*> soundObjects_;

	// tracking queue order for non-in order
	int currentQueueOrderIndex_ = 0;
	std::vector<int> queueOrder_;

	virtual void updateSound_(float dt) override;
	virtual void updateEffects_(float dt) override;
};
