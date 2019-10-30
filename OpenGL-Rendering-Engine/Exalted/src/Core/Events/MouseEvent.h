/***************************************************************************
 * Filename		: MouseEvent.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Contains mouse callback event dispatch handler declarations.
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

//todo: Fix repeat count bug.

namespace Exalted
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const float x, const float y) : m_MouseX(x), m_MouseY(y) {}
		MouseMovedEvent() = delete;
		_NODISCARD inline float GetX() const { return m_MouseX; }
		_NODISCARD inline float GetY() const { return m_MouseY; }
		_NODISCARD std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	private:
		float m_MouseX, m_MouseY; 
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}
		MouseScrolledEvent() = delete;
		_NODISCARD inline float GetXOffset() const { return m_XOffset; }
		_NODISCARD inline float GetYOffset() const { return m_YOffset; }
		_NODISCARD std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_XOffset, m_YOffset;
	};
	 
	class MouseButtonEvent : public Event
	{
	public:
		_NODISCARD inline int GetMouseButton() const { return m_Button; }
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(const int button) : m_Button(button) {}
		MouseButtonEvent() = delete;
	protected:
		int m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const int button, const int repeatCount) : MouseButtonEvent(button), m_RepeatCount(repeatCount) {}
		MouseButtonPressedEvent() = delete;
		_NODISCARD inline int GetRepeatCount() const { return m_RepeatCount; }
		_NODISCARD std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button << " (" << m_RepeatCount << " Repeats)";
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonPressed)
	private:
		int m_RepeatCount;
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const int button) : MouseButtonEvent(button) {}
		MouseButtonReleasedEvent() = delete;
		_NODISCARD std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}