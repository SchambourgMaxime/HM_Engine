/************************************************/
/*												*/
/*	HM_SceneObject.cpp							*/
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

#include "HM_SceneObject.h"
#include "HM_GameMaster.h"
#include "Components/HM_Component.h"



/*				 IMPLEMENTATION					*/

	// --- CONSTRUCTOR & DESTRUCTOR ---

/*		Default constructor
*
*		brief :
*
**/
HM_SceneObject::HM_SceneObject()
{
}

/*		Constructor
*
*		brief : initialize variables and load the content of the file into the
*				list
*
*		params :
*			- name	(string)	: name of the scene object
*
**/
HM_SceneObject::HM_SceneObject(std::string name, HM_SceneObject* parent) :
m_name(name), m_parent(parent)
{
}

/*		Destructor
*
*		brief :
*
**/
HM_SceneObject::~HM_SceneObject()
{

	std::map<std::string, HM_Component*>::const_iterator iter;
	iter = m_componentMap.begin();

	for (; iter != m_componentMap.end(); iter++)
		delete (*iter).second;

	if (m_children.size() != 0U)
	{

		std::map<std::string, HM_SceneObject*>::const_iterator iterObject;
		iterObject = m_children.begin();

		for (; iterObject != m_children.end(); iterObject++)
			delete (*iterObject).second;

	}

}


	// --- ACCESSORS ---

/*		getComponent
*
*		params :
*			- componentName	(string)	: name of the component to get
*
*		return : HM_Component -> pointer to the component with that name
*
**/
HM_Component* HM_SceneObject::getComponent(std::string componentName)
{

	std::map<std::string, HM_Component*>::const_iterator iter;
	iter = m_componentMap.find(componentName);
	
	if(iter != m_componentMap.end())
		return m_componentMap[componentName];

	return NULL;

}

/*		getParent
*
*		return : HM_Component -> pointer to the parent of this object
*								 (NULL if root)
*
**/
HM_SceneObject* HM_SceneObject::getParent()
{

	return m_parent;

}

/*		isRootSceneObject
*
*		return : bool -> determine if the scene has a parent
*
**/
bool HM_SceneObject::isRootSceneObject()
{

	return m_parent == NULL;

}

// --- PUBLIC CLASS FUNCTIONS ---

/*		isValid
*
*		overload : HM_Object
*		
*		brief : determine if the class instance is usable or unset
*
*		return : bool -> indicates if the item can be used or not
*
**/
bool HM_SceneObject::isValid() const
{

	return (m_name != "");

}

/*		load
*		
*		brief : create and load every component and its data
*
*		return : bool -> check to see if loading was successful
*
**/
bool HM_SceneObject::load()
{

	bool isSuccesful = true;

	// File loading
	HM_FileHandler file("./SceneObjects/" +  m_name + ".hmso", HM_FILE_READ);

	if (!file)
	{

		GAME_LOG->_ADDLINETOLOG_LINE("Error : couldn't find scene object file \'SceneObjects/" + m_name + ".hmso\'" , __LINE__ - 5);

		return false;

	}

	// Components Creation
	std::list<std::string> listStringComponents;
	listStringComponents = file.getSectionWithTitle("Components");

	std::list<std::string>::const_iterator iter;
	iter = listStringComponents.begin();

	for (; iter != listStringComponents.end(); iter++)
	{

		std::string currentComponentString = *iter;

		HM_ComponentFactory componentFactory;

		std::vector<std::string> componentsArray;
		componentsArray = hmu::getStringArrayFromString(currentComponentString,
														" ");
		for (unsigned int i = 0; i < componentsArray.size(); i++)			
			m_componentMap[componentsArray[i]] = componentFactory.create(componentsArray[i], this);

	}

	// Components setup
	std::map<std::string, void*> compDataMap;

	listStringComponents = file.getSectionWithTitle("Attributes");
	iter = listStringComponents.begin();

	for (; iter != listStringComponents.end(); iter++)
	{

		unsigned char startAttribute = static_cast<unsigned char>((*iter).find_first_of('='));
		if(startAttribute != 255)
		{

			std::string compKey = (*iter).substr(0, startAttribute);

			compDataMap[compKey] = hmu::getDataFromString((*iter).substr(startAttribute + 1));
			
		}

	}

	// Component data liberation
	std::map<std::string, HM_Component*>::const_iterator iterComp;
	iterComp = m_componentMap.begin();

	for(;iterComp != m_componentMap.end(); iterComp++)
		if (!(*iterComp).second->setup(compDataMap))
		{
			
			GAME_LOG->_ADDLINETOLOG("Error : Can't load component " +
				(*iterComp).first);

			isSuccesful = false;
			break;

		}

	std::map<std::string, void*>::const_iterator iterData;
	iterData = compDataMap.begin();

	for (; iterData != compDataMap.end(); iterData++)
		delete (*iterData).second;

	iterComp = m_componentMap.begin();

	for (; iterComp != m_componentMap.end(); iterComp++)
		if (!(*iterComp).second->onSetupEnd())
		{

			GAME_LOG->_ADDLINETOLOG("Error : Can't load component " +
				(*iterComp).first);

			isSuccesful = false;
			break;

		}

	// Children creation and loading
	std::list<std::string> listStringChildren;
	listStringChildren = file.getSectionWithTitle("Children");

	iter = listStringChildren.begin();

	for (; iter != listStringChildren.end(); iter++)
	{

		HM_SceneObject* currentChild = new HM_SceneObject(*iter, this);
		currentChild->load();

		m_children[*iter] = currentChild;

	}

	return isSuccesful;

}

/*		display
*
*		brief : display all the scene objects
*
**/
void HM_SceneObject::display()
{

	std::map<std::string, HM_Component*>::const_iterator iter;
	iter = m_componentMap.begin();

	for(; iter != m_componentMap.end(); iter++)
		if((*iter).second->isActive())
			(*iter).second->display();

	if (m_children.size() != 0U)
	{
		
		std::map<std::string, HM_SceneObject*>::const_iterator iterObject;
		iterObject = m_children.begin();

		for(; iterObject != m_children.end(); iterObject++)
			(*iterObject).second->display();

	}

}

/*		update
*
*		brief : updates all the scene objects
*
**/
void HM_SceneObject::update()
{

	std::map<std::string, HM_Component*>::const_iterator iter;
	iter = m_componentMap.begin();

	for (; iter != m_componentMap.end(); iter++)
		if ((*iter).second->isActive())
			(*iter).second->update();

	if (m_children.size() != 0U)
	{

		std::map<std::string, HM_SceneObject*>::const_iterator iterObject;
		iterObject = m_children.begin();

		for (; iterObject != m_children.end(); iterObject++)
			(*iterObject).second->update();

	}

}
