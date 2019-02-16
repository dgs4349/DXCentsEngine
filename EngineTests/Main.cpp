
#include <stdlib.h>
#include <Windows.h>

#include "EngineCore.h"

int main(void)
{
#if defined( DEBUG ) || defined ( _DEBUG )
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Logger::GetInstance();
	ObjectManager::GetInstance();

	GameObject* gameObjects[1000];

	for (int i = 0; i < 1000; i++)
	{
		gameObjects[i] = new GameObject("Test Object");
	}


	std::string name;
	for (int i = 0; i < 1000; i++)
	{
		name = gameObjects[i]->name;
	}

	getchar();
	ObjectManager::ReleaseInstance();
	Logger::ReleaseInstance();
	getchar();
	return 1;
}