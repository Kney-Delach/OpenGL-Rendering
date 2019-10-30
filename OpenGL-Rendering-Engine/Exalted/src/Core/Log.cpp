/***************************************************************************
 * Filename		: Log.cpp
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Contains a wrapper class implementation for spdlog logging functionality.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "expch.h"
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Exalted 
{
	Ref<spdlog::logger> Log::s_CoreLogger;
	Ref<spdlog::logger> Log::s_AppLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$"); // severity | timestamp | logger_name | message

		s_CoreLogger = spdlog::stdout_color_mt("EXALTED");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_AppLogger = spdlog::stdout_color_mt("APP");
		s_AppLogger->set_level(spdlog::level::trace);
	}
}