
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

SoundEngine::Scene* SoundEngine::AddScene(Scene* scene, bool start)
{
	scenes_.at(scene->Key) = scene;
	if (start) scene->Start();
	return scene;
}

SoundEngine::Scene* SoundEngine::RemoveScene(std::string const& sceneKey)
{
	auto *const scene = scenes_[sceneKey];
	if(scene->active_)
	{
		activeScenes_.erase(scene->Key);
	}
	scenes_.erase(scene->Key);
	return scene;
}

SoundEngine::Scene& SoundEngine::StartScene(Scene* scene)
{
	if (!initiated_) Init();
	if (scene->active_) return *scene;

	if (scenes_.find(scene->Key) == scenes_.end())
	{
		scenes_[scene->Key] = scene;
	}

	activeScenes_[scene->Key] = scene;
	scene->active_ = true;
	
	return *scene;
}

SoundEngine::Scene& SoundEngine::StopScene(Scene* scene, bool remove)
{
	if(scene->active_) activeScenes_.erase(scene->Key);
	scene->active_ = false;
	if (remove) scenes_.erase(scene->Key);
	return *scene;
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

FlashBang::SoundEngine::Scene::Scene(std::string key, SoundContainer* container)
{
	Key = std::move(key);
	Container = container;

	startCall_ = new StartCallable(this);
	stopCall_ = new StopCallable(this);

	Container->FromStateChange(SOUND_STATE::UNLOADED, startCall_);
	Container->OnStateChange(SOUND_STATE::UNLOADED, stopCall_);
}