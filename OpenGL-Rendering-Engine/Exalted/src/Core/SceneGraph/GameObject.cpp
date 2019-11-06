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
#include <algorithm>
#include "Core/Renderer/Renderer.h"

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
		// 1 - Set the transforms relative to parent GameObjects
		if (m_pParent)
			m_Transform->SetWorldTransform(m_pParent->GetTransform()->WorldTransform);
		else
			m_Transform->SetWorldTransform();

		//todo: Uncomment this when game components are useful.
		// 2 - process all component objects of this object
		//for (std::vector<GameComponent*>::iterator i = m_GameComponents.begin(); i != m_GameComponents.end(); ++i)
		//	(*i)->Update(deltaTime);
		//

		// 3 - process all child objects of this object
		for (std::vector<GameObject*>::iterator i = m_ChildrenObjectsList.begin(); i != m_ChildrenObjectsList.end(); ++i)
		{
			(*i)->Update(deltaTime);
		}
	}

	//void GameObject::Draw() //todo: Verify correct draw ordering
	//{
	//	//for (std::vector<GameObject*>::iterator i = m_ChildrenObjectsList.begin(); i != m_ChildrenObjectsList.end(); ++i)
	//	//{
	//	//	(*i)->Draw();
	//	//}
	//	//todo: Instance materials so we don't have to iterate one by one like this for each mesh, also instance draw calls....

	//	if (m_Mesh)
	//	{
	//		if(m_Texture)
	//		{
	//			m_Texture->Bind();
	//			Renderer::Submit(m_Shader, m_Mesh, m_Transform->GetWorldTransform());
	//			m_Texture->Unbind();
	//		}
	//		else
	//		{
	//			Renderer::Submit(m_Shader, m_Mesh, m_Transform->GetWorldTransform());
	//		}
	//	}
	//}

	void GameObject::RemoveGameComponent(GameComponent* pGameComponent)
	{
		const std::vector<GameComponent*>::iterator objectPosition = std::find(m_GameComponents.begin(), m_GameComponents.end(), &(*pGameComponent));
		if (objectPosition != m_GameComponents.end())
		{
			m_GameComponents.erase(objectPosition);
			pGameComponent->RemovedFromGameObject();
		}
	}

	void GameObject::RemoveChildObject(GameObject* pGameObject) //todo: verify this functionality works.
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
	void GameObject::DestroyGameObject() //todo: Verify this works correctly.
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
		ImGui::InputFloat3(GetUiText("Position [x,y,z]").c_str(), (float*) & (m_Transform->Position) ,"%.3f");
		ImGui::InputFloat3(GetUiText("Rotation [x,y,z]").c_str(), (float*) & (m_Transform->Rotation), "%.3f");
		ImGui::InputFloat3(GetUiText("Scale [x,y,z]").c_str(), (float*) & (m_Transform->Scale), "%.3f");
		for (std::vector<GameObject*>::iterator i = m_ChildrenObjectsList.begin(); i != m_ChildrenObjectsList.end(); ++i)
		{
			(*i)->OnImGuiRender();
		}
	}
}