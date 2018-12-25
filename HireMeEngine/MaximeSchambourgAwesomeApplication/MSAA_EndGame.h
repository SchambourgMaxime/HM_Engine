/************************************************/
/*												*/
/*	MSAA_EndGame.h								*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-11-28						*/
/*												*/
/*	Last Update : 2016-11-28					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#pragma once

#include "..\Components\HM_Component.h"



/*				     HEADER						*/

class MSAA_EndGame : public HM_Component
{

public:
	/** Public functions **/

	// Constructor
	MSAA_EndGame(HM_SceneObject* owner) : HM_Component(owner),
	m_isCameraLerping(false), m_cameraLerp(glm::vec3(0.0f)){}
	~MSAA_EndGame();

	// Create an instance of the character component for the factory
	MSAA_EndGame* create(HM_SceneObject* owner) const override;

	// Initialize the position, rotation and scale of the component
	bool setup(std::map<std::string, void*> descr) override;

	virtual void update() override;

	virtual void display() override {};

	virtual void onCollision(HM_SceneObject* other, Direction direction) override;


private:

	bool m_isCameraLerping;
	bool m_inputActive;
	glm::vec3 m_cameraLerp;

	float heightLimitUp = 6.47f;
	float heightLimitDown = 4.64f;

	HM_Camera* m_camera;

};

