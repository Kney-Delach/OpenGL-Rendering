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

namespace Exalted
{
	GameObject::GameObject(std::string objectName) 
	: m_ObjectName(objectName)
	{
		m_Transform = Exalted::CreateRef<GameTransform>();
		m_Shader = Shader::Create(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
		EX_CORE_INFO("GameObject {0} Constructed with no shader.", m_ObjectName);
	}

	GameObject::GameObject(std::string objectName, std::string& shaderVertex, std::string& shaderFragment)
		: m_ObjectName(objectName)
	{
		m_Transform = Exalted::CreateRef<GameTransform>();
		m_Shader = Shader::Create(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
		EX_CORE_INFO("GameObject {0} Constructed with shaders\nVertex: {1} \nand\nFragment: {2}.", objectName, shaderVertex, shaderFragment);
	}

	void GameObject::Update(Timestep deltaTime)
	{
		// 1 - Set the transforms relative to parent GameObjects
		if (m_pParent) //todo: verify this passes if parent not null
			m_Transform->SetWorldTransform(m_pParent->GetTransform()->GetWorldTransform() * m_Transform->GetLocalTransform());
		else
			m_Transform->SetWorldTransform(m_Transform->GetLocalTransform());

		//todo: Uncomment this when game components are useful.
		// 2 - process all component objects of this object
		//for (std::vector<GameComponent*>::iterator i = m_GameComponents.begin(); i != m_GameComponents.end(); ++i)
		//	(*i)->Update(deltaTime);
		//

		// 3 - process all child objects of this object
		for (std::vector<GameObject*>::iterator i = m_ChildrenObjectsList.begin(); i != m_ChildrenObjectsList.end(); ++i)
			(*i)->Update(deltaTime);
	}

	void GameObject::Draw()
	{
		if(m_Mesh)
			Renderer::Submit(m_Shader, m_Mesh, m_Transform->GetWorldTransform());
		for (std::vector<GameObject*>::iterator i = m_ChildrenObjectsList.begin(); i != m_ChildrenObjectsList.end(); ++i)
			(*i)->Draw();
	}

	void GameObject::RemoveChildObject(GameObject* pGameObject) //todo: verify this functionality works.
	{
		const std::vector<GameObject*>::iterator objectPosition = std::find(m_ChildrenObjectsList.begin(), m_ChildrenObjectsList.end(), pGameObject);
		if (objectPosition != m_ChildrenObjectsList.end())
		{
			m_ChildrenObjectsList.erase(objectPosition);
			delete pGameObject; 
		}
		else
			EX_CORE_WARN("Gameobject {0} is not a child of {1}, cannot remove it.", pGameObject->m_ObjectName, m_ObjectName);
	}

	void GameObject::RemoveChildObjectPointer(GameObject* pGameObject)
	{
		const std::vector<GameObject*>::iterator objectPosition = std::find(m_ChildrenObjectsList.begin(), m_ChildrenObjectsList.end(), pGameObject);
		if (objectPosition != m_ChildrenObjectsList.end())
		{
			m_ChildrenObjectsList.erase(objectPosition);
		}
		EX_CORE_WARN("Removed {0} from {1}.", pGameObject->m_ObjectName, m_ObjectName);
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

	void GameObject::DestroyGameObject() //todo: Verify this works correctly.
	{
		for (unsigned int i = 0; i < m_ChildrenObjectsList.size(); ++i)
			delete m_ChildrenObjectsList[i];
		if (m_pParent != nullptr)
			m_pParent->RemoveChildObjectPointer(this);
	}

}