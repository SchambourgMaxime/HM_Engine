/************************************************/
/*												*/
/*	HM_Cube.h									*/
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

#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>



/*				     HEADER						*/

class HM_Cube
{

public:
	/** Public functions **/

	// Default constructor
	HM_Cube();
	// Constructor with all points
	HM_Cube(float xMax, float xMin,
			float yMax, float yMin,
			float zMax, float zMin);
	// Copy constructor
	HM_Cube(HM_Cube const & other);
	// Constructor with vectors
	HM_Cube(glm::vec3 vecMax, glm::vec3 vecMin);
	// Destructor
	~HM_Cube();

	HM_Cube& operator=(HM_Cube const & other);
	HM_Cube operator*(float factor) const;
	HM_Cube operator*(glm::vec3 factor) const;
	HM_Cube operator+(float factor) const;
	HM_Cube operator+(glm::vec3 factor) const;

	HM_Cube operator*=(float factor);
	HM_Cube operator*=(glm::vec3 factor);
	HM_Cube operator+=(float factor);
	HM_Cube operator+=(glm::vec3 factor);

	float getXMax() const;
	float getXMin() const;
	float getYMax() const;
	float getYMin() const;
	float getZMax() const;
	float getZMin() const;

	glm::vec3 getMin() const;
	glm::vec3 getMax() const;

	float getWidth() const;
	float getHeight() const;
	float getDepth() const;

	std::vector<glm::vec3> getvertices() const;

	void setXMax(float xMax);
	void setXMin(float xMin);
	void setYMax(float yMax);
	void setYMin(float yMin);
	void setZMax(float zMax);
	void setZMin(float zMin);

	void setMin(glm::vec3 min);
	void setMax(glm::vec3 max);

	// Checks to see if a given HM_Cube instersects with this one
	bool intersectsWith(HM_Cube const & other);

	HM_Cube getRotatedCube(glm::vec3 eulerAngles) const;

	void rotate(glm::vec3 eulerAngles);

	void rotateOnX(float theta);
	void rotateOnY(float theta);
	void rotateOnZ(float theta);

	void draw(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 eulerAngles = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) const;

private:
	/** Private functions **/

	void copyCube(HM_Cube const & other);

	// Setup for the width, height and depth
	void setupSize(float xMax, float xMin,
				   float yMax, float yMin,
				   float zMax, float zMin);


	/** Private variables **/

	float m_xMax, m_xMin;
	float m_yMax, m_yMin;
	float m_zMax, m_zMin;

	float m_width, m_height, m_depth;

};

