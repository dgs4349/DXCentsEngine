#include "CentsAudioHandler.h"

using namespace DirectX;

CentsAudioHandler::CentsAudioHandler()
{
	effects = std::vector<CentsSoundEffect*>();
	managedEffects = std::vector<CentsSoundEffect*>();
}


CentsAudioHandler::~CentsAudioHandler()
{
	if (audEngine)
	{
		audEngine->Suspend();
	}
	for (int i = 0; i < managedEffects.size(); i++) delete managedEffects[i];
}

void CentsAudioHandler::Update(float deltaTime, float totalTime)
{
	 if (!audEngine->Update())
	{
		if (audEngine->IsCriticalError())
		{
			printf("AUDIO DEVICE LOST\n");
		}
	}
	 else {
		 for (int i = 0; i < effects.size(); i++) {
			 effects[i]->Update(deltaTime, totalTime);
		 }
		 for (int i = fades.size() - 1; i >= 0; i--) {
			 if (!fades[i].active) continue;
			 if (fades[i].startTime <= 0.0f) fades[i].startTime = totalTime;
			 
			 printf("fades active: %f\n", totalTime - (fades[i].startTime + fades[i].timeMillis));

			 float desiredVolume = ((fades[i].toVolume - fades[i].fromVolume) / fades[i].timeMillis) * (totalTime - fades[i].startTime);
			 if (desiredVolume >= fades[i].toVolume) {
				 desiredVolume = fades[i].toVolume;
				 fades[i].startTime = 0.0f;
				 fades[i].active = false;
			 }
			 effects[fades[i].effectIndex]->Set(desiredVolume);
		 }
	 }
}

void CentsAudioHandler::Init()
{
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
	#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
	#endif
	audEngine = std::make_unique<AudioEngine>(eflags);

	isSilent = !audEngine->IsAudioDevicePresent();
}

void CentsAudioHandler::Suspend()
{
	audEngine->Suspend();
}

void CentsAudioHandler::Resume()
{
	audEngine->Resume();
}

CentsSoundEffect * CentsAudioHandler::CreateSoundEffect(const wchar_t* location, bool loop)
{
	CentsSoundEffect* effect = new CentsSoundEffect(audEngine.get(), location, loop);
	Add(effect);
	Manage(effect);
	return effect;
}

void CentsAudioHandler::Add(CentsSoundEffect * effect)
{
	effects.push_back(effect);
}

CentsAudioHandler::Fade CentsAudioHandler::CreateFade(CentsSoundEffect * effect, float fromVolume, float toVolume, float timeMillis)
{
	std::vector<CentsSoundEffect*>::iterator it = std::find(effects.begin(), effects.end(), effect);
	int i;
	if (it == effects.end()) {
		effects.push_back(effect);
		i = effects.size() - 1;
	}
	else {
		i = std::distance(effects.begin(), it);
	}
	Fade fade = { i, fades.size(), fromVolume, toVolume, timeMillis, 0.0f, false };
	fades.push_back(fade);
	return fade;
}

CentsAudioHandler::Fade CentsAudioHandler::CreateStartFade(CentsSoundEffect * effect, float fromVolume, float toVolume, float timeMillis)
{
	return StartFade(CreateFade(effect, fromVolume, toVolume, timeMillis));
}

CentsAudioHandler::Fade CentsAudioHandler::StartFade(Fade fade)
{
	fades[fade.fadeIndex].active = true;
	return fade;
}

CentsAudioHandler::Fade CentsAudioHandler::ReplaceFade(Fade oldFade, Fade newFade)
{
	if (oldFade.fadeIndex >= 0 && oldFade.fadeIndex < fades.size()) {
		fades[oldFade.fadeIndex] = newFade;
		newFade.startTime = 0;
		newFade.active = true;
	}
	else StartFade(newFade);
	return newFade;
}

void CentsAudioHandler::Manage(CentsSoundEffect * effect)
{
	managedEffects.push_back(effect);
}
