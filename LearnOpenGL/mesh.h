#pragma once

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm> 
#include <vector>

#include "shader.h"
#include "opengldebug.h"

struct Vertex {
	glm::vec3 Position;
};

class Mesh {
public:
	/*  Mesh Data  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO;

	/*  Functions  */
	// constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	
	// render the mesh
	void Draw(Shader shader);

	/* Fill the VBO with changed vertex data */
	void changeMesh(std::vector<glm::vec3> new_positions);

	/* Return a list of the positions for the vertices */
	std::vector<glm::vec3> getPositions();

	/* Print all positions of all vertices to console */
	void printPositions();

	/* Return a list of lists of positions within a layer of equal height vertices */
	std::vector<std::vector<glm::vec3>> getLayerPositions();

	/* Print all vertices for each layer of the mesh */
	void printLayerPositions();

private:
	/*  Render data  */
	unsigned int VBO, EBO;

	/*  Functions    */
	// initializes all the buffer objects/arrays
	void setupMesh();
};