/************************************************/
/*												*/
/*	MSAA_Character.cpp							*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-11-11						*/
/*												*/
/*	Last Update : 2016-11-11					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#include "MSAA_Character.h"
#include "../Components/HM_MotionComponent.h"



/*				 IMPLEMENTATION					*/

// --- OVERRIDEN PUBLIC FUNCTIONS ---

/*		create
*
*		brief : create a new instance of the transform component
*
*		params :
*			- owner	(string)	: pointer to the scene object possessing the
*								  component
*
*		return : HM_TransformComponent -> pointer to the created instance
*
**/
MSAA_Character* MSAA_Character::create(HM_SceneObject* owner) const
{

	return new MSAA_Character(owner);

}

/*		setup
*
*		brief : initialize the position, rotation and scale of the component
*
*		params :
*			- descr	(map<string, void*>)	: map of the data for components
*
**/
bool MSAA_Character::setup(std::map<std::string, void*> descr)
{

	std::map<std::string, void*>::iterator iter;
	iter = descr.find("jumpStrength");

	if (iter != descr.end())
		m_jumpStrength = hmu::getDataFromVoid<float>((*iter).second);

	iter = descr.find("maxJumpStrength");

	if (iter != descr.end())
		m_maxJumpStrength = hmu::getDataFromVoid<float>((*iter).second);

	iter = descr.find("speed");

	if (iter != descr.end())
		m_speed = hmu::getDataFromVoid<float>((*iter).second);

	iter = descr.find("maxSpeed");

	if (iter != descr.end())
		m_maxSpeed = hmu::getDataFromVoid<float>((*iter).second);

	iter = descr.find("minSpeed");

	if (iter != descr.end())
		m_minSpeed = hmu::getDataFromVoid<float>((*iter).second);


	HM_Input* input = HM_GameMaster::instance()->getInputsManager();

	HM_Component* componentMotion = m_owner->getComponent("motion");

	if (componentMotion)
	{

		HM_MotionComponent* motion =
			static_cast<HM_MotionComponent*>(componentMotion);

		if(motion)
			motion->setTranslationvelocityX(m_speed);

	}

	return true;

}

void MSAA_Character::update()
{

	HM_Input* input = HM_GameMaster::instance()->getInputsManager();

	HM_Component* componentMotion = m_owner->getComponent("motion");

	if (input->getKey(SDL_SCANCODE_W) && m_isGrounded)
	{

		m_currentFallSpeed += m_jumpStrength;
		m_isGrounded = false;
		m_jumpButtonPressed = true;

	}
	else if (input->getKey(SDL_SCANCODE_W) && !m_isGrounded)
	{

		if (m_jumpButtonPressed)
		{

			m_currentFallSpeed += (GRAVITY * 0.1f) + (m_jumpStrength * 0.06f);

			if (m_currentFallSpeed >= m_maxJumpStrength)
			{

				m_currentFallSpeed = m_maxJumpStrength;
				m_jumpButtonPressed = false;

			}

		}

	}
	else if (!input->getKey(SDL_SCANCODE_W))
	{

		m_jumpButtonPressed = false;

	}
		

	if (componentMotion)
	{

		HM_MotionComponent* motion =
			static_cast<HM_MotionComponent*>(componentMotion);

		if (motion)
		{

			if (!m_isGrounded)
				m_currentFallSpeed -= GRAVITY * 0.1f;

			motion->setTranslationvelocityY(m_currentFallSpeed);
				

		}
			//motion->setTranslationvelocityX(m_speed);

	}

}

void MSAA_Character::onUpdateEnd()
{

	//m_isGrounded = false;

}

void MSAA_Character::onCollision(HM_SceneObject* other, Direction direction)
{

	if (direction == UP)
	{

		m_isGrounded = true;
		m_currentFallSpeed = 0;

	}

}

