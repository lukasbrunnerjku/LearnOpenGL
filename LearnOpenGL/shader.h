#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* Must include glad to get all the required OpenGL function pointers */
#include <glad/glad.h> 

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "opengldebug.h"


class Shader
{
public:
	// the program id
	unsigned int id;

	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);

	// use/activate the shader
	void use();

	// utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVector(const std::string& name, glm::vec3 vector) const;
	void setVector(const std::string& name, float x, float y, float z) const;
	void setMatrix(const std::string& name, glm::mat4 matrix) const;
};