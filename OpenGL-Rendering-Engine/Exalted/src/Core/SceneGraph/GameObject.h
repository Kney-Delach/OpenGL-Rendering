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
#include "Core/Renderer/Skybox/Skybox.h"

//todo: Implement a better gui hierarchy

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
			const unsigned int gameComponentsSize = static_cast<const unsigned int>(m_GameComponents.size());

			for (unsigned int i = 0; i < gameComponentsSize; i++)
				delete m_GameComponents[i];
			
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
				std::dynamic_pointer_cast<OpenGLShader>(m_Shader)->SetUniformMatFloat4("u_Model", m_Transform->WorldTransform);
				if(m_Material)
				{
					m_Material->Bind(m_Shader);
					int count = 4;
					for (int i = 0; i < NumberOfSpotLights; i++)
					{
						std::string number = std::to_string(i);
						std::dynamic_pointer_cast<OpenGLShader>(m_Shader)->SetUniformInt1(("u_ShadowMap[" + number + "]").c_str(), (count++));
					}
					std::dynamic_pointer_cast<OpenGLShader>(m_Shader)->SetUniformInt1("u_DirectionalShadowMap", count++);
					for (int i = 0; i < NumberOfPointLights; i++)
					{
						std::string number = std::to_string(i);
						std::dynamic_pointer_cast<OpenGLShader>(m_Shader)->SetUniformInt1(("u_PointShadowMap[" + number + "]").c_str(), (count++));
					}
					Renderer::Submit(m_Mesh);
					m_Material->Unbind();
				}
				else //todo: Assuming that if there is no material, it reflects the skybox cubemap
				{
					if (m_PointLight)
						std::dynamic_pointer_cast<OpenGLShader>(m_Shader)->SetUniformFloat3("u_Color", m_PointLight->Diffuse);
					std::dynamic_pointer_cast<OpenGLShader>(m_Shader)->SetUniformInt1("u_Skybox", 0);
					Skybox::GetTexture()->Bind(0);
					Renderer::Submit(m_Mesh);
					Skybox::GetTexture()->Unbind();
				}
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
		}//todo: remove me

		inline void SetMaterial(Ref<Material>& material) { m_Material = material; }
		inline Ref<Material>& GetMaterial() { return m_Material; }

		void SetMesh(Ref<Mesh>& mesh) { m_Mesh = mesh; }
		inline Ref<Mesh>& GetMesh() { return m_Mesh; }

		void SetSpotLight(Ref<SpotLight>& light) { m_SpotLight = light; }
		void SetPointLight(Ref<PointLight>& light) { m_PointLight = light; }
		
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
	public: //todo: change naming conventions 
		static inline int NumberOfSpotLights; 
		static inline int NumberOfPointLights;
	private:
		void DestroyGameObject();
	private:
		static int s_GAME_OBJECT_COUNT;
	private:
		Ref<GameTransform> m_Transform;
		Ref<Mesh> m_Mesh;
		Ref<Material> m_Material; 
		Ref<Shader> m_Shader;
		Ref<SpotLight> m_SpotLight;
		Ref<PointLight> m_PointLight;
		GameObject* m_pParent = nullptr;
		std::vector<GameComponent*> m_GameComponents;
		std::vector<GameObject*> m_ChildrenObjectsList;
		bool m_IsTransparent;
		bool m_Active = true;
		std::string m_ObjectName;
		uint32_t m_Id;
		float m_BoundingRadius; // to keep things simple in initial implementation, using a bounding sphere for each object, represented by single radius -> todo: implement bounding volume class. 
		float m_DistanceFromCamera;
	};
}