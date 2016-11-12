/************************************************/
/*												*/
/*	HM_MeshComponent.cpp						*/
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

#include "HM_MeshComponent.h"
#include "HM_TransformComponent.h"
#include "../HM_Math.h"



/*				 IMPLEMENTATION					*/

	// --- OVERRIDEN PUBLIC FUNCTIONS ---

/*		create
*
*		brief : create a new instance of the mesh component
*
*		params :
*			- owner	(string)	: pointer to the scene object possessing the
*								  component
*
*		return : HM_MeshComponent -> pointer to the created instance
*
**/
HM_MeshComponent* HM_MeshComponent::create(HM_SceneObject* owner) const
{

	return new HM_MeshComponent(owner);

}

/*		setup
*
*		brief : initialize the mesh, texture and shader of the mesh component
*
*		params :
*			- descr	(map<string, void*>)	: map of the data for components
*
**/
bool HM_MeshComponent::setup(std::map<std::string, void*> descr)
{

	m_pGraphicsManager = HM_GameMaster::instance()->getGraphicsManager();

	if (!m_pGraphicsManager)
	{

		GAME_LOG->_ADDLINETOLOG_LINE("Error : graphics manager is not set for mesh \'" + m_meshFilePath + "\'", __LINE__ - 5);

		m_pMesh = NULL;
		m_owner = NULL;
		return false;

	}

	setupMesh("meshFile", descr);

	setupTexture("textureFile", descr);

	setupShader("vertexShader", "fragmentShader", descr);

	setupLinearCopy("linearCopyNumber", "linearCopyOffset", "linearCopyAxis",
		descr);

	return true;

}

bool HM_MeshComponent::onSetupEnd(std::map<std::string, void*> descr)
{

	if(m_linearCopyNumber > 1)
	{

		HM_Component* componentTransform =
			m_owner->getComponent("transform");

		if (componentTransform)
		{

			HM_TransformComponent* transform =
				static_cast<HM_TransformComponent*>(componentTransform);

			if (transform)
			{

				std::map<std::string, void*>::const_iterator iter =
					descr.find("linearCopyAxis"); // "linearCopyAxis"

				if (iter != descr.end())
				{

				glm::vec3 axisCopy(hmu::getDataFromVoid<glm::vec3>(
					(*iter).second));

				m_linearCopyOffset = glm::vec3(
					axisCopy.x > 0 ? axisCopy.x *
					(m_pMesh->getBoundingBox().getXMax() +
						m_pMesh->getBoundingBox().getWidth() -
						m_pMesh->getBoundingBox().getXMax()) :
					-axisCopy.x *
					(m_pMesh->getBoundingBox().getXMin() -
						m_pMesh->getBoundingBox().getWidth() -
						m_pMesh->getBoundingBox().getXMin()),
					axisCopy.y > 0 ? axisCopy.y *
					(m_pMesh->getBoundingBox().getYMax() +
						m_pMesh->getBoundingBox().getHeight() -
						m_pMesh->getBoundingBox().getYMax()) :
					-axisCopy.y *
					(m_pMesh->getBoundingBox().getYMin() -
						m_pMesh->getBoundingBox().getHeight()
						- m_pMesh->getBoundingBox().getYMin()),
					axisCopy.z > 0 ? axisCopy.z *
					(m_pMesh->getBoundingBox().getZMax() +
						m_pMesh->getBoundingBox().getDepth() -
						m_pMesh->getBoundingBox().getZMax()) :
					-axisCopy.z *
					(m_pMesh->getBoundingBox().getZMin() -
						m_pMesh->getBoundingBox().getDepth() -
						m_pMesh->getBoundingBox().getZMin())
				);

				m_linearCopyOffset *= transform->getLocalScale();

				m_linearCopyOffset = hmm::rotateVertexOnX(m_linearCopyOffset, transform->getLocalRotation().x);
				m_linearCopyOffset = hmm::rotateVertexOnY(m_linearCopyOffset, transform->getLocalRotation().y);
				m_linearCopyOffset = hmm::rotateVertexOnZ(m_linearCopyOffset, transform->getLocalRotation().z);

			}

		}

	}
	
	return true;

	}
}

/*		display
*
*		brief : display the mesh with the texture using the shader
*
**/
void HM_MeshComponent::display()
{

	HM_TransformComponent* transformComp;
	transformComp = static_cast<HM_TransformComponent*>(
		m_owner->getComponent("transform"));

	glm::vec3 worldPosition(0.0f, 0.0f, 0.0f);
	glm::vec3 worldRotation(0.0f, 0.0f, 0.0f);
	glm::vec3 worldScale(1.0f, 1.0f, 1.0f);

	if (transformComp)
	{

		worldPosition = transformComp->getWorldPosition();
		worldRotation = transformComp->getWorldRotation();
		worldScale = transformComp->getWorldScale();

	}

	glm::vec3 actualLinearCopyOffset = m_linearCopyOffset;

	for (unsigned int i = 0; i < m_linearCopyNumber; i++)
	{

		if (m_pGraphicsManager->isVisibleByCamera(
				m_pMesh->getBoundingBox(),
				worldPosition + (actualLinearCopyOffset * (float)i)))
		{

			if (transformComp)
				m_pGraphicsManager->drawMesh(m_pMesh, m_pTexture, m_pShader,
					worldPosition + (actualLinearCopyOffset * (float)i),
					worldRotation,
					worldScale);
			else
				m_pGraphicsManager->drawMesh(m_pMesh, m_pTexture, m_pShader);

		}

		if (DEBUG)
		{

			//(m_pMesh->getBoundingBox() * worldScale).draw(
				//worldPosition + (actualLinearCopyOffset * (float)i));

		}

	}

}

HM_Mesh* const HM_MeshComponent::getMesh() const
{

	return m_pMesh;

}

HM_Cube const HM_MeshComponent::getBoundingBox() const
{

	return m_pMesh->getBoundingBox();

}

HM_Cube const HM_MeshComponent::getScaledBoundingBox() const
{

	HM_Component* componentTransform = m_owner->getComponent("transform");

	if(componentTransform)
	{

		HM_TransformComponent* transformComponent = 
			static_cast<HM_TransformComponent*>(componentTransform);

		if(transformComponent)
			return m_pMesh->getBoundingBox() * 
				transformComponent->getWorldScale();

	}

	return m_pMesh->getBoundingBox();
}

unsigned int HM_MeshComponent::getLinearCopyNumber() const
{

	return m_linearCopyNumber;

}

glm::vec3 HM_MeshComponent::getLinearcopyOffest() const
{

	return m_linearCopyOffset;

}

/*		setupMesh
*
*		brief : loads the mesh using the graphics manager
*
*		params :
*			- meshFilePath	(string)	: path to the mesh description file
*
**/
bool HM_MeshComponent::setupMesh(std::string attributeName,
	std::map<std::string, void*> descr)
{

	// Mesh loading
	std::map<std::string, void*>::const_iterator iter = descr.find(attributeName);

	if (iter != descr.end())
	{

		std::string meshFilePath = hmu::getDataFromVoid<std::string>((*iter).second);

		if (m_owner->getComponent("textureAnimation"))
			m_pMesh = m_pGraphicsManager->loadMesh(meshFilePath,
				HM_VBO_STATIC,
				HM_VBO_DYNAMIC);
		else
			m_pMesh = m_pGraphicsManager->loadMesh(meshFilePath,
				HM_VBO_STATIC,
				HM_VBO_STATIC);

		if (m_pMesh)
			m_meshFilePath = meshFilePath;

	}
	else
	{

		GRAPHICS_LOG("Error : a mesh decribing the file must be given");

		return false;

	}

	return true;

}

/*		setupTexture
*
*		brief : loads the texture using the graphics manager
*
*		params :
*			- textureFilePath	(string)	: path to the image file
*
**/
bool HM_MeshComponent::setupTexture(std::string attributeName,
	std::map<std::string, void*> descr)
{

	// Texture loading
	std::map<std::string, void*>::const_iterator iter = descr.find(attributeName);

	if (iter != descr.end())
	{

		std::string textureFilePath = hmu::getDataFromVoid<std::string>((*iter).second);

		m_pTexture = m_pGraphicsManager->loadTexture(textureFilePath);

		if (m_pTexture)
		{
		
			m_textureFilePath = textureFilePath;

		}
		else
		{

			GRAPHICS_LOG("Error : Can't load texture");

			return false;

		}

	}
	else
	{

		GRAPHICS_LOG("Error : Can't find a texture file path reference");

		m_pTexture = NULL;

		return false;

	}

	return true;

}

/*		setupShader
*
*		brief : loads the shader using the graphics manager
*
*		params :
*			- vertexShaderFilePath	(string)	: path to the vertex shader
*												  file
*			- fragmentShaderFilePath	(string)	: path to the vertex shader
*													  file
*
**/
bool HM_MeshComponent::setupShader(std::string attribute1Name,
	std::string attribute2Name, std::map<std::string, void*> descr)
{

	// Shader loading
	std::string vertexShaderFilePath(""), fragmentShaderFilePath("");
	std::map<std::string, void*>::const_iterator iter;

	iter = descr.find(attribute1Name);
	if (iter != descr.end())
	{

		vertexShaderFilePath = hmu::getDataFromVoid<std::string>(
		(*iter).second);

	}
	else
	{

		GRAPHICS_LOG("Error : Can't find vertex shader file path");

		return false;

	}
	
	iter = descr.find(attribute2Name);
	if (iter != descr.end())
	{

		fragmentShaderFilePath = hmu::getDataFromVoid<std::string>(
		(*iter).second);
		
	}
	else
	{

		GRAPHICS_LOG("Error : Can't find fragment shader file path");

		return false;

	}

	if (vertexShaderFilePath != "" && fragmentShaderFilePath != "")
	{

		m_pShader = m_pGraphicsManager->loadShader(vertexShaderFilePath,
			fragmentShaderFilePath);

		if (m_pShader)
		{

			m_vertexShaderFilePath = vertexShaderFilePath;
			m_fragmentShaderFilePath = fragmentShaderFilePath;

		}
		else
		{

			GRAPHICS_LOG("Error : Can't load shader program");

			return false;

		}

	}
	else
	{

		GRAPHICS_LOG("Error : Vertex or fragment shader can't be empty");

		return false;

	}

	return true;

}

bool HM_MeshComponent::setupLinearCopy(std::string attribute1Name,
	std::string attribute2Name, std::string attribute3Name,
	std::map<std::string, void*> descr)
{

	

	std::map<std::string, void*>::const_iterator iter = descr.find(attribute1Name); // "linearCopyNumber"

	if (iter != descr.end())
		m_linearCopyNumber = hmu::getDataFromVoid<unsigned int>(
		(*iter).second);
	else
		m_linearCopyNumber = 1;

	if (m_linearCopyNumber > 1)
	{

		iter = descr.find(attribute2Name); // "linearCopyOffset"

		if (iter != descr.end())
			m_linearCopyOffset = hmu::getDataFromVoid<glm::vec3>(
			(*iter).second);
		else
		{
			iter = descr.find(attribute3Name); // "linearCopyAxis"

			if (iter != descr.end())
			{



			}
			else
			{

				m_linearCopyNumber = 1;

			}

		}

	}

	return true;

}

