/************************************************/
/*												*/
/*	HM_SoundManager.h							*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-10-17						*/
/*												*/
/*	Last Update : 2016-10-17					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#pragma once

#include <map>
#include <string>
#include <SDL2/SDL_mixer.h>



/*				     HEADER						*/

class HM_SoundManager
{

public:
	/** Public functions **/

	// Constructor
	HM_SoundManager();
	// Destructor
	~HM_SoundManager();
	// Create and load a music if it is not already loaded
	Mix_Music* loadMusic(std::string musicFilePath);
	// Create and load a sound effect if it is not already loaded
	Mix_Chunk* loadSFX(std::string sfxFilePath);

	void playMusic(Mix_Music* music, bool loop = false,
		int fadeTransitionTime = 0);

	void playMusic(std::string musicFilePath, bool loop = false,
		int fadeTransitionTime = 0);

	void pauseMusic();

	void stopMusic(int fadeTransitionTime = 0);

	void playSFX(Mix_Chunk* sfx, bool loop = false);

	void playSFX(std::string sfxFilePath, bool loop = false);


private:
	/** Private variables **/
	
	// Storage for all used musics to avoid loading several times
	static std::map<std::string, Mix_Music*> m_mapOfLoadedMusics;
	// Storage for all used soundEffects to avoid loading several times
	static std::map<std::string, Mix_Chunk*> m_mapOfLoadedSFX;

	Mix_Music* currentMusic;

	Mix_Chunk* currentChannels[8];

};

