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
#include "Core/Renderer/FrameBuffer.h"

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
		void SetupSceneLightUBOs() const;
		void UpdateScene(Timestep deltaTime);
		void RenderScene()
		{
			BuildObjectLists(m_SceneRoot.get());
			SortObjectLists();
			// render to shadow framebuffers todo: this
			//DrawObjectLists();
			//ClearObjectLists();
		}
		//////////////////// USE THESE //////////////////
		void DrawOpaqueObjects();
		void DrawTransparentObjects();

		// used for shadow mapping
		void DrawOpaqueBindless(const Ref<Shader>& shadowShader);
		void DrawTransparentBindless(const Ref<Shader>& shadowShader);
		
		void ClearObjectLists();
		void RenderSkybox() const { m_Skybox->Draw(); } //todo: use this internally?
		void SetCamera(Ref<EditorCamera>& camera) { m_Camera = camera; }
		void SetLightManager(Ref<LightManager>& lightManager) { m_LightsManager = lightManager; } //todo: verify this works now....
		void SetSceneRoot(Ref<GameObject>& sceneRoot) { m_SceneRoot = sceneRoot; }
		void OnEvent(Exalted::Event& event);
		Ref<GameObject>& GetSceneRoot() { return m_SceneRoot; }
	public:
		static inline bool s_IsCameraFree = false; // initially camera will be on the track
	private:
		void BuildObjectLists(GameObject* gameObject);
		void SortObjectLists(); 
		
		void DrawObjectLists();
		static __forceinline void DrawObject(GameObject* gameObject) { gameObject->Draw(); }
		static __forceinline void DrawObjectBindless(GameObject* gameObject, const Ref<Shader>& shadowShader) { gameObject->DrawBindless(shadowShader); }
	private:
		Ref<EditorCamera> m_Camera;
		Ref<GameObject> m_SceneRoot;
		Ref<Skybox> m_Skybox;
		Ref<LightManager> m_LightsManager;
		//Ref<LightManager> m_DynamicLightManager;
		//Ref<FrameBuffer> m_PostProcessingFrameBuffer; // maybe move this to post processing management? 
		Frustum m_Frustum;
		std::vector<GameObject*> m_OpaqueObjects;
		std::vector<GameObject*> m_TransparentObjects;
	};
}