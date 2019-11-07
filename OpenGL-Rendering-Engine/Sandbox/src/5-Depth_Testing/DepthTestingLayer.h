/***************************************************************************
 * Filename		: DepthTestingLayer.h
 * Name			: Ori Lazar
 * Date			: 01/11/2019
 * Description	: This layer contains a scene showcasing depth testing capabilities
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
	class DepthTestingLayer : public Exalted::Layer
	{
	public:
		DepthTestingLayer();
		virtual ~DepthTestingLayer() = default;
		virtual void OnUpdate(Exalted::Timestep deltaTime) override;
		virtual void OnImGuiRender() override;
		virtual void OnInactiveImGuiRender() override;
		virtual void OnEvent(Exalted::Event& event) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
	private:
		Exalted::Ref<Exalted::Mesh> m_MeshCube;
		Exalted::Ref<Exalted::Texture2D> m_CubeTexture;
		Exalted::Ref<Exalted::Texture2D> m_CubeTexture2;
		Exalted::Ref<Exalted::Mesh> m_MeshFloor;
		Exalted::Ref<Exalted::Texture2D> m_FloorTexture;
		std::vector<glm::mat4> m_FloorTransforms;
		std::vector<glm::mat4> m_FloorTransformsDepth;
		Exalted::Ref<Exalted::Shader> m_Shader;
		Exalted::Ref<Exalted::Shader> m_DepthShader;
		Exalted::EditorCamera m_EditorCamera;
		bool m_EnableDepthTest = true;
	private:
		static const unsigned m_FloorTileCount = 10;
	};
}