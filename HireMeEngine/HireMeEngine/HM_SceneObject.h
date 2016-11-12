/************************************************/
/*												*/
/*	HM_SceneObject.h							*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-08-09						*/
/*												*/
/*	Last Update : 2016-08-09					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#pragma once

#include "HM_Object.h"
#include "HM_FileHandler.h"

#include <string>
#include <map>


class HM_GameMaster;
class HM_Component;



/*				     HEADER						*/

class HM_SceneObject : HM_Object
{

public:
	/** Public functions **/

	// Default Constructor
	HM_SceneObject();
	// Constructor
	HM_SceneObject(std::string name, HM_SceneObject* parent);
	//Destructor
	~HM_SceneObject();


		// --- Accessors ---

	HM_Component* getComponent(std::string componentName);

	HM_SceneObject* getParent();

	bool isRootSceneObject();


	// Check if object is valid
	virtual bool isValid() const;
	
	// Create and load every component and its data
	bool load();

	void onUpdateStart();
	void update();
	void onUpdateEnd();

	// Display all the scene objects
	void display();


	/** Public variables **/

	glm::vec3& m_position;
	glm::vec3& m_eulerAngles;
	glm::vec3& m_scale;



private:
	/** Private variables **/

	static const std::vector<std::string> updateComponentList;

	// Name of the scene object
	std::string m_name;

	// Parent scene object of this object
	HM_SceneObject* m_parent;

	// Map of all the scene object's component
	std::map<std::string, HM_Component*> m_componentMap;

	// Map of all the scene object's children
	std::map<std::string, HM_SceneObject*> m_children;

};