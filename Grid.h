#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Grid
{
public:
	Grid();
	~Grid();
	GLuint generateGridHorizontal();
	GLuint generateGridVertical();
	void drawGrid(int);
	GLuint generateGroundGrid(int,GLuint);
	void drawGround(int);
	void updateGroundMaterial(int);

private:
	GLuint VAO_vertical;
	GLuint VAO_horizontal;

	GLuint VAO_ground;
};

