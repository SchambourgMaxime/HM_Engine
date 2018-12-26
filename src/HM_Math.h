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
	static glm::vec3 getClosestPointOnBox(const HM_Cube& box, const glm::vec3 point);
	static glm::vec3 getClosestPointOnBox(const glm::vec3& boxMin, const glm::vec3& boxMax, const glm::vec3 point);

	inline static float dotProduct(const glm::vec2& v1, const glm::vec2& v2);
	inline static float dotProduct(const glm::vec3& v1, const glm::vec3& v2);

	inline static void translate(glm::mat4& m, const glm::vec3& v);
	inline static void scale(glm::mat4& m, const glm::vec3& v);

	static glm::vec3 getClosestPointOnBox(const HM_Cube& box, const glm::vec3 point)
	{
		return getClosestPointOnBox(box.getMin(), box.getMax(), point);
	}

	static glm::vec3 getClosestPointOnBox(const glm::vec3& boxMin, const glm::vec3& boxMax, const glm::vec3 point)
	{
		glm::vec3 closestPoint;

		closestPoint.x = point.x < boxMin.x ? boxMin.x : (point.x > boxMax.x ? boxMax.x : point.x);
		closestPoint.y = point.y < boxMin.y ? boxMin.y : (point.y > boxMax.y ? boxMax.y : point.y);
		closestPoint.z = point.z < boxMin.z ? boxMin.z : (point.z > boxMax.z ? boxMax.z : point.z);

		return closestPoint;
	}

	inline static float dotProduct(const glm::vec2& v1, const glm::vec2& v2)
	{
		return (v1.x * v2.x) + (v1.y * v2.y);
	}

	inline static float dotProduct(const glm::vec3& v1, const glm::vec3& v2)
	{
		return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	}

	inline static void translate(glm::mat4& m, const glm::vec3& v)
	{
		m[3] += (m[0] * v.x) + v.y * m[1] + v.z * m[2];
	}

	inline static void scale(glm::mat4& m, const glm::vec3& v)
	{
		m[0].x *= v.x;
		m[0].y *= v.x;
		m[0].z *= v.x;

		m[1].x *= v.y;
		m[1].y *= v.y;
		m[1].z *= v.y;

		m[2].x *= v.z;
		m[2].y *= v.z;
		m[2].z *= v.z;
	}

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

		float xPrime = vertex.x * cos_t - vertex.y * sin_t;
		float yPrime = vertex.y * cos_t + vertex.x * sin_t;

		return glm::vec3(xPrime, yPrime, vertex.z);

	}

}