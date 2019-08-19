#include "shader.h"


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// 1. retrieve the vertex and fragment source code from the filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. compile shaders
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::cout << "Vertex source code used:\n" << vShaderCode << std::endl;
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::cout << "Fragment source code used:\n" << fShaderCode << std::endl;
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use()
{
	GLCall(glUseProgram(id));
}

void Shader::setBool(const std::string& name, bool value) const
{
	GLCall(glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value));
}

void Shader::setInt(const std::string& name, int value) const
{
	GLCall(glUniform1i(glGetUniformLocation(id, name.c_str()), value));
}

void Shader::setFloat(const std::string& name, float value) const
{
	GLCall(glUniform1f(glGetUniformLocation(id, name.c_str()), value));
}

void Shader::setVector(const std::string& name, glm::vec3 vector) const
{
	GLCall(glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(vector)));
}

void Shader::setVector(const std::string& name, float x, float y, float z) const
{
	GLCall(glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z));
}

void Shader::setMatrix(const std::string& name, glm::mat4 matrix) const
{
	GLCall(glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix)));
}