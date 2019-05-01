#include "CentsSoundEffect.h"



CentsSoundEffect::CentsSoundEffect(){}

CentsSoundEffect::~CentsSoundEffect() {}

CentsSoundEffect::CentsSoundEffect(AudioEngine* audEngine, const wchar_t* location)
{
	soundEffect = std::make_unique<DirectX::SoundEffect>(audEngine, location);
	soundEffectInstance = soundEffect->CreateInstance();
	rtpcs = std::vector<RTPC>();
}
CentsSoundEffect::CentsSoundEffect(AudioEngine * audEngine, const wchar_t* location, bool loop)
{
	soundEffect = std::make_unique<DirectX::SoundEffect>(audEngine, location);
	soundEffectInstance = soundEffect->CreateInstance();
	Loop = loop;
	rtpcs = std::vector<RTPC>();
}

void CentsSoundEffect::Update(float deltaTime, float totalTime)
{
	switch (state) {
	case DelayStart:
		Play(totalTime);
		state = Starting;
		// fallthrough
	case Starting:
		Start(totalTime);
		break;
	case Playing:
		// update value, and then set 
		for (RTPC rtpc : rtpcs) {
			if (rtpc.cval != *rtpc.control) {
				rtpcUpdate = true;
				rtpc.cval = *rtpc.control;
				rtpc.pval = rtpc.pmin + ((rtpc.pmax - rtpc.pmin) / (rtpc.cmax - rtpc.cmin)) * (rtpc.cval - rtpc.cmin);
			}
		}
		if (rtpcUpdate) {
			Set(RTPCParameters);
			rtpcUpdate = false;
		}
		if (!Loop && totalTime > endTime) {
			state = Completed;
			if (SetLoopDelayed) {
				Loop = true;
				Play(startTime);
			}
			else if (linked != nullptr) {
				linked->rtpcs = rtpcs;
				linked->Play(totalTime);
			}
		}
		break;
	case Completed:
		if(linked == nullptr) soundEffectInstance->Stop(true);
		state = Ready;
		break;
	}
}

void CentsSoundEffect::Start(float totalTime) {
	startTime = totalTime;
	endTime = startTime + (soundEffect->GetSampleDurationMS() / 1000.0f);
	state = Playing;
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


void CentsSoundEffect::Play()
{
	soundEffectInstance->Play(Loop);
	state = Starting;
}

void CentsSoundEffect::Play(float volume, float pitch, float pan)
{
	Set(volume, pitch, pan);
	Play();
}

void CentsSoundEffect::Play(float totalTime)
{
	soundEffectInstance->Play(Loop);
	Start(totalTime);
}

void CentsSoundEffect::Play(float volume, float pitch, float pan, float totalTime)
{
	Set(volume, pitch, pan);
	soundEffectInstance->Play(Loop);
	Start(totalTime);
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

void CentsSoundEffect::Bind(float ** param, float * control, float pmin, float pmax, float cmin, float cmax)
{
	rtpcs.push_back(RTPC{ param, pmin, pmax, **param, control, cmin, cmax, *control });
	bound = true;
	*param = &rtpcs[rtpcs.size() - 1].pval;
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

void CentsSoundEffect::Set(AudioParams params)
{
	soundEffectInstance->SetVolume(*params.volume);
	soundEffectInstance->SetPitch(*params.pitch);
	soundEffectInstance->SetPan(*params.pan);
}

bool CentsSoundEffect::IsReady()
{
	return state == Ready;
}
