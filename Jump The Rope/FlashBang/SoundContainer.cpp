#include "SoundContainer.h"

using namespace FlashBang;


void SoundContainer::Finish()
{
	if (soundObjects_[current_]->Playing()) soundObjects_[current_]->Finish();
}

ISoundObject* SoundContainer::Queue(bool finish = false)
{
	// queue some shit
	return nullptr;
}

ISoundObject* SoundContainer::Queue(ISoundObject* previous, bool finish = false)
{
	return nullptr;
}

ISoundObject* SoundContainer::After(bool finish = false)
{
	return nullptr;
}

ISoundObject* SoundContainer::After(ISoundObject* next, bool finish = false)
{
	if (soundObjects_[current_]->Playing()) soundObjects_[current_]->After(next, finish);
	// else?
}

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

SOUND_STATE SoundContainer::handleState_(SOUND_STATE state)
{
	return state;
}

void SoundContainer::updateSound_(float dt)
{
	if (state_ < SOUND_STATE::READY) return;

	switch (state_) {

	case SOUND_STATE::READY:

		



		// update the sound effect
		soundObjects_[current_]->Update(dt);

		// sound effect should queue the next sound automatically on end
		// but we need to update our logic here to keep track
		if (!soundObjects_[current_]->Playing()) {

			reverse_ ? ++currentQueueOrderIndex_ : --currentQueueOrderIndex_;

			if (abs(currentQueueOrderIndex_) >= queueOrder_.size()) {
				loop_++;
				if (currentLoop_ == loop_) {

				}
			}

			current_ = currentQueueOrderIndex_;
		}
	}

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

int SoundContainer::AddSoundObject(ISoundObject& soundObject)
{
	soundObjects_.push_back(&soundObject);
	return soundObjects_.size() - 1;
}

int SoundContainer::AddSoundObject(std::string const& key, ISoundObject& soundObject)
{
	soundObjects_.push_back(&soundObject);
	auto i = soundObjects_.size() - 1;
	(json)*this[key] = i;
	return i;
}

void SoundContainer::AddSoundObjects(std::vector<ISoundObject*> const& soundObjects)
{
	for (auto sound : soundObjects) {

	}
}

void SoundContainer::AddSoundObjects(std::map<std::string, ISoundObject*> const& keysoundObjects)
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

ISoundObject* SoundContainer::CurrentlyPlaying()
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

ISoundObject* SoundContainer::createSound_(json const& j)
{
	// also have to handle indeces and what have you
}

ISoundObject* SoundContainer::createSoundContainer_(json const& attr)
{
}

ISoundObject* SoundContainer::createSound_(std::string const& key, json const& j)
{
}

ISoundObject* SoundContainer::createSoundContainer_(std::string const& key, json const& attr)
{
}

