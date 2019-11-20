#pragma once

#include "Exalted.h"

namespace Sandbox
{
	class EnvironmentSceneLayer : public Exalted::Layer
	{
	public:
		EnvironmentSceneLayer();
		virtual ~EnvironmentSceneLayer() = default;
		virtual void OnUpdate(Exalted::Timestep deltaTime) override;
		virtual void OnImGuiRender() override;
		virtual void OnInactiveImGuiRender() override;
		virtual void OnEvent(Exalted::Event& event) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
	private:
		Exalted::Ref<Exalted::Scene> m_SceneManager;
		Exalted::Ref<Exalted::EditorCamera> m_EditorCamera;
		
		Exalted::Ref<Exalted::UniformBuffer> m_LightUniformBuffer;

		std::vector<Exalted::Ref<Exalted::PointLight>> m_PointLights;
		std::vector<Exalted::Ref<Exalted::GameTransform>> m_PointLightTransforms;

		Exalted::Ref<Exalted::Shader> m_LightSourceShader;
		Exalted::Ref<Exalted::Mesh> m_LightSourceMesh;
		
		//Exalted::Ref<Exalted::LightManager> dynamicLightManager;
		//Exalted::Ref<Exalted::LightManager> staticLightManager;
	};
}