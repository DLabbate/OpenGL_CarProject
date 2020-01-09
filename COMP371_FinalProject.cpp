// OpenGL_Practice.cpp : This file contains the 'main' function. Program execution begins and ends there.
// COMP 371 - Assignment 2
// Name: Domenic Labbate
// ID: 40063296
// *Notes: Please take a look at README text which lists all controls!

#include "pch.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Time.h"
#include "Camera.h"
#include "CarModel.h"
#include "Grid.h"
#include "Axes.h"
#include <FreeImageIO.h>
#include "Light.h"
#include "Quad.h"
#include "Spawner.h"
#include "ColliderBox.h"
#include "Collider.h"
#include "Border.h"

//Framebuffer callback not needed for this assignment
void framebuffer_size_callback(GLFWwindow* w, int width, int height);

void processInput(GLFWwindow* w);
void updateWorldOrientation(GLFWwindow* w, float&, float&, float, Camera&);

enum ground_texture_mode { grid, grass };
ground_texture_mode ground = grid;

enum car_shader_mode { regular, lighting };
car_shader_mode car_shader = lighting;

GLint polygonmode; // Global Variables
int keystate_N = GLFW_RELEASE;
int keystate_PgUp = GLFW_RELEASE;
int keystate_Enter = GLFW_RELEASE;
int keystate_Backspace = GLFW_RELEASE;
int keystate_Del = GLFW_RELEASE;
bool displayHeadlights = 0;
bool move_random = false;
bool bumpercar_animation = true;
int border_texture;
int border_texture_ID;

float WINDOW_WIDTH = 1024;
float WINDOW_HEIGHT = 768;


const char* getVertexShaderSource() //This string is found in the vertexshader GLSL file
{
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"
		"layout (location = 1) in vec3 aColor;"
		"uniform mat4 worldMatrix;"
		"uniform mat4 viewMatrix = mat4(1.0);"
		"uniform mat4 projectionMatrix = mat4(1.0);"
		"uniform mat4 worldOrientation = mat4(1.0);"
		"out vec3 vertexColor;"
		"void main()"
		"{"
		"   vertexColor = aColor;"
		"	mat4 modelViewProjection = projectionMatrix *viewMatrix * worldOrientation * worldMatrix;"
		"   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
		"}";
}

const char* getFragmentShaderSource() //This string is found in the fragmentshader GLSL file
{
	return
		"#version 330 core\n"
		"in vec3 vertexColor;"
		"out vec4 FragColor;"
		"void main()"
		"{"
		"   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
		"}";
}

int compileAndLinkShaders()
{
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//const char* vertexShaderSource = getVertexShaderSource();
	//char* vertexShaderSource;
	std::ifstream file1;
	file1.open("vertexshader.glsl");
	std::string str, line;
	while (std::getline(file1, line))
	{
		std::cout << line << std::endl;
		str += line + "\n";
	}
	const char* vertexShaderSource = str.c_str(); //Converts string to char*

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//const char* fragmentShaderSource = getFragmentShaderSource();
	std::ifstream file2;
	file2.open("fragmentshader.glsl");
	std::string str2, line2;
	while (std::getline(file2, line2))
	{
		std::cout << line2 << std::endl;
		str2 += line2 + "\n";
	}
	const char* fragmentShaderSource = str2.c_str(); //Converts string to char*

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int compileAndLinkShaders(const char* vertexshader_name, const char* fragmentshader_name)
{
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//const char* vertexShaderSource = getVertexShaderSource();
	//char* vertexShaderSource;
	std::ifstream file1;
	file1.open(vertexshader_name);
	std::string str, line;
	while (std::getline(file1, line))
	{
		std::cout << line << std::endl;
		str += line + "\n";
	}
	const char* vertexShaderSource = str.c_str(); //Converts string to char*

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//const char* fragmentShaderSource = getFragmentShaderSource();
	std::ifstream file2;
	file2.open(fragmentshader_name);
	std::string str2, line2;
	while (std::getline(file2, line2))
	{
		std::cout << line2 << std::endl;
		str2 += line2 + "\n";
	}
	const char* fragmentShaderSource = str2.c_str(); //Converts string to char*

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

GLuint getVAO() // Returns a Unit Cube VAO
{
	glm::vec3 vertices[] = {
		glm::vec3(-0.5f,-0.5f,0.5f),	//Position1
		glm::vec3(0.0f,0.0f,1.0f),		//Colour1

		glm::vec3(0.5f,-0.5f,0.5f),		//Position2
		glm::vec3(0.0f,0.0f,1.0f),		//Colour2...

		glm::vec3(0.5f,0.5f,0.5f),
		glm::vec3(0.0f,0.0f,1.0f),

		glm::vec3(-0.5f,0.5f,0.5f),
		glm::vec3(0.0f,0.0f,1.0f),

		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(0.3f,0.5f,1.0f),

		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.3f,0.5f,1.0f),

		glm::vec3(0.5f,0.5f,-0.5f),
		glm::vec3(0.3f,0.5f,1.0f),

		glm::vec3(-0.5f,0.5f,-0.5f),
		glm::vec3(0.3f,0.5f,1.0f),
	};


	GLuint index[] = {
		0,1,2,0,2,3, //Face 1

		4,5,6,4,6,7, //Back

		4,0,3,4,3,7, //Left

		1,5,2,5,6,2, //Right

		3,2,6,3,6,7, //Top

		0,1,5,0,5,4

	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);


	return VAO;
}

GLuint getSquareGrid() //Returns a VAO of a grid consisting of GL_POINTS --> (Not used for this assignment)
{
	glm::vec3 vertices[(100 + 1) * (100 + 1)]; //A 100 by 100 grid consists of 101 * 101 vertices
	vertices[0] = glm::vec3(-100 / 2, 0, -100 / 2);
	//std::cout << (int)vertices[0].x;
	unsigned int index = 1;

	for (unsigned int i = 0; i <= (100); i++)
	{
		for (unsigned int w = 0; w < (100); w++)
		{
			//std::cout << (int)vertices[index-1].z<<" ";
			vertices[index++] = glm::vec3(vertices[index - 1].x + 1, 0.0f, vertices[index - 1].z);
		}

		if (index < sizeof(vertices) / sizeof(glm::vec3))
		{
			//std::cout << std::endl;
			vertices[index++] = glm::vec3(vertices[index - (100 + 1)].x, 0.0f, vertices[index - (100 + 1)].z + 1);
		}
	}

	glm::vec3 colours[(100 + 1) * (100 + 1)]; //A 100 by 100 grid consists of 101 * 101 vertices
	for (unsigned int i = 0; i < sizeof(vertices) / sizeof(glm::vec3); i++)
	{
		colours[i] = glm::vec3(1.0f, 0.0f, 0.0f);
	}

	//unsigned int indices[(100)*(101) + (100)*(101)];

	for (unsigned int i = 0; i < (100 + 1)*(100 + 1); i++)
	{
		colours[i] = glm::vec3(1.0f, 1.0f, 1.0f);
	}


	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	GLuint colourVBO;
	glGenBuffers(1, &colourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(1);

	/*unsigned int x = 0;
	for (unsigned int i = 0; i < (10); i++)
	{
		for (unsigned int w = 0; w < (10); w++)
		{
			std::cout << (double) vertices[x++].x << "," << (double) vertices[x++].z<<"	";
		}
		std::cout << std::endl;
	}*/

	return VAO;
}

GLuint getAxes() //Returns VAO of Coordinate Axes
{
	glm::vec3 vertices[] = {
		//Axis 1
		glm::vec3(0.0f,0.0f,0.0f),		//Position 0
		glm::vec3(1.0f,0.0f,0.0f),

		glm::vec3(5.0f, 0.0f, 0.0f),	//Position 1
		glm::vec3(1.0f,0.0f,0.0f),

		//Axis 2
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(0.0f,1.0f,0.0f),

		glm::vec3(0.0f, 5.0f, 0.0f),
		glm::vec3(0.0f,1.0f,0.0f),

		//Axis 3
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(0.1f,0.5f,1.0f),

		glm::vec3(0.0f, 0.0f, 5.0f),
		glm::vec3(0.1f,0.5f,1.0f)
	};

	unsigned int indices[] = {
		0,1,
		0,2,
		0,3,
	};

	GLuint AxesVAO;
	glGenVertexArrays(1, &AxesVAO);
	glBindVertexArray(AxesVAO);

	GLuint AxesVBO;
	glGenBuffers(1, &AxesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, AxesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	GLuint AxesEBO;
	glGenBuffers(1, &AxesEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AxesEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	return AxesVAO;


	return 0;
}

void change_draw_mode(GLFWwindow* w)	//Changes between GL_FILL,GL_POINT,and GL_LINE
{
	if (glfwGetKey(w, GLFW_KEY_T) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(w, GLFW_KEY_P) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}

	if (glfwGetKey(w, GLFW_KEY_L) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

GLuint getCircle()
{

	float pi = glm::pi<float>();
	glm::vec3 vertices[4 * 3 * (72 + 1)];

	glm::vec3 normals[4 * 3 * (72 + 1)];
	glm::vec2 uv[4 * 3 * (72 + 1)];

	int index = 0;

	//for (float x = -0.5f; x <= 0.5f; x += 0.5f)
	{
		for (float theta = 0.0f; theta < 2 * pi; theta = theta + pi / 36)
		{
			std::cout << index << std::endl;
			vertices[index] = glm::vec3(0.5f, 0.0f, 0.0f);
			vertices[++index] = glm::vec3(0.5f, 0.5*sin(theta), 0.5*cos(theta));
			vertices[++index] = glm::vec3(0.5f, 0.5*sin(theta + pi / 36), 0.5*cos(theta + pi / 36));
			index++;
		}
	}

	{
		for (float theta = 0.0f; theta < 2 * pi; theta = theta + pi / 36)
		{
			std::cout << index << std::endl;
			vertices[index] = glm::vec3(-0.5f, 0.0f, 0.0f);
			vertices[++index] = glm::vec3(-0.5f, 0.5*sin(theta), 0.5*cos(theta));
			vertices[++index] = glm::vec3(-0.5f, 0.5*sin(theta + pi / 36), 0.5*cos(theta + pi / 36));
			index++;
		}
	}

	for (float theta = 0.0f; theta < 2 * pi; theta = theta + pi / 36) //Executes 73 times
	{
		//std::cout << index << std::endl;
		vertices[index] = glm::vec3(-0.5f, 0.5*sin(theta), 0.5*cos(theta));
		vertices[++index] = glm::vec3(-0.5f, 0.5*sin(theta + pi / 36), 0.5*cos(theta + pi / 36));
		vertices[++index] = glm::vec3(-0.5f + 1, 0.5*sin(theta + pi / 36), 0.5*cos(theta + pi / 36));
		index++;
	}

	for (float theta = 0.0f; theta < 2 * pi; theta = theta + pi / 36) //Executes 73 times
	{
		//std::cout << index << std::endl;
		vertices[index] = glm::vec3(0.5f, 0.5*sin(theta), 0.5*cos(theta));
		vertices[++index] = glm::vec3(0.5f, 0.5*sin(theta + pi / 36), 0.5*cos(theta + pi / 36));
		vertices[++index] = glm::vec3(-0.5f, 0.5*sin(theta), 0.5*cos(theta));
		index++;
	}

	for (int i = 0; i < sizeof(vertices) / sizeof(glm::vec3); i++)
	{
		normals[i] = vertices[i]; //Since it is centered at (0,0,0)
		//uv[i] = glm::vec2(vertices[i].z, vertices[i].y);
		uv[i] = glm::vec2(atan(vertices[i].y / vertices[i].z), vertices[i].x);
	}

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	GLuint VBO_uv;
	glGenBuffers(1, &VBO_uv);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(2);

	GLuint VBO_normals;
	glGenBuffers(1, &VBO_normals);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(3);

	return VAO;

}

int loadTexture(const char* imagepath)
{
	// Load image using the Free Image library
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(imagepath, 0);
	FIBITMAP* image = FreeImage_Load(format, imagepath);
	FIBITMAP* image32bits = FreeImage_ConvertTo32Bits(image);

	// Get an available texture index from OpenGL
	GLuint texture = 0;
	glGenTextures(1, &texture);
	assert(texture != 0);

	// Set OpenGL filtering properties (bi-linear interpolation)
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Retrieve width and hight
	int width = FreeImage_GetWidth(image32bits);
	int height = FreeImage_GetHeight(image32bits);

	// This will upload the texture to the GPU memory
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32bits));

	// Free images
	FreeImage_Unload(image);
	FreeImage_Unload(image32bits);

	return texture;
}

GLuint generateUBO()
{
	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4) + sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	return UBO;
}

void setUniformMatrix4(int shaderprogram, const char* name, glm::mat4 newmatrix)
{
	GLuint location = glGetUniformLocation(shaderprogram, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, &newmatrix[0][0]);
}

void setUniformVec3(int shaderprogram, const char* name, glm::vec3 newvector)
{
	GLuint location = glGetUniformLocation(shaderprogram, name);
	glUniform3f(location, newvector.x, newvector.y, newvector.z);
}


int main()
{

	if (!glfwInit())
	{
		std::cout << "Could not initialize glfw";
		glfwTerminate();
		return -1;
	}
	//We could do the following if we want to ensure GLFW Context 3.3 and greater
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#if defined(PLATFORM_OSX)	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// On windows, we set OpenGL version to 2.1, to support more hardware
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

	GLFWwindow* window = glfwCreateWindow(1024, 768, "OpenGL_Practice", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to Open Window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	//glViewport(0, 0, 1024, 768);
	//glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
	glClearColor(160.0f/255, 216.0f/255, 243.0f/255, 0.0f);
	//glClearColor(112.0f / 255, 217.0f / 255, 249.0f / 255, 0.0f);

	//Enable Depth Test
	glEnable(GL_DEPTH_TEST);

	//int shaderprogram = compileAndLinkShaders();
	//SHADER PROGRAMS (Car, Light, etc.)
	int shaderprogram = compileAndLinkShaders("vertexshader.glsl", "fragmentshader.glsl");
	int shaderprogram_textured = compileAndLinkShaders("texturedvertexshader.glsl", "texturedfragmentshader.glsl");
	int shaderprogram_light = compileAndLinkShaders("vertexshader_light.glsl", "fragmentshader_light.glsl");
	int shaderprogram_car = compileAndLinkShaders("vertexshader_car.glsl", "fragmentshader_car.glsl");
	int shaderprogram_simpledepth = compileAndLinkShaders("simpledepth_vs.glsl", "simpledepth_fs.glsl");
	int shaderprogram_particle = compileAndLinkShaders("particle_vs.glsl", "particle_fs.glsl");


	glUseProgram(shaderprogram_simpledepth);
	glUseProgram(shaderprogram_textured);
	glUniform1i(glGetUniformLocation(shaderprogram_textured, "texturesampler"), 0); //Texture Unit 0
	glUniform1i(glGetUniformLocation(shaderprogram_textured, "shadowMap"), 1); //Texture Unit 1

	unsigned int shaderprogram_index = glGetUniformBlockIndex(shaderprogram, "Matrices");
	unsigned int shaderprogramtextured_index = glGetUniformBlockIndex(shaderprogram_textured, "Matrices");
	unsigned int shaderprogramlight_index = glGetUniformBlockIndex(shaderprogram_light, "Matrices");
	unsigned int shaderprogramcar_index = glGetUniformBlockIndex(shaderprogram_car, "Matrices");
	unsigned int shaderprogramparticle_index = glGetUniformBlockIndex(shaderprogram_particle, "Matrices");
	glUniformBlockBinding(shaderprogram, shaderprogram_index, 0);
	glUniformBlockBinding(shaderprogram_textured, shaderprogramtextured_index, 0);
	glUniformBlockBinding(shaderprogram_light, shaderprogramlight_index, 0);
	glUniformBlockBinding(shaderprogram_car, shaderprogramcar_index, 0);
	glUniformBlockBinding(shaderprogram_particle, shaderprogramparticle_index, 0);

	GLuint ubo = generateUBO();
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, 3 * sizeof(glm::mat4) + sizeof(glm::vec3)); //3 Matrices, 1 vec3

	unsigned int grass_texure = loadTexture("Textures/Grass.jpg");
	unsigned int rubber_texure = loadTexture("Textures/Rubber3.jpg");
	unsigned int smoke_texure = loadTexture("Textures/Smoke.png");
	unsigned int wood_texture = loadTexture("Textures/Wood3.jpg");
	unsigned int wood_texture2 = loadTexture("Textures/Wood2.jpg");


	GLuint gridVAO = getSquareGrid();	//Grid VAO
	GLuint axesVAO = getAxes();			//Axes VAO
	GLuint vao = getVAO();				//Unit cube VAO
	glBindVertexArray(vao);
	glLineWidth(6.0f);
	glPointSize(1.5f);

	Time t;
	float deltatime;
	Camera camera(shaderprogram, glm::vec3(5.0f, 8.0f, 30.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), window);

	CarModel car(-2.0f, 0.5f, -2.0f);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Grid g;
	g.generateGridHorizontal(); //Generate VAO for horizontal lines
	g.generateGridVertical();	//Generate VAO for vertical lines
	g.generateGroundGrid(100, grass_texure);

	float world_orientation_angle_x = 0.0f;
	float world_orientation_angle_y = 0.0f;

	Axes axes;

	Light light;

	GLuint circletest = getCircle();



	//depthMap and FBO generation is used from https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
	//*************************************************************************************************************************
	const unsigned int SHADOW_WIDTH = 1024 * 2 * 2 * 2, SHADOW_HEIGHT = 1024 * 2 * 2 * 2;
	unsigned int depthMap, depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);

	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float bordercolor[] = { 1.0f,1.0f,1.0f,1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	const float VIEW_WIDTH = 1024.0f, VIEW_HEIGHT = 768.0f;
	float near_plane = 1.0f, far_plane = 75.5f;
	//glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane);


	//*************************************************************************************************************************

	glm::mat4 projection_matrix = glm::perspective(70.0f, WINDOW_WIDTH / WINDOW_HEIGHT, 0.01f, 200.0f);

	Light headlight1;
	Light headlight2;
	Light taillight1;
	taillight1.setDiffuse(glm::vec3(1.0f, 0.0f, 0.0f)); //Red
	Light taillight2;
	taillight2.setDiffuse(glm::vec3(1.0f, 0.0f, 0.0f)); //Red

	Quad quad;

	Spawner spawner;

	ColliderBox coll;
	Collider collider;
	CarModel car2(-20.0f, 0.5f, -20.0f);

	/*std::vector<CarModel> cars = 
	{ CarModel(-20.0f,0.5f,-20.0f),CarModel(-30.0f, 0.5f, -30.0f),CarModel(20.0f, 0.5f, 30.0f), CarModel(5.0f,0.5f,15.0f),
		CarModel(30.0f,0.5f,3.0f), CarModel(-4.0f, 0.5f, -7.0f), CarModel(-21.0f, 0.5f, -14.0f), CarModel(-30.0f, 0.5f, 20.0f)
	};*/

	std::vector<CarModel> cars =
	{ CarModel(-20.0f,0.5f,-20.0f),CarModel(-30.0f, 0.5f, -30.0f),CarModel(20.0f, 0.5f, 30.0f), CarModel(5.0f,0.5f,15.0f),
		CarModel(30.0f,0.5f,3.0f), CarModel(-4.0f, 0.5f, -7.0f), CarModel(-20.0f,0.5f,-20.0f)
	};

	//std::vector<Light> headlights = {};
	//std::vector<Light> taillights = {};
	std::vector<glm::vec3> headlight_positions = {};
	std::vector<glm::vec3> taillight_positions = {};
	for (int i = 0; i < 12; i++) //Allocate space for headlights and taillights (2 headlights and 2 taillights per car)
	{
		headlight_positions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		taillight_positions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	}
	
	Border border;

	int border_texture = wood_texture;

	while (!glfwWindowShouldClose(window))
	{
		//glm::mat4 lightProjection = glm::perspective(glm::radians(45.0f), (float)(SHADOW_WIDTH / SHADOW_HEIGHT), 70.0f, 140.0f);
		glm::mat4 lightProjection = glm::perspective(glm::radians(70.0f), (float)(SHADOW_WIDTH / SHADOW_HEIGHT), 50.0f, 140.0f);
		//glm::mat4 lightView = glm::lookAt(glm::vec3(-10.0f,light.getPosition().y+80.0f,-10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightView = glm::lookAt(glm::vec3(light.getPosition()), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::mat4 lightView = glm::lookAt(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		glLineWidth(6.0f);
		glPointSize(7.5f);
		change_draw_mode(window);
		deltatime = t.getDeltaTime();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderprogram);
		processInput(window);

		//Projection

		/*GLuint projection_matrix_location = glGetUniformLocation(shaderprogram, "projectionMatrix");
		glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, &projection_matrix[0][0]);*/
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &projection_matrix[0][0]);

		//glBindVertexArray(vao); // Unit Cube


		//Part 1 (Render to Shadow Map)
		//***********************************************************************************
		glCullFace(GL_FRONT);
		glUseProgram(shaderprogram_simpledepth);
		glViewport(0, 0, SHADOW_HEIGHT, SHADOW_WIDTH);
		setUniformMatrix4(shaderprogram_simpledepth, "lightSpaceMatrix", lightSpaceMatrix);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D,);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		car.DrawCar(shaderprogram_simpledepth, shaderprogram_simpledepth, deltatime, rubber_texure);
		for (int i = 0; i < cars.size(); i++)
		{
			cars[i].DrawCar(shaderprogram_simpledepth, shaderprogram_simpledepth, deltatime, rubber_texure);
		}
		g.drawGround(shaderprogram_simpledepth);
		border.DrawBorder(shaderprogram_simpledepth, wood_texture);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);
		//***********************************************************************************


		//RESET VIEWPORT
		//***********************************************************************************
		//glViewport(0, 0, 1024.0f, 768.0f);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//***********************************************************************************


		//Part 2 (Render normally)
		//***********************************************************************************
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &projection_matrix[0][0]); //Configure projection matrix

		
		//LIGHT
		glUseProgram(shaderprogram_light);
		light.draw(shaderprogram_light);
		//light.processInput(window, deltatime);
		g.updateGroundMaterial(shaderprogram_textured);
		headlight1.setPosition(car.getHeadLight1());
		headlight2.setPosition(car.getHeadLight2());
		headlight1.updateHeadLight_1(shaderprogram_textured);
		headlight1.updateHeadLight_1(shaderprogram_particle);
		headlight2.updateHeadLight_2(shaderprogram_textured);
		headlight2.updateHeadLight_2(shaderprogram_particle);

		taillight1.setPosition(car.getTailLight1());
		taillight2.setPosition(car.getTailLight2());
		taillight1.updateTailLight_1(shaderprogram_textured);
		taillight2.updateTailLight_2(shaderprogram_textured);

		light.updateShader(shaderprogram_car);
		light.updateShader(shaderprogram_textured);
		light.updateShader(shaderprogram_simpledepth);
		light.updateShader(shaderprogram_particle);
		
		int light_counter = 0;
			for (int i = 0; i < 6; i++)
			{
				headlight_positions[light_counter] = (cars[i].getHeadLight1());
				taillight_positions[light_counter] = (cars[i].getTailLight1());

				headlight_positions[light_counter + 1] = (cars[i].getHeadLight2());
				taillight_positions[light_counter + 1] = (cars[i].getTailLight2());
				light_counter += 2;
				//std::cout << light_counter << std::endl;
			}
		
		

		glUseProgram(shaderprogram_textured);
		
		for (int i = 0; i < 12; i++)
		{
			std::string s = std::to_string(i);
			int headlights_location = glGetUniformLocation(shaderprogram_textured, ("headlight_positions["+s+"]").c_str());
			glUniform3f(headlights_location, headlight_positions[i].x, headlight_positions[i].y, headlight_positions[i].z);
		}

		for (int i = 0; i < 12; i++)
		{
			std::string s = std::to_string(i);
			int taillights_location = glGetUniformLocation(shaderprogram_textured, ("taillight_positions["+s+"]").c_str());
			glUniform3f(taillights_location, taillight_positions[i].x, taillight_positions[i].y, taillight_positions[i].z);
		}


		
		/*int headlights_location = glGetUniformLocation(shaderprogram_textured, "headlight_positions");
		glUniform3fv(headlights_location,12,&(headlight_positions.data()[0].x));*/
		

		glUseProgram(shaderprogram_textured);
		int displayHeadLights_location = glGetUniformLocation(shaderprogram_textured, "displayHeadLights");

		if ((glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) && keystate_PgUp == GLFW_RELEASE)
		{
			glUniform1i(displayHeadLights_location, displayHeadlights);
			displayHeadlights = !displayHeadlights;
		}
		keystate_PgUp = glfwGetKey(window, GLFW_KEY_PAGE_UP);
		/*else if ((glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS))
		{
			glUniform1i(displayHeadLights_location, 1);
		}*/



		if ((glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS))
		{
			car_shader = regular;
		}

		else if ((glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS))
		{
			car_shader = lighting;
		}

		if (car_shader == lighting)
		{
			glPointSize(7.5f);
			glUseProgram(shaderprogram_car);
			car.DrawCar(shaderprogram_car, shaderprogram_textured, deltatime, rubber_texure);
			car.ProcessInputs(window, deltatime);
		}

		else
		{
			glPointSize(7.5f);
			glUseProgram(shaderprogram);
			car.DrawCar(shaderprogram, shaderprogram, deltatime, rubber_texure);
			car.ProcessInputs(window, deltatime);
		}


		//Model (worldMatrix)
		glm::mat4 model_matrix = glm::mat4(1.0);
		model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 0.0f, 0.0f));
		GLuint model_matrix_location = glGetUniformLocation(shaderprogram, "worldMatrix");

		glUseProgram(shaderprogram);
		glLineWidth(7.5f);
		//glBindVertexArray(axesVAO);
		glm::mat4 axes_transform = glm::mat4(1.0);
		glUniformMatrix4fv(model_matrix_location, 1, GL_FALSE, &axes_transform[0][0]);
		//glDrawArrays(GL_LINES, 0, 6); 
		axes.DrawAxes(shaderprogram);

		/*glPointSize(1.5f);
		glBindVertexArray(gridVAO);
		glDrawArrays(GL_POINTS, 0, 101 * 101);*/

		if ((glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS))
		{
			ground = grass;
		}

		else if ((glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS))
		{
			ground = grid;
		}


		if (ground == grid)
		{
			glUseProgram(shaderprogram);
			glLineWidth(1.0f);
			g.drawGrid(shaderprogram);
		}

		else
		{
			glGetIntegerv(GL_POLYGON_MODE, &polygonmode);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glUseProgram(shaderprogram_textured);
			setUniformMatrix4(shaderprogram_textured, "lightSpaceMatrix", lightSpaceMatrix);
			//setUniformMatrix4(shaderprogram_textured, "lightSpaceMatrix", lightSpaceMatrix);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, grass_texure);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			g.drawGround(shaderprogram_textured);
			glPolygonMode(GL_FRONT_AND_BACK, polygonmode); //Reset polygon mode to what it was before
			/*glBindTexture(GL_TEXTURE_2D, rubber_texure);
			glBindVertexArray(circletest);
			glDrawArrays(GL_TRIANGLES, 0, 4 * 3 * 73);*/
		}

		updateWorldOrientation(window, world_orientation_angle_x, world_orientation_angle_y, deltatime, camera);
		glm::mat4 orientation_matrix = glm::mat4(1.0);
		orientation_matrix = glm::rotate(orientation_matrix, glm::radians(world_orientation_angle_x), glm::vec3(0.0f, 1.0f, 0.0f));
		orientation_matrix = glm::rotate(orientation_matrix, glm::radians(world_orientation_angle_y), glm::vec3(1.0f, 0.0f, 0.0f));
		/*GLuint orientation_matrix_location = glGetUniformLocation(shaderprogram, "worldOrientation");
		glUniformMatrix4fv(orientation_matrix_location, 1, GL_FALSE, &orientation_matrix[0][0]);*/
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), &orientation_matrix[0][0]);


		//glUseProgram(shaderprogram_textured);
		//g.drawGround();


		//car.TranslateRandom(deltatime);
		//car2.DrawCar(shaderprogram_car, shaderprogram_textured, deltatime, rubber_texure);
		//car.UpdateTransformations();
		//car2.UpdateTransformations();
		//car.UpdateCollisionBoxes();
		//car2.UpdateCollisionBoxes();
		//car2.TranslateRandom(deltatime);


		//if (glm::length(car.getPosition() - car2.getPosition()) < 15.0f)
		//{
		//	collider.checkCollisionCar(car, car2, deltatime);
		//}

		//collider.checkCollisionCar(car, cars[2], deltatime);
		car.TranslateRandom(deltatime);
		car.UpdateTransformations();
		car.UpdateCollisionBoxes();
		
		
		//for (int i = 0; i < 20; i++)
		//{
		//	car.DrawCar(shaderprogram_car, shaderprogram_textured, deltatime, rubber_texure);
		//}


	

		for (int i = 0; i< cars.size(); i++)
		{
			cars[i].UpdateTransformations();
			cars[i].UpdateCollisionBoxes();
			if (glm::length(car.getPosition() - cars[i].getPosition()) < 10.0f)
			{
				collider.checkCollisionCar(car, cars[i], deltatime);
			}
			
			cars[i].DrawCar(shaderprogram_car, shaderprogram_textured, deltatime, rubber_texure);
			if (bumpercar_animation == true)
			{
				cars[i].TranslateRandom(deltatime);
			}
		}
	
		for (int i = 0; i < cars.size(); i++)
		{
			for (int j = i; j < cars.size(); j++)
			{
				if (i != j)
				{
					if (glm::length(cars[i].getPosition() - cars[j].getPosition()) < 10.0f)
					{
						collider.checkCollisionCar(cars[i], cars[j], deltatime);
					}
				}
			}	
		}

		if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS && (keystate_Del == GLFW_RELEASE))
		{
			bumpercar_animation = !bumpercar_animation;
		}
		keystate_Del = glfwGetKey(window, GLFW_KEY_DELETE);

		//BORDER TEST********************************************************************************************
		if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS && (keystate_Backspace == GLFW_RELEASE))
		{
			border_texture_ID++;
			border_texture_ID = border_texture_ID % 2;
		}
		keystate_Backspace = glfwGetKey(window, GLFW_KEY_BACKSPACE);
		if (border_texture_ID == 0)
		{
			border_texture = wood_texture;
		}
		else if (border_texture_ID == 1)
		{
			border_texture = wood_texture2;
		}

		border.DrawBorder(shaderprogram_textured, border_texture);
		//********************************************************************************************************

		
		//QUAD TEST***********************************************
		//glUseProgram(shaderprogram_textured);
		glUseProgram(shaderprogram_particle);
		Quad::updateMaterial(shaderprogram_particle);
		setUniformMatrix4(shaderprogram_particle, "lightSpaceMatrix", lightSpaceMatrix);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, smoke_texure);
		//quad.Draw(shaderprogram_textured);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		spawner.spawnSmoke(deltatime,car.getExhaust());
		spawner.updateRotation(camera);
		spawner.updateSmoke(deltatime);
		spawner.drawSmoke(shaderprogram_particle,deltatime);
		glEnable(GL_DEPTH_TEST);
		//*********************************************************
		
		//MOTION BLUR TEST*********************************************************
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		spawner.spawnCars(deltatime, car);
		spawner.updateCars(deltatime, shaderprogram_particle);
		spawner.drawCars(shaderprogram_particle, shaderprogram_particle, deltatime, rubber_texure);
		glEnable(GL_DEPTH_TEST);
		//**************************************************************************


		if ((glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) && (keystate_Enter == GLFW_RELEASE))
		{
			move_random = !move_random;
			car.setRandomMovement(move_random);
		}
		keystate_Enter = glfwGetKey(window, GLFW_KEY_ENTER);

		glfwSwapBuffers(window);
		glfwPollEvents();


		//Update Uniform Buffer (View Matrix, and Camera Position)
		camera.processInputs(window, deltatime, shaderprogram);
		camera.FollowCar(car);


		glm::mat4 view = camera.getviewMatrix();
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view[0][0]); //Update view matrix in shaders
		glm::vec3 camera_position = camera.getPosition();
		glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), sizeof(glm::vec3), &camera_position); //Update projection matrix shaders

		glUseProgram(shaderprogram_textured);
		int displayShadows_location = glGetUniformLocation(shaderprogram_textured, "displayShadows");

		if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
		{
			glUniform1i(displayShadows_location, 0);
		}

		else if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
		{
			glUniform1i(displayShadows_location, 1);
		}

		glUseProgram(shaderprogram_textured);
		setUniformVec3(shaderprogram_textured, "headlight1", car.getHeadLight1());
		setUniformVec3(shaderprogram_textured, "headlight2", car.getHeadLight2());

		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetWindowAspectRatio(window, 1024, 768);

	}

	std::cout << "Hello World!\n";

	glfwTerminate();
	return 0;


}

//Not used for this assignment
void framebuffer_size_callback(GLFWwindow* w, int width, int height)
{
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
	glViewport(0, 0, width, height);
	std::cout << "Window Adjusted. " << "Width: " << width << "Height: " << height << std::endl;
}

void processInput(GLFWwindow*w)
{
	if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(w, GL_TRUE);
	}
}


void updateWorldOrientation(GLFWwindow* w, float &orientation_angle_x, float &orientation_angle_y, float dt, Camera &c)
{
	if (glfwGetKey(w, GLFW_KEY_Z) == GLFW_PRESS)
	{
		orientation_angle_x -= 20 * dt;
	}

	if (glfwGetKey(w, GLFW_KEY_X) == GLFW_PRESS)
	{
		orientation_angle_x += 20 * dt;
	}

	if (glfwGetKey(w, GLFW_KEY_C) == GLFW_PRESS)
	{
		orientation_angle_y += 20 * dt;
	}

	if (glfwGetKey(w, GLFW_KEY_V) == GLFW_PRESS)
	{
		orientation_angle_y -= 20 * dt;
	}

	if (glfwGetKey(w, GLFW_KEY_HOME) == GLFW_PRESS)
	{
		orientation_angle_x = 0;
		orientation_angle_y = 0;
		c.Reset();
	}
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
