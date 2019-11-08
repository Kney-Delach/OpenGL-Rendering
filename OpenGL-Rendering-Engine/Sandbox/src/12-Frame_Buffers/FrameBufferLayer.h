/***************************************************************************
 * Filename		: FrameBufferLayer.h
 * Name			: Ori Lazar
 * Date			: 08/11/2019
 * Description	: This layer contains a scene which utilizes frame buffers to 
                  perform post processing techniques. 
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
	class FrameBufferLayer : public Exalted::Layer
	{
	public:
		FrameBufferLayer();
		virtual ~FrameBufferLayer() = default;
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
		Exalted::Ref<Exalted::FrameBuffer> m_FrameBuffer;
		Exalted::Ref<Exalted::Shader> m_FrameBufferShader;
		Exalted::Ref<Exalted::Mesh> m_QuadMesh;
		int m_PostProcessChoice = 0;
	};
}