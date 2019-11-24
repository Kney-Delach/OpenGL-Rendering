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
#include "imgui.h"

namespace Exalted 
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		EX_CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;

		m_Window = Window::Create();
		m_Window->SetEventCallback(EX_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

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
		dispatcher.Dispatch<WindowClosedEvent>(EX_BIND_EVENT_FN(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(EX_BIND_EVENT_FN(Application::OnWindowResize));

		/** Iterate through layer stack backwards until the event is handled. */
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			if((*--it)->IsActive())
			{
				(*it)->OnEvent(e);
				if (e.m_Handled)
					break;
			}
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			if (!m_Minimized)
			{
				const float time = static_cast<float>(glfwGetTime());
				Timestep deltaTime = time - m_LastFrameTime;
				m_LastFrameTime = time;

				for (Layer* layer : m_LayerStack)
				{
					if (layer->IsActive())
						layer->OnUpdate(deltaTime);
				}
			}

			m_ImGuiLayer->Begin();
			ImGui::Begin("Renderer");
			ImGui::Text("Application Runtime: %.2fms", static_cast<float>(m_LastFrameTime));
			ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
			auto& caps = RendererAPI::GetCapabilities();
			ImGui::Text("GPU Vendor: %s", caps.Vendor.c_str());
			ImGui::Text("GPU Renderer: %s", caps.Renderer.c_str());
			ImGui::Text("GPU Driver: %s", caps.Version.c_str());
			ImGui::Text("Maximum Multi-Sample Samples: %i", caps.MaxSamples);
			ImGui::Text("Maximum Anisotropic Filtering: %i", static_cast<int>(caps.MaxAnisotropy));
			ImGui::Text("Maximum Vertex Uniform Components: %i", caps.MaxVertexUniformComponents);
			ImGui::Text("Maximum Uniform Buffer Components: %i", caps.MaxUniformBufferComponents);
			ImGui::End();
			for (Layer* layer : m_LayerStack)
			{
				if (layer->IsActive())
					layer->OnImGuiRender();
				else
					layer->OnInactiveImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClosed(WindowClosedEvent& closedEvent)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& resizeEvent)
	{
		const unsigned width = resizeEvent.GetWidth();
		const unsigned height = resizeEvent.GetHeight();
		if (width == 0 || height == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		RenderCommand::SetViewport(0, 0, width, height);
		return false;
	}
}
