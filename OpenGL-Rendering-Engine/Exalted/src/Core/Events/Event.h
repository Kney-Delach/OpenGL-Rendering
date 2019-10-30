/***************************************************************************
 * Filename		: Event.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Contains the implementation for a currently blocking event-dispatch 
 *                system, a future extension fo this engine will include an event bus / queue
 *                to process events as a seperate part of the update stage.
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
#include "expch.h"
#include "Core/Core.h"

//todo: Implement type safety handling in dispatcher. 
//todo: Remove name debugging for distribution builds. 

namespace Exalted
{
	enum class EventType
	{
		None = 0,
		WindowClosed, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseMoved, MouseScrolled,
		MouseButtonPressed, MouseButtonReleased
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication		= BIT(0),
		EventCategoryInput				= BIT(1),
		EventCategoryKeyboard			= BIT(2),
		EventCategoryMouse				= BIT(3),
		EventCategoryMouseButton		= BIT(4)
	};

	class Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual int GetCategoryFlags() const = 0;
		inline bool IsInCategory(EventCategory category) const { return GetCategoryFlags() & category; }
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }
	public:
		bool m_Handled = false;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFunc = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) : m_Event(event) {}
		template<typename T>
		bool Dispatch(EventFunc<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*static_cast<T*>(&m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}


/** Macros for a cleaner implementation in event children implementations. **/
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
					virtual EventType GetEventType() const override { return GetStaticType(); }\
					virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }
}