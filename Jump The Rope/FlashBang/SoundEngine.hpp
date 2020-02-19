#pragma once

#include <Audio.h>

#include "FlashBang.hpp"

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
	
public:
	SoundEngine(SoundEngine const&) = delete;
	void operator=(SoundEngine const*) = delete;
};