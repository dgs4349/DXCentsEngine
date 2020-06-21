#include "SoundContainer.hpp"

#include <ctime>
#include <random>
#include <algorithm>

using namespace FlashBang;

SoundContainer::SoundContainer()
{
	srand(randomDevice_());
}

SoundContainer::SoundContainer(const json& j)
{
	auto* container = new SoundContainer();
	from_json(j, *container);
}

SoundContainer::SoundContainer(const std::string& s)
{
	json j = s;
	auto* container = new SoundContainer();
	from_json(j, *container);
}

//TODO: fix incorrect = operator
SoundContainer& SoundContainer::operator=(const json& j)
{
	auto* soundContainer = new SoundContainer(j);
	return *soundContainer;
}

SoundContainer& SoundContainer::operator=(const std::string& s)
{
	auto* soundContainer = new SoundContainer(s);
	return *soundContainer;
}

void SoundContainer::Load()
{
	for (auto* el : soundObjects_) {
		el->Load();
	}
	Reset();
}

void SoundContainer::Unload()
{
	for (auto* el : soundObjects_) el->Unload();
}

void SoundContainer::reset_(const bool resetIndices)
{
	queueOrder_ = std::vector<int>(soundObjects_.size());
	switch (playbackOrder_) {
	case SOUNDCONTAINER_PLAYBACK_ORDER::IN_ORDER:
		if (orderSet_) return; // if order already set and we're in order, no need to continue
		std::iota(std::begin(queueOrder_), std::end(queueOrder_), 0);
		break;
	case SOUNDCONTAINER_PLAYBACK_ORDER::RANDOM:
		std::shuffle(queueOrder_.begin(), queueOrder_.end(), randomDevice_);
		break;
	/*case SOUNDCONTAINER_PLAYBACK_ORDER::RANDOM_EACH:
		std::iota(std::begin(queueOrder_), std::end(queueOrder_), 0);
		std::shuffle(
			std::begin(queueOrder_),
			std::end(queueOrder_),
			randomDevice_);
		break;
	case SOUNDCONTAINER_PLAYBACK_ORDER::RANDOM:
		std::generate(
			std::begin(queueOrder_),
			std::end(queueOrder_),
			RandomIndex
		);
		break;
	case SOUNDCONTAINER_PLAYBACK_ORDER::RANDOM_OTHER:
		std::generate(
			std::begin(queueOrder_),
			std::end(queueOrder_),
			RandomOther
		);*/
	}
	orderSet_ = true;

	if (resetIndices) {
		currentQueueOrderIndex_ = 0;
		currentSoundObjectIndex_ = queueOrder_[0];
	}
}

void FlashBang::SoundContainer::Reverse()
{
	std::reverse(queueOrder_.begin(), queueOrder_.end());
}

SoundObject* SoundContainer::Current()
{
	return soundObjects_[currentSoundObjectIndex_];
}

SoundObject* SoundContainer::Next()
{
	return soundObjects_[NextIndex()];
}

SoundObject* SoundContainer::At(int index)
{
	return soundObjects_[index];
}

SoundObject* SoundContainer::At(std::string const& key)
{
	return soundObjects_[static_cast<json>(*this)[key]];
}

int SoundContainer::Index(std::string const& key)
{
	auto i = find(key);
	return i == end() ? -1 : static_cast<json>(*this).at(key).get<int>();
}

int SoundContainer::CurrentIndex()
{
	return currentSoundObjectIndex_;
}

int SoundContainer::NextIndex()
{
	return queueOrder_[(currentQueueOrderIndex_ + 1) % queueOrder_.size()];
}

void SoundContainer::PlayChild(int index, bool stopCurrent)
{
	if (stopCurrent) {
		soundObjects_[currentSoundObjectIndex_]->Stop();
		soundObjects_[index]->Play();
	}
	else {
		soundObjects_[currentSoundObjectIndex_]->Queue(soundObjects_[index], true);
	}
}

void SoundContainer::PlayChild(std::string const& key, bool stopCurrent)
{
	PlayChild(Index(key), stopCurrent);
}

void SoundContainer::PlayNextChild(bool stopCurrent)
{
	PlayChild(NextIndex(), stopCurrent);
}

void SoundContainer::QueueChild(int index, bool finishCurrent)
{
	soundObjects_[currentSoundObjectIndex_]->Queue(soundObjects_[index], finishCurrent);
}

void SoundContainer::QueueChild(std::string const& key, bool finishCurrent)
{
	QueueChild(Index(key), finishCurrent);
}

int SoundContainer::AddSoundObject(SoundObject* soundObject)
{
	soundObject->OnStateChange(&onCompleteHook_);
	soundObjects_.push_back(soundObject);
	orderSet_ = false;
	return soundObjects_.size() - 1;
}

int SoundContainer::AddSoundObject(std::string const& key, SoundObject* soundObject)
{
	soundObject->OnStateChange(&onCompleteHook_);
	soundObjects_.push_back(soundObject);
	int i = soundObjects_.size() - 1;
	static_cast<json>(*this)[key] = i;
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

void SoundContainer::queueNext_()
{
	if (currentSoundObjectIndex_ == LastIndex()) {
		if (state_ == SOUND_STATE::FINISHING) return; // we're done, don't queue

		// if we're playing in order we're safe to just queue Next() (which is wrapped)
		// otherwise we are a random container, and we must reshuffle the queueOrder container
		else if (playbackOrder_ != SOUNDCONTAINER_PLAYBACK_ORDER::IN_ORDER) {
			reset_(false);
		}
	}
	// isn't this so pretty? it can be faster but why wouldn't you want to look at this?
	Current()->Queue(Next());
}

void SoundContainer::updateCurrentIndex_()
{
	currentQueueOrderIndex_ = (++currentQueueOrderIndex_) % queueOrder_.size();
	currentSoundObjectIndex_ = queueOrder_[currentQueueOrderIndex_];

	// edge case logic handled in queueNext
	if (playbackBehavior_ == SOUNDCONTAINER_PLAYBACK_BEHAVIOR::PLAYLIST) queueNext_();
}

float SoundContainer::handleVolume_(float val)
{
	if (soundObjects_.size() <= 0) return val;
	soundObjects_[currentSoundObjectIndex_]->Volume(val);
	return val;
}

float SoundContainer::handleTune_(float val)
{
	if (soundObjects_.size() <= 0) return val;
	soundObjects_[currentSoundObjectIndex_]->Tune(val);
	getDuration_(val);
	return val;
}

float SoundContainer::handlePan_(float val)
{
	if (soundObjects_.size() < 0) return val;
	soundObjects_[currentSoundObjectIndex_]->Pan(val);
	return val;
}

SOUNDCONTAINER_PLAYBACK_ORDER SoundContainer::handlePlayback_(SOUNDCONTAINER_PLAYBACK_ORDER val)
{
	return SOUNDCONTAINER_PLAYBACK_ORDER();
}

SoundObject* FlashBang::SoundContainer::createSound_(json const& j)
{
	auto* container = new SoundContainer(j);
	AddSoundObject(container);
	return container;
}

SoundObject* FlashBang::SoundContainer::createSoundContainer_(json const& j)
{
	auto* sound = new Sound(j);
	AddSoundObject(sound);
	return sound;
}

void SoundContainer::handlePlay_()
{
	if (!orderSet_) Reset();
	soundObjects_[currentSoundObjectIndex_]->Play();
	if (playbackBehavior_ == SOUNDCONTAINER_PLAYBACK_BEHAVIOR::PLAYLIST) queueNext_();
}

void SoundContainer::handlePause_()
{
	if (soundObjects_.size() <= 0) return;
	soundObjects_[currentSoundObjectIndex_]->Pause();
}

void SoundContainer::handleResume_()
{
	if (soundObjects_.size() <= 0) return;
	soundObjects_[currentSoundObjectIndex_]->Resume();
}

void SoundContainer::handleFinish_()
{
	// nothing, additional logic handled in queueNext_()
}

void SoundContainer::handleStop_()
{
	if (soundObjects_.size() <= 0) return;
	soundObjects_[currentSoundObjectIndex_]->Stop();
	Reset();
}

float SoundContainer::getDuration_() {
	duration_ = 0.f;
	for (auto* s : soundObjects_) {
		if (s->Duration() < LOOP_CAP) duration_ += s->Duration();
		else {
			// max duration of any soundcontainer is 1B seconds, or 32 years fun fact
			// well actually 2B if tuned down but w/e
			duration_ = static_cast<float>(LOOP_CAP);
			break;
		}
	}
	return duration_;
}

float SoundContainer::getDuration_(float tune) {
	// if duration hasn't been calculated do it here
	if (duration_ < 0.f) getDuration_();
	// otherwise, if tune hasn't changed we can assume we've already
	//  calculated duration and it's effects
	else if (tune == tune_) return duration_;

	// otherwise, duration has been calculated, but we need to
	//	still calculate tune effects. Since we're doing this on the whole
	//	container, this may not be terribly accurate
	// additionally, do we need to figure out the difference between tunes?
	return duration_ *= pow(2.f, tune);
}