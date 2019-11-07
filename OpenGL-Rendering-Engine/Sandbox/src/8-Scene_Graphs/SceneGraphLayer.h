/***************************************************************************
 * Filename		: SceneGraphLayer.h
 * Name			: Ori Lazar
 * Date			: 03/11/2019
 * Description	: This layer contains a scene which utilizes this engines 
                  scene graph storage structure.
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
	class SceneGraphLayer : public Exalted::Layer
	{
	public:
		SceneGraphLayer();
		virtual ~SceneGraphLayer() = default;
		virtual void OnUpdate(Exalted::Timestep deltaTime) override;
		virtual void OnImGuiRender() override;
		virtual void OnInactiveImGuiRender() override;
		virtual void OnEvent(Exalted::Event& event) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
	private:
		void OnWindowResize(Exalted::WindowResizeEvent& resizeEvent);
	private:
		Exalted::Ref<Exalted::GameObject> m_SceneRoot; 
		Exalted::EditorCamera m_EditorCamera;
		Exalted::GameObject* m_pBoxObject;
		Exalted::Ref<Exalted::Texture2D> m_FloorTexture;
		Exalted::Ref<Exalted::Mesh> m_MeshQuad;
		Exalted::Ref<Exalted::Shader> m_Shader;
		std::vector<glm::mat4> m_FloorTransforms;
	private:
		static const unsigned m_FloorTileCount = 10;
	};
}