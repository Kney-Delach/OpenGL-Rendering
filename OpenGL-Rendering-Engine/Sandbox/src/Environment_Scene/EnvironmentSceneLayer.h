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


		//////////////////////////////////////////////
		////todo: Stuff that needs to go to the scene graph
		//////////////////////////////////////////////
		/////todo: uniform buffers
		Exalted::Ref<Exalted::UniformBuffer> m_LightUniformBuffer;
		Exalted::Ref<Exalted::UniformBuffer> m_LightSpaceDataUniformBuffer;

		/////////////////////////////////////////////////
		////// Directional Shadow Variable Setup ////////
		/////////////////////////////////////////////////
		std::vector<Exalted::Ref<Exalted::FrameBuffer>> m_DepthFrameBuffers; 
		Exalted::Ref<Exalted::Shader> m_ObjectDepthShader;
		Exalted::Ref<Exalted::Shader> m_QuadDepthShader;
		Exalted::Ref<Exalted::Mesh> m_QuadMesh;

		/////////////////////////////////
		//// Debugging Utilities
		/////////////////////////////////
		bool DEBUG_ColorChange = false;

		float DEBUG_FOV = 90.f;
		float DEBUG_NEAR = 0.1f;
		float DEBUG_FAR = 20.f;
	};
}