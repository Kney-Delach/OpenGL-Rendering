/***************************************************************************
 * Filename		: ImGuiLayer.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Declaration of a layer for the ImGui processing loop.
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
#include "Core/Layers/Layer.h"

namespace Exalted
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;
		virtual void OnImGuiRender() override;
		virtual void OnInactiveImGuiRender() override { EX_CORE_ASSERT(false, "ImGui layer should always be active!"); }
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}
