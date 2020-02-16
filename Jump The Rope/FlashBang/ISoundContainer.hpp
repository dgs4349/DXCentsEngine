#pragma once

#include "nlohmann/json.hpp"
#include <boost/algorithm/string.hpp>

#include "SoundObject.hpp"


using namespace FlashBang;

/*
	ISoundContainer
	- handles parsing
	- abstracts basic behaviors
*/

class ISoundContainer : protected SoundObject, public json
{
public:

	ISoundContainer() = delete;
	~ISoundContainer() = default;

	virtual ISoundContainer& operator=(const json& j) = 0;
	virtual ISoundContainer& operator=(const std::string& s) = 0;
	
	virtual SoundObject* operator[] (std::string const& s) = 0;
	virtual SoundObject* operator[] (int i) = 0;

	// todo: deleting sounds at an index or string could be useful
	// void operator delete(void*);
	
	std::vector<std::string> Files;

	static void from_json(const json& j, ISoundContainer& s);

	virtual int AddSoundObject(SoundObject& soundObject) = 0;
	virtual int AddSoundObject(std::string const& key, SoundObject& soundObject) = 0;

	virtual void AddSoundObjects(std::vector<SoundObject*> const& soundObjects) = 0;
	virtual void AddSoundObjects(std::map<std::string, SoundObject*> const& keysoundObjects) = 0;

	virtual SoundObject* Current() = 0;
	virtual SoundObject* Next() = 0;

	virtual int CurrentIndex() = 0;
	virtual int NextIndex() = 0;

	virtual void PlayChild( int index, bool stopCurrent=false ) = 0;
	virtual void PlayChild( std::string& key, bool stopCurrent=false ) = 0;
	virtual void PlayNextChild( bool stopCurrent=false ) = 0;

	virtual void QueueChild( int index, bool finishCurrent=true ) = 0;
	virtual void QueueChild(std::string& key, bool finishCurrent = true ) = 0;
	virtual void QueueNextChild(bool finishCurrent=true ) = 0;

	virtual void StopThenJump(int newPosition, bool makeSkipPermenant = false) = 0;
	virtual void QueueJump(int newPosition, bool finish = true, bool makeSkipPermenant = false) = 0;

	virtual int SwapIndex(int oldIndex, int newIndex) = 0;
	virtual int ShiftIndex(int oldIndex, int newIndex) = 0;

	bool DefaultOrderOnComplete() { return resetOrderOnComplete_; }
	bool DefaultOrderOnComplete(bool val) { 
		resetOrderOnComplete_ = val; 
		return resetOrderOnComplete_; 
	}

	bool Reverse() { return reverse_; }
	bool Reverse(bool val = true) { reverse_ = val; return reverse_; }

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

	int current_ = 0;

	bool reverse_ = false;
	bool resetOrderOnComplete_ = true;

	SOUNDCONTAINER_TYPE type_ = SOUNDCONTAINER_TYPE::INDIVIDUAL;
	SOUNDCONTAINER_PLAYBACK playback_ = SOUNDCONTAINER_PLAYBACK::IN_ORDER;

	virtual SOUNDCONTAINER_PLAYBACK handlePlayback_(SOUNDCONTAINER_PLAYBACK val) = 0;

	virtual SoundObject* createSound_(json const& j) = 0;
	virtual SoundObject* createSoundContainer_(json const& attr) = 0;

	virtual SoundObject* createSound_(std::string const& key, json const& j) = 0;
	virtual SoundObject* createSoundContainer_(std::string const& key, json const& attr) = 0;

	void parseParam_(std::string& key, const json& j) override;
	void parseFile_(std::string& fileKey, const json& j) override;

	bool checkFileString_(std::string& s);

	// atrocious name, but this needs to be clear
	void checkSplitThenAddFileString_(std::string& f);
	
	void parseAddSoundObjectKey_(std::string const& originalKey, int soundObjectIndex);
	void parseSoundObject_(json const& j);
	void parseSoundObject_(std::string const& key, json const& j);

};
