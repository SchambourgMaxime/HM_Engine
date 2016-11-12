/************************************************/
/*												*/
/*	HM_MotionComponent.h						*/
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

/*		Class HM_MotionComponent
*
*		pre : need a transform component
*
*		brief : Component representing a movement in a scene object
*
**/
class HM_MotionComponent : public HM_Component
{

public:

	// Constructor
	HM_MotionComponent(HM_SceneObject* owner) : HM_Component(owner) {}

	// Create an instance of the motion component for the factory
	HM_MotionComponent* create(HM_SceneObject* owner) const override;
	// Initialize the velocity of the motion component
	bool setup(std::map<std::string, void*> descr) override;

	// TODO : update position based on motion
	virtual void onUpdateStart() override;
	// TODO : update position based on motion
	virtual void update() override {};

	// No display for the box collider
	virtual void display() override {};


	glm::vec3 getVelocity() const;

	void setTranslationvelocityX(float translationVelocityX);
	void setTranslationvelocityY(float translationVelocityY);
	void setTranslationvelocityZ(float translationVelocityZ);

	void setTranslationvelocity(glm::vec3 translationVelocity);
	void setRotationvelocity(glm::vec3 rotationVelocity);
	void setScalevelocity(glm::vec3 scaleVelocity);

private:

	// Velocity of the motion applied
	glm::vec3 m_translationVelocity;
	// Velocity of the motion when the component was setup
	glm::vec3 m_originalTranslationVelocity;

	// Velocity of the motion applied
	glm::vec3 m_rotationVelocity;
	// Velocity of the motion when the component was setup
	glm::vec3 m_originalRotationVelocity;

	// Velocity of the motion applied
	glm::vec3 m_scalingVelocity;
	// Velocity of the motion when the component was setup
	glm::vec3 m_originalScalingVelocity;

	glm::vec3 m_velocity;

	// Time in milliseconds of last frame
	Uint32 m_timeLastFrame;

};
