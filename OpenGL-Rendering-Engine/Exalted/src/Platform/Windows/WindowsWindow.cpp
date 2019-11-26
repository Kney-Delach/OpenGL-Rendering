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
#include "Core/MouseButtonCodes.h"
#include <GLFW/glfw3.h>
#include "Core/KeyCodes.h"

#include "Core/SceneGraph/Scene.h" // for mouse visibility
namespace Exalted
{
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		EX_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Scope<Window> Window::Create(const WindowProperties& properties)
	{
		return CreateScope<WindowsWindow>(properties);
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
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
			glfwTerminate();
		
		EX_CORE_INFO("Shutting Down Windows OS Window");
	}

	void WindowsWindow::Init(const WindowProperties& properties)
	{
		m_WindowData.Properties = WindowProperties(properties);
		EX_CORE_INFO("Creating window {0} ({1}, {2})", properties.Title, properties.Width, properties.Height);
		if (s_GLFWWindowCount == 0)
		{			
			int success = glfwInit();
			EX_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}
		InitGLFWWindow();
		SetGLFWConfigurations();
		SetGLFWCallbacks();
	}

	void WindowsWindow::InitGLFWWindow()
	{
		//glfwGetPrimaryMonitor()
		m_Window = glfwCreateWindow(static_cast<int>(m_WindowData.Properties.Width), static_cast<int>(m_WindowData.Properties.Height), m_WindowData.Properties.Title.c_str(), nullptr, nullptr); // replcae glfwGetPrimaryMonitor with a nullptr to set it to non-fullscreen
		s_GLFWWindowCount++;
		m_RenderingContext = RenderingContext::Create(m_Window);
		m_RenderingContext->Init();
		glfwSetWindowUserPointer(m_Window, &m_WindowData);
		//glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void WindowsWindow::SetGLFWConfigurations()
	{
		SetVSync(false); 
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
					if(key == EX_KEY_F1 || EX_KEY_F2) //todo: remove this after coursework hand-in
						glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					if(key == EX_KEY_ESCAPE)
						glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					
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
			
			if(Scene::s_IsCameraFree)
			{
				switch (action)
				{
					case GLFW_PRESS:
					{
						if(button == EX_MOUSE_BUTTON_2)
						{
							glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
						}
						MouseButtonPressedEvent event(button, 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						if (button == EX_MOUSE_BUTTON_2)
						{
							glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
						}
						MouseButtonReleasedEvent event(button);
						data.EventCallback(event);
						break;
					}
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
