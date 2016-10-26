#pragma once

// Include Windows
#ifdef WIN32
#include <GL/glew.h>

// Include Mac
#elif __APPLE__
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>

// Include UNIX/Linux
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif

// Includes GLM

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Common includes
#include <iostream>
#include <string>
#include <fstream>
#include <map>


class HM_Shader
{
	public :

		HM_Shader();
		HM_Shader(std::string sourceVertex, std::string sourceFragment);
		HM_Shader(HM_Shader const &shaderToCopy);
		~HM_Shader();

		HM_Shader& operator=(HM_Shader const &shaderToCopy);

		bool load();
		bool compileShader(GLuint &shader, GLenum type, std::string const &sourceFile);
		void sendMat4(std::string name, glm::mat4 matrix);

		GLuint getProgramID() const;

	private :

		static std::map<std::string, GLuint> vertexShaderLoaded;
		static std::map<std::string, GLuint> fragmentShaderLoaded;

		int m_matrixLocation;

		GLuint m_vertexID;
		GLuint m_fragmentID;
		GLuint m_programID;

		std::string m_sourceVertex;
		std::string m_sourceFragment;
};
