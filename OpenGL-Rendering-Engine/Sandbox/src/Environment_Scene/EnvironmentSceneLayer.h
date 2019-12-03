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

		// explosion effect data
		Exalted::Ref<Exalted::Shader> m_ExplosionShader;
		Exalted::GameObject* UFOC;
		
		// spot light shadow data 
		std::vector<Exalted::Ref<Exalted::FrameBuffer>> m_DepthFrameBuffers; 
		Exalted::Ref<Exalted::Shader> m_ObjectDepthShader;
		
		// sun directional light shadow data
		std::vector<Exalted::Ref<Exalted::FrameBuffer>> m_SunlightDepthFrameBuffers;

		// shadow debug shader
		Exalted::Ref<Exalted::Shader> m_QuadDepthShader; // used to display the shadow maps to quads, used for debugging.

		Exalted::Ref<Exalted::Mesh> m_QuadMesh; // used for rendering quads to the screen

		/////////////////////////////////////////////////
		////// Post Processing //////////////////////////
		/////////////////////////////////////////////////
		Exalted::Ref<Exalted::FrameBuffer> m_PostProcessingFrameBuffer; 
		Exalted::Ref<Exalted::Shader> m_PostProcessingShader;
	};
}