/***************************************************************************
 * Filename		: GLSLExplorationLayer.h
 * Name			: Ori Lazar
 * Date			: 09/11/2019
 * Description	: This layer contains a scene I used to explore GLSL. 
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
	class GLSLExplorationLayer : public Exalted::Layer
	{
	public:
		GLSLExplorationLayer();
		virtual ~GLSLExplorationLayer() = default;
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
		Exalted::Ref<Exalted::Shader> m_ModelShader2;

		Exalted::Ref<Exalted::Shader> m_NormalsShader;
		Exalted::Ref<Exalted::Shader> m_ExplosionShader;

		Exalted::Ref<Exalted::UniformBuffer> m_MatUniformBuffer;

		bool m_NormalToggle = false;
		bool m_ExplosionToggle = false;
	};
}