/************************************************/
/*												*/
/*	HM_Math.h									*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-11-04						*/
/*												*/
/*	Last Update : 2016-11-04					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>



/*				     HEADER						*/

namespace hmm
{

	static glm::vec3 rotateVertexOnX(glm::vec3 vertex, float angle)
	{

		float sin_t = sin(angle * (float)M_PI / 180);
		float cos_t = cos(angle * (float)M_PI / 180);

		float yPrime = vertex.y * cos_t - vertex.z * sin_t;
		float zPrime = vertex.z * cos_t + vertex.y * sin_t;

		return glm::vec3(vertex.x, yPrime, zPrime);

	}

	static glm::vec3 rotateVertexOnY(glm::vec3 vertex, float angle)
	{

		float sin_t = sin(angle * (float)M_PI / 180);
		float cos_t = cos(angle * (float)M_PI / 180);

		float xPrime = vertex.x * cos_t - vertex.z * sin_t;
		float zPrime = vertex.z * cos_t + vertex.x * sin_t;

		return glm::vec3(xPrime, vertex.y, zPrime);

	}

	static glm::vec3 rotateVertexOnZ(glm::vec3 vertex, float angle)
	{

		float sin_t = sin(angle * (float)M_PI / 180);
		float cos_t = cos(angle * (float)M_PI / 180);

		float xPrime = vertex.x * cos_t - vertex.z * sin_t;
		float zPrime = vertex.z * cos_t + vertex.x * sin_t;

		return glm::vec3(xPrime, vertex.y, zPrime);

	}

}