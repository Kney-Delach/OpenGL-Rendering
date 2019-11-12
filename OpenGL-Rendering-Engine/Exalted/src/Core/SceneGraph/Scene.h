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

#define DEFAULT_ROOT_MESH "Resources/Defaults/GameObjects/RootMesh.obj"
//todo: Create the scene root always at location 0,0,0, and contains a mesh sphere which can be toggled visible.

namespace Exalted
{
	class Scene
	{
	public:
		Scene(Ref<EditorCamera>& camera, bool generateRoot = false) : m_Camera(camera)
		{
			m_Skybox = Skybox::Create();
			if(generateRoot)
			{
				Ref<Mesh> rootMesh = Mesh::Create();
				rootMesh->SetVertexArray(ObjLoader::Load(DEFAULT_ROOT_MESH));
				m_SceneRoot = Exalted::CreateRef<GameObject>("Scene Root");
				m_SceneRoot->SetBoundingRadius(FLT_MAX);
				m_SceneRoot->SetMesh(rootMesh);
				m_SceneRoot->SetActive(false);
			}
		}
		~Scene() = default;

		void UpdateScene(Timestep deltaTime);
		void RenderScene()
		{
			BuildObjectLists(m_SceneRoot.get());
			SortObjectLists();
			DrawObjectLists();
			ClearObjectLists();
		}
		void RenderSkybox() const { m_Skybox->Draw(); } //todo: Make private and add call to RenderScene function.
		void SetCamera(Ref<EditorCamera>& camera) { m_Camera = camera; }
		Ref<GameObject>& GetSceneRoot() { return m_SceneRoot; }
		void SetSceneRoot(Ref<GameObject>& sceneRoot) { m_SceneRoot = sceneRoot; }
		static void OnEvent(Exalted::Event& event);
	private:
		void BuildObjectLists(GameObject* gameObject);
		void SortObjectLists(); 
		void ClearObjectLists();
		void DrawObjectLists();
		static __forceinline void DrawObject(GameObject* gameObject) { gameObject->Draw(); }
	private:
		Ref<EditorCamera> m_Camera;
		Ref<GameObject> m_SceneRoot;
		Ref<Skybox> m_Skybox;
		Frustum m_Frustum;
		std::vector<GameObject*> m_OpaqueObjects;
		std::vector<GameObject*> m_TransparentObjects;
	};
}