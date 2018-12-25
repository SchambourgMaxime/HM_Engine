/************************************************/
/*												*/
/*	HM_Texture.h								*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-08-22						*/
/*												*/
/*	Last Update : 2016-08-22					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#pragma once

#include "HM_GraphicsManager.h"

#include <SDL2/SDL.h>
#include <iostream>
#include <string>

// Include Windows

#ifdef _WIN32
#include <GL/glew.h>
#include <SDL2/SDL_image.h>


// Include Mac

#elif __APPLE__
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>
#include <SDL2_image/SDL_image.h>


// Include UNIX/Linux

#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#include <SDL2/SDL_image.h>

#endif

class HM_GraphicsManager;



/*				     HEADER						*/

/*		Class HM_Texture
*
*		brief : transforms image files into textures
*
**/
class HM_Texture
{
public :
	/** Public functions **/
	
	// Default constructor
	HM_Texture();
	// Copy constructor
	//HM_Texture(HM_Texture const &textureToCopy);
	// Constructor
	HM_Texture(std::string imageFile);
	// Destructor
	~HM_Texture();

	// Copy operator=
	//HM_Texture& operator=(HM_Texture const &textureToCopy);

	// Load
	bool load();
	GLuint getID() const;
	glm::vec2 const & getSize() const;
	bool getIstransparent() const;
	void setImageFile(const std::string &imageFile);

private :
	/** Private variables **/

	GLuint m_id;
	std::string m_imageFile;

	glm::vec2 m_size;

	bool isTransparent;


	/** Private functions **/

	SDL_Surface* invertPixels(SDL_Surface* sourceImage) const;


};