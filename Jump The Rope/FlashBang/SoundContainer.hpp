#pragma once

#include "FlashBang.hpp"
#include "ISoundContainer.hpp"
#include "Sound.hpp"


class FlashBang::SoundContainer : public ISoundContainer
{
public:
	SoundContainer(const json& j);
	SoundContainer(const std::string& s);

	~SoundContainer() {
		for (auto el : soundObjects_) {
			delete el;
		}
		SoundObject::~SoundObject();
	}

	virtual ISoundContainer& operator=(const json& j) override;
	virtual ISoundContainer& operator=(const std::string& key) override;
	SoundObject* operator[] (std::string const& key) { return At(key); }
	SoundObject* operator[] (int i) { return At(i); }
	
	void Load() override;
	void Unload() override;

	virtual SoundObject* Current() override;
	virtual SoundObject* Next() override;
	virtual SoundObject* At(int index) override;
	virtual SoundObject* At(std::string const& key) override;

	virtual int Index(std::string const& key) override;
	virtual int CurrentIndex() override;
	virtual int NextIndex() override;

	virtual void PlayChild(int index, bool stopCurrent = false) override;
	virtual void PlayChild(std::string const& key, bool stopCurrent = false) override;
	virtual void PlayNextChild(bool stopCurrent = false) override;

	virtual void QueueChild(int index, bool finishCurrent = true) override;
	virtual void QueueChild(std::string const& key, bool finishCurrent = true) override;

	virtual int AddSoundObject(SoundObject* soundObject) = 0;
	virtual int AddSoundObject(std::string const& key, SoundObject* soundObject) = 0;
	virtual void AddSoundObjects(std::vector<SoundObject*> const& soundObjects) override;
	virtual void AddSoundObjects(std::map<std::string, SoundObject*> const& soundObjectsMap) override;

private:
	SoundContainer() = default;

	std::vector<SoundObject*> soundObjects_;

	// tracking queue order for non-in order
	int currentQueueOrderIndex_ = 0;
	std::vector<int> queueOrder_;

	void updateCurrentIndex();

	SoundObject::StateChangeHook onCompleteHook =
		StateChangeHook(SOUND_STATE::COMPLETE, this->updateCurrentIndex);
	
protected:
	float handleVolume_(float val) override;
	float handleTune_(float val) override;
	float handlePan_(float val) override;
	int handleLoop_(int val) override;
	virtual SOUNDCONTAINER_PLAYBACK handlePlayback_(SOUNDCONTAINER_PLAYBACK val) override;

	SoundObject* createSound_(json const& j) override;
	SoundObject* createSoundContainer_(json const& attr) override;
	SoundObject* createSound_(std::string const& key, json const& j) override;
	SoundObject* createSoundContainer_(std::string const& key, json const& attr) override;
	
	virtual void handlePlay_() override;
	virtual void handlePause_() override;
	virtual void handleResume_() override;
	virtual void handleFinish_() override;
	virtual void handleStop_() override;

	virtual float getDuration_() override;
};
