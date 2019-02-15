
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

	new Object();

	getchar();
	ObjectManager::ReleaseInstance();
	Logger::ReleaseInstance();
	getchar();
	return 1;
}