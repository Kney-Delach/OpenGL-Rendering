/***************************************************************************
 * Filename		: Layer.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Declaration of a layer interface, for the layered architecture of this engine.
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
#include "Core/Events/Event.h"
#include "Core/Core/Timestep.h"

//todo: Add bool config to control layer active status.

namespace Exalted 
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer", bool startupActive = false);
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep deltaTime) {}
		virtual void OnImGuiRender() {}
		virtual void OnInactiveImGuiRender() {}
		virtual void OnEvent(Event& event) {}
		_NODISCARD inline const bool IsActive() { return m_IsActive;  }
		_NODISCARD inline const std::string& GetName() const { return m_DebugName; } //todo: Remove from distribution build.
	protected:
		std::string m_DebugName;
		bool m_IsActive = true;
	};
}