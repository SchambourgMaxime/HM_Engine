/************************************************/
/*												*/
/*	HM_GameMaster.cpp							*/
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

#include "HM_GameMaster.h"
#include "HM_GraphicsManager.h"
#include "HM_SoundManager.h"



/*				 IMPLEMENTATION					*/

	// --- STATIC VARIABLES ---

HM_GameMaster* HM_GameMaster::m_instance = nullptr;


	// --- CONSTRUCTOR & DESTRUCTOR ---

/*		Constructor
*
*		brief :
*
**/
HM_GameMaster::HM_GameMaster()
{

	m_pGameLog = new HM_Log("Game");
	m_pInputManager = new HM_Input();
	m_pGraphicsManager = new HM_GraphicsManager("HM_Engine", 1200, 600);
	m_pSoundManager = new HM_SoundManager();

}

/*		Destructor
*
*		brief :
*
**/
HM_GameMaster::~HM_GameMaster()
{
	m_pGameLog->flush();

	delete m_pGameLog;
	delete m_pGraphicsManager;
}


	// --- ACCESSORS ---

/*		getGameLog
*
*		return : HM_Log -> constant pointer to the main log of the game
*
**/
HM_Log* const HM_GameMaster::getGameLog()
{

	return m_pGameLog;

}

/*		getGraphicsManager
*
*		return : HM_GraphicsManager -> constant pointer to the graphics manager
*									   of the game
*
**/
HM_GraphicsManager* const HM_GameMaster::getGraphicsManager()
{

	return m_pGraphicsManager;

}

/*		getInputsManager
*
*		return : HM_InputManager -> constant pointer to the inputs manager of
*									the game
*
**/
HM_Input* const HM_GameMaster::getInputsManager()
{

	return m_pInputManager;

}


	// --- PUBLIC CLASS FUNCTIONS ---

/*		instance
 *		
 *		brief : create instance of singleton if uninitialized.
 *
 *		return : HM_GameMaster -> pointer to the only instance of class
 **/
HM_GameMaster* const  HM_GameMaster::instance()
{

	if(m_instance == nullptr)
		m_instance = new HM_GameMaster();

	return m_instance;

}

/*		initGame
*
*		brief : Initialize SDL and OpenGL components, as well as
*				create the different managers for the game.
*
*		return : bool -> assessment of the initialization success
**/
bool HM_GameMaster::initGame()
{

	HM_FileHandler gameFile(GAME_PATH, HM_FILE_READ);

	if(!gameFile)
	{
		
		GAME_LOG->_ADDLINETOLOG_LINE("Error : couldn't open the file handler for the main game data", __LINE__ - 5);
		return false;

	}

	std::list<std::string> sceneListAsStrings;
	sceneListAsStrings = gameFile.getSectionWithTitle("Scenes");

	std::list<std::string>::iterator iter;
	iter = sceneListAsStrings.begin();

	for (; iter != sceneListAsStrings.end(); iter++)
	{

		std::string currentSceneString = *iter;
		HM_Scene* newScene = new HM_Scene(currentSceneString);

		m_scenesList.push_back(newScene);

	}

	if (!m_pGraphicsManager->init())
		return false;

	return true;

}

/*		launchGame
*
*		brief : Load scenes and play them in order
*
**/
void HM_GameMaster::launchGame()
{

	std::list<HM_Scene*>::iterator iter;

	for (iter = m_scenesList.begin(); iter != m_scenesList.end(); iter++)
	{
		
		HM_Scene* currentScene = *iter;

		if(!currentScene->load())
		{

			m_pGameLog->_ADDLINETOLOG_LINE("Error : Can't load scene \'" +
				currentScene->getName() + "\'", __LINE__ - 6);
			
			return;

		}

		m_currentScene = currentScene;

		gameLoop();

	}

}


	// --- PRIVATE CLASS FUNCTIONS ---

/*		gameLoop
*
*		brief : Iter through a loop until current scene breaks it
*
**/
void HM_GameMaster::gameLoop()
{

	float frameRate(1000.0f / 60.0f);
	Uint32 loopStart(0), loopEnd(0), elapsedTime(0);

	while (!m_pInputManager->ended())
	{
		loopStart = SDL_GetTicks();


		// Inputs
		m_pInputManager->updateEvents();

		if (DEBUG && m_pInputManager->getKey(SDL_SCANCODE_ESCAPE))
			break;

		// Logic
		m_currentScene->update();

		// Graphics
		m_pGraphicsManager->startDisplay();

			m_currentScene->display();

		m_pGraphicsManager->endDisplay();


		loopEnd = SDL_GetTicks();
		elapsedTime = loopEnd - loopStart;

		if (elapsedTime < frameRate)
			SDL_Delay(static_cast<Uint32>(frameRate - elapsedTime));

	}

}