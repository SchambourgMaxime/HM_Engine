/************************************************/
/*												*/
/*	HM_Mesh.h									*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-08-11						*/
/*												*/
/*	Last Update : 2016-08-22					*/
/*												*/
/*	Author : Yves Chiricota, Maxime Schambourg	*/
/*												*/
/************************************************/


/*				  PREPROCESSOR					*/

#pragma once

// Include Apple
#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif // __APPLE__

// Include Windows
#ifdef _WIN32
#include <GL/glew.h>

// Include Apple
#elif __APPLE__
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>

// Include UNIX/Linux
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif

#include "HM_FileHandler.h"
#include "HM_GameMaster.h"
#include "HM_Texture.h"
#include "HM_Cube.h"

#include <string>

class HM_Triangle;



/*				     HEADER						*/

/*		Class HM_Vertex
*
*		brief : Represents a vertex of a mesh
*
**/
class HM_Vertex
{

public:
	/** Public functions **/

	//Constructor
	HM_Vertex(unsigned int index, glm::vec3 position, glm::vec2 uv);
	// Destructor
	~HM_Vertex();


		// --- Operator overloads ---

	bool operator==(HM_Vertex const & otherVertex);
	bool operator!=(HM_Vertex const & otherVertex);

	float operator[](unsigned char index);


		// --- Accessors ---

	unsigned int getIndex() const;
	glm::vec3 const & getPosition() const;
	glm::vec2 const & getUV() const;

	void addTriangle(HM_Triangle* const triangle);

	GLfloat* putVertexCoords(GLfloat* pointerToFill);
	GLfloat* putVertexTexCoords(GLfloat* pointerToFill);

	void putVertexCoords(std::vector<GLfloat>* vectorToFill);
	void putVertexTexCoords(std::vector<GLfloat>* vectorToFill);

private:
	/** Private variables **/

	unsigned int m_index;

	glm::vec3 m_position;
	glm::vec2 m_uv;
	//glm::vec3 m_normal;

	std::list<const HM_Triangle*> m_triangles;

};

/*		Class HM_Triangle
*
*		brief : Represents a triangle of a mesh
*
**/
class HM_Triangle
{

public:
	/** Public functions **/

	//Constructor
	HM_Triangle(HM_Vertex* v0, HM_Vertex* v1, HM_Vertex* v2);
	// Destructor
	~HM_Triangle();

	int* putTriangles(int* pointerToFill);

	void putTriangles(std::vector<int>* vectorToFill);

private:
	/** Private variables **/

	const HM_Vertex *m_v0, *m_v1, *m_v2;
	//glm::vec3 m_normal;

};

/*		Class HM_Mesh
*
*		brief : Represents a mesh. Can load OBJ file
*
**/
class HM_Mesh
{

public:
	/** Public functions **/

	// Constructor
	HM_Mesh(std::string meshFilePath,
			unsigned char changeFrequencyCoords = HM_VBO_STATIC,
			unsigned char changeFrequencyTexCoords = HM_VBO_STATIC);
	// Destructor
	~HM_Mesh();

		// --- Accessors ---

	size_t getNumberCoords() const;
	size_t getNumberTexCoords() const;
	size_t getNumberIndices() const;

	size_t getByteSizeCoords() const;
	size_t getByteSizeTexCoords() const;
	size_t getByteSizeIndices() const;

	unsigned char getChangeFrequencyCoords() const;
	unsigned char getChangeFrequencyTexCoords() const;

	size_t getVBOBytesOffsetCoords() const;
	size_t getVBOBytesOffsetTexCoords() const;
	size_t getVBOBytesOffsetIndices() const;

	HM_Cube const & getBoundingBox() const;

	void setChangeFrequencyCoords(bool dynamic = true);
	void setChangeFrequencyTexCoords(bool dynamic = true);

	void setVBOBytesOffsetCoords(size_t vboOffsetCoords);
	void setVBOBytesOffsetTexCoords(size_t vboOffsetTexCoords);
	void setVBOBytesOffsetIndices(size_t vboOffsetIndices);

	void setOriginalVBOBytesOffsetCoords(size_t originalVBOOffsetCoords);
	void setOriginalVBOBytesOffsetTexCoords(size_t originalVBOOffsetTexCoords);

	HM_Vertex* const getVertexAtIndex(unsigned int index) const;
	int getVertexIndex(HM_Vertex const & vertex) const;

	void allocVBOData();

	GLfloat* putMeshCoords(GLfloat* pointerToFill);
	GLfloat* putMeshTexCoords(GLfloat* pointerToFill);

	void putMeshCoords(std::vector<GLfloat>* vectorToFill);
	void putMeshTexCoords(std::vector<GLfloat>* vectorToFill);

	int* putMeshIndices(int* vectorToFill);
	void putMeshIndices(std::vector<int>* vectorToFill);

	void draw(HM_Texture* texture);


protected:
	/** Protected functions **/

	HM_Mesh();


	/** Protected variables **/

	std::string m_meshFilePath;

	// Array of vertices
	std::vector<HM_Vertex*> m_vertices;
	// Array of triangles
	std::vector<HM_Triangle*> m_triangles;

	// Box that surrounds the mesh touching the extremes of each coordinates
	HM_Cube m_boundingBox;


	// Number of vertices
	size_t m_nbCoords;
	// Number of UV coordinates
	size_t m_nbTexCoords;
	// Number of triangles
	size_t m_nbIndices;

	// Size of all vertices in bytes
	size_t m_byteSizeCoords;
	// Size of all texture coordinates in bytes
	size_t m_byteSizetexCoords;
	// Size of all indices in bytes
	size_t m_byteSizeIndices;

	// Position of the coords in the graphics VBO
	size_t m_vboBytesOffsetCoords;
	// Position of the texCoords in the graphics VBO
	size_t m_vboBytesOffsetTexCoords;
	// Position of the indices in the graphics VBO
	size_t m_vboBytesOffsetIndices;

	// Position of the coords in the graphics VBO
	size_t m_originalVBOBytesOffsetCoords;
	// Position of the texCoords in the graphics VBO
	size_t m_originalVBOBytesOffsetTexCoords;

	unsigned char m_changeFrequencyCoords;
	unsigned char m_changeFrequencyTexCoords;

private:
	/** Private functions **/

	void loadOBJ(std::string const & objFile);

};

