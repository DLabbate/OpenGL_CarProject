#pragma once
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <vector>
class ColliderBox
{
public:
	ColliderBox();
	~ColliderBox();
	void UpdatePoints();
	void Update(glm::mat4);
	std::vector<glm::vec3> getPoints();
	glm::mat4 getTransform();
	void UpdateCentre();
	glm::vec3 getCentre();

private:
	glm::mat4 transform;
	std::vector<glm::vec3> points;
	glm::vec3 centre = glm::vec3(0.0f,0.0f,0.0f);
	
};

