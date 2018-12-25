/************************************************/
/*												*/
/*	HM_CameraComponent.h						*/
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

#pragma once

#include "HM_Component.h"



/*				     HEADER						*/

/*		Class HM_CameraComponent
*
*		brief : Component representing a camera
*
**/
class HM_CameraComponent : public HM_Component
{

public:
	/** Public functions **/

	HM_CameraComponent(HM_SceneObject* owner) :
		HM_Component(owner), m_name(""), m_camera(NULL), m_behavior(""),
		m_lockOnX(false), m_lockOnY(false), m_lockOnZ(false) {};

	// Create an instance of the mesh component for the factory
	HM_CameraComponent* create(HM_SceneObject* owner) const override;
	// Initialize the mesh, texture and shader of the mesh component
	virtual bool setup(std::map<std::string, void*> descr) override;

	// TODO : update mesh
	virtual void update() override;
	// display the mesh with the texture using the shader
	virtual void display() override {};


	std::string getName() const;

	HM_Camera* const getCamera() const;

private:
	/** Private variables **/

	std::string m_name;

	HM_Camera* m_camera;

	std::string m_behavior;

	bool m_updateModelview;

	bool m_lockOnX;
	bool m_lockOnY;
	bool m_lockOnZ;

};

