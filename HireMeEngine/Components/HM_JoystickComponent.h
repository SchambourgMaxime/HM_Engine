/************************************************/
/*												*/
/*	HM_JoystickComponent.h						*/
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

#pragma once

#include "HM_Component.h"



/*				     HEADER						*/

/*		Class HM_JoystickComponent
*
*		brief : Component representing the input interpretation
*
**/
class HM_JoystickComponent : public HM_Component
{

public:

	// Constructor
	HM_JoystickComponent(HM_SceneObject* owner) : HM_Component(owner) {}

	// Create an instance of the joystick component for the factory
	HM_JoystickComponent* create(HM_SceneObject* owner) const override;
	// Initialize the id and speed of the joystick
	bool setup(std::map<std::string, void*> descr) override;

	// No display for the joystick
	virtual void display() override {};
	// TODO : update for the joystick
	virtual void update() override {};

private:

	// identification of the joystick component
	char m_id;
	// speed of the joystick component
	float m_speed;

};