#pragma once

#include "SoundEngine.hpp"
#include "SoundContainer.hpp"

class IGameAudio
{
public:

	// todo: add plural methods
	
	IGameAudio() { soundEngine_ = FlashBang::SoundEngine::Get();}
	~IGameAudio() { soundEngine_->Release(); }

	class Scene
	{
	public:
		friend class IGameAudio;
		
		Scene(std::string key, SoundContainer* container)
			{ Key = key; Container = container; }
		std::string Key;
		SoundContainer* Container;

	private:
		bool active_ = false;
	};

	void Init()
	{
		soundEngine_->Init();
		for (auto [key, scene] : activeScenes_)
		{
			scene->Container->State(FlashBang::SOUND_STATE::READY);
		}
	}
	
	void Update(float deltaTime)
	{
		soundEngine_->Update(deltaTime);
		for (auto [key, scene] : activeScenes_)
		{
			scene->Container->Update(deltaTime);
		}
		
	}

	
	void SystemSuspend() const { soundEngine_->Suspend(); }
	void SystemResume() const { soundEngine_->Resume(); }

	void PauseAll(){ for (auto [key, scene] : activeScenes_) scene->Container->Pause(); }
	void ResumeAll() { for (auto [key, scene] : activeScenes_) scene->Container->Resume(); }

	void LoadScene(std::string& sceneKey, bool activate = true)
	{
		scenes_[sceneKey]->Container->Load();
		if (start) StartScene(sceneKey);
	}

	void UnloadScene(std::string& sceneKey)
	{
		
	}
	
	void ActivateScene(std::string& sceneKey)
	{
		
	}

	void DeactivateScene(std::string& sceneKey);


	void AddScene(Scene* scene, bool activate = true)
	{
		scenes_[scene->Key] = scene;
		if (scene->active_) activeScenes_[scene->Key] = scene;
	}


	void RemoveScene(std::string sceneKey)
	{
		if (scenes_[sceneKey]->active_) activeScenes_.erase(sceneKey);
		scenes_.erase(sceneKey);
	}

	
protected:
	FlashBang::SoundEngine* soundEngine_;

	std::map<std::string, Scene*> scenes_;
	std::map<std::string, Scene*> activeScenes_;
	
};
