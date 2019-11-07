/***************************************************************************
 * Filename		: ModelLoadingLayer.h
 * Name			: Ori Lazar
 * Date			: 05/11/2019
 * Description	: This layer contains a scene with 3D Model loading!
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
	class ModelLoadingLayer : public Exalted::Layer
	{
	public:
		ModelLoadingLayer();
		virtual ~ModelLoadingLayer() = default;
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
		Exalted::Ref<Exalted::Shader> m_ModelShader;
		Exalted::Ref<Exalted::Mesh> m_ModelMesh;
	};
}