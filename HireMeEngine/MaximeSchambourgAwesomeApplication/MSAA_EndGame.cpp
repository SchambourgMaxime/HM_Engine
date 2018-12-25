/************************************************/
/*												*/
/*	MSAA_EndGame.cpp							*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-11-28						*/
/*												*/
/*	Last Update : 2016-11-28					*/
/*												*/
/*	Author : Eric Laberge, Maxime Schambourg	*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#include "MSAA_EndGame.h"
#include "MSAA_Character.h"
#include "../Components/HM_HUDComponent.h"



/*				 IMPLEMENTATION					*/

MSAA_EndGame* MSAA_EndGame::create(HM_SceneObject* owner) const
{

	return new MSAA_EndGame(owner);

}


bool MSAA_EndGame::setup(std::map<std::string, void*> descr)
{

	HM_SpriteComponent* sprite =
		m_owner->getComponentInChildren<HM_SpriteComponent>("sprite");

	if(sprite)
		sprite->setIsDisplayable(false);

	return true;

}

void MSAA_EndGame::update()
{

	if (m_isCameraLerping)
	{

		if (glm::distance(m_cameraLerp, m_camera->getPosition()) > 0.05f)
		{

			m_camera->setPosition(glm::vec3(
				glm::mix(m_camera->getPosition(), m_cameraLerp, 0.1f)));

		}
		else if (m_isCameraLerping)
		{

			m_isCameraLerping = false;
			m_inputActive = true;

		}

		m_camera->lookAt(HM_GameMaster::instance()->getGraphicsManager()->
			getModelview());

	}
	else if (m_inputActive)
	{

		HM_Input* input = HM_GameMaster::instance()->getInputsManager();

		if(input->getKey(SDL_SCANCODE_W))
			if(m_camera->getPosition().y < heightLimitUp)
				m_camera->setPosition(
					glm::vec3(m_camera->getPosition().x,
							  m_camera->getPosition().y + 0.01f,
							  m_camera->getPosition().z));
			else
				m_camera->setPosition(
					glm::vec3(m_camera->getPosition().x,
						heightLimitUp,
						m_camera->getPosition().z));
		else if (input->getKey(SDL_SCANCODE_S))
			if (m_camera->getPosition().y > heightLimitDown)
				m_camera->setPosition(
					glm::vec3(m_camera->getPosition().x,
						m_camera->getPosition().y - 0.01f,
						m_camera->getPosition().z));
			else
				m_camera->setPosition(
					glm::vec3(m_camera->getPosition().x,
						heightLimitDown,
						m_camera->getPosition().z));

		m_camera->lookAt(HM_GameMaster::instance()->getGraphicsManager()->
			getModelview());

	}

}

void MSAA_EndGame::onCollision(HM_SceneObject* other, Direction direction)
{

	MSAA_Character* character =
		other->getComponentInObject<MSAA_Character>("character");

	if (character)
	{

		character->stopMovement();

		std::vector<HM_HUDComponent*> huds = 
			other->getAllComponentsInChildren<HM_HUDComponent>("hud");

		for(unsigned int i = 0; i < huds.size(); i++)
			huds[i]->setIsDisplayable(false);

		HM_SpriteComponent* sprite =
			m_owner->getComponentInChildren<HM_SpriteComponent>("sprite");

		if (sprite)
			sprite->setIsDisplayable(true);

		HM_CameraComponent* cameraComp =
			other->getComponentInChildren<HM_CameraComponent>("camera");

		cameraComp->setIsUpdatable(false);
		m_camera = cameraComp->getCamera();

		m_camera->turnTowards(22.0,0);

		m_cameraLerp = m_owner->getChild("CV")->getComponentInObject<HM_TransformComponent>("transform")->getWorldPosition();
		m_cameraLerp.z += 1.17f;
		m_cameraLerp.y += 2.42f;
		m_isCameraLerping = true;

	}

}

MSAA_EndGame::~MSAA_EndGame()
{
}
