/************************************************/
/*												*/
/*	HM_HUDComponent.cpp							*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-09-13						*/
/*												*/
/*	Last Update : 2016-09-13					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#include "HM_HUDComponent.h"
#include "HM_TransformComponent.h"
#include "../HM_Sprite.h"

#include <algorithm>



/*				 IMPLEMENTATION					*/

// --- OVERRIDEN PUBLIC FUNCTIONS ---

HM_HUDComponent* HM_HUDComponent::create(HM_SceneObject* owner) const
{

	return new HM_HUDComponent(owner);

}

bool HM_HUDComponent::setup(std::map<std::string, void*> descr)
{

	m_pGraphicsManager = HM_GameMaster::instance()->getGraphicsManager();

	if (!m_pGraphicsManager)
	{

		GAME_LOG->_ADDLINETOLOG_LINE("can't load HUD", __LINE__ - 5);

		return false;

	}

	// Name loading
	std::map<std::string, void*>::const_iterator iter = descr.find(
		"HUDName");
	if (iter != descr.end())
	{

		m_name = hmu::getDataFromVoid<std::string>((*iter).second);

	}
	else
	{

		GRAPHICS_LOG("Error : HUD must have a name");

		return false;

	}

	setupTexture("HUDImage", descr);

	setupLinearCopy("HUDlinearCopyNumber", "HUDlinearCopyOffset", "HUDlinearCopyAxis",
		descr);

	// Loading sprite
	iter = descr.find("HUDAnimationFile");
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
		iter = descr.find("HUDSize");

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

					m_size.x /= m_pGraphicsManager->getWindowWidth();
					m_size.y /= m_pGraphicsManager->getWindowHeight();

				}

			}
			else
			{

				m_size = glm::vec2(1.0f, 1.0f);

			}
		}

		// Pivot loading
		iter = descr.find("HUDPivot");

		if (iter != descr.end())
			m_pivot = hmu::getDataFromVoid<glm::vec2>((*iter).second);
		else
			m_pivot = glm::vec2(0.0f, 0.0f);

		m_pMesh = m_pGraphicsManager->loadSprite(m_name, m_size, m_pivot,
			glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), HM_VBO_STATIC);

		m_isAnimated = false;

	}

	return true;

}

void HM_HUDComponent::display()
{

	HM_TransformComponent* transformComp;
	transformComp = static_cast<HM_TransformComponent*>(
		m_owner->getComponent("transform"));

	glm::vec2 position(0.0f, 0.0f);

	if (transformComp)
	{

		position = glm::vec2(transformComp->getLocalPosition().x,
			transformComp->getLocalPosition().y);

	}

	if (position.x > 1.0f || position.y > 1.0f)
	{

		position.x /= m_pGraphicsManager->getWindowWidth();
		position.y /= m_pGraphicsManager->getWindowHeight();

	}

	HM_Sprite* sprite = static_cast<HM_Sprite*>(m_pMesh);

	if (!sprite)
	{

		GRAPHICS_LOG("Error : mesh not a sprite in HUD \'" + m_name + "\'");

		return;

	}

	for (unsigned int i = 0; i < m_linearCopyNumber; i++)
	{

		m_pGraphicsManager->drawHUD(sprite, m_pTexture,
			position + (glm::vec2(m_linearCopyOffset) * (float)i));

	}

}

