#include "CentsSoundEffect.h"



CentsSoundEffect::CentsSoundEffect(){}

CentsSoundEffect::~CentsSoundEffect() {}

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
		// we have to reference the actual rtpcs[i].pval due to references, foreach is a copy
		for (int i = 0; i < rtpcs.size(); i++) {
			RTPC rtpc = rtpcs[i];
			if (rtpc.cval != *rtpc.control) {
				rtpcUpdate = true;
				rtpc.cval = *rtpc.control;
				rtpcs[i].pval = rtpc.pmin + ((rtpc.pmax - rtpc.pmin) / (rtpc.cmax - rtpc.cmin)) * (rtpc.cval - rtpc.cmin);
			}
		}
		if (rtpcUpdate) {
			SetRTPCs();
			rtpcUpdate = false;
			// pitch adjusts length, length is currently uncalculable if adjusted, this may cause bugs
			if (isLinked && soundEffectInstance->GetState() == STOPPED) endTime = totalTime - 1;
		}
		if (!Loop && totalTime > endTime) {
			state = Completed;
			if (SetLoopDelayed) {
				Loop = true;
				Play(startTime);
			}
			else if (isLinked) {
				linked->rtpcs = rtpcs;
				linked->rtpcParams = rtpcParams;
				linked->Play(totalTime);
			}
		}
		break;
	case Completed:
		if(!isLinked) soundEffectInstance->Stop(true);
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
	isLinked = true;
}

void CentsSoundEffect::Link(CentsSoundEffect * linkee, bool loop)
{
	linked = linkee;
	linked->SetLoop(loop);
	isLinked = true;
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

CentsSoundEffect::RTPCParams* CentsSoundEffect::CreateRTPCParams()
{
	rtpcParams.push_back({ nullptr, nullptr, nullptr });
	return &rtpcParams[rtpcParams.size() - 1];
}

void CentsSoundEffect::Bind(float *& param, float * control, float pmin, float pmax, float cmin, float cmax)
{
	rtpcs.push_back({ pmin, pmax, pmin, control, cmin, cmax, cmin });
	bound = true;
	param = &rtpcs[rtpcs.size() -1].pval;
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
	state = Ready;
	if (isLinked) {
		linked->Stop(immediate);
	}
}

void CentsSoundEffect::Set(float volume, float pitch, float pan, bool setLinked)
{
	if (!paramsSetting) { // prevent recusion loop
		soundEffectInstance->SetVolume(volume);
		soundEffectInstance->SetPitch(pitch);
		soundEffectInstance->SetPan(pan);
		paramsSetting = true;
		if (setLinked && isLinked) {
			linked->Set(volume, pitch, pan, setLinked);
		}
		paramsSetting = false;
	}
}

void CentsSoundEffect::SetRTPCs()
{
	for (RTPCParams params : rtpcParams) {
		if (params.volume != nullptr) soundEffectInstance->SetVolume(*params.volume);
		if (params.pan != nullptr) soundEffectInstance->SetPan(*params.pan);
		if (params.pitch != nullptr) soundEffectInstance->SetPitch(*params.pitch);
	}
}

bool CentsSoundEffect::IsReady()
{
	return state == Ready;
}
