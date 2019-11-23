#pragma once
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <vector>
#include "ColliderBox.h"
#include "CarModel.h"
class Collider
{
public:
	Collider();
	~Collider();
	glm::vec3 checkCollision(ColliderBox, ColliderBox);
	void checkCollisionCar(CarModel&, CarModel&,float);
	void checkCubeSpace(std::vector<glm::vec3>);
};

