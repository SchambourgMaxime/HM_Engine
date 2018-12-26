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

#include "HM_GraphicsManager.h"
#include "HM_Mesh.h"
#include "HM_Sprite.h"
#include "HM_SceneObject.h"
#include "HM_Texture.h"
#include "HM_Math.h"



/*				 IMPLEMENTATION					*/

	// --- STATIC VARIABLES ---

std::map<std::string, HM_Mesh*> HM_GraphicsManager::m_mapOfLoadedMeshes;
std::map<std::string, HM_Sprite*> HM_GraphicsManager::m_mapOfLoadedSprites;
std::map<std::string, HM_Texture*> HM_GraphicsManager::m_mapOfLoadedTextures;
std::map<std::string, HM_Shader*> HM_GraphicsManager::m_mapOfLoadedShaders;
std::map<std::string, HM_Camera*> HM_GraphicsManager::m_cameraMap;


	// --- CONSTRUCTOR & DESTRUCTOR ---

/*		Constructor
*
*		brief :
*
**/
HM_GraphicsManager::HM_GraphicsManager(std::string windowTitle,
int windowWidth, int windowHeight) :
	m_windowTitle(windowTitle),
	m_windowWidth(windowWidth), m_windowHeight(windowHeight),
	m_pMainWindow(NULL),
	m_nbCoords_Static(0), m_nbCoords_Dynamic(0),
	m_nbTexCoords_Static(0), m_nbTexCoords_Dynamic(0),
	m_nbIndices(0),
	m_bytesSizeCoords_Static(0), m_bytesSizeCoords_Dynamic(0),
	m_bytesSizeTexCoords_Static(0), m_bytesSizeTexCoords_Dynamic(0),
	m_VBOID_Static(0), m_VBOID_Dynamic(0), m_VBOID_Indices(0),
	m_pDefaultShader(NULL), m_pHUDShader(NULL),
	m_projection(0), m_modelview(0),
	m_pMainCamera(NULL),
	m_pLogGraphics(NULL)
{

	m_pLogGraphics = new HM_Log("Graphics");

	m_pDefaultShader = new HM_Shader("Shaders/basic2D.vert",
									 "Shaders/basic2D.frag");
	m_pHUDShader = new HM_Shader("Shaders/HUD.vert",
								 "Shaders/HUD.frag");

	// TODO : use settings to initiate
	m_frustumNearLimit = 0.1f;
	m_frustumFarLimit = 1000.0f;

	// Initialisation
	m_projection = glm::perspective(70.0f,
									(float)m_windowWidth / m_windowHeight,
									m_frustumNearLimit,
									m_frustumFarLimit);

	m_modelview = glm::mat4(1.0);

}

/*		Destructor
*
*		brief :
*
**/
HM_GraphicsManager::~HM_GraphicsManager()
{

	m_pLogGraphics->flush();
	
	// VBO destruction
	if(glIsBuffer(m_VBOID_Static) == GL_TRUE)
		glDeleteBuffers(1, &m_VBOID_Static);

	if (glIsBuffer(m_VBOID_Dynamic) == GL_TRUE)
		glDeleteBuffers(1, &m_VBOID_Dynamic);

	if (glIsBuffer(m_VBOID_Indices) == GL_TRUE)
		glDeleteBuffers(1, &m_VBOID_Indices);

	delete m_pLogGraphics;
	delete m_pDefaultShader;
	delete m_pMainCamera;
	SDL_DestroyWindow(m_pMainWindow);
	SDL_Quit();

}


	// --- ACCESSORS ---

glm::mat4& HM_GraphicsManager::getModelview()
{

	return m_modelview;

}

glm::mat4& HM_GraphicsManager::getProjection()
{

	return m_projection;

}

unsigned int HM_GraphicsManager::getWindowHeight() const
{

	return m_windowHeight;

}

unsigned int HM_GraphicsManager::getWindowWidth() const
{

	return m_windowWidth;

}

HM_Shader* const HM_GraphicsManager::getDefaultShader() const
{

	return m_pDefaultShader;

}

HM_Log* const HM_GraphicsManager::getGraphicsLog() const
{

	return m_pLogGraphics;

}

void HM_GraphicsManager::setMainCamera(HM_Camera* mainCamera)
{

	m_pMainCamera = mainCamera;

}


	// --- PUBLIC CLASS FUNCTIONS ---

bool HM_GraphicsManager::init()
{

	if (!initWindow())
		return false;

	if (!initGL())
		return false;

	m_pDefaultShader->load();
	m_pHUDShader->load();

	return true;

}

/*		initWindow
*
*		brief : Initialize SDL window and OpenGL
*
*		return : bool -> assesses if the initialization was successful
*
**/
bool HM_GraphicsManager::initWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{

		m_pLogGraphics->_ADDLINETOLOG_LINE(std::string("Error while initiating SDL : ") + SDL_GetError(), __LINE__ - 3);
		SDL_Quit();

		return false;

	}

#ifdef __APPLE__

	// OpenGL version for Apple
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Récupération du Bundle

	CFURLRef URLBundle = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
	char *cheminResources = new char[PATH_MAX];


	// Changement du 'Working Directory'

	if (CFURLGetFileSystemRepresentation(URLBundle, 1, (UInt8*)cheminResources, PATH_MAX))
		chdir(cheminResources);


	// Libération de la mémoire

	delete[] cheminResources;
	CFRelease(URLBundle);

#else

	// OpenGL version for other OS
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

#endif
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	m_pMainWindow = SDL_CreateWindow(m_windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (m_pMainWindow == 0)
	{

		m_pLogGraphics->_ADDLINETOLOG_LINE(std::string("Error during the window creation : ") + SDL_GetError(), __LINE__ - 5);
		SDL_Quit();

		return false;

	}

	m_openGLContext = SDL_GL_CreateContext(m_pMainWindow);

	if (m_openGLContext == 0)
	{

		m_pLogGraphics->_ADDLINETOLOG_LINE(std::string("Error while creating the GL context : ") + SDL_GetError(), __LINE__ - 5);
		SDL_DestroyWindow(m_pMainWindow);
		SDL_Quit();

		return false;

	}

	return true;
}

/*		initGL
*
*		brief : Initialize open GL and GLEW
*
*		return : bool -> assesses if the initialization was successful
*
**/
bool HM_GraphicsManager::initGL()
{
#ifdef WIN32

	GLenum initGLEW(glewInit());

	if (initGLEW != GLEW_OK)
	{

		// Printing the error with function glewGetErrorString
		m_pLogGraphics->_ADDLINETOLOG_LINE(std::string("Error while initializing GLEW : ") + reinterpret_cast<const char*>(glewGetErrorString(initGLEW)), __LINE__ - 5);

		// Quitting SDL
		SDL_GL_DeleteContext(m_openGLContext);
		SDL_DestroyWindow(m_pMainWindow);
		SDL_Quit();

		return false;

	}

#endif

	glEnable(GL_DEPTH_TEST);

	return true;
}

/*		allocVBOData
*
*		brief : allocates the VBO for all the coordinates, texture coordinates
*				and indices of all the meshes in the scene
*
**/
void HM_GraphicsManager::allocVBOData()
{

	allocVBOData_Static();

	allocVBOData_Dynamic();

	allocVBOData_Indices();

}

/*		allocVBOData_Static
*
*		brief : allocates the static VBO for all the coordinates and texture
*				coordinates	of all the static meshes in the scene
*
**/
void HM_GraphicsManager::allocVBOData_Static()
{

	if (m_nbCoords_Static > 0 || m_nbTexCoords_Static > 0)
	{

		if (glIsBuffer(m_VBOID_Static))
			glDeleteBuffers(1, &m_VBOID_Static);

		glGenBuffers(1, &m_VBOID_Static);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBOID_Static);

			glBufferData(GL_ARRAY_BUFFER,
						 m_bytesSizeCoords_Static + m_bytesSizeTexCoords_Static,
						 0, GL_STATIC_DRAW);

			if (m_nbCoords_Static > 0)
				glBufferSubData(GL_ARRAY_BUFFER, 0,
							m_bytesSizeCoords_Static,
							&m_coordsArray_Static[0]);
			if (m_nbTexCoords_Static > 0)
				glBufferSubData(GL_ARRAY_BUFFER, m_bytesSizeCoords_Static,
							m_bytesSizeTexCoords_Static,
							&m_texCoordsArray_Static[0]);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

}

/*		allocVBOData_Dynamic
*
*		brief : allocates the dynamic VBO for all the coordinates and texture
*				coordinates	of all the dynamic meshes in the scene
*
**/
void HM_GraphicsManager::allocVBOData_Dynamic()
{

	if(m_nbCoords_Dynamic > 0 || m_nbTexCoords_Dynamic > 0)
	{

		if (glIsBuffer(m_VBOID_Dynamic))
			glDeleteBuffers(1, &m_VBOID_Dynamic);

		glGenBuffers(1, &m_VBOID_Dynamic);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBOID_Dynamic);

			glBufferData(GL_ARRAY_BUFFER,
					 m_bytesSizeCoords_Dynamic + m_bytesSizeTexCoords_Dynamic,
					 0, GL_DYNAMIC_DRAW);

				if(m_nbCoords_Dynamic > 0)
				glBufferSubData(GL_ARRAY_BUFFER, 0,
								m_bytesSizeCoords_Dynamic,
								&m_coordsArray_Dynamic[0]);
				if(m_nbTexCoords_Dynamic > 0)
				glBufferSubData(GL_ARRAY_BUFFER, m_bytesSizeCoords_Dynamic,
								m_bytesSizeTexCoords_Dynamic,
								&m_texCoordsArray_Dynamic[0]);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

}

/*		allocVBOData_Indices
*
*		brief : allocates the indices VBO for all the indices of all the meshes
*				in the scene
*
**/
void HM_GraphicsManager::allocVBOData_Indices()
{
	
	if(m_nbIndices > 0)
	{

		if (glIsBuffer(m_VBOID_Indices))
			glDeleteBuffers(1, &m_VBOID_Indices);

		glGenBuffers(1, &m_VBOID_Indices);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOID_Indices);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER,
						 m_bytesSizeIndices,
						 0, GL_STATIC_DRAW);

				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
								m_bytesSizeIndices,
								&m_indicesArray[0]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

}

/*		loadMesh
*
*		brief : loads a mesh from a mesh description file (ex : OBJ) and
*				returns it. If the mesh was already loaded, returns the already
*				existing mesh
*
*		params :
*			- meshFilePath				(string)		: path to the file
*														  describing the mesh
*			- changeFrequencyCoords		(unsigned char)	: specifies if the mesh
*														  coordinates will ever
*														  change
*			- changeFrequencyTexCoords	(unsigned char)	: specifies if the
*														  texture coordinates
*														  will ever change
*
*		return : HM_Mesh* -> instance of existing or new mesh
*
**/
HM_Mesh* HM_GraphicsManager::loadMesh(std::string meshFilePath,
									  unsigned char changeFrequencyCoords,
									  unsigned char changeFrequencyTexCoords)
{
	
	std::map<std::string, HM_Mesh*>::const_iterator iter;
	iter = m_mapOfLoadedMeshes.find(meshFilePath);

	HM_Mesh* meshToReturn;

	if(iter == m_mapOfLoadedMeshes.end())
	{

		meshToReturn = new HM_Mesh(meshFilePath);

		if(meshToReturn)
		{

			addMeshtoVBOData(meshToReturn,
						 changeFrequencyCoords,
						 changeFrequencyTexCoords);

			m_mapOfLoadedMeshes[meshFilePath] = meshToReturn;

		}

	}
	else
	{

		meshToReturn = (*iter).second;

	}

	return meshToReturn;

}

/*		loadSprite
*
*		brief : loads a sprite. if file exists, load its animation also.
*				If the sprite was already loaded, returns the already existing
*				sprite
*
*		params :
*			- spriteFilePath		(string)		: path to the file
*													  describing the sprite
*													  animation
*			- changeFrequencyCoords	(unsigned char)	: specifies if the mesh
*													  coordinates will ever
*													  change
*
*		return : HM_Sprite* -> instance of existing or new sprite
*
**/
HM_Sprite* HM_GraphicsManager::loadSprite(std::string name,
										  std::string spriteFilePath,
										  unsigned char changeFrequencyCoords)
{

	std::map<std::string, HM_Sprite*>::const_iterator iter;
	iter = m_mapOfLoadedSprites.find(name);

	HM_Sprite* spriteToReturn = NULL;

	if (iter == m_mapOfLoadedSprites.end())
	{

		spriteToReturn = new HM_Sprite(spriteFilePath);

		if (spriteToReturn)
		{

			addMeshtoVBOData(spriteToReturn,
							 changeFrequencyCoords,
							 HM_VBO_STATIC);

			m_mapOfLoadedSprites[name] = spriteToReturn;

		}
		else
		{

			if (spriteFilePath != "")
				GRAPHICS_LOG("Error : Can't load sprite from file \'" +
					spriteFilePath + "\'");
			else
				GRAPHICS_LOG("Error while loading sprite");

			delete spriteToReturn;
			spriteToReturn = NULL;

		}

	}
	else
	{

		spriteToReturn = (*iter).second;

	}

	return spriteToReturn;

}

HM_Sprite* HM_GraphicsManager::loadSprite(std::string name, 
										  glm::vec2 size,
										  glm::vec2 pivot,
										  glm::vec2 uvMin,
										  glm::vec2 uvMax,
										  unsigned char changeFrequencyCoords)
{

	std::map<std::string, HM_Sprite*>::const_iterator iter;
	iter = m_mapOfLoadedSprites.find(name);

	HM_Sprite* spriteToReturn = NULL;

	if (iter == m_mapOfLoadedSprites.end())
	{

		spriteToReturn = new HM_Sprite(size, pivot, uvMin, uvMax);

		if (spriteToReturn)
		{

			addMeshtoVBOData(spriteToReturn,
				changeFrequencyCoords,
				HM_VBO_STATIC);

			m_mapOfLoadedSprites[name] = spriteToReturn;

		}
		else
		{

			GRAPHICS_LOG("Error while loading sprite");

			delete spriteToReturn;
			spriteToReturn = NULL;

		}

	}
	else
	{

		spriteToReturn = (*iter).second;

	}

	return spriteToReturn;

}

void HM_GraphicsManager::addMeshtoVBOData(HM_Mesh* meshToAdd,
									  unsigned char changeFrequencyCoords,
									  unsigned char changeFrequencyTexCoords)
{

	if (changeFrequencyCoords == HM_VBO_STATIC)
	{

		meshToAdd->setVBOBytesOffsetCoords(
			m_coordsArray_Static.size() * sizeof(float));
		meshToAdd->setOriginalVBOBytesOffsetCoords(
			m_coordsArray_Static.size() * sizeof(float));
		m_nbCoords_Static += meshToAdd->getNumberCoords();
		m_bytesSizeCoords_Static += meshToAdd->getByteSizeCoords();
		meshToAdd->putMeshCoords(&m_coordsArray_Static);

	}
	else if (changeFrequencyCoords == HM_VBO_DYNAMIC)
	{

		meshToAdd->setVBOBytesOffsetCoords(
			m_coordsArray_Dynamic.size() * sizeof(float));
		meshToAdd->setOriginalVBOBytesOffsetCoords(
			m_coordsArray_Dynamic.size() * sizeof(float));
		m_nbCoords_Dynamic += meshToAdd->getNumberCoords();
		m_bytesSizeCoords_Dynamic += meshToAdd->getByteSizeCoords();
		meshToAdd->putMeshCoords(&m_coordsArray_Dynamic);

	}

	// Check change frequency texture coordinates
	if (changeFrequencyTexCoords == HM_VBO_STATIC)
	{

		meshToAdd->setVBOBytesOffsetTexCoords(
			m_texCoordsArray_Static.size() * sizeof(float));
		meshToAdd->setOriginalVBOBytesOffsetTexCoords(
			m_texCoordsArray_Static.size() * sizeof(float));
		m_nbTexCoords_Static += meshToAdd->getNumberTexCoords();
		m_bytesSizeTexCoords_Static += meshToAdd->getByteSizeTexCoords();
		meshToAdd->putMeshTexCoords(&m_texCoordsArray_Static);

	}
	else if (changeFrequencyTexCoords == HM_VBO_DYNAMIC)
	{

		meshToAdd->setVBOBytesOffsetTexCoords(
			m_texCoordsArray_Dynamic.size() * sizeof(float));
		meshToAdd->setOriginalVBOBytesOffsetTexCoords(
			m_texCoordsArray_Dynamic.size() * sizeof(float));
		m_nbTexCoords_Dynamic += meshToAdd->getNumberTexCoords();
		m_bytesSizeTexCoords_Dynamic += meshToAdd->getByteSizeTexCoords();
		meshToAdd->putMeshTexCoords(&m_texCoordsArray_Dynamic);

	}

	meshToAdd->setVBOBytesOffsetIndices(m_indicesArray.size() * sizeof(float));
	m_nbIndices += meshToAdd->getNumberIndices();
	m_bytesSizeIndices += meshToAdd->getByteSizeIndices();
	meshToAdd->putMeshIndices(&m_indicesArray);

}

void HM_GraphicsManager::refreshVBOData(HM_Mesh* mesh,
										unsigned char changeFrequencyCoords,
										unsigned char changeFrequencyTexCoords)
{

	if (changeFrequencyCoords == HM_VBO_STATIC)
	{

		mesh->putMeshCoords(
			&m_coordsArray_Static[mesh->getVBOBytesOffsetCoords()
			/ sizeof(float)]);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBOID_Static);

			glBufferSubData(GL_ARRAY_BUFFER, mesh->getVBOBytesOffsetCoords(),
				mesh->getByteSizeCoords(),
				&m_coordsArray_Static[mesh->getVBOBytesOffsetCoords()
					/ sizeof(float)]);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}


	else if (changeFrequencyCoords == HM_VBO_DYNAMIC)
	{

		mesh->putMeshCoords(
			&m_coordsArray_Dynamic[mesh->getVBOBytesOffsetCoords()
				/ sizeof(float)]);

// 		glBindBuffer(GL_ARRAY_BUFFER, m_VBOID_Dynamic);
// 
// 			glBufferSubData(GL_ARRAY_BUFFER, mesh->getVBOBytesOffsetCoords(),
// 				mesh->getVBOBytesOffsetCoords() + mesh->getByteSizeCoords(),
// 				&m_coordsArray_Dynamic[mesh->getVBOBytesOffsetCoords()
// 					/ sizeof(float)]);
// 
// 		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	// Check change frequency texture coordinates
	if (changeFrequencyTexCoords == HM_VBO_STATIC)
	{

		mesh->putMeshTexCoords(
			&m_texCoordsArray_Static[mesh->getVBOBytesOffsetTexCoords()
				/ sizeof(float)]);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBOID_Static);

			glBufferSubData(GL_ARRAY_BUFFER,
				m_bytesSizeCoords_Static + mesh->getVBOBytesOffsetTexCoords(),
				mesh->getByteSizeTexCoords(),
				&m_texCoordsArray_Static[mesh->getVBOBytesOffsetTexCoords()
					/ sizeof(float)]);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	else if (changeFrequencyTexCoords == HM_VBO_DYNAMIC)
		mesh->putMeshTexCoords(
			&m_texCoordsArray_Dynamic[mesh->getVBOBytesOffsetTexCoords()
				/ sizeof(float)]);


	mesh->putMeshIndices(&m_indicesArray[mesh->getVBOBytesOffsetIndices()
		/ sizeof(float)]);

}

/*		loadTexture
*
*		brief : loads a texture from an image file and returns it. If the
*				texture was already loaded, returns the already existing
*				texture
*
*		params :
*			- textureFilePath	(string)	: path to the image file
*
*		return : HM_Texture* -> instance of existing or new texture
*
**/
HM_Texture* HM_GraphicsManager::loadTexture(std::string textureFilePath)
{

	std::map<std::string, HM_Texture*>::const_iterator iter;
	iter = m_mapOfLoadedTextures.find("./Meshes/" + textureFilePath);

	HM_Texture* textureToReturn;

	if (iter == m_mapOfLoadedTextures.end())
	{
		
		textureToReturn = new HM_Texture("./Meshes/" + textureFilePath);
		textureToReturn->load();
		m_mapOfLoadedTextures[textureFilePath] = textureToReturn;
		
	}
	else
	{
		
		textureToReturn = (*iter).second;

	}

	return textureToReturn;

}

/*		loadShader
*
*		brief : loads a shader program (vertex + fragment) into the shader
*				programs collection. If the shader program was already loaded,
*				returns the already existing shader program
*
*		params :
*			- vertexShaderFilePath		(string)	: path to the vertex shader
*			- fragmentShaderFilePath	(string)	: path to the fragment
*													  shader
*
*		return : HM_Shader* -> instance of existing or new shader program
*
**/
HM_Shader* HM_GraphicsManager::loadShader(std::string vertexShaderFilePath,
										  std::string fragmentShaderFilePath)
{
	std::string shaderName;
	shaderName = "./Shaders/" + vertexShaderFilePath + fragmentShaderFilePath;

	std::map<std::string, HM_Shader*>::const_iterator iter;
	iter = m_mapOfLoadedShaders.find(shaderName);

	HM_Shader* shaderToReturn;

	if (iter == m_mapOfLoadedShaders.end())
	{

		shaderToReturn = new HM_Shader("./Shaders/" + vertexShaderFilePath,
									   "./Shaders/" + fragmentShaderFilePath);
		shaderToReturn->load();
		m_mapOfLoadedShaders[shaderName] = shaderToReturn;

	}
	else
	{

		shaderToReturn = (*iter).second;

	}

	return shaderToReturn;

}

/*		loadCamera
*
*		brief : loads a camera into the camera collection
*
*		params :
*			- name			(string)	: name of the new camera
*			- position		(vec3)		: world position of the new camera
*			- targetPoint	(vec3)		: point targeted by the new camera
*			- sensibility	(float)		: sensibility of the mouse input on
*										  camera movement
*			- speed			(float)		: sensibility arrow keys on camera
*										  movement
*			- main			(bool)		: specifies if camera should be main
*
*		return : HM_Camera* -> instance of newly created camera
*
**/
HM_Camera* HM_GraphicsManager::loadCamera(std::string name,
										  glm::vec3 position,
										  glm::vec3 targetPoint,
										  float sensibility,
										  float speed,
										  bool main)
{

	HM_Camera* temp = NULL;

	std::map<std::string, HM_Camera*>::const_iterator iter;
	iter = m_cameraMap.find(name);

	if (iter == m_cameraMap.end())
	{
		temp = new HM_Camera(position, targetPoint, glm::vec3(0, 1, 0),
			sensibility, speed);

		if (!m_pMainCamera || main == true)
			m_pMainCamera = temp;

		m_cameraMap[name] = temp;
	}

	return temp;

}

/*		startDisplay
*
*		brief : initializes the screen for the drawing
*
**/
void HM_GraphicsManager::startDisplay()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

/*		endDisplay
*
*		brief : stops drawing and flips screen
*
**/
void HM_GraphicsManager::endDisplay()
{

	SDL_GL_SwapWindow(m_pMainWindow);

}

/*		isVisibleByCamera
*
*		brief : checks if a position surrounded by a bounding box is visible
*				by the main camera
*
*		params :
*			- boundingBox	(HM_Cube)	: bounding box of the position given
*			- position		(vec3)		: position to check
*
*		return : bool -> specifies if the position or its bounding box is
*						 visible by the camera
*
**/
bool HM_GraphicsManager::isVisibleByCamera(HM_Cube const & boundingBox,
										   glm::vec3 const & position) const
{

	float frustumFarLimitSquared = m_frustumFarLimit * m_frustumFarLimit;

	glm::vec3 closestPoint = hmm::getClosestPointOnBox(boundingBox + position, m_pMainCamera->getPosition());

	glm::vec3 distanceVector = closestPoint - m_pMainCamera->getPosition();
	float distanceSquared = glm::dot(distanceVector, distanceVector);

	if (distanceSquared > frustumFarLimitSquared)
		return false;

	// Check to see if the position is in front of the camera
	if (glm::dot(m_pMainCamera->getOrientation(), distanceVector) <= 0.0f)
		return false;

	return true;

}

/*		drawMesh
*
*		brief : draws a specific mesh with a certain transform and a certain
				texture and shader
*
*		params :
*			- mesh			(HM_Mesh)		: mesh to draw
*			- texture		(HM_Texture)	: texture to apply on mesh
*			- shader		(HM_Shader)		: shader program to apply on draw
*			- translation	(vec3)			: position of the mesh
*			- rotation		(vec3)			: rotation of the mesh
*			- scale			(vec3)			: scale of the mesh
*
**/
void HM_GraphicsManager::drawMesh(HM_Mesh* const mesh,
								  HM_Texture* texture,
								  HM_Shader* shader,
								  glm::vec3 translation,
								  glm::vec3 rotation,
								  glm::vec3 scale)
{
	
	if (mesh != NULL)
	{

		HM_Shader* shaderToUse;

		glm::mat4 modelview = m_modelview;

		if(shader)
			shaderToUse = shader;
		else
			shaderToUse = m_pDefaultShader;

		glUseProgram(shaderToUse->getProgramID());


			if(translation != glm::vec3(0.0f))
				modelview = glm::translate(modelview, translation);

			if (rotation != glm::vec3(0.0f))
			{

				modelview = glm::rotate(modelview,
					rotation.x * (float)M_PI / 180, glm::vec3(1, 0, 0));
				modelview = glm::rotate(modelview,
					rotation.y * (float)M_PI / 180, glm::vec3(0, 1, 0));
				modelview = glm::rotate(modelview,
					rotation.z * (float)M_PI / 180, glm::vec3(0, 0, 1));

			}

			if (scale != glm::vec3(1.0f, 1.0f, 1.0f))
				modelview = glm::scale(modelview, scale);

			shaderToUse->sendMat4("modelview", modelview);
			shaderToUse->sendMat4("projection", m_projection);

			if (texture && texture->getIstransparent())
			{

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			}

			if(mesh->getChangeFrequencyCoords() == HM_VBO_STATIC)
				glBindBuffer(GL_ARRAY_BUFFER, m_VBOID_Static);
			else if(mesh->getChangeFrequencyCoords() == HM_VBO_DYNAMIC)
				glBindBuffer(GL_ARRAY_BUFFER, m_VBOID_Dynamic);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 
					BUFFER_OFFSET(mesh->getVBOBytesOffsetCoords()));
				glEnableVertexAttribArray(0);

				if (texture)
				{
					
					if (mesh->getChangeFrequencyTexCoords() == HM_VBO_STATIC)
					{

						if (mesh->getChangeFrequencyCoords() !=
							mesh->getChangeFrequencyTexCoords())
							glBindBuffer(GL_ARRAY_BUFFER, m_VBOID_Static);

						glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0,
							BUFFER_OFFSET(mesh->getVBOBytesOffsetTexCoords() +
								m_bytesSizeCoords_Static));
						glEnableVertexAttribArray(2);

					}
					else if (mesh->getChangeFrequencyCoords() == HM_VBO_DYNAMIC)
					{

						if (mesh->getChangeFrequencyCoords() !=
							mesh->getChangeFrequencyTexCoords())
							glBindBuffer(GL_ARRAY_BUFFER, m_VBOID_Dynamic);

						glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0,
							BUFFER_OFFSET(mesh->getVBOBytesOffsetTexCoords() +
								m_bytesSizeCoords_Dynamic));
						glEnableVertexAttribArray(2);

					}

				}

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOID_Indices);

					mesh->draw(texture);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			if (texture && texture->getIstransparent())
				glDisable(GL_BLEND);

		glUseProgram(0);


	}
	else
	{
		
		GRAPHICS_LOG("Can't draw mesh if given mesh is NULL");

	}

}

void HM_GraphicsManager::drawHUD(HM_Sprite* const sprite,
								 HM_Texture* texture,
								 glm::vec2 position)
{

	glUseProgram(m_pHUDShader->getProgramID());

		if (texture && texture->getIstransparent())
		{

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		}

		if (sprite->getChangeFrequencyCoords() == HM_VBO_STATIC)
			glBindBuffer(GL_ARRAY_BUFFER, m_VBOID_Static);
		else if (sprite->getChangeFrequencyCoords() == HM_VBO_DYNAMIC)
			glBindBuffer(GL_ARRAY_BUFFER, m_VBOID_Dynamic);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET(sprite->getVBOBytesOffsetCoords()));
			glEnableVertexAttribArray(0);

			if (texture)
			{

				if (sprite->getChangeFrequencyTexCoords() == HM_VBO_STATIC)
				{

					if (sprite->getChangeFrequencyCoords() !=
						sprite->getChangeFrequencyTexCoords())
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOID_Static);

					glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0,
						BUFFER_OFFSET(sprite->getVBOBytesOffsetTexCoords() +
							m_bytesSizeCoords_Static));
					glEnableVertexAttribArray(2);

				}
				else if (sprite->getChangeFrequencyCoords() == HM_VBO_DYNAMIC)
				{

					if (sprite->getChangeFrequencyCoords() !=
						sprite->getChangeFrequencyTexCoords())
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOID_Dynamic);

					glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0,
						BUFFER_OFFSET(sprite->getVBOBytesOffsetTexCoords() +
							m_bytesSizeCoords_Dynamic));
					glEnableVertexAttribArray(2);

				}

			}

			glVertexAttrib2f(
				glGetAttribLocation(m_pHUDShader->getProgramID(), "in_Pos"),
				position.x, position.y);
			

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOID_Indices);

				sprite->draw(texture);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (texture && texture->getIstransparent())
			glDisable(GL_BLEND);

	glUseProgram(0);

}

