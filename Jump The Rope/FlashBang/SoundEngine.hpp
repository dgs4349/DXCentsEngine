#pragma once

#include <Audio.h>
#include <nlohmann/json.hpp>

#include "FlashBang_Fwd.hpp"

#include "SoundContainer.hpp"
#include "Effect.hpp"

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
	static SoundEngine* Get();
	static SoundEngine* GetOnce();

	void Release();

	void Update(float deltaTime);

	void Init();
	void Suspend() const;
	void Resume() const;

	class Scene
	{
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

		Scene(std::string key, SoundContainer* container)
		{
			Key = std::move(key);
			Container = container;

			startCall_ = new StartCallable(this);
			stopCall_ = new StopCallable(this);

			Container->FromStateChange(SOUND_STATE::UNLOADED, startCall_);
			Container->OnStateChange(SOUND_STATE::UNLOADED, stopCall_);
		}
		~Scene() { delete startCall_; delete stopCall_; }

		void Start() { instance_->StartScene(this); }
		void Stop() { instance_->StopScene(this); }

	private:
		StartCallable* startCall_;
		StopCallable* stopCall_;

		bool active_ = false;
	};
	
	struct GetContainer
	{
		SoundContainer& operator[](std::string const& key)
		const { return *(instance_->scenes_[key]->Container); }

		SoundContainer& operator[](const char* key)
		const { return *(instance_->scenes_[std::string(key)]->Container); }
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

	static void SystemSuspend() { instance_->Suspend(); }
	static void SystemResume() { instance_->Resume(); }

	void PauseAll();
	void ResumeAll();

	Scene* AddScene(Scene* scene, bool start = true);
	Scene* RemoveScene(std::string const& sceneKey);
	
	Scene& StartScene(Scene* scene);
	Scene& StopScene(Scene* scene, bool remove = true);

	bool FreeControlRegisterOnUnload = true;
	float FreeControlRegisterDelay = 100.f;
	void FreeControlRegister() { unregisterTimer_ = 0.f; }

	// TODO: from_json
	// TODO: move to smart pointer
	struct EffectControl
	{
		float* controlVarPtr;
		std::string soundKey;
		std::string effectKey;
		float controlMin = 0.f;
		float controlMax = 0.f;
		Effect::Connection to_connection() const
		{
			return { controlVarPtr, controlMin, controlMin };
		};
	};

	// todo: investigate a url-type path for soundContainer.../sound/effect for easy traversal

	void RegisterEffectControl(EffectControl const& control);
	void RegisterEffectControl(float* controlVarPtr, const char* soundKeyCStr, const char* effectKeyCStr, float controlMin, float controlMax);
	void RegisterEffectControls(std::vector<EffectControl> const& controls);
	

	static float StaticRegisterEffectControl(float initialValue, EffectControl const& control)
	{
		instance_->RegisterEffectControl(control);
		return initialValue;
	}
	
	// TODO: void RegisterEffectControls(const json& j);

	/*
	 * I am okay with game-side logic managing instances of SoundEngine
	 * I am not comfortable with Sounds doing so, simply too many calls to too many instances
	 * For logic that Sounds need to call, these should be static
	 *
	 * TODO: move copy-args to smart pointers
	 */
	static void QueueUnregisterEffectControls(const std::string& soundKey) {
		instance_->unregisterCache_.push_back(soundKey);
		if (instance_->FreeControlRegisterOnUnload)  instance_->unregisterTimer_ = 0.f;
	}
	static void QueueUnregisterEffectControls(const std::vector<std::string> soundKeys) {
		for (auto e : soundKeys)  instance_->unregisterCache_.push_back(e);
		if (instance_->FreeControlRegisterOnUnload)  instance_->unregisterTimer_ = 0.f;
	}
	
	static std::vector<std::pair<std::string, EffectControl>>&
		GetEffectControls(std::string const& soundKey)
		{
			return instance_->effectControls_[soundKey];
		}

	static std::unique_ptr<DirectX::SoundEffect> LoadSoundDX(const wchar_t* location)
	{
		if (!initiated_) GetOnce();
		return std::make_unique<DirectX::SoundEffect>(instance_->DirectXAudioEngine, location);
	}

	/* *
	 * TODO: move these to one container with an id. This id can be used with index to maintain and manage
	 *	positions within array. If changing index is necessary, just swap with element at that position, but
	 *	hopefully the index within sound engine shouldn't be touched
	 *
	 *	but do we need this? i guess we could set it to nullptr so it doesnt shift indeces, that could make sense
	 *		when a new scene is loaded all objects of that scene should be cleared or changed.
	 *		but then we'd need to find new empty positions in the array
	 *		maybe this should really be a hash then or something
	 *
	 *		does this have a real advantage? Guess it would maintain a sort of cache so vector won't reshuffle as much
	 * */

	DirectX::AudioEngine* DirectXAudioEngine;

protected:
	SoundEngine();
	~SoundEngine();

	// rename to add _
	std::unique_ptr<DirectX::AudioEngine> directXAudioEnginePointer;
	bool isSilent_ = false;

	static SoundEngine* instance_;

	static bool initiated_;

	static int refs_;
	static void addRef_() { ++refs_; }
	static void releaseRef_() { --refs_; }

	// list of effect key-value pairs, assigned to a sound key
	// soundKey: [effectKey: {effect...}, ...]
	std::unordered_map<std::string,
						std::vector<
						std::pair<std::string, EffectControl>>> effectControls_;

	std::vector<const std::string> unregisterCache_;
	float unregisterTimer_ = -1.f;

	std::unordered_map<std::string, Scene*> scenes_;
	std::map<std::string, Scene*> activeScenes_;
	
public:
	SoundEngine(SoundEngine const&) = delete;
	SoundEngine& operator=(SoundEngine const*) = delete;
};
