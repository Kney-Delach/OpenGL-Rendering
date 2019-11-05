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

#include <vector>

//todo: Create the scene root always at location 0,0,0, and contains a mesh sphere which can be toggled visible.

namespace Exalted
{
	class Scene
	{
	public:
		Scene(Ref<EditorCamera>& camera) : m_Camera(camera) {}
		~Scene() = default;

		void UpdateScene(Timestep deltaTime);
		void RenderScene()
		{
			BuildObjectLists(m_SceneRoot.get());
			SortObjectLists();
			DrawObjectLists();
			ClearObjectLists();
		}

		void SetCamera(Ref<EditorCamera>& camera) { m_Camera = camera; }
		Ref<GameObject>& GetSceneRoot() { return m_SceneRoot; }
		void SetSceneRoot(Ref<GameObject>& sceneRoot) { m_SceneRoot = sceneRoot; }
	private:
		void BuildObjectLists(GameObject* gameObject);
		void SortObjectLists(); 
		void ClearObjectLists();
		void DrawObjectLists();
		static __forceinline void DrawObject(GameObject* gameObject)
		{
			gameObject->Draw();
		}
	private:
		Ref<EditorCamera> m_Camera;
		Ref<GameObject> m_SceneRoot;
		Frustum m_Frustum;
		std::vector<GameObject*> m_OpaqueObjects;
		std::vector<GameObject*> m_TransparentObjects;
	};
}
