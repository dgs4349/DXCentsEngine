#include "Logger.h"

Logger* Logger::GetInstance(void)
{
	static Logger* instance = nullptr;
	if (instance == nullptr)
	{
		instance = new Logger();
		LOG_TRACE("Created Logger Singleton");
	}
	return instance;
}

void Logger::ReleaseInstance(void)
{
	delete GetInstance();
}

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
	LOG_TRACE("Released Logger Singleton");
}
