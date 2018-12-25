/************************************************/
/*												*/
/*	HM_Scene.h									*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-07-27						*/
/*												*/
/*	Last Update : 2016-08-09					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#pragma once

#include "HM_SceneObject.h"
#include "HM_FileHandler.h"
#include "HM_Object.h"

#include <list>
#include <string.h>

class HM_GameMaster;



/*				     HEADER						*/

/*		Class HM_Scene
*
*		brief : Represents a scene or a "level"
*
**/
class HM_Scene : public HM_Object
{

public:
	/** Public functions **/

	// Constructor
	HM_Scene(std::string name);
	// Destructor
	~HM_Scene();

	// Object overloaded function : checks validity of object (is it usable?)
	virtual bool isValid() const;


		// --- Accessors ---

	std::string const & getName() const;


	// Loads all the scene objects 
	bool load();
	// 
	void launchLevel();
	// Calls the display functions of scene objects
	void display();

	void update();

private:
	/** Private variables **/

	// Name of the scene (also name of the file containing the scene 
	// description)
	std::string m_name;

	// list of objects in the scene
	std::list<HM_SceneObject*> m_sceneObjectsList;

};

