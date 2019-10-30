/***************************************************************************
 * Filename		: KeyEvent.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Contains keypress callback event dispatch handler declarations.
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

#include "Event.h"

namespace Exalted
{
	 
	class KeyEvent : public Event
	{
	public:
		_NODISCARD inline int GetKeyCode() const { return m_KeyCode; }
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(const int keycode) : m_KeyCode(keycode) {}
		KeyEvent() = delete;
	protected:
		int m_KeyCode;
	};
	 
	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const int keycode, const int repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount) {}
		KeyPressedEvent() = delete; 
		_NODISCARD inline int GetRepeatCount() const { return m_RepeatCount; }
		_NODISCARD std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyPressed) 
	private:
		int m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const int keycode) : KeyEvent(keycode) {}
		KeyReleasedEvent() = delete;
		_NODISCARD std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode ;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const int keycode) : KeyEvent(keycode) {}
		KeyTypedEvent() = delete;
		_NODISCARD std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyTyped)
	};
}