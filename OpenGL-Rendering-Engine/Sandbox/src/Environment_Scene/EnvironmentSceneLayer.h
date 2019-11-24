/***************************************************************************
 * Filename		: EnvironmentSceneLayer.h
 * Name			: Ori Lazar
 * Date			: 20/11/2019
 * Description	: Contains the declaration for the environment layer used to 
                  demonstrate the capabilities of this engine. 
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
		Exalted::Ref<Exalted::Shader> m_LightSourceShader; // source shader 

		//todo: remove these
		Exalted::Ref<Exalted::Mesh> m_LightSourceMesh;
		Exalted::Ref<Exalted::Mesh> m_SpotLightMesh;

		/////////////////////////////////////////////////
		////// Directional Shadow Variable Setup ////////
		/////////////////////////////////////////////////
		std::vector<Exalted::Ref<Exalted::FrameBuffer>> m_DepthFrameBuffers;  //todo: Note that this is only for spot lights
		Exalted::Ref<Exalted::Shader> m_ObjectDepthShader;
		Exalted::Ref<Exalted::Shader> m_QuadDepthShader;
		Exalted::Ref<Exalted::Mesh> m_QuadMesh;
		
		// directional light
		std::vector<Exalted::Ref<Exalted::FrameBuffer>> m_SunlightDepthFrameBuffers; //todo: Instead of this being a vector, make it a single framebuffer

		/////////////////////////////////
		//// Debugging Utilities
		/////////////////////////////////
		bool DEBUG_ColorChange = false;

		float DEBUG_FOV = 90.f;
		float DEBUG_NEAR = 0.1f;
		float DEBUG_FAR = 20.f;
	};
}