/************************************************/
/*												*/
/*	HM_BoxColliderComponent.cpp					*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-08-30						*/
/*												*/
/*	Last Update : 2016-08-30					*/
/*												*/
/*	Author : Eric Laberge, Maxime Schambourg	*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#include "HM_JoystickComponent.h"



/*				 IMPLEMENTATION					*/

	// --- OVERRIDEN PUBLIC FUNCTIONS ---

/*		create
*
*		brief : create a new instance of the joystick component
*
*		params :
*			- owner	(string)	: pointer to the scene object possessing the
*								  component
*
*		return : HM_JoystickComponent -> pointer to the created instance
*		
**/
HM_JoystickComponent* HM_JoystickComponent::create(HM_SceneObject* owner) const
{

	return new HM_JoystickComponent(owner);

}

/*		setup
*
*		brief : Initialize the id and speed of the joystick
*
*		params :
*			- descr	(map<string, void*>)	: map of the data for components
*
**/
bool HM_JoystickComponent::setup(std::map<std::string, void*> descr)
{

	std::map<std::string, void*>::const_iterator iter = descr.find("joyID");

	if (iter != descr.end())
		m_id = *(static_cast<char*>((*iter).second));

	iter = descr.find("joySpeed");

	if (iter != descr.end())
		m_speed = *(static_cast<float*>((*iter).second));

	return true;

}