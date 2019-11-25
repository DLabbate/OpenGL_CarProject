#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Light
{
public:
	Light();
	~Light();
	void draw(int);
	glm::vec3 getLightColour() const;
	glm::vec3 getPosition() const;
	void updateShader(int);
	void processInput(GLFWwindow*,float);
	void setPosition(glm::vec3);
	void updateHeadLight_1(int);
	void updateHeadLight_2(int);
	void updateTailLight_1(int);
	void updateTailLight_2(int);
	void setDiffuse(glm::vec3);

private:
	GLuint vao;
	glm::vec3 lightcolour = glm::vec3(1.0f,1.0f,1.0f);
	glm::vec3 position = glm::vec3(-50.0f,50.0f,50.0f);
	glm::vec3 ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
};

