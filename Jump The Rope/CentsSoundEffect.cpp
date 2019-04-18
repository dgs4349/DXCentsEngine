#include "CentsSoundEffect.h"



CentsSoundEffect::CentsSoundEffect()
{
}

CentsSoundEffect::CentsSoundEffect(AudioEngine* audEngine, const wchar_t* location)
{
	soundEffect = std::make_unique<DirectX::SoundEffect>(audEngine, location);
	soundEffectInstance = soundEffect->CreateInstance();
}

CentsSoundEffect::CentsSoundEffect(AudioEngine * audEngine, const wchar_t* location, bool loop)
{
	soundEffect = std::make_unique<DirectX::SoundEffect>(audEngine, location);
	soundEffectInstance = soundEffect->CreateInstance();
	Loop = loop;
}


CentsSoundEffect::~CentsSoundEffect()
{
}

void CentsSoundEffect::Update(float deltaTime, float totalTime)
{
	if (state == DelayStart) {
		Play(totalTime);
		state = Playing;
		endTime = startTime + (soundEffect->GetSampleDurationMS() / 1000.0f);
	}
	if (state == Starting) {
		startTime = totalTime;
		state = Playing;
		endTime = startTime + (soundEffect->GetSampleDurationMS() / 1000.0f);
	}
	if (state == Playing && !Loop) {
		if (totalTime > endTime) {
			state = Completed;
			if (SetLoopDelayed) {
				Loop = true;
				Play(startTime);
			}
			else if (linked != nullptr) {
				linked->Play(totalTime);
			}
		}
	}
}

void CentsSoundEffect::Link(CentsSoundEffect * linkee)
{
	linked = linkee;
}

void CentsSoundEffect::Link(CentsSoundEffect * linkee, bool loop)
{
	linked = linkee;
	linked->SetLoop(loop);
}

void CentsSoundEffect::Play(float volume, float pitch, float pan)
{
	Set(volume, pitch, pan);
	soundEffectInstance->Play(Loop);
}

void CentsSoundEffect::Play(float totaltime)
{
	soundEffectInstance->Play(Loop);
	state = Playing;
	startTime = totaltime;
}

void CentsSoundEffect::Play(float volume, float pitch, float pan, float totaltime)
{
	Set(volume, pitch, pan);
	Play(totaltime);
}

void CentsSoundEffect::Play()
{
	soundEffectInstance->Play(Loop);
	state = Playing;
}

void CentsSoundEffect::PlayOnUpdate()
{
	state = DelayStart;
}

void CentsSoundEffect::PlayOnUpdate(float volume, float pitch, float pan)
{
	Set(volume, pitch, pan);
	state = DelayStart;
}

void CentsSoundEffect::SetLoop(bool loop)
{
	if (Loop != loop && Playing) {
		if (Loop) SetLoopDelayed = true;
		else soundEffectInstance->Stop(false);
	}
	Loop = loop;
}

void CentsSoundEffect::Stop(bool immediate)
{
	soundEffectInstance->Stop(immediate);
	if (linked != nullptr) linked->Stop(immediate);
}

void CentsSoundEffect::Set(float volume, float pitch, float pan, bool setLinked)
{
	if (!paramsSetting) { // prevent recusion loop
		soundEffectInstance->SetVolume(volume);
		soundEffectInstance->SetPitch(pitch);
		soundEffectInstance->SetPan(pan);
		paramsSetting = true;
		if (setLinked && linked != nullptr) {
			linked->Set(volume, pitch, pan, setLinked);
		}
		paramsSetting = false;
	}
}

bool CentsSoundEffect::IsComplete()
{
	return state == Completed;
}
