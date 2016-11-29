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

// --- STATIC VARIABLES ---

const std::vector<std::string> HM_SceneObject::updateComponentList =
{

	"joystick",
	"character",
	"activator",
	"endGame",
	"damageDealer",
	"motion",
	"transform",
	"boxCollider",
	"camera",
	"mesh",
	"sprite",
	"hud"

};

	// --- CONSTRUCTOR & DESTRUCTOR ---

/*		Default constructor
*
*		brief :
*
**/
HM_SceneObject::HM_SceneObject() :
	m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_eulerAngles(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_scale(glm::vec3(1.0f, 1.0f, 1.0f)),
	m_isUpdatable(true), m_isDisplayable(true)
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
m_name(name),
m_parent(parent),
m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
m_eulerAngles(glm::vec3(0.0f, 0.0f, 0.0f)),
m_scale(glm::vec3(1.0f, 1.0f, 1.0f)),
m_isUpdatable(true), m_isDisplayable(true)
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


std::string HM_SceneObject::getName() const
{

	return m_name;

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

std::map<std::string, HM_SceneObject*> const & HM_SceneObject::getChildren() const
{

	return m_children;

}

HM_SceneObject* HM_SceneObject::getChild(std::string childName)
{

	std::map<std::string, HM_SceneObject*>::const_iterator iter;
	iter = m_children.find(childName);

	if (iter != m_children.end())
		return m_children[childName];

	return NULL;

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

// 	bool isSuccesful = true;
// 
// 
// 
// 	isSuccesful = onSetupStart();
// 	isSuccesful = setup();
// 	isSuccesful = onSetupEnd();
// 
// 	// Memory liberation
// 	std::map<std::string, void*>::const_iterator iterData;
// 	iterData = compDataMap.begin();
// 
// 	for (; iterData != compDataMap.end(); iterData++)
// 		delete (*iterData).second;
// 
// 
// 	// Children creation and loading
// 	std::list<std::string> listStringChildren;
// 	listStringChildren = file.getSectionWithTitle("Children");
// 
// 	iter = listStringChildren.begin();
// 
// 	for (; iter != listStringChildren.end(); iter++)
// 	{
// 
// 		HM_SceneObject* currentChild = new HM_SceneObject(*iter, this);
// 		isSuccesful = currentChild->load();
// 
// 		m_children[*iter] = currentChild;
// 
// 	}
// 
 	return true;

}

void HM_SceneObject::loadDataMap(HM_FileHandler file)
{

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
			m_componentMap[componentsArray[i]] =
				componentFactory.create(componentsArray[i], this);

	}

	// Components setup

	listStringComponents = file.getSectionWithTitle("Attributes");
	iter = listStringComponents.begin();

	for (; iter != listStringComponents.end(); iter++)
	{

		unsigned char startAttribute =
			static_cast<unsigned char>((*iter).find_first_of('='));
		if (startAttribute != 255)
		{

			std::string compKey = (*iter).substr(0, startAttribute);

			m_componentDataMap[compKey] =
				hmu::getDataFromString((*iter).substr(startAttribute + 1));

		}

	}

}

bool HM_SceneObject::onSetupStart()
{

	// File loading
	HM_FileHandler file("./SceneObjects/" + m_name + ".hmso", HM_FILE_READ);

	if (!file)
	{

		GAME_LOG->_ADDLINETOLOG_LINE("Error : couldn't find scene object file \'SceneObjects/" + m_name + ".hmso\'", __LINE__ - 5);

		return false;

	}

	loadDataMap(file);

	// Start
	std::map<std::string, HM_Component*>::const_iterator iterComp;
	iterComp = m_componentMap.begin();

	for (; iterComp != m_componentMap.end(); iterComp++)
		if (!(*iterComp).second->onSetupStart(m_componentDataMap))
		{

			GAME_LOG->_ADDLINETOLOG("Error : Can't load component " +
				(*iterComp).first);

			return false;

		}


	bool isSuccesful = true;

	// Children creation and loading
	std::list<std::string> listStringChildren;
	listStringChildren = file.getSectionWithTitle("Children");

	std::list<std::string>::const_iterator iter = listStringChildren.begin();

	for (; iter != listStringChildren.end(); iter++)
	{

		HM_SceneObject* currentChild = new HM_SceneObject(*iter, this);
		isSuccesful = currentChild->onSetupStart();

		m_children[*iter] = currentChild;

	}

	return isSuccesful;

}

bool HM_SceneObject::setup()
{

	// Setup
	std::map<std::string, HM_Component*>::const_iterator iterComp;
	iterComp = m_componentMap.begin();

	for (; iterComp != m_componentMap.end(); iterComp++)
		if (!(*iterComp).second->setup(m_componentDataMap))
		{

			GAME_LOG->_ADDLINETOLOG("Error : Can't load component " +
				(*iterComp).first);

			return false;

		}


	bool isSuccesful = true;

	// Children creation and loading
	std::map<std::string, HM_SceneObject*>::const_iterator iter;
	iter = m_children.begin();

	for (; iter != m_children.end(); iter++)
	{

		HM_SceneObject* currentChild = iter->second;
		isSuccesful = currentChild->setup();

	}

		return isSuccesful;

}

bool HM_SceneObject::onSetupEnd()
{

	// End
	std::map<std::string, HM_Component*>::const_iterator iterComp;
	iterComp = m_componentMap.begin();

	for (; iterComp != m_componentMap.end(); iterComp++)
		if (!(*iterComp).second->onSetupEnd(m_componentDataMap))
		{

			GAME_LOG->_ADDLINETOLOG("Error : Can't load component " +
				(*iterComp).first);

			return false;

		}
		
	bool isSuccesful = true;

	// Children creation and loading
	std::map<std::string, HM_SceneObject*>::const_iterator iter;
	iter = m_children.begin();

	for (; iter != m_children.end(); iter++)
	{

		HM_SceneObject* currentChild = iter->second;
		isSuccesful = currentChild->onSetupEnd();

	}

	return isSuccesful;

}

void HM_SceneObject::onUpdateStart()
{

	// Start
	for (unsigned int i = 0; i < updateComponentList.size(); i++)
	{

		HM_Component* currentComponent = getComponent(updateComponentList[i]);

		if (currentComponent)
			if (currentComponent->isUpdatable())
				currentComponent->onUpdateStart();

	}

	//Children
	if (m_children.size() != 0U)
	{

		std::map<std::string, HM_SceneObject*>::const_iterator iterObject;
		iterObject = m_children.begin();

		for (; iterObject != m_children.end(); iterObject++)
			(*iterObject).second->onUpdateStart();

	}

}

/*		update
*
*		brief : updates all the scene objects
*
**/
void HM_SceneObject::update()
{

	// Update
	for (unsigned int i = 0; i < updateComponentList.size(); i++)
	{

		HM_Component* currentComponent = getComponent(updateComponentList[i]);

		if (currentComponent)
			if (currentComponent->isUpdatable())
				currentComponent->update();

	}

	//Children
	if (m_children.size() != 0U)
	{

		std::map<std::string, HM_SceneObject*>::const_iterator iterObject;
		iterObject = m_children.begin();

		for (; iterObject != m_children.end(); iterObject++)
			(*iterObject).second->update();

	}

}


void HM_SceneObject::onUpdateEnd()
{

	// End
	for (unsigned int i = 0; i < updateComponentList.size(); i++)
	{

		HM_Component* currentComponent = getComponent(updateComponentList[i]);

		if (currentComponent)
			if (currentComponent->isUpdatable())
				currentComponent->onUpdateEnd();

	}

	//Children
	if (m_children.size() != 0U)
	{

		std::map<std::string, HM_SceneObject*>::const_iterator iterObject;
		iterObject = m_children.begin();

		for (; iterObject != m_children.end(); iterObject++)
			(*iterObject).second->onUpdateEnd();

	}

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

	// Start
	for (; iter != m_componentMap.end(); iter++)
		if ((*iter).second->isDisplayable())
			(*iter).second->onDisplayStart();

	iter = m_componentMap.begin();

	// Display
	for (; iter != m_componentMap.end(); iter++)
		if ((*iter).second->isDisplayable())
			(*iter).second->display();

	iter = m_componentMap.begin();

	// End
	for (; iter != m_componentMap.end(); iter++)
		if ((*iter).second->isDisplayable())
			(*iter).second->onDisplayEnd();


	// Children
	if (m_children.size() != 0U)
	{

		std::map<std::string, HM_SceneObject*>::const_iterator iterObject;
		iterObject = m_children.begin();

		for (; iterObject != m_children.end(); iterObject++)
			(*iterObject).second->display();

	}

}

void HM_SceneObject::onCollision(HM_SceneObject* other, Direction direction)
{

	if (m_parent)
		m_parent->onCollision(other, direction);

	std::map<std::string, HM_Component*>::const_iterator iter;
	iter = m_componentMap.begin();

	// Start
	for (; iter != m_componentMap.end(); iter++)
		if ((*iter).second->isUpdatable())
			(*iter).second->onCollision(other, direction);

// 	if (m_children.size() > 0U)
// 	{
// 
// 		std::map<std::string, HM_SceneObject*>::const_iterator iter;
// 		iter = m_children.begin();
// 		for (;iter != m_children.end(); iter++)
// 			(*iter).second->onCollision(other, direction);
// 
// 	}

}

bool HM_SceneObject::isUpdatable() const
{

	return m_isUpdatable;

}

bool HM_SceneObject::isDisplayable() const
{

	return m_isDisplayable;

}

void HM_SceneObject::setIsUpdatable(bool isActive)
{

	if (m_isUpdatable == !isActive)
	{

		std::map<std::string, HM_Component*>::const_iterator iterComp =
			m_componentMap.begin();

			for (; iterComp != m_componentMap.end(); iterComp++)
				(*iterComp).second->setIsUpdatable(isActive);

		std::map<std::string, HM_SceneObject*>::const_iterator iterChildren =
			m_children.begin();

		for (; iterChildren != m_children.end(); iterChildren++)
			(*iterChildren).second->setIsUpdatable(isActive);

	}
		
	m_isUpdatable = isActive;

}

void HM_SceneObject::setIsDisplayable(bool isActive)
{

	if (m_isUpdatable == !isActive)
	{

		std::map<std::string, HM_Component*>::const_iterator iterComp =
			m_componentMap.begin();

		for (; iterComp != m_componentMap.end(); iterComp++)
			(*iterComp).second->setIsDisplayable(isActive);

		std::map<std::string, HM_SceneObject*>::const_iterator iterChildren =
			m_children.begin();

		for (; iterChildren != m_children.end(); iterChildren++)
			(*iterChildren).second->setIsDisplayable(isActive);

	}

	m_isDisplayable = isActive;

}

