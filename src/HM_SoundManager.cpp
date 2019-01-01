/************************************************/
/*												*/
/*	HM_GraphicsManager.cpp						*/
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

#include "HM_SoundManager.h"
#include "HM_GameMaster.h"


/*				 IMPLEMENTATION					*/

// --- STATIC VARIABLES ---

std::map<std::string, Mix_Music*> HM_SoundManager::m_mapOfLoadedMusics;
std::map<std::string, Mix_Chunk*> HM_SoundManager::m_mapOfLoadedSFX;


HM_SoundManager::HM_SoundManager()
{

	Mix_Music *music1 = NULL;
	Mix_Music *music2 = NULL;

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		return;
	}

	music2 = Mix_LoadMUS(HM_Path("Azureflux_-_06_-_Catnip.mp3").getAbsolutePath().c_str());
	Mix_VolumeMusic(40);

	Mix_PlayMusic(music2, -1);

}

HM_SoundManager::~HM_SoundManager()
{
}

Mix_Music* HM_SoundManager::loadMusic(std::string musicFilePath)
{

	std::map<std::string, Mix_Music*>::const_iterator iter;
	iter = m_mapOfLoadedMusics.find(musicFilePath);

	Mix_Music* musicToReturn = NULL;

	if (iter == m_mapOfLoadedMusics.end())
	{

		musicToReturn = Mix_LoadMUS(musicFilePath.c_str());

		if (musicToReturn)
		{

			m_mapOfLoadedMusics[musicFilePath] = musicToReturn;

		}
		else
		{

			GAME_LOG->_ADDLINETOLOG("Error : Can't load this music : "
				+ musicFilePath);

			return NULL;

		}

	}
	else
	{

		musicToReturn = (*iter).second;

	}

	return musicToReturn;

}

Mix_Chunk* HM_SoundManager::loadSFX(std::string sfxFilePath)
{

	std::map<std::string, Mix_Chunk*>::const_iterator iter;
	iter = m_mapOfLoadedSFX.find(sfxFilePath);

	Mix_Chunk* sfxToReturn = NULL;

	if (iter == m_mapOfLoadedSFX.end())
	{
		sfxToReturn = Mix_LoadWAV(HM_Path(sfxFilePath).getAbsolutePath().c_str());
		Mix_VolumeChunk(sfxToReturn, MIX_MAX_VOLUME);

		if (sfxToReturn)
		{

			m_mapOfLoadedSFX[sfxFilePath] = sfxToReturn;

		}
		else
		{

			GAME_LOG->_ADDLINETOLOG("Error : Can't load this sound effect : "
				+ sfxFilePath);

			return NULL;

		}

	}
	else
	{

		sfxToReturn = (*iter).second;

	}

	return sfxToReturn;

}

void HM_SoundManager::playMusic(Mix_Music* music, bool loop,
	int fadeTransitionTime)
{

	if (fadeTransitionTime == 0.0f)
	{

		if(loop)
			Mix_PlayMusic(music, -1);
		else
			Mix_PlayMusic(music, 0);

	}
	else
	{

		if(loop)
			Mix_FadeInMusic(music, -1, fadeTransitionTime);
		else
			Mix_FadeInMusic(music, 0, fadeTransitionTime);

	}

}

void HM_SoundManager::playMusic(std::string musicFilePath, bool loop,
	int fadeTransitionTime)
{

	std::map<std::string, Mix_Music*>::const_iterator iter;
	iter = m_mapOfLoadedMusics.find(musicFilePath);

	if(iter != m_mapOfLoadedMusics.end())
		playMusic((*iter).second, loop, fadeTransitionTime);
	else
		GAME_LOG->_ADDLINETOLOG("Warning : Trying to play music \'"
			+ musicFilePath + "\' but it is not loaded");

}

void HM_SoundManager::pauseMusic()
{

	Mix_PauseMusic();

}

void HM_SoundManager::stopMusic(int fadeTransitionTime)
{

	if(fadeTransitionTime == 0.0f)
		Mix_HaltMusic();
	else
		Mix_FadeOutMusic(fadeTransitionTime);

}

void HM_SoundManager::playSFX(Mix_Chunk* sfx, bool loop)
{

	if (loop)
		Mix_PlayChannel(-1, sfx, -1);
	else
		Mix_PlayChannel(-1, sfx, 0);

}

void HM_SoundManager::playSFX(std::string sfxFilePath, bool loop /*= false*/)
{

	std::map<std::string, Mix_Chunk*>::const_iterator iter;
	iter = m_mapOfLoadedSFX.find(sfxFilePath);

	if (iter != m_mapOfLoadedSFX.end())
		playSFX((*iter).second, loop);
	else
		GAME_LOG->_ADDLINETOLOG("Warning : Trying to play sound effect \'"
			+ sfxFilePath + "\' but it is not loaded");

}
