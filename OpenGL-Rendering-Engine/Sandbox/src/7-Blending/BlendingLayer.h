/***************************************************************************
 * Filename		: BlendingLayer.h
 * Name			: Ori Lazar
 * Date			: 02/11/2019
 * Description	: This layer contains a scene displaying object blending.
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
	class BlendingLayer : public Exalted::Layer
	{
	public:
		BlendingLayer();
		virtual ~BlendingLayer() = default;
		virtual void OnUpdate(Exalted::Timestep deltaTime) override;
		virtual void OnImGuiRender() override;
		virtual void OnInactiveImGuiRender() override;
		virtual void OnEvent(Exalted::Event& event) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
	private:
		void OnWindowResize(Exalted::WindowResizeEvent& resizeEvent);
	private:
		Exalted::Ref<Exalted::Mesh> m_MeshCube;
		Exalted::Ref<Exalted::Mesh> m_MeshQuad;
		Exalted::Ref<Exalted::Texture2D> m_CubeTexture;
		Exalted::Ref<Exalted::Texture2D> m_FloorTexture;
		Exalted::Ref<Exalted::Texture2D> m_WindowTexture;
		Exalted::Ref<Exalted::Texture2D> m_StainedWindowTexture;
		Exalted::Ref<Exalted::Texture2D> m_GrassTexture;
		glm::mat4 m_CubeTransform1;
		glm::mat4 m_CubeTransform2;
		std::vector<glm::mat4> m_FloorTransforms;
		std::vector<glm::vec3> m_WindowTransforms;
		glm::mat4 m_GrassTransform; 
		Exalted::Ref<Exalted::Shader> m_Shader;
		Exalted::Ref<Exalted::Shader> m_WindowShader;
		Exalted::Ref<Exalted::Shader> m_GrassShader;
		Exalted::EditorCamera m_EditorCamera;
		bool m_EnableBlending = true;
		bool m_ProcessingMouseMovement = false;
		bool m_MouseMoving = false;
		bool m_ProcessingCameraMovement = true;
		bool m_FirstMouseMovement = true;
		float m_LastMouseX, m_LastMouseY;
	private:
		static const unsigned m_FloorTileCount = 10;
	};
}