#pragma once
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include "Quad.h"
#include <list>
#include "Camera.h"
class Spawner
{
public:
	Spawner();
	~Spawner();
	void spawnSmoke(float);
	void spawnSmoke(float, glm::vec3);
	void drawSmoke(int,float);
	void updateSmoke(float);
	void updateRotation(Camera);

private:
	float spawn_timer = 0.0f;
	float spawn_interval = 0.025f/2;
	std::list<Quad*> smokeparticles;
};

