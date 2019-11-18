#include "Sound.h"



Sound::Sound(){}

Sound::~Sound() {}

Sound::Sound(AudioEngine* audioEngineDX, const wchar_t* location)
{
	soundEffect = std::make_unique<DirectX::SoundEffect>(audioEngineDX, location);
	soundEffectInstance = soundEffect->CreateInstance();
}
Sound::Sound(AudioEngine * audioEngineDX, const wchar_t* location, bool loop)
{
	soundEffect = std::make_unique<DirectX::SoundEffect>(audioEngineDX, location);
	soundEffectInstance = soundEffect->CreateInstance();
	Loop = loop;
}

void Sound::Update(float deltaTime, float totalTime)
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

	if (fadeStartTime == 0.0f) {

		fadeStartTime = totalTime;
		fadeEndTime += fadeStartTime;

	}
	if (totalTime < fadeEndTime) {
		Set(map(totalTime, fadeStartTime, fadeEndTime, fadeFrom, fadeTo));
	}

}

void Sound::Start(float totalTime) {
	startTime = totalTime;
	endTime = startTime + (soundEffect->GetSampleDurationMS() / 1000.0f);
	state = Playing;
}

float Sound::map(float in, float inmin, float inmax, float outmin, float outmax)
{
	if (in < inmin) in = inmin;
	if (in > inmax) in = inmax;
	return outmin + ((outmax - outmin) / (inmax - inmin)) * (in - inmin);
}

void Sound::Link(Sound * linkee)
{
	linked = linkee;
	isLinked = true;
}

void Sound::Link(Sound * linkee, bool loop)
{
	linked = linkee;
	linked->SetLoop(loop);
	isLinked = true;
}


void Sound::Play()
{
	soundEffectInstance->Play(Loop);
	state = Starting;
}

void Sound::Play(float volume, float pitch, float pan)
{
	Set(volume, pitch, pan);
	Play();
}

void Sound::Play(float totalTime)
{
	soundEffectInstance->Play(Loop);
	Start(totalTime);
}

void Sound::Play(float volume, float pitch, float pan, float totalTime)
{
	Set(volume, pitch, pan);
	soundEffectInstance->Play(Loop);
	Start(totalTime);
}

void Sound::PlayOnUpdate()
{
	state = DelayStart;
}

void Sound::PlayOnUpdate(float volume, float pitch, float pan)
{
	Set(volume, pitch, pan);
	state = DelayStart;
}

Sound::RTPCParams* Sound::CreateRTPCParams()
{
	rtpcParams.push_back({ nullptr, nullptr, nullptr });
	return &rtpcParams[rtpcParams.size() - 1];
}

void Sound::Bind(float *& param, float * control, float pmin, float pmax, float cmin, float cmax)
{
	rtpcs.push_back({ pmin, pmax, pmin, control, cmin, cmax, cmin });
	bound = true;
	param = &rtpcs[rtpcs.size() -1].pval;
}

void Sound::SetLoop(bool loop)
{
	if (Loop != loop && Playing) {
		if (Loop) SetLoopDelayed = true;
		else soundEffectInstance->Stop(false);
	}
	Loop = loop;
}

void Sound::Stop(bool immediate)
{
	soundEffectInstance->Stop(immediate);
	state = Ready;
	if (isLinked) {
		linked->Stop(immediate);
	}
}

void Sound::Set(float volume, float pitch, float pan, bool setLinked)
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

void Sound::SetRTPCs()
{
	for (RTPCParams params : rtpcParams) {
		if (params.volume != nullptr) soundEffectInstance->SetVolume(*params.volume);
		if (params.pan != nullptr) soundEffectInstance->SetPan(*params.pan);
		if (params.pitch != nullptr) soundEffectInstance->SetPitch(*params.pitch);
	}
}

void Sound::Fade(float from, float to, float timeMillis)
{
	fadeStartTime = 0.0f;
	fadeTo = to;
	fadeFrom = from;
	fadeEndTime = timeMillis;
}

void Sound::Fade(float from, float to, float timeMillis, float startTime)
{
	fadeStartTime = startTime;
	fadeTo = to;
	fadeFrom = from;
	fadeEndTime = startTime + timeMillis;
}

bool Sound::IsReady()
{
	return state == Ready;
}
