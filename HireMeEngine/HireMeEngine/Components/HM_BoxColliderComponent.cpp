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
#include "HM_MotionComponent.h"
#include "HM_MeshComponent.h"
#include "HM_SpriteComponent.h"



/*				 IMPLEMENTATION					*/

	// --- STATIC VARIABLES ---

std::vector<HM_BoxColliderComponent*> HM_BoxColliderComponent::colliders;


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
		m_originalBox = *(static_cast<HM_Cube*>((*iter).second));
	else if(m_owner->getComponent("mesh") || m_owner->getComponent("sprite"))
		m_isBoundingBox = true;


	iter = descr.find("colliderIsTrigger");

	if (iter != descr.end())
		m_isTrigger = *(static_cast<bool*>((*iter).second));

	return true;

}


bool HM_BoxColliderComponent::onSetupEnd(std::map<std::string, void*> descr)
{

	if (m_isBoundingBox)
	{

		HM_Component* component = m_owner->getComponent("mesh");

		if (component)
		{

			HM_MeshComponent* mesh = static_cast<HM_MeshComponent*>(
				component);

			if (mesh)
				m_originalBox = mesh->getBoundingBox();

		}
		else
		{

			component = m_owner->getComponent("sprite");

			if (component)
			{

				HM_SpriteComponent* sprite = static_cast<HM_SpriteComponent*>(
					component);

				if (sprite)
					m_originalBox = sprite->getBoundingBox();

			}

		}

	}

	m_transformedBox = m_originalBox;

	HM_Component* component = m_owner->getComponent("transform");

	if (component)
	{

		HM_TransformComponent* transformComponent =
			static_cast<HM_TransformComponent*>(component);

		if (transformComponent)
		{
			
			m_transformedBox = m_originalBox * transformComponent->getWorldScale();
			m_transformedBox += transformComponent->getWorldPosition();
			m_transformedBox.rotate(transformComponent->getWorldRotation());

		}

	}

	m_previousTransformedBox = m_transformedBox;

	colliders.push_back(this);

	return true;

}

void HM_BoxColliderComponent::onUpdateStart()
{
}

void HM_BoxColliderComponent::update()
{

	unsigned int linearCopyNumber = 1;
	glm::vec3 linearCopyOffset;

	HM_Component* component = m_owner->getComponent("mesh");

	if (component)
	{

		HM_MeshComponent* meshComponent = static_cast<HM_MeshComponent*>(
			component);

		if (meshComponent)
		{

			linearCopyNumber = meshComponent->getLinearCopyNumber();
			linearCopyOffset = meshComponent->getLinearcopyOffest();

		}

	}

	HM_Cube originalTransformBox = m_transformedBox;

	for (unsigned int i = 0; i < colliders.size(); i++)
		if(colliders[i] != this)
		{

			for(unsigned int j = 0; j < linearCopyNumber; j++)
			{
				
				m_transformedBox = originalTransformBox + (linearCopyOffset * (float)j);

				if (m_transformedBox.intersectsWith(colliders[i]->m_transformedBox))
					onCollision(*colliders[i]);

			}

		}

	m_transformedBox = originalTransformBox;

}

void HM_BoxColliderComponent::onUpdateEnd()
{

	recalculateTransformedBox();

}

void HM_BoxColliderComponent::display()
{

	if(DEBUG)
	{

		unsigned int linearCopyNumber = 1;
		glm::vec3 linearCopyOffset;

		HM_Component* component = m_owner->getComponent("mesh");

		if (component)
		{

			HM_MeshComponent* meshComponent = static_cast<HM_MeshComponent*>(
				component);

			if (meshComponent)
			{

				linearCopyNumber = meshComponent->getLinearCopyNumber();
				linearCopyOffset = meshComponent->getLinearcopyOffest();

			}

		}

		for (unsigned int j = 0; j < linearCopyNumber; j++)
			(m_transformedBox + linearCopyOffset * (float)j).draw();

	}

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
void HM_BoxColliderComponent::onCollision(HM_BoxColliderComponent& other)
{

	if (!m_isTrigger && !other.m_isTrigger)
	{

		HM_Component* componentTransform = other.m_owner->getComponent("transform");

		if (componentTransform)
		{

			HM_Component* componentMotion = other.m_owner->getComponent("motion");


			if (componentMotion)
			{
				
				HM_MotionComponent* motionComponent = 
					static_cast<HM_MotionComponent*>(componentMotion);

				HM_TransformComponent* transformComponent =
					static_cast<HM_TransformComponent*>(componentTransform);

				// collided with the top
				if (other.m_previousTransformedBox.getYMin() >= m_transformedBox.getYMax() &&
					other.m_transformedBox.getYMin() <= m_transformedBox.getYMax())
				{

					transformComponent->translate(
						glm::vec3(0.0f, m_transformedBox.getYMax() -
							other.m_transformedBox.getYMin(), 0.0f));

					other.m_owner->getComponent("character")->onCollision(other.m_owner, UP);

				}
				// collided with the bottom
				else if (other.m_previousTransformedBox.getYMax() <= m_transformedBox.getYMin() &&
					other.m_transformedBox.getYMax() >= m_transformedBox.getYMin())
					transformComponent->setLocalPositionY(
					(transformComponent->getLocalPosition().y -
						(glm::abs(other.m_transformedBox.getYMax() -
								  m_transformedBox.getYMin()))));

				other.recalculateTransformedBox();

			}
			
		}

	}

}

void HM_BoxColliderComponent::recalculateTransformedBox()
{

	HM_Component* component = m_owner->getComponent("transform");

	if (component)
	{

		HM_TransformComponent* transformComponent =
			static_cast<HM_TransformComponent*>(component);

		if (transformComponent)
		{

			m_transformedBox = m_originalBox * transformComponent->getWorldScale();
			m_transformedBox += transformComponent->getWorldPosition();
			m_transformedBox.rotate(transformComponent->getWorldRotation());

		}

	}

}
