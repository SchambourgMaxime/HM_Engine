/************************************************/
/*												*/
/*	HM_Cube.cpp									*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-08-22						*/
/*												*/
/*	Last Update : 2016-08-22					*/
/*												*/
/*	Author : Eric Laberge, Maxime Schambourg	*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#include "HM_Cube.h"
#include "../HM_GameMaster.h"
#include "../HM_GraphicsManager.h"



/*				 IMPLEMENTATION					*/

HM_Cube::HM_Cube() : 
	m_xMax(0), m_xMin(0),
	m_yMax(0), m_yMin(0),
	m_zMax(0), m_zMin(0),
	m_width(0), m_height(0), m_depth(0)
{

}

HM_Cube::HM_Cube(float xMax, float xMin, 
				 float yMax, float yMin,
				 float zMax, float zMin) :
	m_xMax(xMax), m_xMin(xMin), 
	m_yMax(yMax), m_yMin(yMin),
	m_zMax(zMax), m_zMin(zMin)
{
	
	setupSize(xMax, xMin, yMax, yMin, zMax, zMin);

}

HM_Cube::HM_Cube(glm::vec3 vecMax, glm::vec3 vecMin) :
	m_xMax(vecMax.x), m_xMin(vecMin.x),
	m_yMax(vecMax.y), m_yMin(vecMin.y),
	m_zMax(vecMax.z), m_zMin(vecMin.z)
{

	setupSize(vecMax.x, vecMin.x, vecMax.y, vecMin.y, vecMax.z, vecMin.z);

}

HM_Cube::~HM_Cube()
{
}

float HM_Cube::getXMax() const
{

	return m_xMax;

}

float HM_Cube::getXMin() const
{

	return m_xMin;


}

float HM_Cube::getYMax() const
{

	return m_yMax;


}

float HM_Cube::getYMin() const
{

	return m_yMin;

}

float HM_Cube::getZMax() const
{

	return m_zMax;

}

float HM_Cube::getZMin() const
{

	return m_zMin;

}

glm::vec3 HM_Cube::getMin() const
{

	return glm::vec3(m_xMin, m_yMin, m_zMin);

}

glm::vec3 HM_Cube::getMax() const
{

	return glm::vec3(m_xMax, m_yMax, m_zMax);

}

float HM_Cube::getWidth() const
{

	return m_width;

}

float HM_Cube::getHeight() const
{

	return m_height;

}

float HM_Cube::getDepth() const
{

	return m_depth;

}

std::vector<glm::vec3> HM_Cube::getvertices() const
{

	std::vector<glm::vec3> vertices;

	vertices.push_back(glm::vec3(m_xMin, m_yMin, m_zMin));
	vertices.push_back(glm::vec3(m_xMax, m_yMin, m_zMin));
	vertices.push_back(glm::vec3(m_xMax, m_yMax, m_zMin));
	vertices.push_back(glm::vec3(m_xMin, m_yMax, m_zMin));
	vertices.push_back(glm::vec3(m_xMin, m_yMin, m_zMax));
	vertices.push_back(glm::vec3(m_xMax, m_yMin, m_zMax));
	vertices.push_back(glm::vec3(m_xMax, m_yMax, m_zMax));
	vertices.push_back(glm::vec3(m_xMin, m_yMax, m_zMax));

	return vertices;

}

void HM_Cube::setXMax(float xMax)
{

	m_xMax = xMax;

}

void HM_Cube::setXMin(float xMin)
{

	m_xMin = xMin;

}

void HM_Cube::setYMax(float yMax)
{

	m_yMax = yMax;

}

void HM_Cube::setYMin(float yMin)
{

	m_yMin = yMin;

}

void HM_Cube::setZMax(float zMax)
{

	m_zMax = zMax;

}

void HM_Cube::setZMin(float zMin)
{

	m_zMin = zMin;

}

void HM_Cube::setMin(glm::vec3 min)
{

	m_xMin = min.x;
	m_yMin = min.y;
	m_zMin = min.z;

}

void HM_Cube::setMax(glm::vec3 max)
{

	m_xMax = max.x;
	m_yMax = max.y;
	m_zMax = max.z;

}

HM_Cube& HM_Cube::operator=(HM_Cube const & other)
{
	
	m_xMax = other.m_xMax;
	m_xMin = other.m_xMin;

	m_yMax = other.m_yMax;
	m_yMin = other.m_yMin;

	m_zMax = other.m_zMax;
	m_zMin = other.m_zMin;

	setupSize(m_xMax, m_xMin, m_yMax, m_yMin, m_zMax, m_zMin);

	return *this;

}

HM_Cube HM_Cube::operator*(float factor) const
{

	return HM_Cube(m_xMax * factor, m_xMin * factor,
				   m_yMax * factor, m_yMin * factor,
				   m_zMax * factor, m_zMin * factor);

}

HM_Cube HM_Cube::operator*(glm::vec3 factor) const
{

	return HM_Cube(m_xMax * factor.x, m_xMin * factor.x,
				   m_yMax * factor.y, m_yMin * factor.y,
				   m_zMax * factor.z, m_zMin * factor.z);

}

bool HM_Cube::intersectsWith(HM_Cube const & other)
{
	
	return !((m_xMin >= other.m_xMax) || (m_xMax <= other.m_xMin) ||
			 (m_yMin >= other.m_yMax) || (m_yMax <= other.m_yMin) ||
			 (m_zMin >= other.m_zMax) || (m_zMax <= other.m_zMin));

}

void HM_Cube::draw(glm::vec3 position) const
{

	float xMin = m_xMin + position.x; float xMax = m_xMax + position.x;
	float yMin = m_yMin + position.y; float yMax = m_yMax + position.y;
	float zMin = m_zMin + position.z; float zMax = m_zMax + position.z;

	float vertices[] = { xMin, yMin, zMax, // 0
						 xMin, yMax, zMax, // 1
						 xMax, yMax, zMax, // 2
						 xMax, yMin, zMax, // 3
						 xMin, yMin, zMin, // 4
						 xMin, yMax, zMin, // 5
						 xMax, yMax, zMin, // 6
						 xMax, yMin, zMin, // 7
						 };

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(0);

	unsigned int triangles[] = {0, 1, 2, // face front
								2, 3, 0, // face front
								3, 2, 6, // face right
								6, 7, 3, // face right
								4, 5, 6, // face back
								6, 7, 4, // face back
								1, 0, 4, // face left
								4, 5, 1, // face left
								4, 0, 3, // face bottom
								3, 7, 4, // face bottom
								5, 1, 2,
								2, 6, 5
								};

	glUseProgram(HM_GameMaster::instance()->getGraphicsManager()->
		getDefaultShader()->getProgramID());

	glm::mat4 modelviewProjection(HM_GameMaster::instance()->
		getGraphicsManager()->getProjection() *
		HM_GameMaster::instance()->getGraphicsManager()->getModelview());

	HM_GameMaster::instance()->getGraphicsManager()->
		getDefaultShader()->sendMat4("modelviewProjection", modelviewProjection);



	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, triangles);

	glUseProgram(0);


}

void HM_Cube::setupSize(float xMax, float xMin,
						float yMax, float yMin,
						float zMax, float zMin)
{
	
	m_width = xMax - xMin;
	m_height = yMax - yMin;
	m_depth = zMax - zMin;

}