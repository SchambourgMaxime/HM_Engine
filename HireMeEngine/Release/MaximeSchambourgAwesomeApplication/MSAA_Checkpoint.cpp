/************************************************/
/*												*/
/*	MSAA_Checkpoint.cpp							*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-11-22						*/
/*												*/
/*	Last Update : 2016-11-24					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#include "MSAA_Checkpoint.h"
#include "MSAA_Character.h"



/*				 IMPLEMENTATION					*/

MSAA_Checkpoint::~MSAA_Checkpoint()
{
}

MSAA_Checkpoint* MSAA_Checkpoint::create(HM_SceneObject* owner) const
{

	return new MSAA_Checkpoint(owner);

}

void MSAA_Checkpoint::onCollision(HM_SceneObject* other, Direction direction)
{

	MSAA_Character* character =
		other->getComponentInObject<MSAA_Character>("character");

	if (character)
	{

		HM_TransformComponent* characterTransform =
			other->getComponentInObject<HM_TransformComponent>("transform");

		if (characterTransform)
		{

			characterTransform->setOriginalPosition(
				characterTransform->getLocalPosition());

		}

	}

}
