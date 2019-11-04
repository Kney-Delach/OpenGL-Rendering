/***************************************************************************
 * Filename		: Core.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Contains some helpful utility declarations used throughout the engine.
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
#include <memory>

/** Debug macro implementation. */
#ifdef EX_DEBUGGING
	#define EX_ENABLE_ASSERTS
#endif

#ifdef EX_ENABLE_ASSERTS
	#define EX_ASSERT(x, ...) { if(!(x)) { EX_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define EX_CORE_ASSERT(x, ...) { if(!(x)) { EX_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define EX_ASSERT(x, ...)
	#define EX_CORE_ASSERT(x, ...)
#endif

/** bitwise bitfield macro, used for applying multiple categories to a single event type. */
#define BIT(x) (1 << x)

/** Event binding macro with single placeholder. */
#define EX_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

/** Scope and Reference definitions. */
namespace Exalted
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}