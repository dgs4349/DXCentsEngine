#include "Sound.h"

using namespace FlashBang;

::Sound::~Sound()
{
	unload_();
}

// todo: figure out how we manage memory deallocation or use smart pointers
// todo: sound update logic

Sound::Sound(const json& j) {
	from_json(j, *this);
}

Sound::Sound(const std::string& s) {
	json j = s;
	from_json(j, *this);
}

ISoundObject& Sound::operator=(const json& j)
{
	Sound* sound = new Sound(j);
	return  *sound;
}

ISoundObject& Sound::operator=(const std::string& s)
{
	Sound* sound = new Sound(s);
	return *sound;
}


void Sound::updateSound_(float dt)
{
	//enum class SOUND_STATE { UNLOADED, IDLE, PAUSED, QUEUED, READY, PLAYING, FINISHING };

	// IIRC, soundEffect/Instance doesn't always catch playing/stopped/pausing accurately
	//  so it's important to track this ourselves to keep audio as seemless as possible
	//  but this can be tested and verified later during unit testing

	if (state_ < SOUND_STATE::READY) return;

	switch (state_) {

	case SOUND_STATE::READY:

		State(SOUND_STATE::PLAYING);

		Play();

		// fall through

	case SOUND_STATE::PLAYING:

		elapsedTime_ += dt;

		if (elapsedTime_ > duration_) {

			if (currentLoop_ < loop_) {

				++currentLoop_;
			}
			else if (currentLoop_ == loop_) {

				DirectXSoundEffectInstance->Stop(false);
				State(SOUND_STATE::FINISHING);
			}

			// reset elapsed time 0 + overlap
			elapsedTime_ = elapsedTime_ - duration_;
		}
		break;

	case SOUND_STATE::FINISHING:

		elapsedTime_ += dt;

		if (elapsedTime_ > duration_) {

			State(SOUND_STATE::IDLE);
			// auto unload here?

			if (afteree_ != nullptr) {
				afteree_->CopyParams(*this);

				// won't there be a potential jitter if this also has to wait a frame
				afteree_->Play();
			}
		}

	}

}

void Sound::updateEffects_(float dt)
{
	if (state_ == SOUND_STATE::PLAYING || state_ == SOUND_STATE::FINISHING) {
		for (auto it = Effects.begin(); it != Effects.end(); ++it) it->second->Update(dt);
	}
}

void ::Sound::Play()
{
	updateEffects_(0.f); // do a quick rtpc update before play logic

	// if state was set to playing, or there is no following sound, play immediately 
	if (state_ == SOUND_STATE::PLAYING || afteree_ == nullptr) {
		DirectXSoundEffectInstance->Play(loop_ > 0);
		currentLoop_ = 0;
		elapsedTime_ = 0;
		State(SOUND_STATE::PLAYING);
	}
	else {
		// if we have a sound effect queued after (afteree), wait a frame to sync up this is to prevent 
		//		a delay longer than a single frame (which might be noticable) when playing a following sound
		// Sound will be set ready, the update loop will automatically set state to playing and therefore
		//		start playing the sound on the synced frame
		State(SOUND_STATE::READY);
	}
}

void ::Sound::Pause()
{
	DirectXSoundEffectInstance->Pause();
	State(SOUND_STATE::PAUSED);
}

void ::Sound::Resume()
{
	updateEffects_(0.f); // call a quick update in case something changed
	DirectXSoundEffectInstance->Resume();
	State(SOUND_STATE::PLAYING);
}

void Sound::Finish()
{
	DirectXSoundEffectInstance->Stop(false);
	State(SOUND_STATE::FINISHING);
}

void ::Sound::Stop()
{
	DirectXSoundEffectInstance->Stop(true);
	State(SOUND_STATE::IDLE);
}

void ::Sound::Load()
{
	// should we check for '|' characters?
	
	// should we keep File as a wstring?
	const wchar_t* file_str = std::wstring(File.begin(), File.end()).c_str();
	DirectXSoundEffect = SoundEngine::LoadSoundDX(file_str);

	// note: we can add flags here such as NoPitch or ReverbUseFilters
	DirectXSoundEffectInstance = DirectXSoundEffect->CreateInstance();

	// initiate all params on dx sound effects
	handleVolume_(volume_);
	handleTune_(tune_);
	handlePan_(pan_);
	handleLoop_(loop_);

	State(SOUND_STATE::IDLE);
}

float Sound::handleVolume_(float val)
{
	DirectXSoundEffectInstance->SetVolume(val);
	return val;
}

float Sound::handleTune_(float val)
{
	DirectXSoundEffectInstance->SetPitch(val);

	// pitches up to 1 octave(duration*2) or down one octave (duration * 0.5)
	duration_ = DirectXSoundEffect->GetSampleDuration * pow(2, val);
	return val;
}

float Sound::handlePan_(float val)
{
	DirectXSoundEffectInstance->SetPan(val);
	return val;
}

int Sound::handleLoop_(int val){
	if (val < 0) {
		return INT_MAX;
	}
	return val;
}

SOUND_STATE Sound::handleState_(SOUND_STATE state)
{
	return state;
}


ISoundObject* Sound::Queue(bool finish = false)
{
	if (queuer_ == nullptr) {
		throw std::exception("Cannot implicitly call Queue(): no 'previous' ISoundObject provided to queue this Sound");
	}
	else queuer_->After(this, finish);
	return queuer_;
}

ISoundObject* Sound::Queue(ISoundObject* previous, bool finish = false)
{
	queuer_ = previous;
	queuer_->After(this, finish);
	return queuer_;
}

ISoundObject* Sound::After(bool finish = false)
{
	if (afteree_ == nullptr) {
		throw std::exception("Cannot implicitly call After(): no 'next' ISoundObject provided to call.");
	}
	else if (finish) State(SOUND_STATE::FINISHING);
	return afteree_;
}

ISoundObject* Sound::After(ISoundObject* next, bool finish = false)
{
	afteree_ = next;
	return After(finish);
}

void Sound::unload_() 
{
	if (DirectXSoundEffectInstance->GetState() != DirectX::STOPPED) {
		DirectXSoundEffectInstance->Stop(true);
	}
	DirectXSoundEffectInstance.release();
	DirectXSoundEffect.release();
	State(SOUND_STATE::UNLOADED);
}
