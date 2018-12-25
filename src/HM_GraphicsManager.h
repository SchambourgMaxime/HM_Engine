/************************************************/
/*												*/
/*	HM_GraphicsManager.h						*/
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

#include "HM_Log.h"
#include "HM_Shader.h"
#include "HM_Camera.h"

// Include Apple
#ifdef __APPLE__
	#include <CoreFoundation/CoreFoundation.h>
#endif // __APPLE__

// Include Windows
#ifdef _WIN32
	#include <GL/glew.h>

// Include Apple
#elif __APPLE__
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>

// Include UNIX/Linux
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <map>

#define HM_VBO_STATIC 0
#define HM_VBO_DYNAMIC 1

#ifndef BUFFER_OFFSET

#define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

class HM_Mesh;
class HM_Sprite;
class HM_SceneObject;
class HM_Texture;



/*				     HEADER						*/

class HM_GraphicsManager
{

public:
	/** Public functions **/

	// Constructor
	HM_GraphicsManager(std::string, int, int);
	// Destructor
	~HM_GraphicsManager();

	// Getter to print in the game log
	HM_Log* const getGraphicsLog() const;
	// Getter for the modelview matrix
	glm::mat4& getModelview();
	// Getter for the projection matrix
	glm::mat4& getProjection();
	// Getter for the height of the window
	unsigned int getWindowHeight() const;
	// Getter for the width of the window
	unsigned int getWindowWidth() const;
	// Getter for the default shader
	HM_Shader* const getDefaultShader() const;

	void setMainCamera(HM_Camera* mainCamera);

	// Inits SDL, OpenGL and attributes of the class
	bool init();
	// Initialization of SDL and creation the SDL window and GL context
	bool initWindow();
	// Initialization of open GL and GLEW
	bool initGL();


	// Creates the VBO and stores all meshes (coords, texCoords and indices)
	void allocVBOData();

	// Creates the static VBO and stores data (static coords ans texCoords)
	void allocVBOData_Static();

	// Creates the dynamic VBO and stores data (dynamic coords ans texCoords)
	void allocVBOData_Dynamic();

	// Creates the element array VBO stores data (triangles indices)
	void allocVBOData_Indices();


	// Adds a scene object with a mesh
	/*void addDrawableSceneObject(HM_SceneObject*);*/

	// Create and load a mesh if it is not already loaded
	HM_Mesh* loadMesh(std::string meshFilePath,
					  unsigned char changeFrequencyCoords,
					  unsigned char changeFrequencyTexCoords);

	// Create and load a sprite if it is not already loaded
	HM_Sprite* loadSprite(std::string name,
						  std::string spriteFilePath,
						  unsigned char changeFrequencyCoords);

	// Create and load a sprite if it is not already loaded
	HM_Sprite* loadSprite(std::string name,
						  glm::vec2 pivot,
						  glm::vec2 size,
						  glm::vec2 uvMin,
						  glm::vec2 uvMax,
						  unsigned char changeFrequencyCoords);

	void addMeshtoVBOData(HM_Mesh* meshToAdd,
						  unsigned char changeFrequencyCoords,
						  unsigned char changeFrequencyTexCoords);

	void refreshVBOData(HM_Mesh* mesh,
						unsigned char changeFrequencyCoords,
						unsigned char changeFrequencyTexCoords);

	// Create and load a texture if it is not already loaded
	HM_Texture* loadTexture(std::string textureFilePath);

	// Create and load a shader if it is not already loaded
	HM_Shader* loadShader(std::string vertexShaderFilePath,
						  std::string fragmentShaderFilePath);

	HM_Camera* loadCamera(std::string name,
						  glm::vec3 position = glm::vec3(0, 0, 0),
						  glm::vec3 targetPoint = glm::vec3(0, 0, 1),
						  float sensibility = 0.5f,
						  float speed = 0.5f,
						  bool main = false);

	// Initiate the drawing
	void startDisplay();

	// Finish the drawing and flush every thing to screen
	void endDisplay();

	bool isVisibleByCamera(HM_Cube const & boundingBox,
						   glm::vec3 const & position) const;

	void drawMesh(HM_Mesh* const mesh,
				  HM_Texture* texture = NULL,
				  HM_Shader* shader = NULL,
				  glm::vec3 translation = glm::vec3(0.0f),
				  glm::vec3 rotation = glm::vec3(0.0f),
				  glm::vec3 scale = glm::vec3(0.0f));

	void drawHUD(HM_Sprite* const sprite,
		HM_Texture* texture = NULL,
		glm::vec2 position = glm::vec2(0.0f));

private:
	/** Private Variables **/

	// Name of the window
	std::string m_windowTitle;

	// Width and height of window (in pixels)
	unsigned int m_windowWidth;
	unsigned int m_windowHeight;

	// Reference to the main SDL window
	SDL_Window* m_pMainWindow;
	// OpenGL context for SDL
	SDL_GLContext m_openGLContext;


	// Storage for all used meshes to avoid loading several times
	static std::map<std::string, HM_Mesh*> m_mapOfLoadedMeshes;
	// Storage for all used sprites to avoid loading several times
	static std::map<std::string, HM_Sprite*> m_mapOfLoadedSprites;
	// Storage for all used textures to avoid loading several times
	static std::map<std::string, HM_Texture*> m_mapOfLoadedTextures;
	// Storage for all used shaders to avoid loading several times
	static std::map<std::string, HM_Shader*> m_mapOfLoadedShaders;
	// Reference to all the camera in the scene
	static std::map<std::string, HM_Camera*> m_cameraMap;


	// Number of coords in all stored meshes
	unsigned int m_nbCoords_Static;
	// Number of texCoords in all stored meshes
	unsigned int m_nbTexCoords_Static;
	// Number of coords in all stored meshes
	unsigned int m_nbCoords_Dynamic;
	// Number of texCoords in all stored meshes
	unsigned int m_nbTexCoords_Dynamic;
	// Number of indices in all stored meshes
	unsigned int m_nbIndices;

	// Size of all the vertices stored in bytes
	unsigned int m_bytesSizeCoords_Static;
	// Size of all the vertices stored in bytes
	unsigned int m_bytesSizeCoords_Dynamic;

	// Size of all the texture coordinates stored in bytes
	unsigned int m_bytesSizeTexCoords_Static;
	// Size of all the texture coordinates stored in bytes
	unsigned int m_bytesSizeTexCoords_Dynamic;

	// Size of all triangle indices stored in bytes
	unsigned int m_bytesSizeIndices;

	std::vector<GLfloat> m_coordsArray_Static;
	std::vector<GLfloat> m_texCoordsArray_Static;

	std::vector<GLfloat> m_coordsArray_Dynamic;
	std::vector<GLfloat> m_texCoordsArray_Dynamic;

	std::vector<int> m_indicesArray;

	// ID of the vbo for static coords and texCoords
	GLuint m_VBOID_Static;
	// ID of the vbo for dynamic coords and texCoords
	GLuint m_VBOID_Dynamic;
	// ID of the vbo for triangles indices
	GLuint m_VBOID_Indices;

	// Default shader used when no shader is defined for a mesh
	HM_Shader* m_pDefaultShader;
	// shader used for HUD elements
	HM_Shader* m_pHUDShader;

	// View matrices
	glm::mat4 m_projection;
	glm::mat4 m_modelview;

	HM_Camera* m_pMainCamera;

	float m_frustumNearLimit;
	float m_frustumFarLimit;

	// Log for the graphics section
	HM_Log* m_pLogGraphics;

};

