/************************************************/
/*												*/
/*	HM_CollisionManager.h						*/
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

#pragma once

#include <vector>

#include "HM_Octree.h"
#include "HM_Cube.h"
#include "HM_SceneObject.h"



/*				     HEADER						*/

struct BoxCollider
{

	size_t id;

	HM_Cube collider;
	bool isTrigger;

	HM_SceneObject* owner;

};

class HM_CollisionManager
{

public:
	/** Public functions **/

	HM_CollisionManager();
	~HM_CollisionManager();

	size_t createNewBoxCollider(HM_Cube collider,
		bool isTrigger,
		HM_SceneObject* owner);

private:
	/** Private variables **/
	
	std::vector<BoxCollider> m_colliders;
	HM_Octree m_octree;


};

