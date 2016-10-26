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

	//for (unsigned int i = 0; i < 8; i++)
	//	m_root.children­[i]­ = NULL;

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

		return 0;

	}


}
