#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
class Axes
{
public:
	Axes();
	~Axes();
	void DrawAxes(int);
private:
	float length;
	GLuint x_axis_VAO;
	GLuint y_axis_VAO;
	GLuint z_axis_VAO;
	
};

