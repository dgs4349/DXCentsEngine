#pragma once

#include <iostream>

#define SPDLOG_TRACE_ON
#define SPDLOG_DEBUG_ON

#include "spdlog.h"
#include "sinks\stdout_sinks.h"

class Logger
{
public:

	static Logger* GetInstance(void);
	static void ReleaseInstance(void);

	static std::shared_ptr<spdlog::logger> GetCurrentConsole();

	Logger(Logger const&) = delete;
	void operator=(Logger const&) = delete;

protected:

	Logger();
	~Logger();

private:

	std::shared_ptr<spdlog::logger> console;
};

#if defined( DEBUG ) || defined ( _DEBUG )

#define  LOG_TRACE( ... )    Logger::GetCurrentConsole()->info( __VA_ARGS__ )
#define  LOG_WARNING( ... )     Logger::GetCurrentConsole()->warn( __VA_ARGS__ )
#define  LOG_ERROR( ... )    Logger::GetCurrentConsole()->error( __VA_ARGS__ )

#else

#define  LOG_TRACE( ... ) 
#define  LOG_WARNING( ... )  
#define  LOG_ERROR( ... ) 

#endif