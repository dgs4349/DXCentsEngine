#include "SoundContainer.hpp"
#include <random>

using namespace FlashBang;

SoundContainer::SoundContainer()
{
	srand(time(NULL));
}

SoundContainer::SoundContainer(const json& j)
{
	auto container = new SoundContainer();
	from_json(j, *container);
}

SoundContainer::SoundContainer(const std::string& s)
{
	json j = s;
	auto container = new SoundContainer();
	from_json(j, *container);
}

ISoundContainer& SoundContainer::operator=(const json& j)
{
	ISoundContainer* soundContainer = new SoundContainer(j);
	return *soundContainer;
}

ISoundContainer& SoundContainer::operator=(const std::string& s)
{
	ISoundContainer* soundContainer = new SoundContainer(s);
	return *soundContainer;
}

void SoundContainer::Load()
{
	for (auto el : soundObjects_) {
		el->Load();
	}
	Reset();
}

void SoundContainer::Unload()
{
	for (auto el : soundObjects_) el->Unload();
}

void SoundContainer::Reset()
{
	currentQueueOrderIndex_ = reverse_ ? soundObjects_.size() - 1 : 0;
	queueOrder_ = std::vector<int>(soundObjects_.size());
	switch (playback_) {
	case SOUNDCONTAINER_PLAYBACK::IN_ORDER:
		std::iota(std::begin(queueOrder_), std::end(queueOrder_), 0);
		break;
	case SOUNDCONTAINER_PLAYBACK::RANDOM_EACH:
		std::iota(std::begin(queueOrder_), std::end(queueOrder_), 0);
		std::shuffle(
			std::begin(queueOrder_), 
			std::end(queueOrder_), 
			std::default_random_engine());
		break;
	case SOUNDCONTAINER_PLAYBACK::RANDOM:
		std::generate(
			std::begin(queueOrder_), 
			std::end(queueOrder_), 
			randomIndex_
			);
		break;
	case SOUNDCONTAINER_PLAYBACK::RANDOM_OTHER:
		std::generate(
			std::begin(queueOrder_),
			std::end(queueOrder_),
			randomOther_
		);
	}
	orderSet_ = true;
}

SoundObject* SoundContainer::Current()
{
	return soundObjects_[current_];
}

SoundObject* SoundContainer::Next()
{
	return soundObjects_[current_]->Queued();
}

SoundObject* SoundContainer::At(int index)
{
	return soundObjects_[index];
}

SoundObject* SoundContainer::At(std::string const& key)
{
	return soundObjects_[((json)*this)[key]];
}

int SoundContainer::Index(std::string const& key)
{
	auto i = find(key);
	return i == end() ? -1 : ((json)*this).at(key).get<int>();
}

int SoundContainer::CurrentIndex()
{
	return current_;
}

int SoundContainer::NextIndex()
{
	return queueOrder_[(currentQueueOrderIndex_ + (reverse_? 1 : -1)) % queueOrder_.size()];
}

void SoundContainer::PlayChild(int index, bool stopCurrent = false)
{
	if (stopCurrent) {
		soundObjects_[current_]->Stop();
		soundObjects_[index]->Play();
	}
	else {
		soundObjects_[current_]->Queue(soundObjects_[index], true);
	}
}

void SoundContainer::PlayChild(std::string const& key, bool stopCurrent = false)
{
	PlayChild(Index(key), stopCurrent);
}

void SoundContainer::PlayNextChild(bool stopCurrent = false)
{
	PlayChild(NextIndex(), stopCurrent);
}

void SoundContainer::QueueChild(int index, bool finishCurrent = true)
{
	soundObjects_[current_]->Queue(soundObjects_[index], finishCurrent);
}

void SoundContainer::QueueChild(std::string const& key, bool finishCurrent = true)
{
	QueueChild(Index(key), finishCurrent);
}

int SoundContainer::AddSoundObject(SoundObject* soundObject)
{
	soundObject->ConnectStateChangeHook(onCompleteHook);
	soundObjects_.push_back(soundObject);
	orderSet_ = false;
	return soundObjects_.size() - 1;
}

int SoundContainer::AddSoundObject(std::string const& key, SoundObject* soundObject)
{
	soundObject->ConnectStateChangeHook(onCompleteHook);
	soundObjects_.push_back(soundObject);
	int i = soundObjects_.size() - 1;
	parseAddSoundObjectKey_(key, i);
	orderSet_ = false;
	return i;
}

void SoundContainer::AddSoundObjects(std::vector<SoundObject*> const& soundObjects)
{
	for (auto soundObject : soundObjects) AddSoundObject(soundObject);
}

void SoundContainer::AddSoundObjects(std::map<std::string, SoundObject*> const& soundObjectsMap)
{
	for (auto it = soundObjectsMap.begin(); it != soundObjectsMap.end(); ++it) {
		AddSoundObject(it->first, it->second);
	}
}
void SoundContainer::updateCurrentIndex()
{
	currentQueueOrderIndex_ =
		reverse_ ? currentQueueOrderIndex_-- : currentQueueOrderIndex_++;

	if (playback_ == SOUNDCONTAINER_PLAYBACK::IN_ORDER) {
		currentQueueOrderIndex_ %= queueOrder_.size();
	}
	else if (currentQueueOrderIndex_ < 0 || currentQueueOrderIndex_ >= queueOrder_.size()) {
		currentQueueOrderIndex_ = 0;
		Reset();
	}
	else current_ = queueOrder_[currentQueueOrderIndex_];
}

SoundObject* SoundContainer::createSound_(json const& j)
{
	Sound sound = j;
	AddSoundObject(&sound);
	return &sound;
}

SoundObject* SoundContainer::createSoundContainer_(json const& attr)
{
	SoundContainer* soundContainer = new SoundContainer(attr);
	AddSoundObject(soundContainer);
	return soundContainer;
}

SoundObject* SoundContainer::createSound_(std::string const& key, json const& j)
{
	Sound sound = j;
	AddSoundObject(key, &sound);
	return &sound;
}

SoundObject* SoundContainer::createSoundContainer_(std::string const& key, json const& attr)
{
	SoundContainer* soundContainer = new SoundContainer(attr);
	AddSoundObject(key, soundContainer);
	return soundContainer;
}

float SoundContainer::handleVolume_(float val)
{
	soundObjects_[current_]->Volume(val);
	return val;
}

float SoundContainer::handleTune_(float val)
{
	// tune (pitch) will change duration
	//	since these tune properties *should* be global
	//	we can recalculate the total duration of the container
	//	by calculating the effect here

	duration_ = 

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

SOUNDCONTAINER_PLAYBACK SoundContainer::handlePlayback_(SOUNDCONTAINER_PLAYBACK val)
{
	return SOUNDCONTAINER_PLAYBACK();
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

void SoundContainer::handlePlay_()
{
	if (!orderSet_) Reset();
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
	float total = 0.f;
	for (auto soundObject : soundObjects_) total += soundObject->Duration();
	return total;
}

