/************************************************/
/*												*/
/*	MSAA_Activator.cpp							*/
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

#include "MSAA_Activator.h"
#include "MSAA_DamageDealer.h"
#include "MSAA_Character.h"



MSAA_Activator::~MSAA_Activator()
{
}

MSAA_Activator* MSAA_Activator::create(HM_SceneObject* owner) const
{

	return new MSAA_Activator(owner);

}

bool MSAA_Activator::setup(std::map<std::string, void*> descr)
{

	std::map<std::string, void*>::iterator iter;
	iter = descr.find("activatedTime");

	if (iter != descr.end())
		m_activatedTime = hmu::getDataFromVoid<Uint32>((*iter).second);

	return true;

}

bool MSAA_Activator::onSetupEnd(std::map<std::string, void*> descr)
{

	std::map<std::string, HM_SceneObject*> children =
		m_owner->getChildren();
	std::map<std::string, HM_SceneObject*>::const_iterator iter =
		children.begin();

	for(; iter != children.end(); iter++)
		(*iter).second->setIsUpdatable(false);

	return true;

}

void MSAA_Activator::update()
{

	if (m_activatedTime != 0 && m_isActivated)
	{

		Uint32 time = SDL_GetTicks();
		Uint32 deltaTime = time - m_activationStart;

		if (deltaTime > m_activatedTime)
		{

			std::map<std::string, HM_SceneObject*> children =
				m_owner->getChildren();
			std::map<std::string, HM_SceneObject*>::const_iterator iter =
				children.begin();

			for (; iter != children.end(); iter++)
				(*iter).second->setIsUpdatable(false);

			m_isActivated = false;
			m_activationStart = 0;

		}

	}

}

void MSAA_Activator::onCollision(HM_SceneObject* other, Direction direction)
{

	MSAA_Character* character =
		other->getComponentInObject<MSAA_Character>("character");

	if(character)
	{

		std::map<std::string, HM_SceneObject*> children =
			m_owner->getChildren();
		std::map<std::string, HM_SceneObject*>::const_iterator iter =
			children.begin();

		for (; iter != children.end(); iter++)
			(*iter).second->setIsUpdatable(true);

		m_isActivated = true;
		m_activationStart = SDL_GetTicks();

	}

}

