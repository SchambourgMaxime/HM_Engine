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
#include "HM_GameMaster.h"
#include "HM_GraphicsManager.h"
#include "HM_Math.h"



/*				 IMPLEMENTATION					*/

HM_Cube::HM_Cube() : 
	m_min(0.f, 0.f, 0.f), m_max(0.f, 0.f, 0.f)
{

}

HM_Cube::HM_Cube(float xMin, float yMin, float zMin,
				 float xMax, float yMax, float zMax) :
	m_min(xMin, yMin, zMin), m_max(xMax, yMax, zMax)
{
}

HM_Cube::HM_Cube(glm::vec3 min, glm::vec3 max) :
	m_min(min), m_max(max)
{
}

HM_Cube::HM_Cube(HM_Cube const & other)
{

	copyCube(other);

}

HM_Cube::~HM_Cube()
{
}

HM_Cube& HM_Cube::operator=(HM_Cube const & other)
{

	copyCube(other);

	return *this;

}

HM_Cube HM_Cube::operator*(float factor) const
{

	return HM_Cube(m_min * factor, m_max * factor);

}

HM_Cube HM_Cube::operator*(glm::vec3 factor) const
{

	return HM_Cube(m_min.x * factor.x, m_min.y * factor.y, m_min.z * factor.z,
				   m_max.x * factor.x, m_max.y * factor.y, m_max.z * factor.z );

}

HM_Cube HM_Cube::operator+(float factor) const
{

	return HM_Cube(m_min + factor, m_max + factor);

}

HM_Cube HM_Cube::operator+(glm::vec3 factor) const
{

	return HM_Cube(m_min.x + factor.x, m_min.y + factor.y, m_min.z + factor.z,
	               m_max.x + factor.x, m_max.y + factor.y, m_max.z + factor.z);

}

HM_Cube HM_Cube::operator*=(float factor)
{

	m_max.x *= factor;
	m_min.x *= factor;
	m_max.y *= factor;
	m_min.y *= factor;
	m_max.z *= factor;
	m_min.z *= factor;

	return HM_Cube(*this);

}

HM_Cube HM_Cube::operator*=(glm::vec3 factor)
{

	m_max.x *= factor.x;
	m_min.x *= factor.x;
	m_max.y *= factor.y;
	m_min.y *= factor.y;
	m_max.z *= factor.z;
	m_min.z *= factor.z;

	return HM_Cube(*this);

}

HM_Cube HM_Cube::operator+=(float factor)
{

	m_max.x += factor;
	m_min.x += factor;
	m_max.y += factor;
	m_min.y += factor;
	m_max.z += factor;
	m_min.z += factor;

	return HM_Cube(*this);

}

HM_Cube HM_Cube::operator+=(glm::vec3 factor)
{

	m_max.x += factor.x;
	m_min.x += factor.x;
	m_max.y += factor.y;
	m_min.y += factor.y;
	m_max.z += factor.z;
	m_min.z += factor.z;

	return HM_Cube(*this);

}

float HM_Cube::getXMax() const
{

	return m_max.x;

}

float HM_Cube::getXMin() const
{

	return m_min.x;


}

float HM_Cube::getYMax() const
{

	return m_max.y;


}

float HM_Cube::getYMin() const
{

	return m_min.y;

}

float HM_Cube::getZMax() const
{

	return m_max.z;

}

float HM_Cube::getZMin() const
{

	return m_min.z;

}

float HM_Cube::getWidth() const
{

	return m_max.x - m_min.x;

}

float HM_Cube::getHeight() const
{

	return m_max.y - m_min.y;

}

float HM_Cube::getDepth() const
{

	return m_max.z - m_min.z;

}

glm::vec3 HM_Cube::getVertice(unsigned int index) const
{
	switch (index)
	{
		case 0 : return glm::vec3(m_min.x, m_min.y, m_min.z);
		case 1 : return glm::vec3(m_max.x, m_min.y, m_min.z);
		case 2 : return glm::vec3(m_max.x, m_max.y, m_min.z);
		case 3 : return glm::vec3(m_min.x, m_max.y, m_min.z);
		case 4 : return glm::vec3(m_min.x, m_min.y, m_max.z);
		case 5 : return glm::vec3(m_max.x, m_min.y, m_max.z);
		case 6 : return glm::vec3(m_max.x, m_max.y, m_max.z);
		case 7 : return glm::vec3(m_min.x, m_max.y, m_max.z);
		default : return glm::vec3();
	}

}

void HM_Cube::getVertices(std::vector<glm::vec3>& vertices) const
{
	if (vertices.size() != 8)
		vertices.resize(8);

	vertices.push_back(glm::vec3(m_min.x, m_min.y, m_min.z));
	vertices.push_back(glm::vec3(m_max.x, m_min.y, m_min.z));
	vertices.push_back(glm::vec3(m_max.x, m_max.y, m_min.z));
	vertices.push_back(glm::vec3(m_min.x, m_max.y, m_min.z));
	vertices.push_back(glm::vec3(m_min.x, m_min.y, m_max.z));
	vertices.push_back(glm::vec3(m_max.x, m_min.y, m_max.z));
	vertices.push_back(glm::vec3(m_max.x, m_max.y, m_max.z));
	vertices.push_back(glm::vec3(m_min.x, m_max.y, m_max.z));

}

void HM_Cube::setXMax(float xMax)
{

	m_max.x = xMax;

}

void HM_Cube::setXMin(float xMin)
{

	m_min.x = xMin;

}

void HM_Cube::setYMax(float yMax)
{

	m_max.y = yMax;

}

void HM_Cube::setYMin(float yMin)
{

	m_min.y = yMin;

}

void HM_Cube::setZMax(float zMax)
{

	m_max.z = zMax;

}

void HM_Cube::setZMin(float zMin)
{

	m_min.z = zMin;

}

void HM_Cube::setMin(glm::vec3 min)
{

	m_min.x = min.x;
	m_min.y = min.y;
	m_min.z = min.z;

}

void HM_Cube::setMax(glm::vec3 max)
{

	m_max.x = max.x;
	m_max.y = max.y;
	m_max.z = max.z;

}

bool HM_Cube::intersectsWith(HM_Cube const & other)
{
	if (m_min.x >= other.m_max.x) return false;
	if (m_max.x <= other.m_min.x) return false;
	if (m_min.y >= other.m_max.y) return false;
	if (m_max.y <= other.m_min.y) return false;
	if (m_min.z >= other.m_max.z) return false;
	if (m_max.z <= other.m_min.z) return false;

	return true;

}

HM_Cube HM_Cube::getRotatedCube(glm::vec3 eulerAngles) const
{

	HM_Cube returnCube(*this);

	returnCube.rotate(eulerAngles);

	return returnCube;

}

void HM_Cube::rotate(glm::vec3 eulerAngles)
{
// 	if(eulerAngles.x != 0.0f)
// 		rotateOnX(eulerAngles.x);
// 	if (eulerAngles.y != 0.0f)
// 		rotateOnY(eulerAngles.y);
// 	if (eulerAngles.z != 0.0f)
// 		rotateOnZ(eulerAngles.z);

}

// void HM_Cube::rotateOnX(float theta)
// {
// 
// 	float sin_t = sin(theta * (float)M_PI / 180);
// 	float cos_t = cos(theta * (float)M_PI / 180);
// 
// 	float yMax = 0, yMin = 0;
// 	float zMax = 0, zMin = 0;
// 
// 	std::vector<glm::vec3> vertices;
// 	getvertices(vertices);
// 
// 	for (unsigned int i = 0; i < vertices.size(); i++)
// 	{
// 
// 		glm::vec3 rotatedVertex = hmm::rotateVertexOnX(vertices[i], theta);
// 
// 		if(rotatedVertex.y > yMax)
// 			yMax = rotatedVertex.y;
// 		else if(rotatedVertex.y < yMin)
// 			yMin = rotatedVertex.y;
// 
// 		if (rotatedVertex.z > zMax)
// 			zMax = rotatedVertex.z;
// 		else if (rotatedVertex.z < zMin)
// 			zMin = rotatedVertex.z;
// 	}
// 
// 	m_max.y = yMax;
// 	m_max.z = zMax;
// 	m_min.y = yMin;
// 	m_min.z = zMin;
// 
// }
// 
// void HM_Cube::rotateOnY(float theta)
// {
// 
// 	theta = -theta;
// 
// 	float sin_t = sin(theta * (float)M_PI / 180);
// 	float cos_t = cos(theta * (float)M_PI / 180);
// 
// 	float xMax = 0, xMin = 0;
// 	float zMax = 0, zMin = 0;
// 
// 	std::vector<glm::vec3> vertices;
// 	getvertices(vertices);
// 
// 	for (unsigned int i = 0; i < vertices.size(); i++)
// 	{
// 
// 		glm::vec3 rotatedVertex = hmm::rotateVertexOnY(vertices[i], theta);
// 
// 		if (rotatedVertex.x > xMax)
// 			xMax = rotatedVertex.x;
// 		else if (rotatedVertex.x < xMin)
// 			xMin = rotatedVertex.x;
// 
// 		if (rotatedVertex.z > zMax)
// 			zMax = rotatedVertex.z;
// 		else if (rotatedVertex.z < zMin)
// 			zMin = rotatedVertex.z;
// 	}
// 
// 	m_max.x = xMax;
// 	m_max.z = zMax;
// 	m_min.x = xMin;
// 	m_min.z = zMin;
// 
// }
// 
// void HM_Cube::rotateOnZ(float theta)
// {
// 
// 	float sin_t = sin(theta * (float)M_PI / 180);
// 	float cos_t = cos(theta * (float)M_PI / 180);
// 
// 	float xMax = 0, xMin = 0;
// 	float yMax = 0, yMin = 0;
// 
// 	std::vector<glm::vec3> vertices;
// 	getvertices(vertices);
// 
// 	for (unsigned int i = 0; i < vertices.size(); i++)
// 	{
// 
// 		glm::vec3 vertice = vertices[i];
// 		float xPrime = vertice.x * cos_t - vertice.y * sin_t;
// 		float yPrime = vertice.y * cos_t + vertice.x * sin_t;
// 
// 		if (xPrime > xMax)
// 			xMax = xPrime;
// 		else if (xPrime < xMin)
// 			xMin = xPrime;
// 
// 		if (yPrime > yMax)
// 			yMax = yPrime;
// 		else if (yPrime < yMin)
// 			yMin = yPrime;
// 	}
// 
// 	m_max.x = xMax;
// 	m_max.y = yMax;
// 	m_min.x = xMin;
// 	m_min.y = yMin;
// 
// }

void HM_Cube::draw(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale) const
{

	HM_Cube temp(*this);

	if(eulerAngles != glm::vec3(0.0f, 0.0f, 0.0f))
		temp.rotate(eulerAngles);

	//temp *= 4;

	float xMin = temp.m_min.x + position.x;
	float xMax = temp.m_max.x + position.x;
	float yMin = temp.m_min.y + position.y;
	float yMax = temp.m_max.y + position.y;
	float zMin = temp.m_min.z + position.z;
	float zMax = temp.m_max.z + position.z;

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


	glUseProgram(HM_GameMaster::instance()->getGraphicsManager()->
		getDefaultShader()->getProgramID());

	glm::mat4 modelviewProjection(HM_GameMaster::instance()->
		getGraphicsManager()->getProjection() *
		HM_GameMaster::instance()->getGraphicsManager()->getModelview());

	HM_GameMaster::instance()->getGraphicsManager()->
		getDefaultShader()->sendMat4("modelviewProjection", modelviewProjection);

	if (xMax == xMin)
	{

		unsigned int triangles[] = {0, 1,	// face front
									1, 5,	// face front
									5, 4,	// face front
									4, 0,	// face front
									};


		glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, triangles);

	}
	else if (yMax == yMin)
	{

		unsigned int triangles[] = {0, 4,	// face front
									4, 7,	// face front
									7, 3,	// face front
									3, 0,	// face front
									};

		glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, triangles);

	}
	else if (zMax == zMin)
	{

		unsigned int triangles[] = {0, 1,	// face front
									1, 2,	// face front
									2, 3,	// face front
									3, 0,	// face front
									};

		glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, triangles);

	}
	else
	{

		unsigned int triangles[] = {0, 1,	// face front
									1, 2,	// face front
									2, 3,	// face front
									3, 0,	// face front
									2, 6,	// face right
									3, 7,	// face right
									6, 5,	// face back
									5, 4,	// face back
									4, 7,	// face back
									7, 6,	// face back
									5, 1,	// face left
									4, 0,	// face left
									};

		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, triangles);

	}

	glUseProgram(0);


}

void HM_Cube::copyCube(HM_Cube const & other)
{

	m_max = other.m_max;
	m_min = other.m_min;

	m_max.y = other.m_max.y;
	m_min.y = other.m_min.y;

	m_max.z = other.m_max.z;
	m_min.z = other.m_min.z;

}
