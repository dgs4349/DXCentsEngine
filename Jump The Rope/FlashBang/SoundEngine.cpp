
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
	if (!DirectXAudioEngine->Update())
	{
		if (DirectXAudioEngine->IsCriticalError())
		{
			printf("AUDIO DEVICE LOST\n");
			throw std::exception("Critical DirectX::AudioEngine error occured!");
		}
	}
	if(unregisterTimer_ >= 0.f)
	{
		unregisterTimer_ += deltaTime;
		if(unregisterTimer_ > FreeControlRegisterDelay)
		{
			// TODO: find thread-safe solution to allow non-blocking clear
			const auto it = unregisterCache_.begin();
			
			for(int i = unregisterCache_.size() -1; i >= 0; --i)
			{
				effectControls_.erase(unregisterCache_[i]);
				unregisterCache_.erase(it + i);
			}

			unregisterTimer_ = -1.f;
		}
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

void SoundEngine::Suspend() const
{
	DirectXAudioEngine->Suspend();
}

void SoundEngine::Resume() const
{
	DirectXAudioEngine->Resume();
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

void SoundEngine::RegisterEffectControl(EffectControl const& control)
{
	effectControls_[control.soundKey].push_back(
		std::make_pair(control.effectKey, control)
	);
}

void SoundEngine::RegisterEffectControl(float* controlVarPtr, const char* soundKeyCStr, const char* effectKeyCStr, float controlMin, float controlMax)
{
	EffectControl e = {
		controlVarPtr,
		std::string(soundKeyCStr),
		std::string(effectKeyCStr),
		controlMin,
		controlMax };
	RegisterEffectControl(e);
}

void SoundEngine::RegisterEffectControls(std::vector<EffectControl> const& controls)
{
	for (auto c : controls)
		effectControls_[c.soundKey].push_back(
			std::make_pair(c.effectKey, c));
}

SoundEngine::SoundEngine()
{
	Init();
}

SoundEngine::~SoundEngine()
{
	if (DirectXAudioEngine)
	{
		DirectXAudioEngine->Suspend();
		directXAudioEnginePointer.release();
	}
	/*for (int i = 0; i < sounds_.size(); i++) delete sounds_[i];
	for (auto it = sounds_.begin(); )*/
}
