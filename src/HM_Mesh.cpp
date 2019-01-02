/************************************************/
/*												*/
/*	HM_Mesh.cpp									*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-08-09						*/
/*												*/
/*	Last Update : 2016-08-22					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#include "HM_Mesh.h"
#include "HM_GraphicsManager.h"



/*				 IMPLEMENTATION					*/


HM_Vertex::HM_Vertex(unsigned int index, glm::vec3 position, glm::vec2 uv):
m_index(index), m_position(position), m_uv(uv)
{
}

HM_Vertex::~HM_Vertex()
{
}

bool HM_Vertex::operator==(HM_Vertex const & otherVertex)
{

	if(this->m_position == otherVertex.m_position &&
	   this->m_uv == otherVertex.m_uv)
		return true;
	else
		return false;

}

bool HM_Vertex::operator!=(HM_Vertex const & otherVertex)
{

	return !(*this == otherVertex);

}

float HM_Vertex::operator[](unsigned char index)
{
	return m_position[index];
}

unsigned int HM_Vertex::getIndex() const
{
	return m_index;
}

glm::vec3 const & HM_Vertex::getPosition() const
{
	return m_position;
}

glm::vec2 const & HM_Vertex::getUV() const
{
	return m_uv;
}

void HM_Vertex::addTriangle(HM_Triangle* const triangle)
{

	m_triangles.push_back(triangle);

}

GLfloat* HM_Vertex::putVertexCoords(GLfloat* pointerToFill)
{

	*pointerToFill++ = m_position.x;
	*pointerToFill++ = m_position.y;
	*pointerToFill++ = m_position.z;
	 
	 return pointerToFill;

}

void HM_Vertex::putVertexCoords(std::vector<GLfloat>* vectorToFill)
{
	
	vectorToFill->push_back(m_position.x);
	vectorToFill->push_back(m_position.y);
	vectorToFill->push_back(m_position.z);

}

GLfloat* HM_Vertex::putVertexTexCoords(GLfloat* pointerToFill)
{

	*pointerToFill++ = m_uv.x;
	*pointerToFill++ = m_uv.y;

	return pointerToFill;

}

void HM_Vertex::putVertexTexCoords(std::vector<GLfloat>* vectorToFill)
{

	vectorToFill->push_back(m_uv.x);
	vectorToFill->push_back(m_uv.y);

}



HM_Triangle::HM_Triangle(HM_Vertex* v0, HM_Vertex* v1, HM_Vertex* v2) :
m_v0(v0), m_v1(v1), m_v2(v2)
{

	v0->addTriangle(this);
	v1->addTriangle(this);
	v2->addTriangle(this);

}

HM_Triangle::~HM_Triangle()
{

}

int* HM_Triangle::putTriangles(int* pointerToFill)
{
	
	*pointerToFill++ = m_v0->getIndex();
	*pointerToFill++ = m_v1->getIndex();
	*pointerToFill++ = m_v2->getIndex();

	return pointerToFill;

}

void HM_Triangle::putTriangles(std::vector<int>* vectorToFill)
{

	vectorToFill->push_back(m_v0->getIndex());
	vectorToFill->push_back(m_v1->getIndex());
	vectorToFill->push_back(m_v2->getIndex());
	
}



HM_Mesh::HM_Mesh(std::string meshFilePath,
				 unsigned char changeFrequencyCoords,
				 unsigned char changeFrequencyTexCoords) :
	m_meshFilePath(meshFilePath),
	m_changeFrequencyCoords(changeFrequencyCoords),
	m_changeFrequencyTexCoords(changeFrequencyTexCoords)
{
	
	std::string trueFilePath = "./Meshes/" + meshFilePath;

	loadOBJ(trueFilePath);

}

HM_Mesh::HM_Mesh()
{

}

HM_Mesh::~HM_Mesh()
{
}

size_t HM_Mesh::getNumberCoords() const
{

	return m_nbCoords;

}

size_t HM_Mesh::getNumberIndices() const
{

	return m_nbIndices;

}

size_t HM_Mesh::getNumberTexCoords() const
{

	return m_nbTexCoords;

}

size_t HM_Mesh::getByteSizeCoords() const
{

	return m_byteSizeCoords;

}

size_t HM_Mesh::getByteSizeIndices() const
{

	return m_byteSizeIndices;

}

size_t HM_Mesh::getByteSizeTexCoords() const
{

	return m_byteSizetexCoords;

}

unsigned char HM_Mesh::getChangeFrequencyCoords() const
{

	return m_changeFrequencyCoords;

}

unsigned char HM_Mesh::getChangeFrequencyTexCoords() const
{
	
	return m_changeFrequencyTexCoords;

}

size_t HM_Mesh::getVBOBytesOffsetCoords() const
{

	return m_vboBytesOffsetCoords;

}

size_t HM_Mesh::getVBOBytesOffsetTexCoords() const
{

	return m_vboBytesOffsetTexCoords;

}

size_t HM_Mesh::getVBOBytesOffsetIndices() const
{

	return m_vboBytesOffsetIndices;

}

HM_Cube const & HM_Mesh::getBoundingBox() const
{

	return m_boundingBox;

}

void HM_Mesh::setChangeFrequencyCoords(bool dynamic)
{

	if(dynamic)
		m_changeFrequencyCoords = HM_VBO_DYNAMIC;
	else
		m_changeFrequencyCoords = HM_VBO_STATIC;

}

void HM_Mesh::setChangeFrequencyTexCoords(bool dynamic /*= true*/)
{

	if (dynamic)
		m_changeFrequencyTexCoords = HM_VBO_DYNAMIC;
	else
		m_changeFrequencyTexCoords = HM_VBO_STATIC;

}

void HM_Mesh::setVBOBytesOffsetCoords(size_t vboOffsetCoords)
{

	m_vboBytesOffsetCoords = vboOffsetCoords;

}

void HM_Mesh::setVBOBytesOffsetTexCoords(size_t vboOffsetTexCoords)
{

	m_vboBytesOffsetTexCoords = vboOffsetTexCoords;

}

void HM_Mesh::setVBOBytesOffsetIndices(size_t vboOffsetIndices)
{

	m_vboBytesOffsetIndices = vboOffsetIndices;

}

void HM_Mesh::setOriginalVBOBytesOffsetCoords(size_t originalVBOOffsetCoords)
{

	m_originalVBOBytesOffsetCoords = originalVBOOffsetCoords;

}

void HM_Mesh::setOriginalVBOBytesOffsetTexCoords(size_t originalVBOOffsetTexCoords)
{

	m_originalVBOBytesOffsetTexCoords = originalVBOOffsetTexCoords;
	
}

HM_Vertex* const HM_Mesh::getVertexAtIndex(unsigned int index) const
{

	return m_vertices[index];

}

int HM_Mesh::getVertexIndex(HM_Vertex const & vertex) const
{

	for (unsigned int i = 0; i < m_vertices.size(); i++)
		if (*(m_vertices[i]) == vertex)
			return i;

	return -1;

}

GLfloat* HM_Mesh::putMeshCoords(GLfloat* pointerToFill)
{

	//pointerToFill = new GLfloat[m_nbCoords];
	GLfloat* temp_coords = pointerToFill;

	for (unsigned int i = 0; i < m_vertices.size(); i++)
		temp_coords = m_vertices[i]->putVertexCoords(temp_coords);

	return pointerToFill;

}

void HM_Mesh::putMeshCoords(std::vector<GLfloat>* vectorToFill)
{
	
	for (unsigned int i = 0; i < m_vertices.size(); i++)
		m_vertices[i]->putVertexCoords(vectorToFill);

}

GLfloat* HM_Mesh::putMeshTexCoords(GLfloat* pointerToFill)
{

	//pointerToFill = new GLfloat[m_nbTexCoords];
	GLfloat* temp_texCoords = pointerToFill;

	for (unsigned int i = 0; i < m_vertices.size(); i++)
		temp_texCoords = m_vertices[i]->putVertexTexCoords(temp_texCoords);

	return pointerToFill;

}

void HM_Mesh::putMeshTexCoords(std::vector<GLfloat>* vectorToFill)
{

	for (unsigned int i = 0; i < m_vertices.size(); i++)
		m_vertices[i]->putVertexTexCoords(vectorToFill);

}

int* HM_Mesh::putMeshIndices(int* pointerToFill)
{

	pointerToFill = new int[m_nbIndices];
	int* temp_indices = pointerToFill;

	for (unsigned int i = 0; i < m_triangles.size(); i++)
		temp_indices = m_triangles[i]->putTriangles(pointerToFill);

	return pointerToFill;

}

void HM_Mesh::putMeshIndices(std::vector<int>* vectorToFill)
{

	for (unsigned int i = 0; i < m_triangles.size(); i++)
		m_triangles[i]->putTriangles(vectorToFill);

}

void HM_Mesh::draw(HM_Texture* texture)
{
	
	//glBindVertexArray(m_vaoID);

		if(texture)
			glBindTexture(GL_TEXTURE_2D, texture->getID());

			glDrawElements(GL_TRIANGLES, (GLsizei)m_nbIndices, GL_UNSIGNED_INT,
						   BUFFER_OFFSET(m_vboBytesOffsetIndices));

		if(texture)
			glBindTexture(GL_TEXTURE_2D, 0);
	
	//glBindVertexArray(0);

}

void HM_Mesh::loadOBJ(std::string const & objFilePath)
{

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	HM_FileHandler objFile(objFilePath, HM_FILE_READ);

	if (!objFile) 
	{

		GAME_LOG->_ADDLINETOLOG_LINE("Error : couldn't find mesh file \'" + objFilePath + "\'", __LINE__ - 6);

		return;

	}

	glm::vec3 boundingBoxMax(0);
	glm::vec3 boundingBoxMin(0);

	std::list<std::string> objLines(objFile.getFileContentStringList());
	std::list<std::string>::const_iterator iter;

	for(iter = objLines.begin(); iter != objLines.end(); iter++)
	{

		std::string currentLine(*iter);
		int stringCursor = 0, stringOldCursor = 0;

		if (currentLine[0] == 'v')
		{

			if(currentLine[1] == ' ')
			{

				std::vector<float> vertex(hmu::getFloatsInString(currentLine));

				if (vertex.size() != 2 && vertex.size() != 3)
				{

					GRAPHICS_LOG("Error : OBJ vertex must have 2 or 3 coordinates. In file : " +
								 objFilePath);

					return;

				}

				if(vertex.size() == 3)
					temp_vertices.push_back(glm::vec3(vertex[0], vertex[1],
													  vertex[2]));
				else
					temp_vertices.push_back(glm::vec3(vertex[0], vertex[1], 
													  0.0f));

				if (vertex[0] < boundingBoxMin.x)
					boundingBoxMin.x = vertex[0];
				if (vertex[1] < boundingBoxMin.y)
					boundingBoxMin.y = vertex[1];
				if (vertex[2] < boundingBoxMin.z)
					boundingBoxMin.z = vertex[2];

				if (vertex[0] > boundingBoxMax.x)
					boundingBoxMax.x = vertex[0];
				if (vertex[1] > boundingBoxMax.y)
					boundingBoxMax.y = vertex[1];
				if (vertex[2] > boundingBoxMax.z)
					boundingBoxMax.z = vertex[2];

			}
			else if (currentLine[1] == 't')
			{

				
				std::vector<float> uv(hmu::getFloatsInString(currentLine));

				if (uv.size() != 2)
				{

					GRAPHICS_LOG("Error : OBJ texture coordinates must have 2 coordinates. In file : " +
								 objFilePath);

					return;

				}

				temp_uvs.push_back(glm::vec2(uv[0], uv[1]));

			}
			else if (currentLine[1] == 'n')
			{

				std::vector<float> normal(hmu::getFloatsInString(currentLine));

				if (normal.size() != 3)
				{

					GRAPHICS_LOG("Error : OBJ normals must have 3 coordinates. In file : " +
								 objFilePath);

					return;

				}

				temp_normals.push_back(glm::vec3(normal[0], normal[1], normal[2]));

			}

		}
		else if (currentLine[0] == 'f')
		{

			std::vector<int> indices(hmu::getIntsInString(currentLine));

			if (indices.size() % 3 != 0 || indices.size() > 9)
			{

				GRAPHICS_LOG("Error : OBJ indices must represent triangles. In File : " + objFilePath);

			}

			unsigned char nbIndices;
			nbIndices = static_cast<unsigned char>(indices.size()) / 3;

			for (int i = 0; i < 3; i++)
			{

				vertexIndices.push_back(indices[i * nbIndices] - 1);

				if (nbIndices == 3 || (nbIndices == 2 && temp_uvs.size() != 0))
					uvIndices.push_back(indices[i * nbIndices + 1] - 1);
				else if (nbIndices == 3 || (nbIndices == 2 && temp_normals.size() != 0))
					normalIndices.push_back(indices[i * nbIndices + (nbIndices - 1)] - 1);

			}

		}

	}

	int index = 0;
	int indexCounter = 0;

	HM_Vertex* triangleIndices[3] = {NULL, NULL, NULL};

	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{

		glm::vec3 currentVertex(temp_vertices[vertexIndices[i]]);

		glm::vec2 currentUV;

		if (temp_uvs.size() != 0)
			currentUV = temp_uvs[uvIndices[i]];
		else
			currentUV = glm::vec2(0);

		if ((index = getVertexIndex(HM_Vertex(-1, currentVertex, currentUV))) == -1)
		{

			index = indexCounter++;

			m_vertices.push_back(new HM_Vertex(index, currentVertex, currentUV));

		}

		triangleIndices[i % 3] = m_vertices[index];

		if(i % 3 == 2)
			m_triangles.push_back(new HM_Triangle(triangleIndices[0],
												  triangleIndices[1],
												  triangleIndices[2]));

	}

	m_boundingBox = HM_Cube(boundingBoxMin, boundingBoxMax);

	m_nbCoords = m_vertices.size() * 3;
	m_nbIndices = m_triangles.size() * 3;
	m_nbTexCoords = m_vertices.size() * 2;

	m_byteSizeCoords = m_nbCoords * sizeof(float);
	m_byteSizeIndices =  m_nbIndices * sizeof(unsigned int);
	m_byteSizetexCoords =  m_nbTexCoords * sizeof(float);
	
}