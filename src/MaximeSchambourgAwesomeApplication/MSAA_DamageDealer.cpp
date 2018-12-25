/************************************************/
/*												*/
/*	MSAA_DamageDealer.cpp						*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-11-24						*/
/*												*/
/*	Last Update : 2016-11-24					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#include "MSAA_DamageDealer.h"
#include "MSAA_Character.h"

#include "../Components/HM_MotionComponent.h"
#include "../Components/HM_BoxColliderComponent.h"



/*				 IMPLEMENTATION					*/

MSAA_DamageDealer::~MSAA_DamageDealer()
{
}

bool MSAA_DamageDealer::setup(std::map<std::string, void*> descr)
{

	std::map<std::string, void*>::iterator iter;
	iter = descr.find("jumpNearCharacter");

	if (iter != descr.end())
		m_jumpNearCharacter = hmu::getDataFromVoid<bool>((*iter).second);

	iter = descr.find("hasGravity");

	if (iter != descr.end())
		m_hasGravity = hmu::getDataFromVoid<bool>((*iter).second);

	iter = descr.find("isBouncy");

	if (iter != descr.end())
		m_isBouncy = hmu::getDataFromVoid<bool>((*iter).second);

	iter = descr.find("bounceStrength");

	if (iter != descr.end())
		m_bounceStrength = hmu::getDataFromVoid<float>((*iter).second);

	iter = descr.find("mass");

	if (iter != descr.end())
		m_mass = hmu::getDataFromVoid<float>((*iter).second);

	return true;

}

MSAA_DamageDealer* MSAA_DamageDealer::create(HM_SceneObject* owner) const
{

	return new MSAA_DamageDealer(owner);

}

void MSAA_DamageDealer::update()
{

	if (m_hasGravity && !m_isGrounded)
	{

		HM_MotionComponent* motion =
			m_owner->getComponentInObject<HM_MotionComponent>("motion");

		if(motion)
			motion->setTranslationVelocityY(motion->getTranslationVelocity().y
												- GRAVITY * m_mass);

	}


}

void MSAA_DamageDealer::onCollision(HM_SceneObject* other, Direction direction)
{

	MSAA_Character* character =
		other->getComponentInObject<MSAA_Character>("character");

	if (character)
	{

		character->damage();

	}
	else
	{

		HM_MotionComponent* motion =
			m_owner->getComponentInObject<HM_MotionComponent>("motion");

		if (motion)
		{

			if (m_isBouncy)
			{

				motion->setTranslationVelocityY(m_bounceStrength);

			}
			else
			{

				HM_BoxColliderComponent* otherCollider =
					other->getComponentInObject<HM_BoxColliderComponent>("boxCollider");

				if (!otherCollider->isTrigger())
				{

					m_isGrounded = true;

					motion->setTranslationVelocityY(0);

				}

			}
		}

	}

}

void MSAA_DamageDealer::onActivation()
{

}

void MSAA_DamageDealer::onDeactivation()
{

	HM_TransformComponent* transform =
		m_owner->getComponentInObject<HM_TransformComponent>("transform");

	if(transform)
		transform->reset();

	HM_MotionComponent* motion =
		m_owner->getComponentInObject<HM_MotionComponent>("motion");

	if (motion)
		motion->reset();

	m_isGrounded = false;

}
