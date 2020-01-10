#include "SoundEngine.h"
#include <set>

using namespace FlashBang;

SoundEngine* SoundEngine::instance_ = nullptr;
int SoundEngine::refs_ = 0;
bool SoundEngine::initiated_ = false;

SoundEngine* SoundEngine::Get()
{
	if (instance_ == nullptr) {
		instance_ = new SoundEngine();
	}

	addRef_();

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
	Update(0.0f);
}

void SoundEngine::Update(float deltaTime)
{

	if (!audioEngineDX_->Update())
	{
		if (audioEngineDX_->IsCriticalError())
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
	if(!initiated_){
		DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;
	
		#ifdef DEBUG
				eflags = eflags | DirectX::AudioEngine_Debug;
		#endif
	
		audioEngineDXPointer_ = std::make_unique<DirectX::AudioEngine>(eflags);
		audioEngineDX_ = audioEngineDXPointer_.get();

		isSilent_ = !audioEngineDX_->IsAudioDevicePresent();
		initiated_ = true;
	}
}

void SoundEngine::Suspend()
{
	audioEngineDX_->Suspend();
}

void SoundEngine::Resume()
{
	audioEngineDX_->Resume();
}

void SoundEngine::AddSoundObject(ISoundObject* object)
{
}

void SoundEngine::AddSoundContainer(ISoundContainer* container)
{
}

SoundEngine::SoundEngine()
{
	sounds_ = std::multiset<int, ISoundObject*>();
	Init();
}


SoundEngine::~SoundEngine()
{
	if(audioEngineDX_)
	{
		audioEngineDX_->Suspend();
		audioEngineDXPointer_.release();
	}
	for (int i = 0; i < sounds_.size(); i++) delete sounds_[i];
	for (auto it = sounds_.begin(); )
}
