/************************************************/
/*												*/
/*	HM_SpriteComponent.h						*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-09-05						*/
/*												*/
/*	Last Update : 2016-09-05					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#pragma once

#include "HM_MeshComponent.h"



/*				     HEADER						*/

/*		Class HM_SpriteComponent
*
*		brief : Component representing a sprite (square with spriteSheet)
*
**/
class HM_SpriteComponent : public HM_MeshComponent
{

public:
	/** Public functions **/

	// Constructor
	HM_SpriteComponent(HM_SceneObject* owner) : HM_MeshComponent(owner),
		m_size(glm::vec2(0.0f)){};

	// Create an instance of the mesh component for the factory
	HM_SpriteComponent* create(HM_SceneObject* owner) const override;
	// Initialize the mesh, texture and shader of the mesh component
	bool setup(std::map<std::string, void*> descr) override;

	virtual void update() override;

protected:
	/** Private functions **/
	
	bool setupSprite(std::string Attribute1Name,
		std::string Attribute2Name,
		std::string Attribute3Name,
		std::map<std::string, void*> descr);

	/** Private variables **/

	std::string m_name;

	bool m_isAnimated;
	
	glm::vec2 m_size;
	glm::vec2 m_pivot;

};