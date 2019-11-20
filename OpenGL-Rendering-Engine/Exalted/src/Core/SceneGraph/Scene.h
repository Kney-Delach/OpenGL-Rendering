/***************************************************************************
 * Filename		: Scene.h
 * Name			: Ori Lazar
 * Date			: 05/11/2019
 * Description	: A container for a scene.
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
#include "GameObject.h"
#include "FrustumCulling/Frustum.h"

#include "Core/Core.h"
#include "Core/Renderer/EditorCamera.h"
#include "Core/Renderer/Skybox/Skybox.h"
#include "Core/Renderer/Loaders/ObjLoader.h"

#include <vector>
#include "LightManager.h"

#define DEFAULT_ROOT_MESH "Resources/Defaults/GameObjects/RootMesh.obj"

namespace Exalted
{
	class Scene
	{
	public:
		Scene(Ref<EditorCamera>& camera, bool generateRoot = false, bool generateMesh = true) : m_Camera(camera)
		{
			m_Skybox = Skybox::Create();
			if(generateRoot)
			{			
				m_SceneRoot = Exalted::CreateRef<GameObject>("Scene Root");
				m_SceneRoot->SetBoundingRadius(FLT_MAX);
				if(!generateMesh)
				{
					m_SceneRoot->SetActive(true);
				}
				else
				{
					Ref<Mesh> rootMesh = Mesh::Create();
					rootMesh->SetVertexArray(ObjLoader::Load(DEFAULT_ROOT_MESH));
					m_SceneRoot->SetMesh(rootMesh);
					m_SceneRoot->SetActive(false);
				}
			}
		}
		~Scene() = default;
		//void CompleteSceneSetup() const;// sets up static scene ubos
		void UpdateScene(Timestep deltaTime);
		void RenderScene()
		{
			BuildObjectLists(m_SceneRoot.get());
			SortObjectLists();
			//DrawObjectLists();
			//ClearObjectLists();
		}
		//////////////////// USE THESE //////////////////
		void DrawOpaqueObjects();
		void DrawTransparentObjects();
		void ClearObjectLists();
		void RenderSkybox() const { m_Skybox->Draw(); } //todo: use this internally?
		void SetCamera(Ref<EditorCamera>& camera) { m_Camera = camera; }
		//void SetStaticLightManager(Ref<LightManager>& staticLightManager) { m_StaticLightManager = staticLightManager; } // update uniform buffer data once a frame
		//void SetDynamicLightManager(Ref<LightManager>& dynamicLightManager) { m_DynamicLightManager = dynamicLightManager; } // set up uniform buffer once at the start
		Ref<GameObject>& GetSceneRoot() { return m_SceneRoot; }
		void SetSceneRoot(Ref<GameObject>& sceneRoot) { m_SceneRoot = sceneRoot; }
		static void OnEvent(Exalted::Event& event);
	private:
		void BuildObjectLists(GameObject* gameObject);
		void SortObjectLists(); 
		
		void DrawObjectLists();
		static __forceinline void DrawObject(GameObject* gameObject) { gameObject->Draw(); }
	private:
		Ref<EditorCamera> m_Camera;
		Ref<GameObject> m_SceneRoot;
		Ref<Skybox> m_Skybox;
		//Ref<LightManager> m_StaticLightManager;
		//Ref<LightManager> m_DynamicLightManager;
		Frustum m_Frustum;
		std::vector<GameObject*> m_OpaqueObjects;
		std::vector<GameObject*> m_TransparentObjects;
	};
}