#include "CentsAudioHandler.h"

using namespace DirectX;

CentsAudioHandler::CentsAudioHandler()
{
	effects = std::vector<CentsSoundEffect*>();
}


CentsAudioHandler::~CentsAudioHandler()
{
	if (audEngine)
	{
		audEngine->Suspend();
	}
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

void CentsAudioHandler::Add(CentsSoundEffect * effect)
{
	effects.push_back(effect);
}
