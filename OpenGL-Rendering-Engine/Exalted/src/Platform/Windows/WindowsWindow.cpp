/***************************************************************************
 * Filename		: WindowsWindow.cpp
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Contains the windows OS specific window setup implementations.
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
#include "WindowsWindow.h"
#include "Core/Events/ApplicationEvent.h"
#include "Core/Events/MouseEvent.h"
#include "Core/Events/KeyEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>

namespace Exalted
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		EX_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProperties& properties)
	{
		return new WindowsWindow(properties);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& properties)
	{
		Init(properties);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_RenderingContext->SwapBuffers();
	}

	void WindowsWindow::SetVSync(const bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_WindowData.VSync = enabled;
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::Init(const WindowProperties& properties)
	{
		m_WindowData.Properties = WindowProperties(properties);
		EX_CORE_INFO("Creating window {0} ({1}, {2})", properties.Title, properties.Width, properties.Height);
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			EX_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}
		InitGLFWWindow();
		SetGLFWConfigurations();
		SetGLFWCallbacks();
	}

	void WindowsWindow::InitGLFWWindow()
	{
		m_Window = glfwCreateWindow(static_cast<int>(m_WindowData.Properties.Width), static_cast<int>(m_WindowData.Properties.Height), m_WindowData.Properties.Title.c_str(), nullptr, nullptr);
		m_RenderingContext = new OpenGLContext(m_Window);
		m_RenderingContext->Init();
		glfwSetWindowUserPointer(m_Window, &m_WindowData);
	}

	void WindowsWindow::SetGLFWConfigurations()
	{
		SetVSync(false); //todo: Make this configurable
	}

	void WindowsWindow::SetGLFWCallbacks() const
	{
		// ------------------------------ Window ------------------------------

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			windowData.Properties.Width = width;
			windowData.Properties.Height = height;

			WindowResizeEvent event(width, height);
			windowData.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			WindowClosedEvent event;
			windowData.EventCallback(event);
		});

		// ------------------------------ Keyboard ------------------------------

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				windowData.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				windowData.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				windowData.EventCallback(event);
				break;
			}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		// ------------------------------ Mouse ------------------------------

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button, 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
			data.EventCallback(event);
		});
	}
}
