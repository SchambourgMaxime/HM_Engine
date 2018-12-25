/************************************************/
/*												*/
/*	HM_Texture.cpp								*/
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

#include "HM_Texture.h"
#include "HM_GraphicsManager.h"



/*				 IMPLEMENTATION					*/

HM_Texture::HM_Texture() :
m_id(0), m_imageFile("")
{
}

//HM_Texture::HM_Texture(HM_Texture const &textureToCopy)
//{
//
//	m_imageFile = textureToCopy.m_imageFile;
//	m_vboMoveFrequency = textureToCopy.m_vboMoveFrequency;
//	load();
//
//}

HM_Texture::HM_Texture(std::string imageFile) :
m_id(0), m_imageFile(imageFile)
{
}

HM_Texture::~HM_Texture()
{

	glDeleteTextures(1, &m_id);

}

//HM_Texture& HM_Texture::operator=(HM_Texture const &textureToCopy)
//{
//
//	m_imageFile = textureToCopy.m_imageFile;
//	load();
//
//	return *this;
//
//}


bool HM_Texture::load()
{

	// Loading the image in a SDL_Surface
	SDL_Surface* imageSDL = IMG_Load(m_imageFile.c_str());

	if (imageSDL == 0)
	{

		std::cout << "Error while loading texture \"" << m_imageFile.c_str() << "\" : "
		<< IMG_GetError() << std::endl;
		return false;

	}

	SDL_Surface* invertedImage = invertPixels(imageSDL);
	SDL_FreeSurface(imageSDL);

	// Destruction of an eventual old texture
	if(glIsTexture(m_id) == GL_TRUE)
		glDeleteTextures(1, &m_id);

	// ID Generation
	glGenTextures(1, &m_id);

	// Locking
	glBindTexture(GL_TEXTURE_2D, m_id);

	// Assessing format and internal format
	GLenum internalFormat(0);
	GLenum format(0);

	// Assessing number of components (alpha or no alpha)
	if (invertedImage->format->BytesPerPixel == 3)
	{

		internalFormat = GL_RGB;

		if (invertedImage->format->Rmask == 0xff)
			format = GL_RGB;
		else
			format = GL_BGR;

		isTransparent = false;

	}
	else if (invertedImage->format->BytesPerPixel == 4)
	{

		internalFormat = GL_RGBA;

		if (invertedImage->format->Rmask == 0xff)
			format = GL_RGBA;
		else
			format = GL_BGRA;

		isTransparent = true;

	}
	else
	{

		std::cout << "Error, unknown format of image \"" << m_imageFile.c_str() << "\" : "
		<< SDL_GetError() << std::endl;
		return false;

	}

	// Storing size of image
	m_size = glm::vec2(invertedImage->w, invertedImage->h);

	// Pixels copying
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,
				 invertedImage->w, invertedImage->h,
				 0, format, GL_UNSIGNED_BYTE,
				 invertedImage->pixels);

	// Filters application
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Unlocking
	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_FreeSurface(invertedImage);
	return true;

}

GLuint HM_Texture::getID() const
{

	return m_id;

}

glm::vec2 const & HM_Texture::getSize() const
{

	return m_size;

}

bool HM_Texture::getIstransparent() const
{

	return isTransparent;

}

void HM_Texture::setImageFile(const std::string &imageFile)
{

	m_imageFile = imageFile;

}

SDL_Surface* HM_Texture::invertPixels(SDL_Surface* sourceImage) const
{

	SDL_Surface* invertedImage = SDL_CreateRGBSurface(0, sourceImage->w, sourceImage->h, sourceImage->format->BitsPerPixel,
													  sourceImage->format->Rmask, sourceImage->format->Gmask, 
													  sourceImage->format->Bmask, sourceImage->format->Amask);

	unsigned char* sourcePixels = (unsigned char*)sourceImage->pixels;
	unsigned char* invertedPixels = (unsigned char*)invertedImage->pixels;
	
	for (int i = 0; i < sourceImage->h; i++)
	{
		for (int j = 0; j < sourceImage->w * sourceImage->format->BytesPerPixel; j++)
		{
			invertedPixels[(sourceImage->w * sourceImage->format->BytesPerPixel * (sourceImage->h - 1 - i)) + j] = 
			sourcePixels[(invertedImage->w * sourceImage->format->BytesPerPixel * i) + j];
		}
	}

	return invertedImage;

}
