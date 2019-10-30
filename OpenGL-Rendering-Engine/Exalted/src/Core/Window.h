/***************************************************************************
 * Filename		: Window.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Contains an abstract windows interface to be implemented per OS.
 *                Also contains universal window properties.
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
#include "Core/Events/Event.h"

namespace Exalted
{
	struct WindowProperties
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProperties(const std::string& title = "Exalted Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height) {}

		WindowProperties(const WindowProperties& copy) {
			Title = copy.Title;
			Width = copy.Width;
			Height = copy.Height;
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;
		virtual void OnUpdate() = 0;

		virtual unsigned int GetWindowWidth() const = 0;
		virtual unsigned int GetWindowHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		virtual void SetVSync(const bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;
		static Window* Create(const WindowProperties& props = WindowProperties());
	};
}