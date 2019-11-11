#pragma once
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
class Quad
{
public:
	Quad();
	Quad(glm::vec3);
	Quad(glm::vec3, glm::vec3);
	~Quad();
	void generateVAO();
	void Draw(int);
	void Update(float);
	bool checkExpired();
	static void updateMaterial(int);
	void setAngle(float); //Rotates along y-axis
	void updateRotation(float);

private:
	GLuint quadVAO;
	float speed = 1100.0f * 1.5;
	float lifetime = 2.5f;
	float age = 0.0f;
	float size = 0.35f;
	float max_size = 4.0f;
	float growth_speed = 0.4f;
	bool isExpired = false;
	glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f);
	glm::vec3 velocity;
	float rotation_angle = 0.0f;
	float angle_z_random;

	float opacity;
	float opacity_final = 0.3f;
	float opacity_initial = 1.0f;
};

