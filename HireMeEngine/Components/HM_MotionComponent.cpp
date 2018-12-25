/************************************************/
/*												*/
/*	HM_MotionComponent.cpp						*/
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

#include "HM_MotionComponent.h"
#include "HM_TransformComponent.h"



/*				 IMPLEMENTATION					*/

	// --- OVERRIDEN PUBLIC FUNCTIONS ---

/*		create
*
*		brief : create a new instance of the motion component
*
*		params :
*			- owner	(string)	: pointer to the scene object possessing the
*								  component
*
*		return : HM_MotionComponent -> pointer to the created instance
*
**/
HM_MotionComponent* HM_MotionComponent::create(HM_SceneObject* owner) const
{

	return new HM_MotionComponent(owner);

}

/*		setup
*
*		brief : initialize the velocity of the motion component
*
*		params :
*			- descr	(map<string, void*>)	: map of the data for components
*
**/
bool HM_MotionComponent::setup(std::map<std::string, void*> descr)
{

	// Translation
	std::map<std::string, void*>::const_iterator iter = descr.find(
		"translateVelocity");

	if (iter != descr.end())
		m_translationVelocity = *(static_cast<glm::vec3*>((*iter).second));

	m_originalTranslationVelocity = m_translationVelocity;

	// Rotation
	iter = descr.find("rotationVelocity");

	if (iter != descr.end())
		m_rotationVelocity = *(static_cast<glm::vec3*>((*iter).second));

	m_originalRotationVelocity = m_rotationVelocity;

	// Scale
	iter = descr.find("scaleVelocity");

	if (iter != descr.end())
		m_scalingVelocity = *(static_cast<glm::vec3*>((*iter).second));

	m_originalScalingVelocity = m_scalingVelocity;

	m_timeLastFrame = SDL_GetTicks();

	return true;

}

void HM_MotionComponent::onUpdateStart()
{

	if (m_translationVelocity.length() != 0 ||
		m_rotationVelocity.length() != 0 ||
		m_scalingVelocity.length() != 0)
	{

		Uint32 time = SDL_GetTicks();

		Uint32 deltaTime = time - m_timeLastFrame;

		if (deltaTime > 0)
		{

			// TODO clean that
			HM_TransformComponent* transform =
				static_cast<HM_TransformComponent*>(
					m_owner->getComponent("transform"));

			if (m_translationVelocity.length() != 0)
			{

				m_velocity = (m_translationVelocity * ((deltaTime / 1000.0f)));

				transform->translate(m_velocity);

			}

			if (m_rotationVelocity.length() != 0)
				transform->rotate(
				(m_rotationVelocity * (deltaTime / 1000.0f)));

			if (m_scalingVelocity.length() != 0)
				transform->resize(
				(m_scalingVelocity * (deltaTime / 1000.0f)));

		}

		m_timeLastFrame = time;

	}

}

void HM_MotionComponent::onActivation()
{

	m_timeLastFrame = SDL_GetTicks();

}

void HM_MotionComponent::reset()
{

	resetTranslationVelocity();
	resetRotationVelocity();
	resetScalingVelocity();

}

void HM_MotionComponent::resetTranslationVelocity()
{

	m_translationVelocity = m_originalTranslationVelocity;

}

void HM_MotionComponent::resetRotationVelocity()
{

	m_rotationVelocity = m_originalRotationVelocity;

}

void HM_MotionComponent::resetScalingVelocity()
{

	m_scalingVelocity = m_originalScalingVelocity;

}

glm::vec3 HM_MotionComponent::getTranslationVelocity() const
{

	return m_translationVelocity;

}

void HM_MotionComponent::setTranslationVelocityX(float translationVelocityX)
{

	m_translationVelocity.x = translationVelocityX;

}

void HM_MotionComponent::setTranslationVelocityY(float translationVelocityY)
{

	m_translationVelocity.y = translationVelocityY;

}

void HM_MotionComponent::setTranslationVelocityZ(float translationVelocityZ)
{

	m_translationVelocity.z = translationVelocityZ;

}

void HM_MotionComponent::setTranslationVelocity(glm::vec3 translationVelocity)
{

	m_translationVelocity = translationVelocity;

}

void HM_MotionComponent::setRotationvelocity(glm::vec3 rotationVelocity)
{

	m_rotationVelocity = rotationVelocity;

}

void HM_MotionComponent::setScalevelocity(glm::vec3 scalingVelocity)
{

	m_scalingVelocity = scalingVelocity;

}

