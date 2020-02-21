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
private:
// if we ever want to change arg names, just do it here
// we should also do this in SoundObject to clear up logic
enum class SOUNDCONTAINER_ARG : char { TYPE = 'T', PLAYBACK = 'P' };

public:
	~ISoundContainer() = default;

	virtual ISoundContainer& operator=(const json& j) = 0;
	virtual ISoundContainer& operator=(const std::string& s) = 0;
	
	virtual SoundObject* operator[] (std::string const& key) = 0;
	virtual SoundObject* operator[] (int i) = 0;

	virtual bool operator== (ISoundContainer const& other) { return Files == other.Files; }
	virtual bool operator!= (ISoundContainer const& other) { return !(*this == other); }
	
	// todo: deleting sounds at an index or string could be useful
	// void operator delete(void*);
	
	std::vector<std::string> Files;

	static void from_json(const json& j, ISoundContainer& s);

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

	virtual void PlayChild( int index, bool stopCurrent=false ) = 0;
	virtual void PlayChild( std::string const& key, bool stopCurrent=false ) = 0;
	virtual void PlayNextChild( bool stopCurrent=false ) = 0;

	virtual void QueueChild( int index, bool finishCurrent=true ) = 0;
	virtual void QueueChild(std::string const& key, bool finishCurrent = true ) = 0;

	// immutable type, no real reason to change type after the fact
	SOUNDCONTAINER_TYPE Type() { return type_; }
	
	// playback is allowed to be changed
	SOUNDCONTAINER_PLAYBACK Playback() { return playback_; }
	SOUNDCONTAINER_PLAYBACK Playback(SOUNDCONTAINER_PLAYBACK val) {
		playback_ = handlePlayback_(val);
		return playback_;
	}

	virtual void Reset() = 0;
	virtual void Reverse() = 0;

protected:

	ISoundContainer() = default;

	int current_ = 0;
	bool orderSet_ = false;

	SOUNDCONTAINER_TYPE type_ = SOUNDCONTAINER_TYPE::ONE_SHOT;
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

private:
		
	// SoundContainer should NEVER be compared as a SoundObject
	virtual bool operator!=(const SoundObject& other) override { return false; }
	virtual bool operator==(const SoundObject& other) override { return false; }


};
