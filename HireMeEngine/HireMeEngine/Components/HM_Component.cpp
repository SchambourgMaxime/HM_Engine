/************************************************/
/*												*/
/*	HM_Component.cpp							*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-08-09						*/
/*												*/
/*	Last Update : 2016-08-09					*/
/*												*/
/*	Author : Eric Laberge, Maxime Schambourg	*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#include "HM_Component.h"
#include "../HM_SceneObject.h"
#include "HM_TransformComponent.h"
#include "HM_MeshComponent.h"
#include "HM_MotionComponent.h"
#include "HM_BoxColliderComponent.h"
#include "HM_JoystickComponent.h"
#include "HM_SpriteComponent.h"
#include "HM_CameraComponent.h"
#include "HM_HUDComponent.h"

#include "../MaximeSchambourgAwesomeApplication/MSAA_Character.h"
#include "../MaximeSchambourgAwesomeApplication/MSAA_DamageDealer.h"
#include "../MaximeSchambourgAwesomeApplication/MSAA_Checkpoint.h"
#include "../MaximeSchambourgAwesomeApplication/MSAA_Activator.h"
#include "../MaximeSchambourgAwesomeApplication/MSAA_EndGame.h"


/*				 IMPLEMENTATION					*/

	// --- CONSTRUCTOR & DESTRUCTOR ---

/*		Constructor
*
*		brief : initialize the reference to the the owner of the component
*
*		params :
*			- owner	(HM_SceneObject)	: Pointer to the scene object
*										  possessing the component
*
**/
HM_Component::HM_Component(HM_SceneObject* owner) : m_owner(owner)
{
}

/*		isValid
*
*		overload : HM_Object
*
*		brief : determine if the class instance is usable or unset
*
*		return : bool -> indicates if the item can be used or not
*
**/
bool HM_Component::isValid() const
{

	return m_owner != NULL;

}

bool HM_Component::isUpdatable() const
{

	return m_isUpdatable;

}

bool HM_Component::isDisplayable() const
{

	return m_isDisplayable;

}

void HM_Component::setIsUpdatable(bool active)
{

	if (!m_isUpdatable && active)
		onActivation();
	else if (m_isUpdatable && !active)
		onDeactivation();

	m_isUpdatable = active;

}

void HM_Component::setIsDisplayable(bool active)
{

	m_isDisplayable = active;

}

/*		create
*
*		brief : creation function for components
*
*		params :
*			- key	(string)			: name of the component to create
*			- owner	(HM_SceneObject)	: scene object owner of the new
*										  component
*
*		return : HM_Component -> pointer to the created component
*		
**/
HM_Component* HM_ComponentFactory::create(const std::string& key, 
HM_SceneObject* owner) const
{

	HM_Component* temp = NULL;
	std::map<std::string, HM_Component*>::const_iterator iter;
	iter = m_mapToFactoryObjects.find(key);

	if (iter != m_mapToFactoryObjects.end())
		temp = ((*iter).second)->create(owner);

	return temp;

}


/*		Constructor
*
*		brief : fills the map of pointer to the available component classes
*
**/
HM_ComponentFactory::HM_ComponentFactory()
{

	m_mapToFactoryObjects["transform"] = new HM_TransformComponent(NULL);
	m_mapToFactoryObjects["mesh"] = new HM_MeshComponent(NULL);
	//m_mapToFactoryObjects["material"] = new HM_MaterialComponent(NULL);
	m_mapToFactoryObjects["motion"] = new HM_MotionComponent(NULL);
	m_mapToFactoryObjects["boxCollider"] = new HM_BoxColliderComponent(NULL);
	m_mapToFactoryObjects["joystick"] = new HM_JoystickComponent(NULL);
	m_mapToFactoryObjects["sprite"] = new HM_SpriteComponent(NULL);
	m_mapToFactoryObjects["camera"] = new HM_CameraComponent(NULL);
	m_mapToFactoryObjects["hud"] = new HM_HUDComponent(NULL);

	m_mapToFactoryObjects["character"] = new MSAA_Character(NULL);
	m_mapToFactoryObjects["damageDealer"] = new MSAA_DamageDealer(NULL);
	m_mapToFactoryObjects["checkpoint"] = new MSAA_Checkpoint(NULL);
	m_mapToFactoryObjects["activator"] = new MSAA_Activator(NULL);
	m_mapToFactoryObjects["endGame"] = new MSAA_EndGame(NULL);
}