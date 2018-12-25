/************************************************/
/*												*/
/*	HM_Component.h								*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-08-09						*/
/*												*/
/*	Last Update : 2016-08-09					*/
/*												*/
/*	Author : Eric Laberge, Maxime Schambourg	*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#pragma once

// Includes GLM

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>

#include "../HM_GameMaster.h"
#include "../HM_Mesh.h"
#include "../HM_Texture.h"
#include "../HM_GraphicsManager.h"
#include "../HM_Shader.h"

class HM_SceneObject;



/*				     HEADER						*/

/*		Class HM_Component
*
*		brief : Base class for components
*
**/
class HM_Component : public HM_Object
{

public:
	/** Public functions **/

	// Constructor
	HM_Component(HM_SceneObject* owner);

		/* -- CREATE -- */
	// Create an instance of the component for the factory
	virtual HM_Component* create(HM_SceneObject* owner) const = 0;

		/* -- SETUP -- */
	// Perform last setup when all components are setup
	virtual bool onSetupStart(std::map<std::string, void*> descr) { return true; };
	// Initialize the variables of the component
	virtual bool setup(std::map<std::string, void*> descr) = 0;
	// Perform last setup when all components are setup
	virtual bool onSetupEnd(std::map<std::string, void*> descr) { return true; };

		/* -- UPDATE -- */
	// Called before everything is updated
	virtual void onUpdateStart() {};
	// Updates the variables of a component every frame
	virtual void update() = 0;
	// Called after everything is updated
	virtual void onUpdateEnd() {};

		/* -- DISPLAY -- */
	// Called before everything is displayed
	virtual void onDisplayStart() {};
	// Displays the component if it can
	virtual void display() = 0;
	// Called after everything is displayed
	virtual void onDisplayEnd() {};


	// Behavior during collision
	virtual void onCollision(HM_SceneObject* other, Direction direction) {};


	// Object overloaded function : checks validity of object (is it usable?)
	virtual bool isValid() const override;

	bool isUpdatable() const;
	bool isDisplayable() const;

	void setIsUpdatable(bool active);
	void setIsDisplayable(bool active);

protected:

	/** Protected variables **/
	
	// Pointer to the scene object possessing the component
	HM_SceneObject* m_owner;

	virtual void onActivation() {};
	virtual void onDeactivation() {};

	bool m_isUpdatable = true;
	bool m_isDisplayable = true;

};

/*		Class HM_ComponentFactory
*
*		brief : factory class to create component
*
**/
class HM_ComponentFactory
{

public:
	/** Public functions **/

	// Constructor
	HM_ComponentFactory();

	// Creation function for components
	HM_Component* create(const std::string& key, HM_SceneObject* owner) const;

private:
	
	// Map of pointer to the available component classes
	std::map<std::string, HM_Component*> m_mapToFactoryObjects;

};
