#include "SoundEngine.hpp"
#include <set>

using namespace FlashBang;

SoundEngine* SoundEngine::instance_ = nullptr;
int SoundEngine::refs_ = 0;
bool SoundEngine::initiated_ = false;

SoundEngine* SoundEngine::Get()
{
	addRef_();

	return GetOnce();
}

SoundEngine* SoundEngine::GetOnce()
{
	if (instance_ == nullptr) {
		instance_ = new SoundEngine();
	}

	return instance_;
}

void SoundEngine::Release()
{
	releaseRef_();

	if ((refs_ == 0) && (instance_ != nullptr)) {
		delete instance_;
		instance_ = nullptr;
	}
}

void SoundEngine::Update()
{
	// this will be called if SoundEngine is running from its own loop
	Update(0.f);
}

void SoundEngine::Update(float deltaTime)
{
	if (!DirectXAudioEngine->Update())
	{
		if (DirectXAudioEngine->IsCriticalError())
		{
			printf("AUDIO DEVICE LOST\n");
			throw std::exception("Critical DirectX::AudioEngine error occured!");
		}
	}
	else
	{
	}
}

void SoundEngine::Init()
{
	if (!initiated_) {
		DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;

#ifdef DEBUG
		eflags = eflags | DirectX::AudioEngine_Debug;
#endif

		directXAudioEnginePointer = std::make_unique<DirectX::AudioEngine>(eflags);
		DirectXAudioEngine = directXAudioEnginePointer.get();

		isSilent_ = !DirectXAudioEngine->IsAudioDevicePresent();
		initiated_ = true;
	}
}

void SoundEngine::Suspend()
{
	DirectXAudioEngine->Suspend();
}

void SoundEngine::Resume()
{
	DirectXAudioEngine->Resume();
}

void SoundEngine::AddSoundContainer(ISoundContainer* container)
{
}

SoundEngine::SoundEngine()
{
	sounds_ = std::multiset<int, SoundObject*>();
	Init();
}

SoundEngine::~SoundEngine()
{
	if (DirectXAudioEngine)
	{
		DirectXAudioEngine->Suspend();
		directXAudioEnginePointer.release();
	}
	for (int i = 0; i < sounds_.size(); i++) delete sounds_[i];
	for (auto it = sounds_.begin(); )
}