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
		virtual void OnEvent(Exalted::Event& event) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
	private:
		Exalted::Ref<Exalted::Mesh> m_MeshTriangle;
		Exalted::Ref<Exalted::Mesh> m_MeshQuad;
		Exalted::Ref<Exalted::Shader> m_ShaderFlat;
		Exalted::Ref<Exalted::Shader> m_ShaderSmooth;
		Exalted::Ref<Exalted::Shader> m_ShaderNoPerspective;
		glm::mat4 m_TriangleTransform;
		glm::mat4 m_QuadTransform;
		Exalted::OrthographicCamera m_SceneCamera;
		int m_PointSize = 5;
		std::string m_DrawMode = "Current Draw Mode: FILL";
		bool m_SmoothInterpolation = true;
		bool m_FlatInterpolation = false;
		bool m_NoPerspInterpolation = false;
		bool m_RecolourTriangle = false;
	};
}