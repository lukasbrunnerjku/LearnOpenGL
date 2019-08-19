#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "shader.h"
#include "opengldebug.h"
#include "mesh.h"
#include "model.h"


/* Followed the tutorial on: https://learnopengl.com and docs.gl offers a detailed documentation! 
Always very helpful to hear things twice with a little twist: https://www.youtube.com/user/TheChernoProject 
Get the assimp libary using Cmake: https://www.youtube.com/watch?v=W_Ey_YPUjMk */

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float currentFrame; // Time of the current frame

/* Initialize mouse position */
float lastX = 400, lastY = 300;
bool firstMouse = true;

/* Create the camera object(using default parameters) which will later manipulate our view matrix accordingly */
Camera camera;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top
	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}

void processInput(GLFWwindow* window)
{
	/* To close the window by pressing ESC key: */
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	/* To move the camera around we take the keyboard inputs into account when calculating the
	new view matrix */
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::Camera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::Camera_Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::Camera_Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)	
		camera.ProcessKeyboard(Camera::Camera_Movement::RIGHT, deltaTime);	
}

int main()
{
	glfwInit();
	/* Specify what version and profile of OpenGL(Core, Compatibility) to use */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int width = 800, height = 600;

	GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	/* To load all OpenGL function pointers */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/* To tell OpenGL that our (-1, 1) space in x should be (0, 800) and (0, 600) in y */
	glViewport(0, 0, 800, 600);
	/* Set the callback function that should be called if the window needs to be resized! */
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	/* Note: We can make a callback function to process joystick input changes, process error messages etc. too */
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	/* Set callback for mouse and scroll wheel to move camera accordingly */
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	/* Load the model from a file using assimp */
	Model actuator_model("C:/Users/Lukas/source/repos/LearnOpenGL/Dependencies/actuator/actuator.obj");	

	/* Create a shader program by compiling and linking shader source files */
	Shader shaderProgram("C:/Users/Lukas/source/repos/LearnOpenGL/LearnOpenGL/shader.vs", 
		"C:/Users/Lukas/source/repos/LearnOpenGL/LearnOpenGL/shader.fs");
	/* All the glUniform and glGetUniformLocation calls need a program in use */
	shaderProgram.use();

	/* A model matrix that places our model where it should be in the world space */
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
	/* Send the model matrix to the shader (need a program in use) */
	shaderProgram.setMatrix("model", model);

	/* A view matrix, each coordinate is as seen from the camera or viewer's point of view */
	glm::mat4 view;
	/* A projection matrix to get the right 3D perspective using the w values of the 4D vectors */
	glm::mat4 projection;

	/* Set a uniform color rgb style */
	shaderProgram.setVector("ourColor", 1.0f, 1.0f, 1.0f);

	/* Needed for 3D objects to get real 3D look, checks which fragments are behind others and 
	set GL_DEPTH_BUFFER_BIT in glClear() to clear the depth buffer every frame */
	glEnable(GL_DEPTH_TEST);

	/* Set the camera position to see the whole actuator */
	camera.Position = glm::vec3(0.0f, 6.0f, 15.0f);
	
	/* Render loop: */
	while (!glfwWindowShouldClose(window))
	{
		/* For the camera speed calculation independent on hardware setup speed */
		currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Use the shader */
		shaderProgram.use();

		/* To be able to change the view with the w-, a-, s-, d-key and by moving the mouse!
		Here the camera class is used which takes in the key strokes and mouse positions in the 
		coresponding callback functions and then processing that information to create a new view matrix */
		view = camera.GetViewMatrix();
		shaderProgram.setMatrix("view", view);

		/* To be able to change the field of view(zoom) in our scene with the mouse wheel!
		In the scroll callback function we let the camera class manipulate the zoom value accordingly */
		projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
		shaderProgram.setMatrix("projection", projection);

		/* Draw the loaded model using the given shader program */
		actuator_model.Draw(shaderProgram);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/* Clear all OpenGL resources: */
	glDeleteProgram(shaderProgram.id);
	/* Clear all the glfw-resources: */
	glfwTerminate();
	return 0;
}