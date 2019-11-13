/***************************************************************************
 * Filename		: LightingLayer.h
 * Name			: Ori Lazar
 * Date			: 12/11/2019
 * Description	: This layer contains a scene which displays different lighting techniques.
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
	class LightingLayer : public Exalted::Layer
	{
	public:
		LightingLayer();
		virtual ~LightingLayer() = default;
		virtual void OnUpdate(Exalted::Timestep deltaTime) override;
		virtual void OnImGuiRender() override;
		virtual void OnInactiveImGuiRender() override;
		virtual void OnEvent(Exalted::Event& event) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
	private:
		Exalted::Ref<Exalted::Scene> m_SceneManager;
		Exalted::Ref<Exalted::EditorCamera> m_EditorCamera;
		Exalted::Ref<Exalted::UniformBuffer> m_MatUniformBuffer;
		Exalted::Ref<Exalted::UniformBuffer> m_LightUniformBuffer;

		Exalted::Ref<Exalted::Shader> m_ObjectShader;
		Exalted::Ref<Exalted::Shader> m_LightSourceShaderA;

		Exalted::Ref<Exalted::Mesh> m_ObjectMesh;
		Exalted::Ref<Exalted::Mesh> m_LightSourceMesh;

		glm::vec3 m_LightPosition; 
		glm::mat4 m_LightSourceTransformationA;
		glm::vec3 m_LightSourceColorA;
		
		std::vector<glm::vec3> m_ObjectColors; 
		std::vector<glm::mat4> m_Transformations;

		int m_ObjectCount = 25;
		bool m_RotateLight = false;
	};
}