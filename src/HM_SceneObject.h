/************************************************/
/*												*/
/*	HM_SceneObject.h							*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-08-09						*/
/*												*/
/*	Last Update : 2016-08-09					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#pragma once

#include "HM_Object.h"
#include "HM_FileHandler.h"
#include "HM_Constants.h"

#include <string>
#include <map>


class HM_GameMaster;
class HM_Component;



/*				     HEADER						*/

class HM_SceneObject : HM_Object
{

public:
	/** Public functions **/

	// Default Constructor
	HM_SceneObject();
	// Constructor
	HM_SceneObject(std::string name, HM_SceneObject* parent);
	//Destructor
	~HM_SceneObject();


		// --- Accessors ---

	std::string getName() const;

	HM_Component* getComponent(std::string componentName);

	HM_SceneObject* getParent();
	std::map<std::string, HM_SceneObject*> const & getChildren() const;
	HM_SceneObject* getChild(std::string childName);


	bool isRootSceneObject();


	// Check if object is valid
	virtual bool isValid() const;
	
	// Create and load every component and its data
	bool load();

	void loadDataMap(HM_FileHandler file);

	bool onSetupStart();
	bool setup();
	bool onSetupEnd();

	void onUpdateStart();
	void update();
	void onUpdateEnd();

	// Display all the scene objects
	void display();

	void onCollision(HM_SceneObject* other, Direction direction);

	template<class T> 
	inline T* getComponentInObject(std::string componentName);

	template<typename T>
	T* getComponentInChildren(std::string componentName);

	template<typename T>
	T* getComponentInChild(std::string componentName,
								std::string childName);

	template<typename T>
	std::vector<T*> getAllComponentsInChildren(std::string componentName);

	bool isUpdatable() const;
	bool isDisplayable() const;

	void setIsUpdatable(bool isActive);
	void setIsDisplayable(bool isActive);

	/** Public variables **/

	glm::vec3& m_position;
	glm::vec3& m_eulerAngles;
	glm::vec3& m_scale;



private:
	/** Private variables **/

	static const std::vector<std::string> updateComponentList;

	// Name of the scene object
	std::string m_name;

	// Parent scene object of this object
	HM_SceneObject* m_parent;

	// Map of all the scene object's component
	std::map<std::string, HM_Component*> m_componentMap;

	// Map of all the scene object's component
	std::vector<HM_Component*> m_componentArray;

	// Map of all the scene object's children
	std::map<std::string, HM_SceneObject*> m_children;

	std::map<std::string, void*> m_componentDataMap;

	bool m_isUpdatable;
	bool m_isDisplayable;

};

template<class T>
T* HM_SceneObject::getComponentInObject(std::string componentName)
{

	{

		HM_Component* component = getComponent(componentName);

		if (component)
		{

			T* convertedComponent = static_cast<T*>(component);

			if (convertedComponent)
				return convertedComponent;
			else
				return NULL;

		}
		else
		{

			return NULL;

		}

	}

}

template<typename T>
T* HM_SceneObject::getComponentInChildren(std::string componentName)
{

	std::map <std::string, HM_SceneObject*>::const_iterator iter;
	iter = m_children.begin();

	T* component = NULL;

	for (; iter != m_children.end(); iter++)
	{

		if ((component =
			(*iter).second->getComponentInObject<T>(componentName)))
			return component;

	}

	return NULL;

}

template<typename T>
T* HM_SceneObject::getComponentInChild(std::string componentName,
	std::string childName)
{

	HM_SceneObject* child = getChild(childName);

	if (child)
		return child->getComponentInObject<T>(componentName);
	else
		return NULL;

}

template<typename T>
std::vector<T*> HM_SceneObject::getAllComponentsInChildren(
	std::string componentName)
{

	std::map <std::string, HM_SceneObject*>::const_iterator iter;
	iter = m_children.begin();

	std::vector<T*> componentArray;

	T* component = NULL;

	for (; iter != m_children.end(); iter++)
	{

		if ((component =
			(*iter).second->getComponentInObject<T>(componentName)))
			componentArray.push_back(component);

		std::vector<T*> childComponents =
			(*iter).second->getAllComponentsInChildren<T>(componentName);
		
		for(unsigned int i = 0; i < childComponents.size(); i++)
			componentArray.push_back(childComponents[i]);

	}

	return componentArray;

}
