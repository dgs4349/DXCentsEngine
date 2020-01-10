#include "FlashBang.hpp"
#include "OldSoundEngine.h"

using namespace FlashBang;


OldSoundEngine* OldSoundEngine::instance = nullptr;
int OldSoundEngine::refs = 0;

OldSoundEngine::OldSoundEngine()
{
	effects = std::vector<Sound*>();
	managedEffects = std::vector<Sound*>();
	audioEngineDX = audioEngineDX.get();
}

OldSoundEngine::~OldSoundEngine()
{
	if (audioEngineDX)
	{
		audioEngineDX->Suspend();
	}
	for (int i = 0; i < managedEffects.size(); i++) delete managedEffects[i];

	instance = nullptr;
}

OldSoundEngine* OldSoundEngine::Get()
{
	if (instance == nullptr) {
		instance = new OldSoundEngine();
	}

	addRef();

	return instance;
}

void OldSoundEngine::Release() {
	releaseRef();

	if ((refs == 0) && (instance != nullptr)) {
		delete instance;
		instance = nullptr;
	}
}

void OldSoundEngine::Update(float deltaTime, float totalTime)
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

void OldSoundEngine::Init()
{
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
	#ifdef %!%DEBUG
	eflags = eflags | AudioEngine_Debug;
	#endif
	audioEngineDX = std::make_unique<AudioEngine>(eflags);

	isSilent = !audioEngineDX->IsAudioDevicePresent();
}

void OldSoundEngine::Suspend()
{
	audioEngineDX->Suspend();
}

void OldSoundEngine::Resume()
{
	audioEngineDX->Resume();
}

void OldSoundEngine::AddSoundObject(ISoundObject * object)
{
}

Sound * OldSoundEngine::CreateSound(const wchar_t* location, bool loop)
{
	Sound* effect = new Sound(audioEngineDX.get(), location, loop);
	Add(effect);
	Manage(effect);
	return effect;
}

void OldSoundEngine::Add(Sound * effect)
{
	effects.push_back(effect);
}

void OldSoundEngine::Manage(Sound * effect)
{
	managedEffects.push_back(effect);
}
