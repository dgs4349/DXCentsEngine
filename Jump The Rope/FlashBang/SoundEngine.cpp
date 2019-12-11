#include "SoundEngine.h"

using namespace DirectX;


SoundEngine* SoundEngine::instance = nullptr;
int SoundEngine::refs = 0;

SoundEngine::SoundEngine()
{
	effects = std::vector<Sound*>();
	managedEffects = std::vector<Sound*>();
}

SoundEngine::~SoundEngine()
{
	if (audioEngineDX)
	{
		audioEngineDX->Suspend();
	}
	for (int i = 0; i < managedEffects.size(); i++) delete managedEffects[i];

	instance = nullptr;
}

SoundEngine* SoundEngine::Get()
{
	if (instance == nullptr) {
		instance = new SoundEngine();
	}

	addRef();

	return instance;
}

void SoundEngine::Release() {
	releaseRef();

	if ((refs == 0) && (instance != nullptr)) {
		delete instance;
		instance = nullptr;
	}
}

void SoundEngine::Update(float deltaTime, float totalTime)
{
	 if (!audioEngineDX->Update())
	{
		if (audioEngineDX->IsCriticalError())
		{
			printf("AUDIO DEVICE LOST\n");
		}
	}
	 else {
		 for (int i = 0; i < effects.size(); i++) {
			 effects[i]->Update(deltaTime, totalTime);
		 }
	 }
}

void SoundEngine::Init()
{
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
	#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
	#endif
	audioEngineDX = std::make_unique<AudioEngine>(eflags);

	isSilent = !audioEngineDX->IsAudioDevicePresent();
}

void SoundEngine::Suspend()
{
	audioEngineDX->Suspend();
}

void SoundEngine::Resume()
{
	audioEngineDX->Resume();
}

Sound * SoundEngine::CreateSound(const wchar_t* location, bool loop)
{
	Sound* effect = new Sound(audioEngineDX.get(), location, loop);
	Add(effect);
	Manage(effect);
	return effect;
}

void SoundEngine::Add(Sound * effect)
{
	effects.push_back(effect);
}

void SoundEngine::Manage(Sound * effect)
{
	managedEffects.push_back(effect);
}
