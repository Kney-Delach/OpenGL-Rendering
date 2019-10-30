/***************************************************************************
 * Filename		: Log.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Contains a wrapper class declaration for spdlog logging functionality.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#pragma once

#include "Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

//todo: Expose log levels of s_ClientLogger and s_CoreLogger.
//todo: Implement logging system to write to files instead of console.
//todo: Remove logging macros for distribution builds.

namespace Exalted
{
	class Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_AppLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_AppLogger;
	};
}

// Core engine logging macros
#define EX_CORE_TRACE(...)    ::Exalted::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EX_CORE_DEBUG(...)    ::Exalted::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define EX_CORE_INFO(...)     ::Exalted::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EX_CORE_WARN(...)     ::Exalted::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EX_CORE_ERROR(...)    ::Exalted::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EX_CORE_CRITICAL(...) ::Exalted::Log::GetCoreLogger()->critical(__VA_ARGS__)


// Application logging macros
#define EX_TRACE(...)    ::Exalted::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EX_DEBUG(...)    ::Exalted::Log::GetClientLogger()->debug(__VA_ARGS__)
#define EX_INFO(...)     ::Exalted::Log::GetClientLogger()->info(__VA_ARGS__)
#define EX_WARN(...)     ::Exalted::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EX_ERROR(...)    ::Exalted::Log::GetClientLogger()->error(__VA_ARGS__)
#define EX_CRITICAL(...) ::Exalted::Log::GetClientLogger()->critical(__VA_ARGS__)