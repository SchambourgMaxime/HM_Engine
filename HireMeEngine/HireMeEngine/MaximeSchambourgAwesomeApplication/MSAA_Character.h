/************************************************/
/*												*/
/*	MSAA_Character.h							*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-11-10						*/
/*												*/
/*	Last Update : 2016-11-10					*/
/*												*/
/*	Author : Maxime Schambourg	*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#pragma once

#include "../Components/HM_Component.h"



/*				     HEADER						*/

class MSAA_Character : public HM_Component
{

public:

	// Constructor
	MSAA_Character(HM_SceneObject* owner) : HM_Component(owner),
	m_jumpStrength(0.0f), m_speed(0.0f), m_currentFallSpeed(0.0f),
	m_isGrounded(false) {}

	// Create an instance of the transform component for the factory
	MSAA_Character* create(HM_SceneObject* owner) const override;
	// Initialize the position, rotation and scale of the component
	bool setup(std::map<std::string, void*> descr) override;

	// No display for the transform component
	virtual void display() override {};
	// No update for the transform component
	virtual void update() override;

	virtual void onUpdateEnd() override;

	virtual void onCollision(HM_SceneObject* other, Direction direction) override;

private:

	float m_jumpStrength;
	float m_maxJumpStrength;
	float m_jumpButtonPressed;

	float m_speed;
	float m_maxSpeed;
	float m_minSpeed;

	bool m_isGrounded;

	float m_currentFallSpeed;
};

