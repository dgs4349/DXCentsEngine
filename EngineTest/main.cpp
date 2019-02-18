
#include <iostream>

#include "CentsEngine.h"

int main()
{

	getchar();
	return 1;
}


/*
	extern "C" {
#include "lua\lua.h"
#include "lua\lauxlib.h"
#include "lua\lualib.h"
}

#include <LuaBridge\LuaBridge.h>

	using namespace luabridge;
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	getGlobalNamespace(L).addFunction("printMessage", printMessage);
	luaL_dofile(L, "script.lua");
	lua_pcall(L, 0, 0, 0);
	LuaRef sumNumbers = getGlobal(L, "sumNumbers");
	int result = sumNumbers(5, 4);
	std::cout << "Result:" << result << std::endl;
	system("pause");
	*/