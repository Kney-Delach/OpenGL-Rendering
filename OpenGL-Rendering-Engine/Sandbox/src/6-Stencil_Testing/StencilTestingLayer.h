/***************************************************************************
 * Filename		: StencilTestingLayer.h
 * Name			: Ori Lazar
 * Date			: 02/11/2019
 * Description	: This layer contains a scene showcasing stencil testing capabilities.
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
	class StencilTestingLayer : public Exalted::Layer
	{
	public:
		StencilTestingLayer();
		virtual ~StencilTestingLayer() = default;
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
		Exalted::Ref<Exalted::Texture2D> m_CubeTexture;
		Exalted::Ref<Exalted::Texture2D> m_CubeTexture2;
		Exalted::Ref<Exalted::Texture2D> m_ChessboardTexture;
		glm::mat4 m_CubeTransform1;
		glm::mat4 m_CubeTransformOutline1;
		glm::mat4 m_CubeTransform2;
		glm::mat4 m_CubeTransformOutline2;
		Exalted::Ref<Exalted::Mesh> m_MeshFloor;
		Exalted::Ref<Exalted::Texture2D> m_FloorTexture;
		std::vector<glm::mat4> m_FloorTransforms;
		Exalted::Ref<Exalted::Shader> m_Shader;
		Exalted::Ref<Exalted::Shader> m_OutlineShader;
		Exalted::Ref<Exalted::Shader> m_ChessboardShader;
		glm::vec4 m_ScissorTestRegion = glm::vec4(0,0,1280,720);
		Exalted::EditorCamera m_EditorCamera;
		bool m_EnableStencilTesting = true;
		bool m_EnableScissorTesting = true;
		bool m_ProcessingMouseMovement = false;
		bool m_MouseMoving = false;
		bool m_ProcessingCameraMovement = true;
		bool m_FirstMouseMovement = true;
		float m_LastMouseX, m_LastMouseY;
	private:
		static const unsigned m_FloorTileCount = 10;
	};
}