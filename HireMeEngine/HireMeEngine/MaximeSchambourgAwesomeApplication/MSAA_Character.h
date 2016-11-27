/************************************************/
/*												*/
/*	MSAA_Character.h							*/
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

#pragma once

#include "../Components/HM_Component.h"
#include "../Components/HM_CameraComponent.h"
#include "../Components/HM_TransformComponent.h"



/*				     HEADER						*/

class MSAA_Character : public HM_Component
{

public:
	/** Public functions **/

	// Constructor
	MSAA_Character(HM_SceneObject* owner) : HM_Component(owner),
	m_jumpStrength(0.0f), m_speed(0.0f), m_currentFallSpeed(0.0f),
	m_isGrounded(false), m_healthPoints(1), m_isInvulnerable(false),
	m_recoveryTime(0), m_inputLocked(false){}

	// Create an instance of the character component for the factory
	MSAA_Character* create(HM_SceneObject* owner) const override;

	// Initialize the position, rotation and scale of the component
	bool setup(std::map<std::string, void*> descr) override;
	bool onSetupEnd(std::map<std::string, void*> descr) override;

	virtual void onUpdateStart() override;
	virtual void update() override;
	virtual void onUpdateEnd() override;

	virtual void display() override {};

	virtual void onCollision(HM_SceneObject* other, Direction direction) override;

	void damage();

private:

	void updateSpeedBar();
	void updateLifeBar();

	void changeAnim(std::string animLabel);

	void onSlideStart();
	void onJumpStart();

	void runningBehaviour(HM_Input const & input);
	void jumpingBehaviour();
	void fallingBehaviour();

	void resurect();
	void onDeath();

	bool m_inputLocked;

	unsigned short m_healthPoints;
	unsigned short m_originalHealthPoints;
	bool m_isInvulnerable;
	Uint32 m_recoveryTime;
	Uint32 m_resurectionTime;

	float m_jumpStrength;
	float m_maxJumpStrength;
	float m_jumpButtonPressed;

	float m_speed;
	float m_maxSpeed;
	float m_minSpeed;
	float m_speedChangeRatio;

	HM_Cube m_currentCollider;

	Uint32 m_slidingTimeMilli;
	HM_Cube m_standingCollider;
	HM_Cube m_slidingCollider;

	bool m_isGrounded;

	float m_currentFallSpeed;

	CharacterState state;
	CharacterHealthState healthState;

	Uint32 m_timeSlideStart;
	Uint32 m_timeRecoveryStart;
};

