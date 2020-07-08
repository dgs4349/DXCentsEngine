
#include "SoundEngine.hpp"

#include <set>

#include "ISoundContainer.hpp"

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
//
//void SoundEngine::Update()
//{
//	// this will be called if SoundEngine is running from its own loop
//	Update(0.f);
//}

void SoundEngine::Update(float deltaTime)
{
	if (!dxAudioEngine_->Update())
	{
		if (dxAudioEngine_->IsCriticalError())
		{
			printf("AUDIO DEVICE LOST\n");
			throw std::exception("Critical DirectX::AudioEngine error occured!");
		}
	}

	connectionsManager_->Update(deltaTime);
}

void SoundEngine::Init()
{
	if (!initiated_) {
		DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;

#ifdef DEBUG
		eflags = eflags | DirectX::AudioEngine_Debug;
#endif

		dxAudioEngine_ = std::make_unique<DirectX::AudioEngine>(eflags);

		isSilent_ = !dxAudioEngine_->IsAudioDevicePresent();
		initiated_ = true;
	}
}

void SoundEngine::Suspend() const
{
	dxAudioEngine_->Suspend();
}

void SoundEngine::Resume() const
{
	dxAudioEngine_->Resume();
}

void SoundEngine::PauseAll()
{
	for (const auto [key, val] : activeScenes_) {
		val->Container->Pause();
	}
}

void SoundEngine::ResumeAll()
{
	for (const auto [key, val] : activeScenes_) {
		val->Container->Resume();
	}
}


void SoundEngine::Scene::Add(bool start) {

	if (!initiated_) instance_->Init();


	if (start) 
	{
		Start();
	}
	else if (instance_->scenes_.find(Key) == instance_->scenes_.end()) 
	{
		instance_->scenes_[Key] = std::move(this);
	}

}

void SoundEngine::Scene::Start() {

	if (active_) return;

	if (!initiated_) instance_->Init();

	if (instance_->scenes_.find(Key) == instance_->scenes_.end())
		instance_->scenes_[Key] = std::move(this);

	if (instance_->activeScenes_.find(Key) == instance_->activeScenes_.end())
		instance_->activeScenes_[Key] = std::move(this);

	active_ = true;
}

void SoundEngine::Scene::Remove() {
	if (active_) {
		instance_->activeScenes_.erase(Key);
	}
	instance_->scenes_.erase(Key);
}


void SoundEngine::Scene::Stop(bool remove) {
	if (remove) Remove();
	if (!active_) return;
	
	Container->Stop();
	instance_->activeScenes_.erase(Key);
	active_ = false;
}

SoundEngine::SoundEngine()
{
	connectionsManager_ = SoundConnectionsManager::Get();
}

SoundEngine::~SoundEngine()
{
	if (dxAudioEngine_)
	{
		dxAudioEngine_->Suspend();
		dxAudioEngine_.release();
	}
	
	connectionsManager_->Release();
}

FlashBang::SoundEngine::Scene::Scene(const char* key, SoundContainer* container, bool start) {
	Key = std::string(key);
	Container = std::move(container);
	startCall_ = new StartCallable(this);
	stopCall_ = new StopCallable(this);

	Container->FromStateChange(SOUND_STATE::UNLOADED, startCall_);
	Container->OnStateChange(SOUND_STATE::UNLOADED, stopCall_);
}

FlashBang::SoundEngine::Scene::Scene(std::string key, SoundContainer* container, bool start)
{
	Key = std::move(key);
	Container = std::move(container);
	startCall_ = new StartCallable(this);
	stopCall_ = new StopCallable(this);

	Container->FromStateChange(SOUND_STATE::UNLOADED, startCall_);
	Container->OnStateChange(SOUND_STATE::UNLOADED, stopCall_);
}