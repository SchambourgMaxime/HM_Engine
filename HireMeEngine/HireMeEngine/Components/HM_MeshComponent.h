/************************************************/
/*												*/
/*	HM_MeshComponent.h							*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-08-30						*/
/*												*/
/*	Last Update : 2016-08-30					*/
/*												*/
/*	Author : Eric Laberge, Maxime Schambourg	*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#pragma once

#include "HM_Component.h"



/*				     HEADER						*/

/*		Class HM_MeshComponent
*
*		brief : Component representing a mesh (+ texture and 
*
**/

class HM_MeshComponent : public HM_Component
{

public:
	/** Public functions **/

	// Constructor
	HM_MeshComponent(HM_SceneObject* owner) : 
		HM_Component(owner), m_pGraphicsManager(NULL),
		m_pMesh(NULL), m_pTexture(NULL), m_pShader(NULL),
		m_meshFilePath(""), m_textureFilePath(""),
		m_vertexShaderFilePath(""), m_fragmentShaderFilePath(""),
		m_linearCopyNumber(1), m_linearCopyOffset(glm::vec3(1.0f, 1.0f, 1.0f))
	{};

	// Create an instance of the mesh component for the factory
	HM_MeshComponent* create(HM_SceneObject* owner) const override;
	// Initialize the mesh, texture and shader of the mesh component
	virtual bool setup(std::map<std::string, void*> descr) override;

	// display the mesh with the texture using the shader
	virtual void display() override;
	// TODO : update mesh
	virtual void update() override {};

	HM_Mesh* const getMesh() const;

	HM_Cube const getBoundingBox() const;

protected:
	/** Private functions **/

	// Loads the mesh using the graphics manager
	bool setupMesh(std::string attributeName,
		std::map<std::string, void*> descr);
	// Loads the texture using the graphics manager
	bool setupTexture(std::string attributeName,
		std::map<std::string, void*> descr);
	// Loads the shader using the graphics manager
	bool setupShader(std::string attribute1Name,
		std::string attribute2Name,
		std::map<std::string, void*> descr);
	// Loads the linear copy detail
	bool setupLinearCopy(std::string attribute1Name,
		std::string attribute2Name,
		std::string attribute3Name,
		std::map<std::string, void*> descr);


	/** Private variables **/

	// Pointer to the graphics manager to use graphics functions
	HM_GraphicsManager* m_pGraphicsManager;

	// Name of the file holding the coordinates and the texCoords of the mesh
	std::string m_meshFilePath;
	// Pointer to the mesh created by the graphics manager
	HM_Mesh* m_pMesh;

	// Name of the file holding the image file of the texture
	std::string m_textureFilePath;
	// Pointer to the texture created by the graphics manager
	HM_Texture* m_pTexture;

	// Name of the file holding the vertex shader
	std::string m_vertexShaderFilePath;
	// Name of the file holding the fragment shader
	std::string m_fragmentShaderFilePath;
	// Pointer to the shader created by the graphics manager
	HM_Shader* m_pShader;

	// Number of copies
	unsigned int m_linearCopyNumber;

	// Offset of copies
	glm::vec3 m_linearCopyOffset;

};