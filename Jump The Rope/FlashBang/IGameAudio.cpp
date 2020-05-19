#include "IGameAudio.hpp"


void IGameAudio::Init()
{
	soundEngine_->Init();
	for (auto [key, scene] : activeScenes_)
	{
		scene->Container->State(FlashBang::SOUND_STATE::READY);
	}
}


void IGameAudio::Update(float deltaTime)
{
	soundEngine_->Update(deltaTime);
	for (auto [key, scene] : activeScenes_)
	{
		scene->Container->Update(deltaTime);
	}
}
