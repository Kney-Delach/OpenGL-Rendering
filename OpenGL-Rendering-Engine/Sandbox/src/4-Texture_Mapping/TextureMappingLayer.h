/***************************************************************************
 * Filename		: TextureMappingLayer.h
 * Name			: Ori Lazar
 * Date			: 31/10/2019
 * Description	: This layer contains a scene showcasing texture mapping capabilities 
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
	class TextureMappingLayer : public Exalted::Layer
	{
	public:
		TextureMappingLayer();
		virtual ~TextureMappingLayer() = default;
		virtual void OnUpdate(Exalted::Timestep deltaTime) override;
		virtual void OnImGuiRender() override;
		virtual void OnInactiveImGuiRender() override;
		virtual void OnEvent(Exalted::Event& event) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
	private:
		Exalted::Ref<Exalted::Mesh> m_Mesh3D;
		Exalted::Ref<Exalted::Texture2D> m_Texture3D; 
		std::vector<Exalted::Ref<Exalted::Mesh>> m_Meshes;
		std::vector<Exalted::Ref<Exalted::Texture2D>> m_Textures;
		std::vector<glm::mat4> meshTransforms; 
		Exalted::Ref<Exalted::Shader> m_Shader;
		Exalted::EditorCamera m_EditorCamera;
		bool m_AnimateCube = false;
		bool m_AnimatePositiveDirection = true;
	};
}