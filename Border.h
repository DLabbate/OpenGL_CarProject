#pragma once
#include <iostream>
#include "CarModel.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Border
{
public:
	Border();
	~Border();
	void DrawBorder(int,unsigned int);
	void generateVAO();

private:
	int vao;

};

