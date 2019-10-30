/***************************************************************************
 * Filename		: Application.cpp
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Contains the implementation of the singleton application wrapper
 *                for engine based applications.
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
#include "Application.h"
#include "Core/Renderer/Renderer.h"

#include <GLFW/glfw3.h>
#include "glad/glad.h" //todo: Abstract me 

namespace Exalted 
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		EX_CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;

		m_Window = Scope<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FN(OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResized));

		/** Iterate through layer stack backwards until the event is handled. */
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			const float time = static_cast<float>(glfwGetTime());
			Timestep deltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;
			
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(deltaTime);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClosed(WindowClosedEvent& closedEvent)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResized(WindowResizeEvent& resizedEvent)
	{
		glViewport(0, 0, resizedEvent.GetWidth(), resizedEvent.GetHeight());

		return false;
	}
}
