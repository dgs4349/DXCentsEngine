#include "OldSound.h"



OldSound::OldSound(){}

OldSound::~OldSound() {}

OldSound::OldSound(AudioEngine* audioEngineDX, const wchar_t* location)
{
	soundEffect = std::make_unique<DirectX::OldSoundEffect>(audioEngineDX, location);
	soundEffectInstance = soundEffect->CreateInstance();
}
OldSound::OldSound(AudioEngine * audioEngineDX, const wchar_t* location, bool loop)
{
	soundEffect = std::make_unique<DirectX::OldSoundEffect>(audioEngineDX, location);
	soundEffectInstance = soundEffect->CreateInstance();
	Loop = loop;
}

void OldSound::Update(float deltaTime, float totalTime)
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
				rtpcs[i].pval = map(rtpc.cval, rtpc.cmin, rtpc.cmax, rtpc.pmin, rtpc.pmax);
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

	if (fadeStartTime == 0.f) {

		fadeStartTime = totalTime;
		fadeEndTime += fadeStartTime;

	}
	if (totalTime < fadeEndTime) {
		Set(map(totalTime, fadeStartTime, fadeEndTime, fadeFrom, fadeTo));
	}

}

void OldSound::Start(float totalTime) {
	startTime = totalTime;
	endTime = startTime + (soundEffect->GetSampleDurationMS() / 1000.f);
	state = Playing;
}

float OldSound::map(float in, float inmin, float inmax, float outmin, float outmax)
{
	if (in < inmin) in = inmin;
	if (in > inmax) in = inmax;
	return outmin + ((outmax - outmin) / (inmax - inmin)) * (in - inmin);
}

void OldSound::Link(OldSound * linkee)
{
	linked = linkee;
	isLinked = true;
}

void OldSound::Link(OldSound * linkee, bool loop)
{
	linked = linkee;
	linked->SetLoop(loop);
	isLinked = true;
}


void OldSound::Play()
{
	soundEffectInstance->Play(Loop);
	state = Starting;
}

void OldSound::Play(float volume, float pitch, float pan)
{
	Set(volume, pitch, pan);
	Play();
}

void OldSound::Play(float totalTime)
{
	soundEffectInstance->Play(Loop);
	Start(totalTime);
}

void OldSound::Play(float volume, float pitch, float pan, float totalTime)
{
	Set(volume, pitch, pan);
	soundEffectInstance->Play(Loop);
	Start(totalTime);
}

void OldSound::PlayOnUpdate()
{
	state = DelayStart;
}

void OldSound::PlayOnUpdate(float volume, float pitch, float pan)
{
	Set(volume, pitch, pan);
	state = DelayStart;
}

OldSound::RTPCParams* OldSound::CreateRTPCParams()
{
	rtpcParams.push_back({ nullptr, nullptr, nullptr });
	return &rtpcParams[rtpcParams.size() - 1];
}

void OldSound::Bind(float *& param, float * control, float pmin, float pmax, float cmin, float cmax)
{
	rtpcs.push_back({ pmin, pmax, pmin, control, cmin, cmax, cmin });
	bound = true;
	param = &rtpcs[rtpcs.size() -1].pval;
}

void OldSound::SetLoop(bool loop)
{
	if (Loop != loop && Playing) {
		if (Loop) SetLoopDelayed = true;
		else soundEffectInstance->Stop(false);
	}
	Loop = loop;
}

void OldSound::Stop(bool immediate)
{
	soundEffectInstance->Stop(immediate);
	state = Ready;
	if (isLinked) {
		linked->Stop(immediate);
	}
}

void OldSound::Set(float volume, float pitch, float pan, bool setLinked)
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

void OldSound::SetRTPCs()
{
	for (RTPCParams params : rtpcParams) {
		if (params.volume != nullptr) soundEffectInstance->SetVolume(*params.volume);
		if (params.pan != nullptr) soundEffectInstance->SetPan(*params.pan);
		if (params.pitch != nullptr) soundEffectInstance->SetPitch(*params.pitch);
	}
}

void OldSound::Fade(float from, float to, float timeMillis)
{
	fadeStartTime = 0.f;
	fadeTo = to;
	fadeFrom = from;
	fadeEndTime = timeMillis;
}

void OldSound::Fade(float from, float to, float timeMillis, float startTime)
{
	fadeStartTime = startTime;
	fadeTo = to;
	fadeFrom = from;
	fadeEndTime = startTime + timeMillis;
}

bool OldSound::IsReady()
{
	return state == Ready;
}
