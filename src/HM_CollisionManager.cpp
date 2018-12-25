/************************************************/
/*												*/
/*	HM_CollisionManager.cpp						*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-10-20						*/
/*												*/
/*	Last Update : 2016-10-20					*/
/*												*/
/*	Author : Eric Laberge, Maxime Schambourg	*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#include "HM_CollisionManager.h"



/*				 IMPLEMENTATION					*/

HM_CollisionManager::HM_CollisionManager()
{



}


HM_CollisionManager::~HM_CollisionManager()
{
}

unsigned int HM_CollisionManager::createNewBoxCollider(HM_Cube collider,
	bool isTrigger,
	HM_SceneObject* owner)
{

	BoxCollider boxCollider;

	boxCollider.id = m_octree.addElement(collider);

	boxCollider.collider = collider;
	boxCollider.isTrigger = isTrigger;
	boxCollider.owner = owner;

	m_colliders.push_back(boxCollider);

	return (unsigned int)m_colliders.size() - 1;

}
