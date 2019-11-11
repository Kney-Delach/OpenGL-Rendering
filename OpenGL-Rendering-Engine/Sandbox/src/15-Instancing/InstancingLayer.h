/***************************************************************************
 * Filename		: InstancingLayer.h
 * Name			: Ori Lazar
 * Date			: 10/11/2019
 * Description	: This layer contains a scene which utilizes instanced drawing.
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
	class InstancingLayer : public Exalted::Layer
	{
	public:
		InstancingLayer();
		virtual ~InstancingLayer() = default;
		virtual void OnUpdate(Exalted::Timestep deltaTime) override;
		virtual void OnImGuiRender() override;
		virtual void OnInactiveImGuiRender() override;
		virtual void OnEvent(Exalted::Event& event) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
	private:
		Exalted::Ref<Exalted::Scene> m_SceneManager;
		Exalted::Ref<Exalted::GameObject> m_SceneRoot;
		Exalted::Ref<Exalted::EditorCamera> m_EditorCamera;

		Exalted::Ref<Exalted::Mesh> m_SkyboxMesh;
		Exalted::Ref<Exalted::TextureCube> m_SkyboxTexture;
		Exalted::Ref<Exalted::Shader> m_SkyboxShader;

		Exalted::Ref<Exalted::Mesh> m_ModelMesh;

		Exalted::Ref<Exalted::Shader> m_ModelShader;
		Exalted::Ref<Exalted::Shader> m_InstancedModelShader;

		std::vector<glm::mat4> m_Transformations;

		Exalted::Ref<Exalted::UniformBuffer> m_MatUniformBuffer;

		bool m_ToggleInstancing = true;
		unsigned m_InstanceCount = 5000;

		Exalted::Ref<Exalted::VertexBuffer> vb;
	};
}