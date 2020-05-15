#pragma once

#include "FlashBang.hpp"
#include "ISoundContainer.hpp"
#include "Sound.hpp"

using namespace FlashBang;

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
	virtual void Reset() override { reset_(true); }
	virtual void Reverse() override;

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

	virtual int AddSoundObject(SoundObject* soundObject) override;
	virtual int AddSoundObject(std::string const& key, SoundObject* soundObject) override;
	virtual void AddSoundObjects(std::vector<SoundObject*> const& soundObjects) override;
	virtual void AddSoundObjects(std::map<std::string, SoundObject*> const& soundObjectsMap) override;

private:
	SoundContainer();

	/*
		DECISIONS
		we are json, we are holding pointers to sound objects
		we should access sound objects from ourselves, but in a way
		where directly adding to json[asd]=asd is permitted

		overwrite what happens when adding?
		or, storing an index attribute on sound object, either works
	*/
	std::vector<SoundObject*> soundObjects_;

	bool orderSet_ = false;

	int currentSoundObjectIndex_ = 0;
	int currentQueueOrderIndex_ = 0;
	std::vector<int> queueOrder_;

	void reset_(bool resetIndeces);
	void updateCurrentIndex_();
	void queueNext_();

	SoundObject::StateChangeHook onCompleteHook =
		StateChangeHook(SOUND_STATE::COMPLETE, *(this->updateCurrentIndex_));

	int randomIndex_() {
		return rand() % queueOrder_.size();
	}

	int randomOther_() {
		static int prev = -1;
		int r = randomIndex_();
		prev = (r == prev) ? r++ : r;
		return prev;
	}

protected:
	float handleVolume_(float val) override;
	float handleTune_(float val) override;
	float handlePan_(float val) override;
	virtual SOUNDCONTAINER_PLAYBACK_ORDER handlePlayback_(SOUNDCONTAINER_PLAYBACK_ORDER val) override;

	SoundObject* createSound_(json const& j) override;

	virtual void handlePlay_() override;
	virtual void handlePause_() override;
	virtual void handleResume_() override;
	virtual void handleFinish_() override;
	virtual void handleStop_() override;

	virtual float getDuration_(float tune) override;
	virtual float getDuration_() override;
};