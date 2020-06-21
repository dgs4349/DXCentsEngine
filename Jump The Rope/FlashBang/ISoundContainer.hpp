#pragma once

#include "nlohmann/json.hpp"

#include "SoundObject.hpp"

using namespace FlashBang;

/*
	ISoundContainer
	- handles parsing
	- abstracts basic behaviors
*/

// TODO: move json => unordered_map, or reevaluate both key and index access
//		add an index_ prop to sound for faster order traversal

class ISoundContainer : public SoundObject, public json
{
public:
	~ISoundContainer() = default;

	virtual SoundObject& operator[] (const char* key) = 0;
	virtual SoundObject& operator[] (int i) = 0;

	virtual size_t size() = 0;
	
	/*virtual bool operator== (ISoundContainer const& other) { return Files == other.Files; }
	virtual bool operator!= (ISoundContainer const& other) { return !(*this == other); }
	*/
	// todo: deleting sounds at an index or string could be useful
	// void operator delete(void*);

	static void from_json(json const& j, ISoundContainer& s);

	virtual int AddSoundObject(SoundObject* soundObject) = 0;
	virtual int AddSoundObject(std::string const& key, SoundObject* soundObject) = 0;

	virtual void AddSoundObjects(std::vector<SoundObject*> const& soundObjects) = 0;
	virtual void AddSoundObjects(std::map<std::string, SoundObject*> const& soundObjectsMap) = 0;

	virtual SoundObject* Current() = 0;
	virtual SoundObject* Next() = 0;
	virtual SoundObject* At(int index) = 0;
	virtual SoundObject* At(std::string const& key) = 0;

	virtual int Index(std::string const& key) = 0;
	virtual int CurrentIndex() = 0;
	virtual int NextIndex() = 0;

	virtual void PlayChild(int index, bool stopCurrent = false) = 0;
	virtual void PlayChild(std::string const& key, bool stopCurrent = false) = 0;
	virtual void PlayNextChild(bool stopCurrent = false) = 0;

	virtual void QueueChild(int index, bool finishCurrent = true) = 0;
	virtual void QueueChild(std::string const& key, bool finishCurrent = true) = 0;

	// immutable type, no real reason to change type after the fact
	SOUNDCONTAINER_PLAYBACK_BEHAVIOR PlaybackBehavior() { return playbackBehavior_; }

	// playback is allowed to be changed
	SOUNDCONTAINER_PLAYBACK_ORDER PlaybackOrder() { return playbackOrder_; }
	SOUNDCONTAINER_PLAYBACK_ORDER PlaybackOrder(SOUNDCONTAINER_PLAYBACK_ORDER val) {
		playbackOrder_ = handlePlayback_(val);
		return playbackOrder_;
	}

	virtual void Reset() = 0;
	virtual void Reverse() = 0;

	int LastIndex() { return NextIndex() <= 0; }

protected:

	ISoundContainer() = default;

	SOUNDCONTAINER_PLAYBACK_BEHAVIOR playbackBehavior_ = SOUNDCONTAINER_PLAYBACK_BEHAVIOR::ONE_SHOT;
	SOUNDCONTAINER_PLAYBACK_ORDER playbackOrder_ = SOUNDCONTAINER_PLAYBACK_ORDER::IN_ORDER;

	virtual SOUNDCONTAINER_PLAYBACK_ORDER handlePlayback_(SOUNDCONTAINER_PLAYBACK_ORDER val) = 0;

	virtual SoundObject* createSound_(json const& j) = 0;
	virtual SoundObject* createSoundContainer_(json const& j) = 0;

	void parseParam_(std::string& key, const json& j) override;
	void parseItems_(const json& items);

	void parseSchema_(const json& schema);
	std::vector<std::string>& processSchemaString_(
		const std::string& str,
		std::vector<std::string>& resultsVector
	);

	static void throwSchemaError_(const std::string& files, const std::string& keys);

	static SOUNDCONTAINER_ITEM_TYPE getItemType_(const std::string& itemKey) {
		auto const it = SOUNDCONATINER_ITEM_TYPE_ARGS.find(itemKey);
		if (it != SOUNDCONATINER_ITEM_TYPE_ARGS.end()) {
			SOUNDCONTAINER_ITEM_TYPE s = it->second;
			return s;
		}
		return  SOUNDCONTAINER_ITEM_TYPE::SOUND;
	}

private:

	// SoundContainer should NEVER be compared as a SoundObject
	virtual bool operator!=(const SoundObject& other) override { return false; }
	virtual bool operator==(const SoundObject& other) override { return false; }
};