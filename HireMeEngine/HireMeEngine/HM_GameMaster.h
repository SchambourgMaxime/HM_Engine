/************************************************/
/*												*/
/*	HM_GameMaster.h								*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-07-27						*/
/*												*/
/*	Last Update : 2016-08-20					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#pragma once

#define GAME_PATH "./GameFiles/GameMasterData.hm"

#define GAME_LOG \
	HM_GameMaster::instance()->getGameLog()

#define GRAPHICS_LOG(message) \
	HM_GameMaster::instance()->getGraphicsManager()->getGraphicsLog()->\
														 _ADDLINETOLOG(message)
#define GRAPHICS_LOG_LINE(message, line) \
	HM_GameMaster::instance()->getGraphicsManager()->getGraphicsLog()->\
		_ADDLINETOLOG_LINE(message, line)

#include "HM_Scene.h"
#include "HM_Log.h"
#include "HM_Input.h"
#include "HM_Utilities.h"
#include "HM_Constants.h"


class HM_GraphicsManager;
class HM_SoundManager;


/*				     HEADER						*/

class HM_GameMaster
{

public:
			/** Public functions **/


	// Accessor for singleton class
	static HM_GameMaster* const instance();

	// Destructor
	~HM_GameMaster();


		// --- Accessors ---

	HM_Log* const getGameLog();
	HM_GraphicsManager* const getGraphicsManager();
	HM_Input* const getInputsManager();


	// Initialization function
	bool initGame();

	// Load scenes and play them in order
	void launchGame();

private:	
			/** Private functions **/


	// Constructor
	HM_GameMaster();

	// Copy constructor (private because singleton)
	HM_GameMaster(HM_GameMaster const&);
	void operator=(HM_GameMaster const&);

	// Iter through a loop until current scene breaks it
	void gameLoop();


			/** Private Variables **/

	// Instance of singleton class
	static HM_GameMaster* m_instance;

	// List of game scenes
	std::list<HM_Scene*> m_scenesList;
	// reference to the scene currently paying
	HM_Scene* m_currentScene;

	// Log of the entire game
	HM_Log* m_pGameLog;

	// Reference to the class managing inputs
	HM_Input* m_pInputManager;
	// Reference to the class managing graphics
	HM_GraphicsManager* m_pGraphicsManager;
	// Reference to the class managing audio
	HM_SoundManager* m_pSoundManager;

};

