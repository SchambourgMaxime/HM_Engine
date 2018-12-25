#include "HM_Shader.h"

std::map<std::string, GLuint> HM_Shader::vertexShaderLoaded;
std::map<std::string, GLuint> HM_Shader::fragmentShaderLoaded;

HM_Shader::HM_Shader() : m_vertexID(0), m_fragmentID(0), m_programID(0),
m_sourceVertex(), m_sourceFragment()
{

}

HM_Shader::HM_Shader(std::string sourceVertex, std::string sourceFragment) : 
m_vertexID(0), m_fragmentID(0), m_programID(0),
m_sourceVertex(sourceVertex), m_sourceFragment(sourceFragment)
{
	
}

HM_Shader::HM_Shader(HM_Shader const &shaderToCopy)
{
	m_sourceVertex = shaderToCopy.m_sourceVertex;
	m_sourceFragment = shaderToCopy.m_sourceFragment;

	load();
}

HM_Shader::~HM_Shader()
{

	glDeleteShader(m_vertexID);
	glDeleteShader(m_fragmentID);
	glDeleteProgram(m_programID);

}

HM_Shader& HM_Shader::operator=(HM_Shader const &shaderToCopy)
{

	m_sourceVertex = shaderToCopy.m_sourceVertex;
	m_sourceFragment = shaderToCopy.m_sourceFragment;

	load();

	return *this;

}

bool HM_Shader::load()
{

	if(glIsShader(m_vertexID))
		glDeleteShader(m_vertexID);

	if (glIsShader(m_fragmentID))
		glDeleteShader(m_fragmentID);

	if(glIsProgram(m_programID))
		glDeleteProgram(m_programID);

	if (!compileShader(m_vertexID, GL_VERTEX_SHADER, m_sourceVertex))
		return false;

	if (!compileShader(m_fragmentID, GL_FRAGMENT_SHADER, m_sourceFragment))
		return false;

	m_programID = glCreateProgram();

	glAttachShader(m_programID, m_vertexID);
	glAttachShader(m_programID, m_fragmentID);

	glBindAttribLocation(m_programID, 0, "in_Vertex");
	glBindAttribLocation(m_programID, 1, "in_Color");
	glBindAttribLocation(m_programID, 2, "in_TexCoord0");

	glLinkProgram(m_programID);

	GLint linkingError(0);
	glGetProgramiv(m_programID, GL_LINK_STATUS, &linkingError);

	if (linkingError != GL_TRUE)
	{
		GLint errorSize(0);
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &errorSize);

		char* error = new char[errorSize + 1];

		glGetProgramInfoLog(m_programID, errorSize, &errorSize, error);
		error[errorSize] = '\0';

		std::cout << "Error while linking program :" << std::endl
				  << error << std::endl;

		delete[] error;
		glDeleteProgram(m_programID);

		return false;
	}

	else
	{

		m_matrixLocation = glGetUniformLocation(m_programID, "modelviewProjection");

		return true;
	}

}

bool HM_Shader::compileShader(GLuint &shader, GLenum type,
							  std::string const &sourceFile)
{

	std::map<std::string, GLuint> mapOfLoadedShader;
	if (type == GL_VERTEX_SHADER)
		mapOfLoadedShader = vertexShaderLoaded;
	else if (type == GL_FRAGMENT_SHADER)
		mapOfLoadedShader = fragmentShaderLoaded;


	std::map<std::string, GLuint>::const_iterator iter;
	iter = mapOfLoadedShader.find(sourceFile);

	if (iter != mapOfLoadedShader.end())
	{

		shader = (*iter).second;
		return true;

	}
	else
	{

		shader = glCreateShader(type);

		if (shader == 0)
		{

			std::cout << "Error, the shader type (" << type << ") does not exist" << std::endl;
			return false;

		}

		std::ifstream file(sourceFile.c_str());

		if (!file)
		{

			std::cout << "Error, the file \"" << sourceFile << "\" could not be found" << std::endl;
			glDeleteShader(shader);

			return false;

		}

		// Strings to read the source code
		std::string line;
		std::string sourceCode;

		while(getline(file, line))
			sourceCode += line + '\n';

		file.close();

		const GLchar* sourceCodeArray = sourceCode.c_str();

		glShaderSource(shader, 1, &sourceCodeArray, 0);

		glCompileShader(shader);

		GLint compilingError(0);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &compilingError);

		if (compilingError != GL_TRUE)
		{

			GLint errorSize(0);

			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorSize);

			char* error = new char[errorSize + 1];

			glGetShaderInfoLog(shader, errorSize, &errorSize, error);
			error[errorSize] = '\0';

			std::cout << "Error while compiling shader \"" << sourceFile << "\" :" << std::endl
					  << error << std::endl;

			delete[] error;
			glDeleteShader(shader);

			return false;

		}
		else
		{

			if(type == GL_VERTEX_SHADER)
				vertexShaderLoaded[sourceFile] = shader;
			else if(type == GL_FRAGMENT_SHADER)
				fragmentShaderLoaded[sourceFile] = shader;

			return true;

		}

	}

}

void HM_Shader::sendMat4(std::string name, glm::mat4 matrix)
{

	glUniformMatrix4fv(m_matrixLocation, 1, GL_FALSE, value_ptr(matrix));

}

GLuint HM_Shader::getProgramID() const
{
	return m_programID;
}

