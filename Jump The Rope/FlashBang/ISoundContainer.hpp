#pragma once

#include "nlohmann/json.hpp"
#include <boost/algorithm/string.hpp>

#include "ISoundObject.hpp"


using namespace FlashBang;

class ISoundContainer : protected ISoundObject, public json
{
public:

	ISoundContainer() = delete;
	~ISoundContainer() = default;

	virtual ISoundContainer& operator=(const json& j) = 0;
	virtual ISoundContainer& operator=(const std::string& s) = 0;
	
	virtual ISoundObject* operator[] (std::string const& s) = 0;
	virtual ISoundObject* operator[] (int i) = 0;

	// todo: deleting sounds at an index or string could be useful
	// void operator delete(void*);
	
	std::vector<std::string> Files;

	static void from_json(const json& j, ISoundContainer& s);

	virtual int AddSoundObject(ISoundObject& soundObject) = 0;
	virtual int AddSoundObject(std::string const& key, ISoundObject& soundObject) = 0;

	virtual void AddSoundObjects(std::vector<ISoundObject*> const& soundObjects) = 0;
	virtual void AddSoundObjects(std::map<std::string, ISoundObject*> const& keysoundObjects) = 0;

	virtual int Next() = 0;
	virtual int Current() = 0;

	virtual ISoundObject* CurrentlyPlaying() = 0;

	/*
	* Queue behavior, can queue like soundobject, but queueNext will automatically queue the next element
	* this is different from PlayBackBehavior which will handle other stuff
	*/
	virtual void Play(int index) = 0;
	virtual void PlayNext() = 0;
	virtual void Queue(int index) = 0;
	virtual void QueueNext() = 0;

	virtual int SwapIndex(int oldIndex, int newIndex) = 0;
	virtual int ShiftIndex(int oldIndex, int newIndex) = 0;

	bool Reverse(bool val = true) { reverse_ = val; return reverse_; }
	bool Reversed() { return reverse_; } // makes more sense thant Reverse()
	bool Reverse() { return Reversed(); } // but included to be consistent with other getters

	// immutable type, no real reason to change type after the fact
	SOUNDCONTAINER_TYPE Type() { return type_; }
	
	// playback is allowed to be changed
	SOUNDCONTAINER_PLAYBACK Playback() { return playback_; }
	SOUNDCONTAINER_PLAYBACK Playback(SOUNDCONTAINER_PLAYBACK val) {
		playback_ = handlePlayback_(val);
		return playback_;
	}

protected:

	// todo, repeat enums to clear up parsing logic
	enum class SOUNDCONTAINER_ARG : char { TYPE='T', PLAYBACK= 'P'};

	bool reverse_ = false;
	int current_ = 0;

	SOUNDCONTAINER_TYPE type_ = SOUNDCONTAINER_TYPE::INDIVIDUAL;
	SOUNDCONTAINER_PLAYBACK playback_ = SOUNDCONTAINER_PLAYBACK::IN_ORDER;

	virtual SOUNDCONTAINER_PLAYBACK handlePlayback_(SOUNDCONTAINER_PLAYBACK val) = 0;

	virtual ISoundObject* createSound_(json const& j) = 0;
	virtual ISoundObject* createSoundContainer_(json const& attr) = 0;

	virtual ISoundObject* createSound_(std::string const& key, json const& j) = 0;
	virtual ISoundObject* createSoundContainer_(std::string const& key, json const& attr) = 0;

	void parseParam_(std::string& key, const json& j) override;
	void parseFile_(std::string& fileKey, const json& j) override;

	bool checkFileString_(std::string& s);

	// atrocious name, but this needs to be clear
	void checkSplitThenAddFileString_(std::string& f);
	
	void parseAddSoundObjectKey_(std::string const& originalKey, int soundObjectIndex);
	void parseSoundObject_(json const& j);
	void parseSoundObject_(std::string const& key, json const& j);

};
