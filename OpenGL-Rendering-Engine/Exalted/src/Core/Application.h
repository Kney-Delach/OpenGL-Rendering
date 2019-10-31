/***************************************************************************
 * Filename		: Application.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Contains the declaration of the singleton application wrapper
 *                for engine based applications. Contains game loop, event handling etc...
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
#include "Window.h"
#include "Core.h"
#include "Core/Layers/LayerStack.h"
#include "Core/Layers/ImGui/ImGuiLayer.h"
#include "Core/Core/Timestep.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

namespace Exalted
{
	class Application
	{
	public:
		Application();
		virtual ~Application() = default;
		inline static Application& Get() { return *s_Instance; }
		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		_NODISCARD inline Window& GetWindow() const { return *m_Window; } 
	private:
		bool OnWindowClosed(WindowClosedEvent& closedEvent);
		bool OnWindowResize(WindowResizeEvent& resizeEvent);
	private:
		static Application* s_Instance;
	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		Timestep m_LastFrameTime;
		bool m_Minimized = false;
	};

	/** This should be defined in the client-side application project. */
	Application* CreateApplication();

	/** Macro used to bind events to application callback functions. */
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
}