#pragma once

#include <Audio.h>
#include <nlohmann/json.hpp>

#include "FlashBang_Fwd.hpp"

#include "SoundContainer.hpp"
#include "SoundConnectionsManager.hpp"

using nlohmann::json;

using namespace FlashBang;


/*
 * TODO:
 *	move dependencies of soundengine effect registering in sounds and sounds
 *		container to sound engine on remove
 * 
 */


class FlashBang::SoundEngine
{
public:

	/////////////////// Objects ///////////////

	/*
	 * SoundContainer update container with automatic load/unload detection
	 */
	class Scene
	{
		friend class SoundEngine;
		
	public:

		Scene() = default;
		
		class StartCallable : public ICallable {
			Scene* s_;
		public:
			StartCallable(Scene* s) { s_ = s; }
			void operator()() override { s_->Start(); }
		};

		class StopCallable : public ICallable {
			Scene* s_;
		public:
			StopCallable(Scene* s) { s_ = s; }
			void operator()() override { s_->Stop(); }
		};

		std::string Key;
		SoundContainer* Container;

		Scene(std::string key, SoundContainer* container);
		~Scene() { delete startCall_; delete stopCall_; }

		void Start() { instance_->StartScene(this); }
		void Stop() { instance_->StopScene(this); }

	private:
		StartCallable* startCall_;
		StopCallable* stopCall_;

		bool active_ = false;
	};


	// GetContainer, GetScene, callable struct namespaces
	
	struct GetContainer
	{
		SoundContainer& operator[](std::string const& key)
		const { return *(instance_->scenes_[key]->Container); }


		/*
			it looks like all scenes are recieving shallow copies of containers
		
		*/

		SoundContainer& operator[](const char* key)
		const { 
			auto& scene = instance_->scenes_[key];
			auto* container = scene->Container;
			return *container;

			//return *(instance_->scenes_[std::string(key)]->Container); 
		}
	};

	struct GetScene
	{
		Scene& operator[](std::string const& key)
		const { return *(instance_->scenes_[key]); }

		Scene& operator[](const char* key)
		const { return *(instance_->scenes_[std::string(key)]); }
	};

	static inline GetContainer Containers = {};
	static inline GetScene Scenes = {};


	/////////////// Methods /////////////////
	
	static SoundEngine* Get();
	static SoundEngine* GetOnce();

	void Release();

	void Update(float deltaTime);

	void Init();
	void Suspend() const;
	void Resume() const;


	static void SystemSuspend() { instance_->Suspend(); }
	static void SystemResume() { instance_->Resume(); }

	void PauseAll();
	void ResumeAll();

	Scene* AddScene(Scene* scene, bool start = true);
	Scene* RemoveScene(std::string const& sceneKey);
	
	Scene& StartScene(Scene* scene);
	Scene& StopScene(Scene* scene, bool remove = true);
	

	/* copy from soundconnections manager, TODO: find a better way instead of forwarding*/
	void RegisterEffectControl(EffectControl const& control) const { connectionsManager_->RegisterEffectControl(control); }
	void RegisterEffectControl(
		float* controlVarPtr, const char* soundKeyCStr, 
		const char* effectKeyCStr, float controlMin, float controlMax)
	const
	{
		connectionsManager_->RegisterEffectControl(
			controlVarPtr, soundKeyCStr, effectKeyCStr,
			controlMin, controlMax
		);
	}
	void RegisterEffectControls(std::vector<EffectControl> const& controls) const { connectionsManager_->RegisterEffectControls(controls); }


protected:
	SoundEngine();
	~SoundEngine();

	SoundConnectionsManager* connectionsManager_;
	
	// rename to add _
	std::unique_ptr<DirectX::AudioEngine> dxAudioEngine_;
	bool isSilent_ = false;
	
	static SoundEngine* instance_;

	static bool initiated_;

	static int refs_;
	static void addRef_() { ++refs_; }
	static void releaseRef_() { --refs_; }

	std::unordered_map<std::string, Scene*> scenes_;
	std::map<std::string, Scene*> activeScenes_;
	
public:
	SoundEngine(SoundEngine const&) = delete;
	SoundEngine& operator=(SoundEngine const*) = delete;
};
