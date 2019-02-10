#include "Logger.h"



std::shared_ptr<spdlog::logger> Logger::GetCurrentConsole()
{
	return GetInstance()->console;
}

Logger::Logger()
{
	console = spdlog::stdout_color_mt("LOG");
	spdlog::set_pattern("[%H:%M:%S] [%^%L%$] [thread %t] %v");
}


Logger::~Logger()
{
}
