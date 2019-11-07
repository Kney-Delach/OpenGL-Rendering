/***************************************************************************
 * Filename		: WindowsWindow.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Contains the windows OS specific window setup declarations.
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
#include "Core/Window.h"
#include "Core/Renderer/RenderingContext.h"

#include <GLFW/glfw3.h>

namespace Exalted
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& properties);
		virtual ~WindowsWindow();
		
		void OnUpdate() override;
		void SetVSync(bool enabled) override;

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_WindowData.EventCallback = callback; }
		inline unsigned int GetWindowWidth() const override { return m_WindowData.Properties.Width; }
		inline unsigned int GetWindowHeight() const override { return m_WindowData.Properties.Height; }
		inline bool IsVSync() const override { return m_WindowData.VSync;  }
		inline void* GetNativeWindow() const override { return m_Window; }
	private:
		virtual void Shutdown();
		virtual void Init(const WindowProperties& properties);
		void InitGLFWWindow();
		void SetGLFWConfigurations();
		void SetGLFWCallbacks() const;
	private:
		struct WindowData
		{
			WindowProperties Properties;
			bool VSync;
			EventCallbackFn EventCallback;
		};
		GLFWwindow* m_Window;
		Scope<RenderingContext> m_RenderingContext;
		WindowData m_WindowData;
	};
}