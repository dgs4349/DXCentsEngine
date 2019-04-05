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
			 effects.at(i)->Update(deltaTime, totalTime);
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

void CentsAudioHandler::Manage(CentsSoundEffect * effect)
{
	managedEffects.push_back(effect);
}
