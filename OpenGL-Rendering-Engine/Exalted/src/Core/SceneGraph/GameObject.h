/***************************************************************************
 * Filename		: GameObject.h
 * Name			: Ori Lazar
 * Date			: 03/11/2019
 * Description	: Contains the declaration for the game object structure.
                  This object may contain any number of game components,
                  These could be rendering, physics or other components.
                  If thinking about this in terms of trees, this is the node.
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
#include "Core/Core/Timestep.h"

#include "GameTransform.h"
#include "GameComponent.h"

#include "Core/Renderer/Mesh.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Renderer.h"

#include "FrustumCulling/FrustumPlane.h"
#include "Core/Renderer/Lights/Material.h"
#include "Core/Renderer/Lights/Light.h"

//todo: Implement a check for if a mesh should be drawn with or without indicies.
//todo: Implement a better gui hierarchy
//todo: Implement bounding volume class for the frustum culling mechanism

namespace Exalted
{
	static const char* DEFAULT_VERTEX_SHADER = "Resources/Shaders/DEFAULT_VERTEX_SHADER.glsl";
	static const char* DEFAULT_FRAGMENT_SHADER = "Resources/Shaders/DEFAULT_FRAGMENT_SHADER.glsl";

	class GameObject
	{
	public:
		GameObject()
			: m_ObjectName("Default GameObject"), m_BoundingRadius(1.f), m_DistanceFromCamera(0.f), m_IsTransparent(false)
		{
			static uint32_t id = 0;
			m_Id = id++;
			EX_CORE_INFO("Default GameObject Constructor Called");
		}
		GameObject(std::string objectName);
		GameObject(std::string objectName, Ref<Shader>& shader);
		~GameObject()
		{
			DestroyGameObject();
			EX_CORE_WARN("Destroyed {0} GamObject.", m_ObjectName);
		}
		void AddChildObject(GameObject* pGameObject)
		{
			const std::vector<GameObject*>::iterator objectPosition = std::find(m_ChildrenObjectsList.begin(), m_ChildrenObjectsList.end(), pGameObject);
			if (objectPosition != m_ChildrenObjectsList.end())
			{
				EX_CORE_CRITICAL("You are attempting to add {0} to {1}, but it already contains it!", pGameObject->m_ObjectName, m_ObjectName);
				return;
			}
			if(pGameObject == this)
			{
				EX_CORE_CRITICAL("You are attempting to add {0} to itself, illegal operation.", m_ObjectName);
				return;
			}
			m_ChildrenObjectsList.emplace_back(&(*pGameObject));
			pGameObject->SetParentObject(this);
			EX_CORE_WARN("Added {0} as a child of {1}.", pGameObject->m_ObjectName, m_ObjectName);
		}
		void RemoveChildObject(GameObject* pGameObject);
		void SetParentObject(GameObject* pGameObject) { m_pParent = &(*pGameObject); }
		void AddGameComponent(GameComponent* pGameComponent) { m_GameComponents.emplace_back(&(*pGameComponent)); }
		void RemoveGameComponent(GameComponent* pGameComponent);
		std::vector<GameObject*>::const_iterator GetChildIteratorStart() { return m_ChildrenObjectsList.begin(); }
		std::vector<GameObject*>::const_iterator GetChildIteratorEnd() { return m_ChildrenObjectsList.end(); }

		void Update(Timestep deltaTime);
		inline void Draw() const
		{
			if (m_Mesh)
			{
				m_Shader->Bind();
				m_Material->Bind(m_Shader);
				for (int i = 0; i < NumberOfPointLights; i++) //todo: move this from here maybe ?
				{
					std::string number = std::to_string(i);
					std::dynamic_pointer_cast<OpenGLShader>(m_Shader)->SetUniformInt1(("u_ShadowMap[" + number + "]").c_str(), (4 + i));

				}
				//std::dynamic_pointer_cast<OpenGLShader>(m_Shader)->SetUniformMatFloat4("u_LightSpaceMatrices", LightSpaceMatrix); //todo: move this to uniform buffer
				std::dynamic_pointer_cast<OpenGLShader>(m_Shader)->SetUniformMatFloat4("u_Model", m_Transform->WorldTransform);
				Renderer::Submit(m_Mesh);
				m_Material->Unbind();
				m_Shader->Unbind();
			}
		}

		inline void DrawBindless(const Ref<Shader>& shadowShader) const
		{
			if (m_Mesh)
			{
				std::dynamic_pointer_cast<OpenGLShader>(shadowShader)->SetUniformMatFloat4("u_Model", m_Transform->WorldTransform);
				Renderer::Submit(m_Mesh);
			}
		}
		
		inline Ref<GameTransform>& GetTransform() { return m_Transform; }

		inline void SetShader(Ref<Shader>& shader) { m_Shader = shader; }

		inline void SetTexture(Ref<Texture2D>& texture)
		{
			//if(!m_IsTransparent)
			//	m_IsTransparent = texture->IsTransparent();
			//m_Texture = texture;
		}
		

		inline void SetMaterial(Ref<Material>& material) { m_Material = material; }
		inline Ref<Material>& GetMaterial() { return m_Material; }

		void SetMesh(Ref<Mesh>& mesh) { m_Mesh = mesh; }
		inline Ref<Mesh>& GetMesh() { return m_Mesh; }

		//void SetSpotLight(Ref<SpotLight>& light) { m_SpotLight = light; }
		//void SetPointLight(Ref<PointLight>& light) { m_PointLight = light; }
		//
		inline bool IsTransparent() const { return m_IsTransparent; }
		inline void SetTransparency(bool transparent) { m_IsTransparent = transparent; }
		
		inline bool IsActive() const { return m_Active; }
		inline void SetActive(bool active) { m_Active = active; }

		void RenderHierarchyGUI();
		void OnImGuiRender(); //todo: Implement imgui rendering

		float GetBoundingRadius() const { return m_BoundingRadius; }
		void SetBoundingRadius(const float boundingRadius) { m_BoundingRadius = boundingRadius; } //todo: Automate this

		float GetDistanceFromCamera() const { return m_DistanceFromCamera; }
		void SetDistanceFromCamera(const float distanceFromCamera) { m_DistanceFromCamera = distanceFromCamera; }

		static bool CompareByCameraDistance(GameObject* objectA, GameObject* objectB)
		{
			return (objectA->m_DistanceFromCamera < objectB->m_DistanceFromCamera) ? true : false;
		}

		uint32_t id() const;
		std::string GetUiText(std::string label)
		{
			std::string id = std::to_string(m_Id);
			std::string separator = "##";
			return (label + separator + id);
		}
	public:
		//static inline std::vector<glm::mat4> LightSpaceMatrices;
		static inline int NumberOfPointLights;
	private:
		void DestroyGameObject();
	private:
		static int s_GAME_OBJECT_COUNT;
	private:
		Ref<GameTransform> m_Transform;
		Ref<Mesh> m_Mesh;
		//Ref<Texture2D> m_Texture;  //todo: Replace with material once that's implemented.
		Ref<Material> m_Material; 
		Ref<Shader> m_Shader;
		//Ref<SpotLight> m_SpotLight;
		//Ref<PointLight> m_PointLight;

		GameObject* m_pParent = nullptr;
		std::vector<GameComponent*> m_GameComponents;
		std::vector<GameObject*> m_ChildrenObjectsList;
		bool m_IsTransparent;
		bool m_Active = true;
		std::string m_ObjectName;
		uint32_t m_Id;
		float m_BoundingRadius; // to keep things simple in initial implementation, using a bounding sphere for each object, represented by single radius -> todo: implement bounding volume class. 
		float m_DistanceFromCamera; // to sort the gameobjects, this stores their distance from the camera

	};
}