/************************************************/
/*												*/
/*	HM_Octree.h									*/
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

// Includes GLM

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "HM_Cube.h"



/*				     HEADER						*/

struct Node
{

	HM_Cube region;

	Node* parent;

	std::vector<HM_Cube> elements;

	bool isLeaf = false;
	Node* children­[8U] = { NULL };

};

class HM_Octree
{

public:
	/** Public functions **/

	HM_Octree(unsigned int elementsPerNodeLimit = 10U,
		HM_Cube root = HM_Cube(glm::vec3(-500.0f, -500.0f, -500.0f),
			glm::vec3(500.0f, 500.0f, 500.0f)));
	~HM_Octree();

	unsigned int addElement(HM_Cube const & element);

	void makeTree();

private:
	/** Private functions **/

	unsigned int recursiveBuild(Node* start);

	void divideNode(Node* node);

	/** Private variables **/

	Node m_root;

	std::vector<Node*> leaves;

	std::vector<HM_Cube> m_elements;

	unsigned int m_elementsPerNodeLimit;

};

