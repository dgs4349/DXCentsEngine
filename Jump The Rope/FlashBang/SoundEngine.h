#pragma once

#include <Audio.h>

#include "FlashBang.hpp"
#include "SoundEngine.h"
#include "ISoundObject.hpp"
#include "ISoundContainer.hpp"

using namespace FlashBang;

class SoundEngine
{
public:
	static SoundEngine* Get();
	static void Release();

	void Update();
	void Update(float deltaTime);

	void Init();
	void Suspend();
	void Resume();

	std::unique_ptr<DirectX::SoundEffect> LoadSoundDX(const wchar_t* location);

	/* *
	 * TODO: move these to one container with an id. This id can be used with order to maintain and manage
	 *	positions within array. If changing order is necessary, just swap with element at that position, but
	 *	hopefully the order within sound engine shouldn't be touched
	 *
	 *	but do we need this? i guesss we could set it to nullptr so it doesnt shift indeces, that could make sense
	 *		when a new scene is loaded all objects of that scene should be cleared or changed.
	 *		but then we'd need to find new empty positions in the array
	 *		maybe this should really be a hash then or something
	 *
	 *		does this have a real advantage? Guess it would maintain a sort of cache so vector won't reshuffle as much
	 * */
	void AddSoundObject(ISoundObject* object);
	void AddSoundContainer(ISoundJson* container);
	
private:
	SoundEngine();
	~SoundEngine();

	std::vector<ISoundObject*> _sounds;

	DirectX::AudioEngine _audioEngineDX;
	
public:
	SoundEngine(SoundEngine const&) = delete;
	void operator=(SoundEngine const*) = delete;
};

