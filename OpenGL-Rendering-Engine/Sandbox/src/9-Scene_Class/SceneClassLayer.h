/***************************************************************************
 * Filename		: SceneClassLayer.h
 * Name			: Ori Lazar
 * Date			: 03/11/2019
 * Description	: This layer contains a scene which utilizes this engines
				  scene storage structure.
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
	class SceneClassLayer : public Exalted::Layer
	{
	public:
		SceneClassLayer();
		virtual ~SceneClassLayer() = default;
		virtual void OnUpdate(Exalted::Timestep deltaTime) override;
		virtual void OnImGuiRender() override;
		virtual void OnInactiveImGuiRender() override;
		virtual void OnEvent(Exalted::Event& event) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
	private:
		void OnWindowResize(Exalted::WindowResizeEvent& resizeEvent);
	private:
		Exalted::Ref<Exalted::Scene> m_SceneManager;
		Exalted::Ref<Exalted::GameObject> m_SceneRoot;
		Exalted::Ref<Exalted::EditorCamera> m_EditorCamera;
		Exalted::GameObject* m_pBoxObject;
		Exalted::Ref<Exalted::Texture2D> m_StainedWindowTexture;
		Exalted::Ref<Exalted::Texture2D> m_FloorTexture;
		Exalted::Ref<Exalted::Mesh> m_MeshQuad;
		Exalted::Ref<Exalted::Shader> m_Shader;
		Exalted::Ref<Exalted::Shader> m_WindowShader;
		std::vector<glm::mat4> m_FloorTransforms;

		bool m_ProcessingMouseMovement = false;
		bool m_MouseMoving = false;
		bool m_ProcessingCameraMovement = true;
		bool m_FirstMouseMovement = true;
		float m_LastMouseX, m_LastMouseY;
	private:
		static const unsigned m_FloorTileCount = 10;
	};
}