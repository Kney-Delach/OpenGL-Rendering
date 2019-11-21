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
		
		//////////////////////////////////////////////
		/////////////////// lights ///////////////////
		//////////////////////////////////////////////
		// directional
		Exalted::Ref<Exalted::DirectionalLight> m_DirectionalLight;
		// point
		std::vector<Exalted::Ref<Exalted::PointLight>> m_PointLights;
		std::vector<Exalted::Ref<Exalted::GameTransform>> m_PointLightTransforms;
		// spot
		std::vector<Exalted::Ref<Exalted::SpotLight>> m_SpotLights;
		std::vector<Exalted::Ref<Exalted::GameTransform>> m_SpotLightTransforms;
		// shaders
		Exalted::Ref<Exalted::Shader> m_LightSourceShader;
		Exalted::Ref<Exalted::Mesh> m_LightSourceMesh;
		Exalted::Ref<Exalted::Mesh> m_SpotLightMesh;

		/////////////////////////////////////////////////
		////// Directional Shadow Variable Setup ////////
		/////////////////////////////////////////////////
		Exalted::Ref<Exalted::FrameBuffer> m_DepthFrameBuffer;
		Exalted::Ref<Exalted::Shader> m_ObjectDepthShader;
		Exalted::Ref<Exalted::Shader> m_QuadDepthShader;
		Exalted::Ref<Exalted::Mesh> m_QuadMesh;

		/////////////////////////////////
		//// Debugging Utilities
		/////////////////////////////////
		bool DEBUG_FlashlightMode = false;
		bool DEBUG_ColorChange = false;

		
	};
}