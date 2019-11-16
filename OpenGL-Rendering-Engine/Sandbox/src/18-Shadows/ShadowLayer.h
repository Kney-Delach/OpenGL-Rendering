/***************************************************************************
 * Filename		: ShadowLayer.h
 * Name			: Ori Lazar
 * Date			: 15/11/2019
 * Description	: This layer contains a scene which displays different shadow mapping techniques.
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
	class ShadowLayer : public Exalted::Layer
	{
	public:
		ShadowLayer();
		virtual ~ShadowLayer() = default;
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

		// floor
		Exalted::Ref<Exalted::Material> m_FloorMaterial;
		glm::mat4 m_FloorTransformation;
		Exalted::Ref<Exalted::Mesh> m_FloorMesh;

		// cubes
		std::vector<Exalted::Ref<Exalted::Material>> m_ObjectMaterials;
		std::vector<glm::mat4> m_ObjectTransformations;

		// point light
		//Exalted::Ref<Exalted::PointLight> m_PointLightA;
		//Exalted::Ref<Exalted::GameTransform> m_PointLightTransform;

		// directional light
		Exalted::Ref<Exalted::DirectionalLight> m_DirectionalLightA;

		// spot light
		//Exalted::Ref<Exalted::SpotLight> m_SpotLightA;
		//Exalted::Ref<Exalted::GameTransform> m_SpotLightTransform;

		// Shadows
		Exalted::Ref<Exalted::FrameBuffer> m_DepthFrameBuffer;
		Exalted::Ref<Exalted::Shader> m_ObjectDepthShader;
		Exalted::Ref<Exalted::Shader> m_QuadDepthShader;
		Exalted::Ref<Exalted::Mesh> m_QuadMesh;

		/////////////////////////////////////////////////////////
		int m_ObjectCount = 5;
		//bool m_RotateLight = false;
		//bool m_ChangeLightColor = false;
		//bool m_ActivateEmission = false;
		//bool m_EmissionTransform = false;
		//bool m_FlashlightMode = false;
		//bool m_BlinnPhong = true;
	};
}