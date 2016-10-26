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

#include "HM_BoxColliderComponent.h"
#include "HM_TransformComponent.h"
#include "HM_MeshComponent.h"



/*				 IMPLEMENTATION					*/

	// --- OVERRIDEN PUBLIC FUNCTIONS ---

/*		create
*
*		brief : create a new instance of the box collider component
*
*		params :
*			- owner	(string)	: pointer to the scene object possessing the
*								  component
*
*		return : HM_BoxColliderComponent -> pointer to the created instance
*		
**/
HM_BoxColliderComponent* HM_BoxColliderComponent::create(HM_SceneObject* owner)
const
{

	return new HM_BoxColliderComponent(owner);

}

/*		setup
*
*		brief : initialize the cube of the box collider
*
*		params :
*			- descr	(map<string, void*>)	: map of the data for components
*
**/
bool HM_BoxColliderComponent::setup(std::map<std::string, void*> descr)
{

	std::map<std::string, void*>::const_iterator iter = descr.find("collider");

	if (iter != descr.end())
		m_box = *(static_cast<HM_Cube*>((*iter).second));
	else if(m_owner->getComponent("mesh"))
		m_isBoundingBox = true;


	iter = descr.find("colliderIsTrigger");

	if (iter != descr.end())
		m_isTrigger = *(static_cast<bool*>((*iter).second));

	return true;

}


bool HM_BoxColliderComponent::onSetupEnd()
{

	if (m_isBoundingBox)
	{

		HM_Component* component = m_owner->getComponent("mesh");

		if (component)
		{

			HM_MeshComponent* meshComponent = static_cast<HM_MeshComponent*>(
				component);

			if (meshComponent)
				m_box = meshComponent->getBoundingBox();

		}

	}

	return true;

}

void HM_BoxColliderComponent::display()
{

	if(DEBUG)
	{

		HM_TransformComponent* transformComp;
		transformComp = static_cast<HM_TransformComponent*>(
			m_owner->getComponent("transform"));

		m_box.draw(transformComp->getWorldPosition());

	}

}

void HM_BoxColliderComponent::update()
{

}

// --- PUBLIC FUNCTIONS ---

/*		onCollision
*
*		brief : TODO -> check collision
*
*		params :
*			- other	(HM_BoxColliderComponent)	: other collider of the
*												  collision
*
**/
void HM_BoxColliderComponent::onCollision(HM_BoxColliderComponent const & other)
{

}