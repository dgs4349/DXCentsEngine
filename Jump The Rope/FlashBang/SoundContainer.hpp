#pragma once

#include "FlashBang.hpp"
#include "ISoundContainer.hpp"
#include "Sound.hpp"

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
		SoundObject::~SoundObject();
	}

	virtual ISoundContainer& operator=(const json& j) override;
	virtual ISoundContainer& operator=(const std::string& s) override;
	
	void Load() override;
	void Unload() override;

	virtual SoundObject* Current() override;
	virtual SoundObject* Next() override;
	virtual int CurrentIndex() override;
	virtual int NextIndex() override;

	virtual void PlayChild(int index, bool stopCurrent = false) override;
	virtual void PlayChild(std::string& key, bool stopCurrent = false) override;
	virtual void PlayNextChild(bool stopCurrent = false) override;

	virtual void QueueChild(int index, bool finishCurrent = true) override;
	virtual void QueueChild(std::string& key, bool finishCurrent = true) override;
	virtual void QueueNextChild(bool finishCurrent = true) override;

	virtual void StopThenJump(int newPosition, bool makeSkipPermenant = false) override;
	virtual void QueueJump(int newPosition, bool finish = true, bool makeSkipPermenant = false) override;

	virtual int SwapIndex(int oldIndex, int newIndex) override;
	virtual int ShiftIndex(int oldIndex, int newIndex) override;

	// should we watch who's being played?
	SoundObject* operator[] (std::string const& s)
	{
		if (find(s) != end())
		{
			const int index = ((json)*this)[s].get<int>();
			return soundObjects_[index];
		}
		return nullptr;
	}

	SoundObject* operator[] (int i)
	{
		return soundObjects_[i];
	}
	
	virtual int AddSoundObject(SoundObject& soundObject) override;
	virtual int AddSoundObject(std::string const& key, SoundObject& soundObject) override;
	virtual void AddSoundObjects(std::vector<SoundObject*> const& soundObjects) override;
	virtual void AddSoundObjects(std::map<std::string, SoundObject*> const& keysoundObjects) override;

protected:
	float handleVolume_(float val) override;
	float handleTune_(float val) override;
	float handlePan_(float val) override;
	int handleLoop_(int val) override;
	virtual SOUNDCONTAINER_PLAYBACK handlePlayback_(SOUNDCONTAINER_PLAYBACK val) override;

	void unload_();

	SoundObject* createSound_(json const& j) override;
	SoundObject* createSoundContainer_(json const& attr) override;
	SoundObject* createSound_(std::string const& key, json const& j) override;
	SoundObject* createSoundContainer_(std::string const& key, json const& attr) override;

	
private:

	std::vector<SoundObject*> soundObjects_;

	// tracking queue order for non-in order
	int currentQueueOrderIndex_ = 0;
	std::vector<int> queueOrder_;

	virtual void handlePlay_() override;
	virtual void handlePause_() override;
	virtual void handleResume_() override;
	virtual void handleFinish_() override;
	virtual void handleStop_() override;

	virtual float getDuration_() override;

};
