/************************************************/
/*												*/
/*	MSAA_DamageDealer.h							*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-11-22						*/
/*												*/
/*	Last Update : 2016-11-22					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#pragma once

#include "../Components/HM_Component.h"



/*				     HEADER						*/

class MSAA_DamageDealer : public HM_Component
{

public:
	/** Public functions **/

	// Constructor
	MSAA_DamageDealer(HM_SceneObject* owner) : HM_Component(owner),
	m_jumpNearCharacter(false), m_hasGravity(false), m_isGrounded(false),
	m_isBouncy(false), m_bounceStrength(20.0f), m_mass(0.03f)
	{};
	// Destructor
	~MSAA_DamageDealer();

	// Initialize the position, rotation and scale of the component
	bool setup(std::map<std::string, void*> descr) override;
	MSAA_DamageDealer* create(HM_SceneObject* owner) const override;
	virtual void update() override;
	virtual void display() override {};

	virtual void onCollision(HM_SceneObject* other, Direction direction) override;

	virtual void onActivation() override;
	virtual void onDeactivation() override;

private:
	/** Private variables **/

	bool m_jumpNearCharacter;
	bool m_hasGravity;
	bool m_isGrounded;
	bool m_isBouncy;

	float m_mass;

	float m_bounceStrength;

};

