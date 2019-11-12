/***************************************************************************
 * Filename		: TessellationLayer.h
 * Name			: Ori Lazar
 * Date			: 11/11/2019
 * Description	: This layer contains a scene which displays tesselated terrain.
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
	class TessellationLayer : public Exalted::Layer
	{
	public:
		TessellationLayer();
		virtual ~TessellationLayer() = default;
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

		Exalted::Ref<Exalted::Mesh> m_TessellationMesh;
		Exalted::Ref<Exalted::Shader> m_TessellationShader;
		Exalted::Ref<Exalted::Texture2D> u_TerrainColorMap;
		Exalted::Ref<Exalted::Texture2D> u_TerrainHeightMap;

		float m_TerrainScale = 1;
		float m_VertexScale = 1;
	};
}