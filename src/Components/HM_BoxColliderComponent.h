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

#include <vector>



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
	m_originalBox(HM_Cube()), m_isTrigger(false), m_isBoundingBox(false) {}

	// Create an instance of the box collider component for the factory
	HM_BoxColliderComponent* create(HM_SceneObject* owner) const override;
	// Initialize the cube of the box collider
	bool setup(std::map<std::string, void*> descr) override;
	// Perform last setup when all components are setup
	virtual bool onSetupEnd(std::map<std::string, void*> descr) override;


	// TODO : check collision
	virtual void onUpdateStart() override;
	virtual void update() override;
	virtual void onUpdateEnd() override;

	// No display for the box collider
	virtual void display() override;

	HM_Cube getOriginalBox() const;

	bool isTrigger() const;
	bool isStatic() const;

	void setOriginalBox(HM_Cube const & box);

private:

	static std::vector<HM_BoxColliderComponent*> colliders;

	// Behavior during collision
	void onCollision(HM_BoxColliderComponent& other);

	Direction collisionDirection(HM_Cube const & previousOther, HM_Cube const & other);

	void recalculateTransformedBox();

	// Cube defining the original hit box
	HM_Cube m_originalBox;


	// Cube defining the transformed hit box
	HM_Cube m_transformedBox;
	// Cube defining the previous transformed hit box
	HM_Cube m_previousTransformedBox;

	std::vector<HM_Cube> m_additionalTransformedBoxes;
	std::vector<HM_Cube> m_additionalpreviousTransformedBoxes;


	// Boolean to determine physical aspect of collider
	bool m_isTrigger;

	// Boolean to determine if the box collider is similar to the mesh bounding
	// box
	bool m_isBoundingBox;

	bool m_isStatic;

};