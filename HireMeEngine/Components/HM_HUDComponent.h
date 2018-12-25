/************************************************/
/*												*/
/*	HM_HUDComponent.h							*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-09-05						*/
/*												*/
/*	Last Update : 2016-09-28					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#pragma once

#include "HM_SpriteComponent.h"



/*				     HEADER						*/

/*		Class HM_HUDComponent
*
*		brief : Component representing a HUD element
*
**/
class HM_HUDComponent : public HM_SpriteComponent
{

public:
	/** Public functions **/

	HM_HUDComponent(HM_SceneObject* owner) :
		HM_SpriteComponent(owner) {};

	// Create an instance of the mesh component for the factory
	HM_HUDComponent* create(HM_SceneObject* owner) const override;
	// Initialize the mesh, texture and shader of the mesh component
	virtual bool setup(std::map<std::string, void*> descr) override;

	// display the mesh with the texture using the shader
	virtual void display() override;

private:
	

};

