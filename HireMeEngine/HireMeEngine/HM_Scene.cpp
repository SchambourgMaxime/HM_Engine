/************************************************/
/*												*/
/*	HM_Scene.cpp								*/
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

#include "HM_Scene.h"
#include "HM_GameMaster.h"
#include "HM_GraphicsManager.h"


/*				 IMPLEMENTATION					*/

	// --- CONSTRUCTOR & DESTRUCTOR ---

/*		Constructor
*
*		brief : initialize the name of the scene
*
**/
HM_Scene::HM_Scene(std::string name) : m_name(name)
{
	
}

/*		Destructor
*
*		brief :
*
**/
HM_Scene::~HM_Scene()
{

	std::list<HM_SceneObject*>::const_iterator iter;
	iter = m_sceneObjectsList.begin();

	for (; iter != m_sceneObjectsList.end(); iter++)
		delete *iter;

}


	// --- ACCESSORS ---

/*		getName
*
*		return : string -> const reference to the name of the scene
*
**/
std::string const & HM_Scene::getName() const
{

	return m_name;

}


	// --- PUBLIC FUNCTIONS ---

/*		isValid
*
*		Overload : HM_Object
*
*		brief : determine if the class instance is usable or unset
*
*		return : bool -> indicates if the item can be used or not
*
**/
bool HM_Scene::isValid() const
{
	return (m_name != "");
}

/*		load
*
*		brief : Loads all the scene objects
*
**/
bool HM_Scene::load()
{

	HM_FileHandler file("./Scenes/" + m_name + ".hm", HM_FILE_READ);

	if (!file)
	{

		GAME_LOG->_ADDLINETOLOG_LINE("Error : Couldn't find file for scene \'Scenes/" + m_name + ".hm\'", __LINE__ - 5);

		return false;

	}

	std::list<std::string> listOfObjectAsStrings;
	listOfObjectAsStrings = file.getSectionWithTitle("Objects");

	std::list<std::string>::iterator iter;
	iter = listOfObjectAsStrings.begin();

	for (; iter != listOfObjectAsStrings.end(); iter++)
	{

		std::string currentObjectString = *iter;

		if(currentObjectString != "")
		{
			HM_SceneObject* newSceneObject;
			newSceneObject = new HM_SceneObject(currentObjectString, NULL);

			if (!newSceneObject->load())
			{

				GAME_LOG->_ADDLINETOLOG("Error : can't load scene object \'" +
					currentObjectString + "\'");

				return false;

			}

			m_sceneObjectsList.push_back(newSceneObject);
		}

	}

	HM_GameMaster::instance()->getGraphicsManager()->allocVBOData();

	return true;

}

/*		launchLevel
*
*		brief :
*
**/
void HM_Scene::launchLevel()
{
	


}

void HM_Scene::update()
{

	std::list<HM_SceneObject*>::const_iterator iter;
	iter = m_sceneObjectsList.begin();

	for (; iter != m_sceneObjectsList.end(); iter++)
		(*iter)->onUpdateStart();

	iter = m_sceneObjectsList.begin();

	for (; iter != m_sceneObjectsList.end(); iter++)
		(*iter)->update();

	iter = m_sceneObjectsList.begin();

	for (; iter != m_sceneObjectsList.end(); iter++)
		(*iter)->onUpdateEnd();

}

/*		display
*
*		brief : Loads all the scene objects
*
**/
void HM_Scene::display()
{

	std::list<HM_SceneObject*>::const_iterator iter;
	iter = m_sceneObjectsList.begin();

	for (; iter != m_sceneObjectsList.end(); iter++)
		(*iter)->display();

}