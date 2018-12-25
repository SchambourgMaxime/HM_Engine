/************************************************/
/*												*/
/*	HM_SpriteComponent.cpp						*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-09-05						*/
/*												*/
/*	Last Update : 2016-09-05					*/
/*												*/
/*	Author : Eric Laberge, Maxime Schambourg	*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#include "HM_SpriteComponent.h"
#include "../HM_Sprite.h"



/*				 IMPLEMENTATION					*/

	// --- OVERRIDEN PUBLIC FUNCTIONS ---

HM_SpriteComponent* HM_SpriteComponent::create(HM_SceneObject* owner) const
{

	return new HM_SpriteComponent(owner);

}

bool HM_SpriteComponent::setup(std::map<std::string, void*> descr)
{

	m_pGraphicsManager = HM_GameMaster::instance()->getGraphicsManager();

	if (!m_pGraphicsManager)
	{

		GAME_LOG->_ADDLINETOLOG_LINE("Error : graphics manager is not set for mesh \'" + m_meshFilePath + "\'", __LINE__ - 5);

		m_pMesh = NULL;
		m_owner = NULL;
		return false;

	}

	// Name loading
	std::map<std::string, void*>::const_iterator iter = descr.find(
		"spriteName");
	if (iter != descr.end())
	{

		m_name = hmu::getDataFromVoid<std::string>((*iter).second);

	}
	else
	{

		GRAPHICS_LOG("Error : sprites must have a name");

		return false;

	}

	setupTexture("spriteTextureFile", descr);

	setupSprite("spriteFile", "spriteSize", "spritePivot",
		"spriteUVMin", "spriteUVMax", descr);

	setupShader("spriteVertexShader", "spriteFragmentShader", descr);

	setupLinearCopy("linearCopyNumber", "linearCopyOffset", "linearCopyAxis",
		descr);

	return true;

}

void HM_SpriteComponent::update()
{

	if (m_isAnimated)
	{

		HM_Sprite* sprite = static_cast<HM_Sprite*>(m_pMesh);

		if (sprite)
			sprite->updateSprite();

	}

}

bool HM_SpriteComponent::setupSprite(std::string Attribute1Name,
	std::string Attribute2Name,
	std::string Attribute3Name,
	std::string Attribute4Name,
	std::string Attribute5Name,
	std::map<std::string, void*> descr)
{

	// Sprite loading
	std::map<std::string, void*>::const_iterator iter = descr.find(Attribute1Name);
	if (iter != descr.end())
	{

		std::string spriteFilePath = hmu::getDataFromVoid<std::string>((*iter).second);

		m_pMesh = m_pGraphicsManager->loadSprite(m_name, spriteFilePath,
			HM_VBO_STATIC);

		if (m_pMesh)
			m_meshFilePath = spriteFilePath;

		m_isAnimated = true;

	}
	else
	{

		// Size loading
		iter = descr.find(Attribute2Name);

		if (iter != descr.end())
		{

			m_size = hmu::getDataFromVoid<glm::vec2>((*iter).second);

		}
		else
		{

			if (m_pTexture)
			{

				m_size = m_pTexture->getSize();

				if (m_size.x > 1.0f || m_size.y > 1.0f)
				{

					float ratioDivisor = std::max(m_size.x, m_size.y);

					m_size.x /= ratioDivisor;
					m_size.y /= ratioDivisor;

				}

			}

		}

		// Pivot loading
		iter = descr.find(Attribute3Name);

		if (iter != descr.end())
			m_pivot = hmu::getDataFromVoid<glm::vec2>((*iter).second);


		// UV min loading
		iter = descr.find(Attribute4Name);

		if (iter != descr.end())
			m_uvMin = hmu::getDataFromVoid<glm::vec2>((*iter).second);

		// UV max loading
		iter = descr.find(Attribute5Name);

		if (iter != descr.end())
			m_uvMax = hmu::getDataFromVoid<glm::vec2>((*iter).second);
		else
			m_uvMax = glm::vec2(1.0f, 1.0f);

		m_pMesh = m_pGraphicsManager->loadSprite(m_name, m_size, m_pivot,
			m_uvMin, m_uvMax, HM_VBO_STATIC);

		m_isAnimated = false;

	}

	return true;

}

