/************************************************/
/*												*/
/*	HM_CameraComponent.cpp						*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-09-13						*/
/*												*/
/*	Last Update : 2016-09-13					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#include "HM_CameraComponent.h"
#include "HM_TransformComponent.h"



/*				 IMPLEMENTATION					*/

// --- OVERRIDEN PUBLIC FUNCTIONS ---

/*		create
*
*		brief : create a new instance of the camera component
*
*		params :
*			- owner	(string)	: pointer to the scene object possessing the
*								  component
*
*		return : HM_CameraComponent -> pointer to the created instance
*
**/
HM_CameraComponent* HM_CameraComponent::create(HM_SceneObject* owner) const
{

	return new HM_CameraComponent(owner);

}

/*		setup
*
*		brief : initialize the mesh, texture and shader of the mesh component
*
*		params :
*			- descr	(map<string, void*>)	: map of the data for components
*
**/
bool HM_CameraComponent::setup(std::map<std::string, void*> descr)
{

	HM_GraphicsManager* pGraphicsManager;
	pGraphicsManager = HM_GameMaster::instance()->getGraphicsManager();

	if (!pGraphicsManager)
	{

		GAME_LOG->_ADDLINETOLOG_LINE("Error : graphics manager is not set for camera", __LINE__ - 5);

		m_camera = NULL;
		return false;

	}

	// Mesh loading
	std::map<std::string, void*>::const_iterator iter;
	iter = descr.find("cameraName");

	if (iter != descr.end())
		m_name = hmu::getDataFromVoid<std::string>((*iter).second);

	glm::vec3 posCamera(0, 0, 0);
	iter = descr.find("position");

	if (iter != descr.end())
		posCamera = hmu::getDataFromVoid<glm::vec3>((*iter).second);

	glm::vec3 targetPoint(0, 0, 1);
	iter = descr.find("cameraTarget");

	if (iter != descr.end())
		targetPoint = hmu::getDataFromVoid<glm::vec3>((*iter).second);

	float sensibility = 0.5f;
	iter = descr.find("cameraSensibility");

	if (iter != descr.end())
		sensibility = hmu::getDataFromVoid<float>((*iter).second);

	float speed = 0.5f;
	iter = descr.find("cameraSpeed");

	if (iter != descr.end())
		speed = hmu::getDataFromVoid<float>((*iter).second);

	iter = descr.find("cameraBehavior");

	if (iter != descr.end())
		m_behavior = hmu::getDataFromVoid<std::string>((*iter).second);

	iter = descr.find("lockX");

	if (iter != descr.end())
		m_lockOnX = hmu::getDataFromVoid<bool>((*iter).second);

	iter = descr.find("lockY");

	if (iter != descr.end())
		m_lockOnY = hmu::getDataFromVoid<bool>((*iter).second);

	iter = descr.find("lockZ");

	if (iter != descr.end())
		m_lockOnZ = hmu::getDataFromVoid<bool>((*iter).second);

	m_camera = pGraphicsManager->loadCamera(m_name, posCamera, targetPoint,
											sensibility, speed);

	m_updateModelview = true;

	return true;

}

void HM_CameraComponent::update()
{

	if (m_behavior == "flying")
	{

		m_camera->move(*(HM_GameMaster::instance()->getInputsManager()));
		m_updateModelview = true;

	}
	else
	{

		HM_TransformComponent* transform =
			m_owner->getComponentInObject<HM_TransformComponent>("transform");

		if (transform)
		{

			m_camera->setPosition(glm::vec3(
				m_lockOnX ? m_camera->getPosition().x : 
					transform->getWorldPosition().x,
				m_lockOnY ? m_camera->getPosition().y : 
					transform->getWorldPosition().y,
				m_lockOnZ ? m_camera->getPosition().z : 
					transform->getWorldPosition().z 
				));

			m_updateModelview = true;

		}

		m_camera->update();

	}



// 	if (m_updateModelview)
// 	{
// 
		m_camera->lookAt(HM_GameMaster::instance()->getGraphicsManager()->
			getModelview());
// 
// 		m_updateModelview = false;
// 
// 	}

}

std::string HM_CameraComponent::getName() const
{

	return m_name;

}

HM_Camera* const HM_CameraComponent::getCamera() const
{

	return m_camera;

}
