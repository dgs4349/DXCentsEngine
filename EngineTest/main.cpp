
#include <iostream>

#include "CentsEngine.h"

#define SOL_CHECK_ARGUMENTS 1
#include <sol.hpp>


void Print(std::string message)
{
	LOG_TRACE("{}", message);
}

void Print2(std::string message)
{
	LOG_TRACE("{}", message);
}


int main()
{
	ObjectManager::GetInstance();

	sol::state lua;
	
	lua.set_function("Print", Print);

	lua.script_file("script.lua");

	getchar();

	ObjectManager::ReleaseInstance();
	return 1;
}