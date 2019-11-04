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
#include "Core/Renderer/Shader.h"

//todo: Implement an ongui render function for each gamobject to draw it in the gameobject hierarchy.

namespace Exalted
{
	static const char* DEFAULT_VERTEX_SHADER = "Resources/Shaders/DEFAULT_VERTEX_SHADER.glsl";
	static const char* DEFAULT_FRAGMENT_SHADER = "Resources/Shaders/DEFAULT_FRAGMENT_SHADER.glsl";

	class GameObject
	{
	public:
		GameObject()
			: m_ObjectName("Default GameObject")
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
		void Draw();

		inline Ref<GameTransform>& GetTransform() { return m_Transform; }
		inline void SetShader(Ref<Shader>& shader) { m_Shader = shader; }
		inline Ref<Mesh>& GetMesh() { return m_Mesh; }
		void SetMesh(Ref<Mesh>& mesh) { m_Mesh = mesh; }

		void RenderHierarchyGUI();
		void OnImGuiRender(); //todo: Implement imgui rendering
		uint32_t id() const;

		std::string GetUiText(std::string label)
		{
			std::string id = std::to_string(m_Id);
			std::string separator = "##";
			return (label + separator + id);
		}
	private:
		void DestroyGameObject();
	private:
		static int s_GAME_OBJECT_COUNT;
	private:
		Ref<GameTransform> m_Transform;
		Ref<Mesh> m_Mesh;
		Ref<Shader> m_Shader; //todo: Replace with material once that's implemented.
		GameObject* m_pParent = nullptr;
		std::vector<GameComponent*> m_GameComponents;
		std::vector<GameObject*> m_ChildrenObjectsList;
		std::string m_ObjectName;
		uint32_t m_Id;
	};
}