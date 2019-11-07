/***************************************************************************
 * Filename		: EditorShowcaseLayer.h
 * Name			: Ori Lazar
 * Date			: 31/10/2019
 * Description	: Contains a scene showcasing the editor camera. 
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
#include "Exalted.h"

namespace Sandbox
{
	class EditorShowcaseLayer : public Exalted::Layer
	{
	public:
		EditorShowcaseLayer();
		virtual ~EditorShowcaseLayer() = default;
		virtual void OnUpdate(Exalted::Timestep deltaTime) override;
		virtual void OnImGuiRender() override;
		virtual void OnInactiveImGuiRender() override;
		virtual void OnEvent(Exalted::Event& event) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
	private:
		Exalted::Ref<Exalted::Mesh> m_MeshCube;
		Exalted::Ref<Exalted::Shader> m_Shader;
		Exalted::EditorCamera m_EditorCamera;
	};
}