/************************************************/
/*												*/
/*	MSAA_Character.cpp							*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-11-11						*/
/*												*/
/*	Last Update : 2016-11-24					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#include "MSAA_Character.h"
#include "../Components/HM_MotionComponent.h"
#include "../Components/HM_SpriteComponent.h"
#include "../Components/HM_HUDComponent.h"
#include "../Components/HM_BoxColliderComponent.h"
#include "../HM_Sprite.h"
#include "../HM_SoundManager.h"



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

	iter = descr.find("speedChangeRatio");

	if (iter != descr.end())
		m_speedChangeRatio = hmu::getDataFromVoid<float>((*iter).second);

	iter = descr.find("slidingTime");

	if (iter != descr.end())
		m_slidingTimeMilli = hmu::getDataFromVoid<Uint32>((*iter).second);

	iter = descr.find("standingCollider");

	if (iter != descr.end())
		m_standingCollider = hmu::getDataFromVoid<HM_Cube>((*iter).second);

	iter = descr.find("slidingCollider");

	if (iter != descr.end())
		m_slidingCollider = hmu::getDataFromVoid<HM_Cube>((*iter).second);

	iter = descr.find("healthPoints");

	if (iter != descr.end())
		m_healthPoints = hmu::getDataFromVoid<unsigned short>((*iter).second);

	m_originalHealthPoints = m_healthPoints;

	iter = descr.find("recoveryTime");

	if (iter != descr.end())
		m_recoveryTime = hmu::getDataFromVoid<Uint32>((*iter).second);

	iter = descr.find("resurectionTime");

	if (iter != descr.end())
		m_resurectionTime = hmu::getDataFromVoid<Uint32>((*iter).second);

	soundManager = HM_GameMaster::instance()->getSoundManager();
	soundManager->loadSFX("jump3.wav");
	soundManager->loadSFX("crash.wav");

	HM_Component* componentMotion = m_owner->getComponent("motion");

	if (componentMotion)
	{

		HM_MotionComponent* motion =
			static_cast<HM_MotionComponent*>(componentMotion);

		if(motion)
			motion->setTranslationVelocityX(m_speed);

	}

	m_timeSlideStart = SDL_GetTicks();
	m_timeRecoveryStart = SDL_GetTicks();

	return true;

}

bool MSAA_Character::onSetupEnd(std::map<std::string, void*> descr)
{

	HM_BoxColliderComponent* boxCollider =
		m_owner->getComponentInObject<HM_BoxColliderComponent>("boxCollider");

	boxCollider->setOriginalBox(m_standingCollider);

	updateLifeBar();

	return true;

}

void MSAA_Character::onUpdateStart()
{
}

void MSAA_Character::update()
{

	HM_Input* input = HM_GameMaster::instance()->getInputsManager();

	bool updateSpeed = false;

	Uint32 time = SDL_GetTicks();
	Uint32 deltaTimeSliding = time - m_timeSlideStart;

	switch (state)
	{

		case IDLE :
			if (m_speed > 0 || input->getKey(SDL_SCANCODE_D))
			{

				changeAnim("run");
				state = RUNNING;

			}
			break;

		case RUNNING :
			
			runningBehaviour(*input);

			if (m_speed == 0.0f)
			{

				changeAnim("idle");
				state = IDLE;

			}
			else if (input->getKey(SDL_SCANCODE_W) && !m_inputLocked)
			{

				soundManager->playSFX("jump3.wav");
				changeAnim("jump");
				state = JUMPING;

				onJumpStart();

			}
			else if (!m_isGrounded)
			{

				changeAnim("jump");
				state = FALLING;

			}
			else if (input->getKey(SDL_SCANCODE_S) && !m_inputLocked)
			{

				changeAnim("slide");
				state = SLIDING;

				onSlideStart();

			}
			break;

		case JUMPING :
			if (input->getKey(SDL_SCANCODE_W))
			{

				jumpingBehaviour();

				if (m_currentFallSpeed >= m_maxJumpStrength)
				{

					m_currentFallSpeed = m_maxJumpStrength;
					state = FALLING;

				}

			}
			else
			{

				state = FALLING;

			}

		case FALLING :

			fallingBehaviour();

			if (m_isGrounded)
			{

				changeAnim("run");
				state = RUNNING;

			}
			break;

		case SLIDING :


			if (deltaTimeSliding > m_slidingTimeMilli)
			{

				HM_BoxColliderComponent* collider =
					m_owner->getComponentInObject<HM_BoxColliderComponent>("boxCollider");

				collider->setOriginalBox(m_standingCollider);

				changeAnim("run");
				state = RUNNING;

			}
			break;

	}

	time = SDL_GetTicks();
	Uint32 deltaTimeRecovery = time - m_timeRecoveryStart;

	HM_SpriteComponent* sprite =
		m_owner->getComponentInObject<HM_SpriteComponent>("sprite");

	HM_MotionComponent* motion =
		m_owner->getComponentInObject<HM_MotionComponent>("motion");

	switch (healthState)
	{

		case SAFE :
			break;

		case RECOVERING:

			if ((deltaTimeRecovery / 200) % 2)
			{

				sprite->setIsDisplayable(false);

			}
			else
			{

				sprite->setIsDisplayable(true);

			}
			if (deltaTimeRecovery > m_recoveryTime)
			{

				sprite->setIsDisplayable(true);
				healthState = SAFE;
				changeAnim("run");

			}
			break;

		case DEAD :
			
			if(m_speed > 0.1)
			{

				m_speed -= m_speed * 0.1f;
				motion->setTranslationVelocityX(m_speed);

			}
			else
			{
				
				changeAnim("death");
				m_speed = 0;
				motion->setTranslationVelocityX(m_speed);

			}

			if (deltaTimeRecovery > m_resurectionTime)
			{

				m_speed = m_minSpeed + (m_maxSpeed - m_minSpeed) / 2;

				sprite->setIsDisplayable(true);
				resurect();
				healthState = SAFE;

			}
			break;

		default :
			break;
	}

}

void MSAA_Character::onUpdateEnd()
{

	m_isGrounded = false;

}

void MSAA_Character::onCollision(HM_SceneObject* other, Direction direction)
{

	HM_BoxColliderComponent* otherCollider =
		other->getComponentInObject<HM_BoxColliderComponent>("boxCollider");

	if (!otherCollider->isTrigger() && direction == UP)
	{

		m_isGrounded = true;
		m_currentFallSpeed = 0;

		HM_TransformComponent* transform = 
			m_owner->getComponentInObject<HM_TransformComponent>("transform");

		HM_CameraComponent* camera = 
			m_owner->getComponentInChildren<HM_CameraComponent>("camera");

		HM_TransformComponent* cameraTransform =
			m_owner->getComponentInChild<HM_TransformComponent>("transform",
				"camera");

		if (camera && cameraTransform && transform)
		{

			glm::vec3 cameraPosition = cameraTransform->getLocalPosition();

			camera->getCamera()->lerpTo(glm::vec3(
				camera->getCamera()->getPosition().x,
				cameraTransform->getLocalPosition().y +
				transform->getWorldPosition().y,
				camera->getCamera()->getPosition().z), 0.1f);

		}

	}
	else if (!otherCollider->isTrigger() && direction == DOWN)
	{

		m_currentFallSpeed = 0;
		m_jumpButtonPressed = false;

	}

}

void MSAA_Character::damage()
{

	if(healthState == SAFE)
	{

		m_healthPoints--;

		updateLifeBar();

		soundManager->playSFX("crash.wav");

		if (m_healthPoints == 0)
		{

			onDeath();
			return;

		}

		m_timeRecoveryStart = SDL_GetTicks();
		healthState = RECOVERING;

		changeAnim("hurt");

	}

}

void MSAA_Character::stopMovement()
{

	m_speed = 0;

}

void MSAA_Character::updateSpeedBar()
{

	HM_HUDComponent* hudComponent =
		m_owner->getComponentInChild<HM_HUDComponent>(
			"hud", "HUDspeedBar");

	if (hudComponent)
	{

		HM_Mesh* mesh = hudComponent->getMesh();

		if (mesh)
		{

			HM_Sprite* sprite = static_cast<HM_Sprite*>(mesh);

			if (sprite)
				sprite->crop(1 - (m_maxSpeed - m_speed) /
				(m_maxSpeed - m_minSpeed), RIGHT);

		}

	}

}

void MSAA_Character::updateLifeBar()
{

	HM_HUDComponent* life =
		m_owner->getComponentInChild<HM_HUDComponent>("hud", "HUDlifeBar");

	if(life)
		life->setLinearCopyNumber(m_healthPoints);

}

void MSAA_Character::changeAnim(std::string animLabel)
{

	HM_SpriteComponent* spriteComponent =
		m_owner->getComponentInObject<HM_SpriteComponent>("sprite");

	HM_Mesh* mesh = spriteComponent->getMesh();

	if (mesh)
	{

		HM_Sprite* sprite = static_cast<HM_Sprite*>(mesh);

		sprite->playAnim(animLabel);

	}

}

void MSAA_Character::onSlideStart()
{

	m_timeSlideStart = SDL_GetTicks();

	HM_BoxColliderComponent* collider =
		m_owner->getComponentInObject<HM_BoxColliderComponent>("boxCollider");

	collider->setOriginalBox(m_slidingCollider);

}

void MSAA_Character::onJumpStart()
{

	m_currentFallSpeed += m_jumpStrength;
	m_isGrounded = false;
	m_jumpButtonPressed = true;

	HM_MotionComponent* motion =
		m_owner->getComponentInObject<HM_MotionComponent>("motion");

	motion->setTranslationVelocityY(m_currentFallSpeed);

}

void MSAA_Character::runningBehaviour(HM_Input const & input)
{

	if(!m_inputLocked)
	{

		if (input.getKey(SDL_SCANCODE_D) && m_speed < m_maxSpeed)
		{

			m_speed += (m_maxSpeed - m_minSpeed) / m_speedChangeRatio;

			if (m_speed > m_maxSpeed)
				m_speed = m_maxSpeed;

		}
		if (input.getKey(SDL_SCANCODE_A) && m_speed > m_minSpeed)
		{

			m_speed -= (m_maxSpeed - m_minSpeed) / m_speedChangeRatio;

			if (m_speed < m_minSpeed)
				m_speed = m_minSpeed;

		}

		HM_MotionComponent* motion =
			m_owner->getComponentInObject<HM_MotionComponent>("motion");

		if (motion)
			motion->setTranslationVelocityX(m_speed);

		HM_SpriteComponent* spriteComponent =
			m_owner->getComponentInObject<HM_SpriteComponent>("sprite");

		HM_Sprite* sprite = static_cast<HM_Sprite*>(spriteComponent->getMesh());

		sprite->setAnimSpeed(((m_maxSpeed - m_speed) / (m_maxSpeed - m_minSpeed))
			+ 0.70f);

		updateSpeedBar();

	}

}

void MSAA_Character::jumpingBehaviour()
{

	m_currentFallSpeed += (m_jumpStrength * 0.15f);

	HM_MotionComponent* motion =
		m_owner->getComponentInObject<HM_MotionComponent>("motion");

	motion->setTranslationVelocityY(m_currentFallSpeed);

}

void MSAA_Character::fallingBehaviour()
{

	HM_MotionComponent* motion =
		m_owner->getComponentInObject<HM_MotionComponent>("motion");

	if (motion)
	{
		
		m_currentFallSpeed -= GRAVITY * 0.1f;

		motion->setTranslationVelocityY(m_currentFallSpeed);

	}

	if(m_currentFallSpeed < 0)
	{

		HM_CameraComponent* cameraComponent =
			m_owner->getComponentInChildren<HM_CameraComponent>("camera");

		HM_TransformComponent* transform =
			m_owner->getComponentInChildren<HM_TransformComponent>("transform");

		HM_TransformComponent* cameraTransform =
			m_owner->getComponentInChild<HM_TransformComponent>("transform",
				"camera");

		HM_Camera* camera = cameraComponent->getCamera();

		if (camera && transform->getWorldPosition().y < camera->getPosition().y - 3.2f)
		{

			camera->lerpTo(glm::vec3(
				camera->getPosition().x,
				(cameraTransform->getLocalPosition().y +
				transform->getWorldPosition().y),
				camera->getPosition().z), 0.8f);

		}

	}

}

void MSAA_Character::resurect()
{

	HM_TransformComponent* transform =
		m_owner->getComponentInObject<HM_TransformComponent>("transform");

	transform->reset();

	m_healthPoints = m_originalHealthPoints;
	updateLifeBar();

	HM_BoxColliderComponent* collider =
		m_owner->getComponentInObject<HM_BoxColliderComponent>("boxCollider");

	collider->setOriginalBox(m_standingCollider);

	m_inputLocked = false;

}

void MSAA_Character::onDeath()
{

	m_timeRecoveryStart = SDL_GetTicks();
	healthState = DEAD;

	changeAnim("preDeath");

	m_inputLocked = true;

}

