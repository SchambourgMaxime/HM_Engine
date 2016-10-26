/************************************************/
/*												*/
/*	HM_BoxColliderComponent.h					*/
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

/*		Class HM_BoxColliderComponent
*
*		brief : Component representing a hit box in the form of a cube
*
**/
class HM_BoxColliderComponent : public HM_Component
{

public:

	// Constructor
	HM_BoxColliderComponent(HM_SceneObject* owner) : HM_Component(owner),
	m_box(HM_Cube()), m_isTrigger(false), m_isBoundingBox(false) {}

	// Create an instance of the box collider component for the factory
	HM_BoxColliderComponent* create(HM_SceneObject* owner) const override;
	// Initialize the cube of the box collider
	bool setup(std::map<std::string, void*> descr) override;
	// Perform last setup when all components are setup
	virtual bool onSetupEnd() override;

	// No display for the box collider
	virtual void display() override;
	// TODO : check collision
	virtual void update() override;
	

private:

	// Behaviour during collision
	void onCollision(HM_BoxColliderComponent const & other);

	// Cube defining the hit box
	HM_Cube m_box;

	// Boolean to determine physical aspect of collider
	bool m_isTrigger;

	// Boolean to determine if the box collider is similar to the mesh bounding
	// box
	bool m_isBoundingBox;


};