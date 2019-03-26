#include "AudioHandler.h"

using namespace DirectX;

AudioHandler::AudioHandler()
{
}


AudioHandler::~AudioHandler()
{
	if (audEngine)
	{
		audEngine->Suspend();
	}
}

void AudioHandler::Update()
{
	 if (!audEngine->Update())
	{
		if (audEngine->IsCriticalError())
		{
			printf("AUDIO DEVICE LOST\n");
		}
	}
}

void AudioHandler::Init()
{
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
	#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
	#endif
	audEngine = std::make_unique<AudioEngine>(eflags);

	isSilent = !audEngine->IsAudioDevicePresent();
}

void AudioHandler::Suspend()
{
	audEngine->Suspend();
}

void AudioHandler::Resume()
{
	audEngine->Resume();
}

std::unique_ptr<DirectX::SoundEffect> AudioHandler::MakeSound(char * location)
{
	return std::make_unique<SoundEffect>(audEngine.get(), location);
}

void AudioHandler::PlayOnce(SoundEffect* sfx, float volume)
{
	// TODO get and set the time on audio effect, add callbacks
}

void AudioHandler::PlayLoop(SoundEffect* sfx, float volume)
{
	// TODO keep reference to current looping audio
}
