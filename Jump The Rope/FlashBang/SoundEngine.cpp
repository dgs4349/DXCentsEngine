#include "SoundEngine.h"


SoundEngine* SoundEngine::Get()
{
}

void SoundEngine::Release()
{
}

void SoundEngine::Update()
{
}

void SoundEngine::Update(float deltaTime)
{
}

void SoundEngine::Init()
{
}

void SoundEngine::Suspend()
{
}

void SoundEngine::Resume()
{
}

std::unique_ptr<DirectX::SoundEffect> SoundEngine::LoadSoundDX(const wchar_t* location)
{
	return 
}

void SoundEngine::AddSoundObject(ISoundObject* object)
{
}

void SoundEngine::AddSoundContainer(ISoundJson* container)
{
}

SoundEngine::SoundEngine()
{
	_audioEngineDX = _audioEngineDX.get();

	_sounds = std::vector<ISoundObject*>();
	
}


SoundEngine::~SoundEngine()
{
}
