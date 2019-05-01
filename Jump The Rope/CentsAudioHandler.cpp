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
			 if (fades[i].startTime <= 0.0f) fades[i].startTime = totalTime;

			 bool remove = false;
			 float desiredVolume = ((fades[i].toVolume - fades[i].fromVolume) / fades[i].timeMillis) * (totalTime - fades[i].startTime);
			 if (desiredVolume >= fades[i].toVolume) {
				 desiredVolume = fades[i].toVolume;
				 remove = true;
			 }
			 effects[fades[i].index]->Set(desiredVolume);
			 if (remove) fades.erase(fades.begin() + i);
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
	return { i, fromVolume, toVolume, timeMillis, 0.0f };
}

CentsAudioHandler::Fade CentsAudioHandler::AddCreateFade(CentsSoundEffect * effect, float fromVolume, float toVolume, float timeMillis)
{
	Fade fade = CreateFade(effect, fromVolume, toVolume, timeMillis);
	AddFade(fade);
	return fade;
}

void CentsAudioHandler::AddFade(Fade fade)
{
	/*std::vector<Fade>::iterator it = std::find(fades.begin(), fades.end(), fade);
	if (it == fades.end()) {
		fades.push_back(fade);
	}*/
}

CentsAudioHandler::Fade CentsAudioHandler::SetFade(Fade oldFade, Fade newFade)
{
	//std::vector<Fade>::iterator it = std::find(fades.begin(), fades.end(), oldFade);
	//if (it == fades.end()) {
	//	fades.push_back(newFade);
	//}
	//else {
	//	int i = std::distance(fades.begin(), it);
	//	fades[i] = newFade;
	//}
	return Fade{};
}

void CentsAudioHandler::Manage(CentsSoundEffect * effect)
{
	managedEffects.push_back(effect);
}
