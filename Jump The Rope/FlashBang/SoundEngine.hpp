#pragma once

#include <Audio.h>
#include <nlohmann/json.hpp>


#include "FlashBang.hpp"
#include "Effect.hpp"

using nlohmann::json;

using namespace FlashBang;

class FlashBang::SoundEngine
{
public:
	static SoundEngine* Get();
	static SoundEngine* GetOnce();
	static void Release();

	void Update();
	void Update(float deltaTime);

	void Init();
	void Suspend();
	void Resume();


	struct EffectControl
	{
		float* control;
		const std::string& effectKey;
		float min;
		float max;
		Effect::Connection to_connection() { return { control, min, max }; };
	};
	
	void AddEffectControl(EffectControl const& control);
	void AddEffectControls(json const& j);
	void AddEffectControls(std::vector<EffectControl> const& effects);

	void RemoveEffectControl(const std::string& effectKey);
	void RemoveEffectControl(EffectControl const& control);
	void RemoveEffectControls(json const& j);
	void RemoveEffectControls(std::vector<std::string> const& effectKeys);
	void RemoveEffectControls(std::vector<EffectControl> const& effects);

	static EffectControl* GetEffectControl(const std::string& effectKey);

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
	 *	but do we need this? i guesss we could set it to nullptr so it doesnt shift indeces, that could make sense
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

	std::map<std::string, EffectControl*> effectControls_;
	
public:
	SoundEngine(SoundEngine const&) = delete;
	void operator=(SoundEngine const*) = delete;
	
};