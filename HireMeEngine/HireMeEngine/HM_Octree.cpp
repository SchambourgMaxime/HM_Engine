/************************************************/
/*												*/
/*	HM_Octree.cpp								*/
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

#include "HM_Octree.h"



/*				 IMPLEMENTATION					*/



HM_Octree::HM_Octree(unsigned int elementsPerNodeLimit, HM_Cube root)
{

	m_elementsPerNodeLimit = elementsPerNodeLimit;

	m_root.region = root;

}

HM_Octree::~HM_Octree()
{
}

unsigned int HM_Octree::addElement(HM_Cube const & element)
{

	if(element.getXMin() < m_root.region.getXMin())
		m_root.region.setXMin(element.getXMin());
	if (element.getYMin() < m_root.region.getYMin())
		m_root.region.setYMin(element.getYMin());
	if (element.getZMin() < m_root.region.getZMin())
		m_root.region.setZMin(element.getZMin());

	if (element.getXMax() < m_root.region.getXMax())
		m_root.region.setXMax(element.getXMax());
	if (element.getYMax() < m_root.region.getYMax())
		m_root.region.setYMax(element.getYMax());
	if (element.getZMax() < m_root.region.getZMax())
		m_root.region.setZMax(element.getZMax());

	m_root.elements.push_back(element);

	m_elements.push_back(element);

	return m_elements.size() - 1;

}

void HM_Octree::makeTree()
{

	if(m_elements.size() < m_elementsPerNodeLimit)
		return;
	else
		recursiveBuild(&m_root);

}

unsigned int HM_Octree::recursiveBuild(Node* node)
{

	if(node->elements.size() == 0U)
		return 0;

	if(node->elements.size() <= m_elementsPerNodeLimit)
	{
		
		if (!node->isLeaf && node->elements.size() > 0)
		{

			leaves.push_back(node);

			node->isLeaf = true;

		}

		return node->elements.size();

	}
	else
	{

		divideNode(node);

		for (unsigned int i = 0; i < node->elements.size(); i++)
			recursiveBuild(node->children­[i]);

		return 0;

	}

	return 0;
}

void HM_Octree::divideNode(Node* node)
{

	float xMiddle = node->region.getXMin() + node->region.getWidth() / 2;
	float yMiddle = node->region.getYMin() + node->region.getHeight() / 2;
	float zMiddle = node->region.getZMin() + node->region.getDepth() / 2;

	// Bottom front left
	node->children­[0]->region = HM_Cube(node->region.getXMin(), xMiddle,
										node->region.getYMin(), yMiddle,
										node->region.getZMin(), zMiddle);
	// Top front left
	node->children­[1]->region = HM_Cube(node->region.getXMin(), xMiddle,
										yMiddle, node->region.getYMax(),
										node->region.getZMin(), zMiddle);
	// Top back left
	node->children­[2]->region = HM_Cube(node->region.getXMin(), xMiddle,
										yMiddle, node->region.getYMax(),
										zMiddle, node->region.getZMax());
	// Bottom back left
	node->children­[3]->region = HM_Cube(node->region.getXMin(), xMiddle,
										node->region.getYMin(), yMiddle,
										zMiddle, node->region.getZMax());
	// Bottom front right
	node->children­[4]->region = HM_Cube(xMiddle, node->region.getXMax(),
										node->region.getYMin(), yMiddle,
										node->region.getZMin(), zMiddle);
	// Top front right
	node->children­[5]->region = HM_Cube(xMiddle, node->region.getXMax(),
										yMiddle, node->region.getYMax(),
										node->region.getZMin(), zMiddle);
	// Top back right
	node->children­[6]->region = HM_Cube(xMiddle, node->region.getXMax(),
										yMiddle, node->region.getYMax(),
										zMiddle, node->region.getZMax());
	// Bottom back right
	node->children­[7]->region = HM_Cube(xMiddle, node->region.getXMax(),
										node->region.getYMin(), yMiddle,
										zMiddle, node->region.getZMax());

	for (unsigned int i = 0; i < node->elements.size(); i++)
	{

		

	}

}
