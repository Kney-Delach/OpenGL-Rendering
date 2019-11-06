/***************************************************************************
 * Filename		: HeightMapLayer.h
 * Name			: Ori Lazar
 * Date			: 05/11/2019
 * Description	: This layer contains a scene with a height map!
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
	class HeightMapLayer : public Exalted::Layer
	{
	public:
		HeightMapLayer();
		virtual ~HeightMapLayer() = default;
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

		//Exalted::Ref<Exalted::HeightMap> m_HeightMap;
		//Exalted::Ref<Exalted::Mesh> m_TerrainMesh;
		Exalted::Ref<Exalted::Shader> m_TerrainShader;
		Exalted::Ref<Exalted::Texture2D> m_TerrainTexture;
		Exalted::GameObject* terrainGameObject;
		Exalted::Ref<Exalted::Mesh> heightMap;
	};
}