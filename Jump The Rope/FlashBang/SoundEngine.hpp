#pragma once

#include <Audio.h>
#include <nlohmann/json.hpp>

#include "FlashBang_Fwd.hpp"
#include "Effect.hpp"

using nlohmann::json;

using namespace FlashBang;

class FlashBang::SoundEngine
{
public:
	static SoundEngine* Get();
	static SoundEngine* GetOnce();
	static void Release();

	void Update(float deltaTime);

	void Init();
	void Suspend();
	void Resume();

	bool FreeControlRegisterOnUnload = true;
	float FreeControlRegisterDelay = 100.f;
	void FreeControlRegister() { unregisterTimer_ = 0.f; }

	// TODO: from_json
	// TODO: move to smart pointer
	struct EffectControl
	{
		float* controlVarPtr;
		std::string& soundKey;
		std::string& effectKey;
		float controlMin = 0.f;
		float controlMax = 0.f;
		Effect::Connection to_connection() const
		{
			return { controlVarPtr, controlMin, controlMin };
		};
		
	};

	// todo: investigate a url-type path for soundContainer.../sound/effect for easy traversal

	void RegisterEffectControl(EffectControl const& control);
	void RegisterEffectControls(std::vector<EffectControl> const& controls);
	
	// TODO: void RegisterEffectControls(const json& j);

	/*
	 * I am okay with game-side logic managing instances of SoundEngine
	 * I am not comfortable with Sounds doing so, simply too many calls to too many instances
	 * For logic that Sounds need to call, these should be static
	 *
	 * TODO: move copy-args to smart pointers
	 */
	static void QueueUnregisterEffectControls(const std::string soundKey) {
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
private:
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

	// soundKey: [effectKey: {effect...}, ...]
	std::unordered_map<std::string,
						std::vector<
						std::pair<std::string, EffectControl>>> effectControls_;

	std::vector<const std::string> unregisterCache_;
	float unregisterTimer_ = -1.f;
	
public:
	SoundEngine(SoundEngine const&) = delete;
	void operator=(SoundEngine const*) = delete;
	
};
