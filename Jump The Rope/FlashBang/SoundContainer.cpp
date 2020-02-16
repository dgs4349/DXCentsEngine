#include "SoundContainer.hpp"

using namespace FlashBang;

float SoundContainer::handleVolume_(float val)
{
	soundObjects_[current_]->Volume(val);
	return val;
}

float SoundContainer::handleTune_(float val)
{
	soundObjects_[current_]->Tune(val);
	return val;
}

float SoundContainer::handlePan_(float val)
{
	soundObjects_[current_]->Pan(val);
	return val;
}

int SoundContainer::handleLoop_(int val)
{
	// sets the current loop of the whole sound container
	return val;
}

void SoundContainer::updateEffects_(float dt)
{
	for (auto it = Effects.begin(); it != Effects.end(); ++it) {
		it->second->Update(dt);
	}
}

SOUNDCONTAINER_PLAYBACK SoundContainer::handlePlayback_(SOUNDCONTAINER_PLAYBACK val)
{
	return SOUNDCONTAINER_PLAYBACK();
}

SoundContainer::SoundContainer(const json& j)
{
}

SoundContainer::SoundContainer(const std::string& s)
{
}

ISoundContainer& SoundContainer::operator=(const json& j)
{
	ISoundContainer* soundContainer = new SoundContainer();
	from_json(j, *soundContainer);
	return *soundContainer;
}

ISoundContainer& SoundContainer::operator=(const std::string& s)
{
	json const& j = s;
	ISoundContainer& soundContainer = j;
	// todo constructors with json and string
	return soundContainer;
}

int SoundContainer::AddSoundObject(SoundObject& soundObject)
{
	soundObjects_.push_back(&soundObject);
	return soundObjects_.size() - 1;
}

int SoundContainer::AddSoundObject(std::string const& key, SoundObject& soundObject)
{
	soundObjects_.push_back(&soundObject);
	auto i = soundObjects_.size() - 1;
	(json)*this[key] = i;
	return i;
}

void SoundContainer::AddSoundObjects(std::vector<SoundObject*> const& soundObjects)
{
	for (auto sound : soundObjects) {

	}
}

void SoundContainer::AddSoundObjects(std::map<std::string, SoundObject*> const& keysoundObjects)
{
	return 0;
}

int SoundContainer::Next()
{
	soundObjects_[++current_]->Play();
	return current_;
}

int SoundContainer::Current()
{
	return current_;
}

SoundObject* SoundContainer::CurrentlyPlaying()
{
	return soundObjects_[current_];
}

void SoundContainer::Play(int index)
{
	current_ = index;
	soundObjects_[current_]->Play();
}

void SoundContainer::PlayNext()
{
	Next();
}

void SoundContainer::Queue(int index)
{
}

void SoundContainer::QueueNext()
{
	soundObjects_[current_]->After(soundObjects_[current_++]);
}

int SoundContainer::SwapIndex(int oldIndex, int newIndex)
{
	// fix
	soundObjects_.swap(oldIndex, newIndex);
	return newIndex;
}

int SoundContainer::ShiftIndex(int oldIndex, int newIndex)
{
	return 0;
}


SoundContainer::~SoundContainer()
{
	unload_();
}

void SoundContainer::Play()
{
	soundObjects_[current_]->Play();
	if (type_ == SOUNDCONTAINER_TYPE::PLAYLIST) {
		QueueNext();
	}
}

void SoundContainer::Pause()
{
	soundObjects_[current_]->Pause();
}

void SoundContainer::Resume()
{
	soundObjects_[current_]->Resume();
}

void SoundContainer::Stop()
{
	soundObjects_[current_]->Stop();
}

void SoundContainer::Load()
{
	for (auto el : soundObjects_) {
		el->Load();
	}
}

void SoundContainer::Unload()
{
	unload_();
}

void SoundContainer::unload_()
{
	for (auto el : soundObjects_) el->Unload();
}

SoundObject* SoundContainer::createSound_(json const& j)
{
	// also have to handle indeces and what have you
}

SoundObject* SoundContainer::createSoundContainer_(json const& attr)
{
}

SoundObject* SoundContainer::createSound_(std::string const& key, json const& j)
{
}

SoundObject* SoundContainer::createSoundContainer_(std::string const& key, json const& attr)
{
}

void SoundContainer::handlePlay_()
{
}

void SoundContainer::handlePause_()
{
}

void SoundContainer::handleResume_()
{
}

void SoundContainer::handleFinish_()
{
}

void SoundContainer::handleStop_()
{
}

float SoundContainer::getDuration_()
{
	return 0.0f;
}

SoundObject* SoundContainer::Current()
{
	return nullptr;
}

SoundObject* SoundContainer::Next()
{
	return nullptr;
}

int SoundContainer::CurrentIndex()
{
	return 0;
}

int SoundContainer::NextIndex()
{
	return 0;
}

void SoundContainer::PlayChild(int index, bool stopCurrent = false)
{
}

void SoundContainer::PlayChild(std::string& key, bool stopCurrent = false)
{
}

void SoundContainer::PlayNextChild(bool stopCurrent = false)
{
}

void SoundContainer::QueueChild(int index, bool finishCurrent = true)
{
}

void SoundContainer::QueueChild(std::string& key, bool finishCurrent = true)
{
}

void SoundContainer::QueueNextChild(bool finishCurrent = true)
{
}

void SoundContainer::StopThenJump(int newPosition, bool makeSkipPermenant = false)
{
}

void SoundContainer::QueueJump(int newPosition, bool finish = true, bool makeSkipPermenant = false)
{
}

