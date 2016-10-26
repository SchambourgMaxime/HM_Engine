/************************************************/
/*												*/
/*	HM_TransformComponent.cpp					*/
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

#include "HM_TransformComponent.h"



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
HM_TransformComponent* HM_TransformComponent::create(HM_SceneObject* owner) const
{

	return new HM_TransformComponent(owner);

}

/*		setup
*
*		brief : initialize the position, rotation and scale of the component
*
*		params :
*			- descr	(map<string, void*>)	: map of the data for components
*
**/
bool HM_TransformComponent::setup(std::map<std::string, void*> descr)
{

	std::map<std::string, void*>::iterator iter;
	iter = descr.find("position");

	if (iter != descr.end())
		m_position = hmu::getDataFromVoid<glm::vec3>((*iter).second);

	m_originalPosition = m_position;


	iter = descr.find("rotation");

	if (iter != descr.end())
		m_rotation = *(static_cast<glm::vec3*>((*iter).second));

	m_originalRotation = m_rotation;


	iter = descr.find("scale");

	if (iter != descr.end())
		m_scale = *(static_cast<glm::vec3*>((*iter).second));

	m_originalScale = m_scale;

	return true;

}


	// --- PUBLIC FUNCTIONS ---

/*		reset
*
*		brief : Reset the position, rotation and scale to their original values
*
**/
void HM_TransformComponent::reset()
{

	m_position = m_originalPosition;
	m_rotation = m_originalRotation;
	m_scale = m_originalScale;

}


	// --- ACCESSORS ---

/*		getLocalPosition
*
*		return : vec3 -> position of transform without position of the parent
*
**/
glm::vec3 HM_TransformComponent::getLocalPosition() const
{

	return m_position;

}
/*		getLocalRotation
*
*		return : vec3 -> rotation of transform without rotation of the parent
*
**/
glm::vec3 HM_TransformComponent::getLocalRotation() const
{

	return m_rotation;

}
/*		getLocalScale
*
*		return : vec3 -> scale of transform without scale of the parent
*
**/
glm::vec3 HM_TransformComponent::getLocalScale() const
{

	return m_scale;

}
/*		getWorldPosition
*
*		return : vec3 -> position of transform with position of the parent
*
**/
glm::vec3 HM_TransformComponent::getWorldPosition() const
{

	if(!m_owner->isRootSceneObject())
	{

		HM_TransformComponent* parentTransform = 
			static_cast<HM_TransformComponent*>(
				m_owner->getParent()->getComponent("transform"));

		if (parentTransform)
			return (parentTransform->getWorldPosition() + getLocalPosition());

	}

	return getLocalPosition();

}
/*		getWorldRotation
*
*		return : vec3 -> rotation of transform with rotation of the parent
*
**/
glm::vec3 HM_TransformComponent::getWorldRotation() const
{

	glm::vec3 rotation(0);

	if (!m_owner->isRootSceneObject())
	{

		HM_TransformComponent* parentTransform =
			static_cast<HM_TransformComponent*>(
				m_owner->getParent()->getComponent("transform"));

		if (parentTransform)
			rotation = parentTransform->getWorldRotation() + getLocalRotation();

	}
	else
	{

		rotation = getLocalRotation();

	}

	return rotation;

}
/*		getWorldScale
*
*		return : vec3 -> scale of transform with scale of the parent
*
**/
glm::vec3 HM_TransformComponent::getWorldScale() const
{

	if (!m_owner->isRootSceneObject())
	{

		HM_TransformComponent* parentTransform =
			static_cast<HM_TransformComponent*>(
				m_owner->getParent()->getComponent("transform"));

		if (parentTransform)
			return (parentTransform->getWorldScale() * getLocalScale());

	}

	return getLocalScale();

}
/*		setLocalPosition
*
*		params :
*			- localPosition	(vec3)	: new local position
*
**/
void HM_TransformComponent::setLocalPosition(glm::vec3 const & localPosition)
{

	m_position = localPosition;

}
/*		setLocalRotation
*
*		params :
*			- localRotation	(vec3)	: new local rotation
*
**/
void HM_TransformComponent::setLocalRotation(glm::vec3 const & localRotation)
{

	m_rotation = localRotation;

}
/*		setLocalScale
*
*		params :
*			- localScale	(vec3)	: new local scale
*
**/
void HM_TransformComponent::setLocalScale(glm::vec3 const & localScale)
{

	m_scale = localScale;

}