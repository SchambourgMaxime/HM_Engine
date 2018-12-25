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

	//recalculateTransformedBox();

	colliders.push_back(this);

	m_isStatic = !m_owner->getComponent("motion");

	return true;

}

void HM_BoxColliderComponent::onUpdateStart()
{

	m_previousTransformedBox = m_transformedBox;

	recalculateTransformedBox();

}

void HM_BoxColliderComponent::update()
{

	recalculateTransformedBox();

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
	{
		HM_BoxColliderComponent* collider = colliders[i];

		if(collider != this && collider->isUpdatable())
		{
			if(m_isStatic && collider->isStatic())
				continue;

			for (unsigned int j = 0; j < linearCopyNumber; j++)
			{

				m_transformedBox =
					originalTransformBox + (linearCopyOffset * (float)j);

				if (m_transformedBox.intersectsWith(
					collider->m_transformedBox))
					onCollision(*collider);

			}

		}

	}

	m_transformedBox = originalTransformBox;

	recalculateTransformedBox();

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

HM_Cube HM_BoxColliderComponent::getOriginalBox() const
{

	return m_originalBox;

}

bool HM_BoxColliderComponent::isTrigger() const
{

	return m_isTrigger;

}

bool HM_BoxColliderComponent::isStatic() const
{
	return m_isStatic;
}

void HM_BoxColliderComponent::setOriginalBox(HM_Cube const & box)
{

	m_originalBox = box;

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

	Direction direction = collisionDirection(
		other.m_previousTransformedBox, other.m_transformedBox);

	if (!m_isTrigger && !other.m_isTrigger)
	{

		HM_Component* componentTransform = other.m_owner->getComponent("transform");

		if (componentTransform)
		{

			HM_TransformComponent* transformComponent =
				static_cast<HM_TransformComponent*>(componentTransform);

			// collided with the top
			if (direction == UP)
				transformComponent->translate(
					glm::vec3(0.0f, m_transformedBox.getYMax() -
						other.m_transformedBox.getYMin(), 0.0f));
			// collided with the bottom
			else if (direction == DOWN)
				transformComponent->translate(
					glm::vec3(0.0f, -(other.m_transformedBox.getYMax() -
					m_transformedBox.getYMin()), 0.0f));
			else
				transformComponent->translate(
					glm::vec3(0.0f, m_transformedBox.getYMax() -
						other.m_transformedBox.getYMin(), 0.0f));

			other.recalculateTransformedBox();

		}

	}

	other.m_owner->onCollision(m_owner, direction);
	m_owner->onCollision(other.m_owner, direction);

}

Direction HM_BoxColliderComponent::collisionDirection(
	HM_Cube const & previousOther, HM_Cube const & other)
{

	if (previousOther.getYMin() >= m_transformedBox.getYMax() &&
		other.getYMin() <= m_transformedBox.getYMax())
		return UP;

	else if (previousOther.getYMax() <= m_transformedBox.getYMin() &&
			 other.getYMax() >= m_transformedBox.getYMin())
		return DOWN;

	return UNDEFINED;

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
