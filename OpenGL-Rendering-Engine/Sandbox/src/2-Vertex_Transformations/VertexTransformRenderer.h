/***************************************************************************
 * Filename		: VertexTransformRenderer.h
 * Name			: Ori Lazar
 * Date			: 30/10/2019
 * Description	: Contains the code for rendering vertex transformations to the screen.
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
	class VertexTransformLayer : public Exalted::Layer
	{
	public:
		VertexTransformLayer();
		virtual ~VertexTransformLayer() = default;
		virtual void OnUpdate(Exalted::Timestep deltaTime) override;
		virtual void OnImGuiRender() override;
		virtual void OnInactiveImGuiRender() override;
		virtual void OnEvent(Exalted::Event& event) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		
	private:
		Exalted::Ref<Exalted::Mesh> m_MeshTriangle;
		Exalted::Ref<Exalted::Mesh> m_MeshTriangle2;
		Exalted::Ref<Exalted::Mesh> m_MeshTriangle3;

		Exalted::Ref<Exalted::Mesh> m_MeshQuad;
		Exalted::Ref<Exalted::Shader> m_Shader;
		glm::mat4 m_TriangleTransform;
		glm::mat4 m_TriangleTransform2;
		glm::mat4 m_TriangleTransform3;

		glm::mat4 m_QuadTransform;
		Exalted::OrthographicCamera m_OrthoCamera;
		Exalted::PerspectiveCamera m_PerspCamera;
		bool m_CameraType = true;
		int m_PointSize = 5;
		std::string m_DrawMode = "Current Draw Mode: FILL";
		std::string m_CameraString = "Current Camera: Orthographic";

		glm::vec3 m_OrthoCameraPosition = glm::vec3(0.0f, 0.0f, -1500.0f);
		float m_OrthoCameraRotation = 0.f;

		glm::vec3 m_PerspCameraPosition = glm::vec3(0.0f, 0.0f, -1500.0f);
		float m_PerspectiveFOV = 45.f;

		glm::vec3 m_TrianglePosition = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 m_TriangleScale = glm::vec3(100.f);

		glm::vec3 m_TrianglePosition2 = glm::vec3(-100.f, -100.f, 500.f);
		glm::vec3 m_TriangleScale2 = glm::vec3(100.f);

		glm::vec3 m_TrianglePosition3 = glm::vec3(-200.f, -200.f, 1000.f);
		glm::vec3 m_TriangleScale3 = glm::vec3(100.f);

		glm::vec3 m_QuadPosition = glm::vec3(200.f, 200.f, 0.f);
		glm::vec3 m_QuadScale = glm::vec3(100.f);

	};
}