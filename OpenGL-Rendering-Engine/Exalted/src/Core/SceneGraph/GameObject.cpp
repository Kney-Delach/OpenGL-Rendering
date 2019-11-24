/***************************************************************************
 * Filename		: GameObject.cpp
 * Name			: Ori Lazar
 * Date			: 03/11/2019
 * Description	: Contains the implementation for the game object structure.
				  This object may contain any number of game components,
				  These could be rendering, physics or other components.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "expch.h"
#include "GameObject.h"
#include "Core/Renderer/Renderer.h"

#include <algorithm>

#include <imgui.h>

namespace Exalted
{
	GameObject::GameObject(std::string objectName) 
	: m_ObjectName(objectName), m_BoundingRadius(1.f), m_DistanceFromCamera(0.f), m_IsTransparent(false)
	{
		static uint32_t id = 0;
		m_Id = id++;
		m_ChildrenObjectsList.reserve(2);
		m_Transform = Exalted::CreateRef<GameTransform>();
		m_Shader = Shader::Create(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
		EX_CORE_INFO("GameObject {0} Constructed with no shader.", m_ObjectName);
	}

	GameObject::GameObject(std::string objectName, Ref<Shader>& shader)
		: m_ObjectName(objectName), m_BoundingRadius(1.f), m_DistanceFromCamera(0.f), m_IsTransparent(false)
	{
		static uint32_t id = 0;
		m_Id = id++;
		m_Transform = Exalted::CreateRef<GameTransform>();
		m_Shader = shader;
		EX_CORE_INFO("GameObject {0} Constructed with a custom shader", objectName);
	}

	void GameObject::Update(Timestep deltaTime)
	{
		for (std::vector<GameComponent*>::iterator i = m_GameComponents.begin(); i != m_GameComponents.end(); ++i)
			(*i)->Update(deltaTime);
		
		if (m_pParent)
			m_Transform->SetWorldTransform(m_pParent->GetTransform()->WorldTransform);
		else
			m_Transform->SetWorldTransform();

		if (m_PointLight)
			m_PointLight->Position = glm::vec3(m_Transform->WorldTransform[3]);
		if (m_SpotLight)
			m_SpotLight->Position = glm::vec3(m_Transform->WorldTransform[3]);

		for (std::vector<GameObject*>::iterator i = m_ChildrenObjectsList.begin(); i != m_ChildrenObjectsList.end(); ++i)
			(*i)->Update(deltaTime);
	}

	void GameObject::RemoveGameComponent(GameComponent* pGameComponent)
	{
		const std::vector<GameComponent*>::iterator objectPosition = std::find(m_GameComponents.begin(), m_GameComponents.end(), &(*pGameComponent));
		if (objectPosition != m_GameComponents.end())
		{
			m_GameComponents.erase(objectPosition);
			pGameComponent->RemovedFromGameObject();
		}
	}

	void GameObject::RemoveChildObject(GameObject* pGameObject)
	{
		const std::vector<GameObject*>::iterator objectPosition = std::find(m_ChildrenObjectsList.begin(), m_ChildrenObjectsList.end(), pGameObject);
		if (objectPosition != m_ChildrenObjectsList.end())
		{
			m_ChildrenObjectsList.erase(objectPosition);
			EX_CORE_WARN("Removed {0} from {1}.", pGameObject->m_ObjectName, m_ObjectName);
			delete pGameObject;
		}
		else
			EX_CORE_WARN("Gameobject {0} is not a child of {1}, cannot remove it.", pGameObject->m_ObjectName, m_ObjectName);
	}
	void GameObject::DestroyGameObject()
	{
		const unsigned int objectListSize = m_ChildrenObjectsList.size(); 
		for (unsigned int i = 0; i < objectListSize; i++)
			delete m_ChildrenObjectsList[i];
	}

	void GameObject::RenderHierarchyGUI()
	{
		ImGui::Begin("Scene Hierarchy");
		OnImGuiRender();
		ImGui::Text("-------------------------------");
		ImGui::End();
	}

	void GameObject::OnImGuiRender()
	{
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll;
		ImGui::Text("-------------------------------");
		ImGui::Text(GetUiText(m_ObjectName).c_str());
		ImGui::Text("-------------------------------");
		ImGui::Checkbox(GetUiText("Active").c_str(), &m_Active);
		ImGui::InputFloat3(GetUiText("Position [x,y,z]").c_str(), (float*) & (m_Transform->Position) ,"%.3f");
		ImGui::InputFloat3(GetUiText("Rotation [x,y,z]").c_str(), (float*) & (m_Transform->Rotation), "%.3f");
		ImGui::InputFloat3(GetUiText("Scale [x,y,z]").c_str(), (float*) & (m_Transform->Scale), "%.3f");
		for (std::vector<GameObject*>::iterator i = m_ChildrenObjectsList.begin(); i != m_ChildrenObjectsList.end(); ++i)
		{
			(*i)->OnImGuiRender();
		}
	}
}