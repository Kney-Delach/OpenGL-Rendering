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
		Exalted::Ref<Exalted::Shader> m_LightSourceShader;

		Exalted::Ref<Exalted::Mesh> m_ObjectMesh;
		Exalted::Ref<Exalted::Mesh> m_LightSourceMesh;

		std::vector<Exalted::Ref<Exalted::Material>> m_ObjectMaterialS;
		std::vector<glm::mat4> m_ObjectTransformations;

		Exalted::Ref<Exalted::Light> m_LightA;

		int m_ObjectCount = 25;
		bool m_RotateLight = false;
	};
}