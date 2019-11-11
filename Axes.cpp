#include "pch.h"
#include "Axes.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

Axes::Axes()
{
	glm::vec3 x_vertices[] = {
		//Axis 1
		glm::vec3(0.0f,0.0f,0.0f),		//Position 0
		glm::vec3(1.0f,0.0f,0.0f),

		glm::vec3(10.0f, 0.0f, 0.0f),	//Position 1
		glm::vec3(1.0f,0.0f,0.0f),
	};

	glm::vec3 y_vertices[] = {
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(0.0f,1.0f,0.0f),

		glm::vec3(0.0f, 10.0f, 0.0f),
		glm::vec3(0.0f,1.0f,0.0f),
	};

	glm::vec3 z_vertices[] = {
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(0.1f,0.5f,1.0f),

		glm::vec3(0.0f, 0.0f, 10.0f),
		glm::vec3(0.1f,0.5f,1.0f)
	};

	GLuint x_VAO;
	glGenVertexArrays(1, &x_VAO);
	glBindVertexArray(x_VAO);

	GLuint x_VBO;
	glGenBuffers(1, &x_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, x_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(x_vertices), &x_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	x_axis_VAO = x_VAO; //We make VAO for the x axis

	
	GLuint y_VAO;
	glGenVertexArrays(1, &y_VAO);
	glBindVertexArray(y_VAO);

	GLuint y_VBO;
	glGenBuffers(1, &y_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, y_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(y_vertices), &y_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	y_axis_VAO = y_VAO; //We make VAO for the y axis


	
	GLuint z_VAO;
	glGenVertexArrays(1, &z_VAO);
	glBindVertexArray(z_VAO);

	GLuint z_VBO;
	glGenBuffers(1, &z_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, z_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(z_vertices), &z_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	z_axis_VAO = z_VAO; //We make VAO for the y axis
}


Axes::~Axes()
{
}

void Axes::DrawAxes(int shaderprogram)
{
	//X Axis
	glm::vec3 x_colour = glm::vec3(0.0f, 0.0f, 1.0f);
	GLuint mycolour_location = glGetUniformLocation(shaderprogram, "mycolour");
	glUniform3f(mycolour_location, x_colour.x, x_colour.y, x_colour.z);

	glBindVertexArray(x_axis_VAO);
	glDrawArrays(GL_LINES, 0, 2);

	//Y Axis
	glm::vec3 y_colour = glm::vec3(0.0f, 1.0f, 0.0f);
	glUniform3f(mycolour_location, y_colour.x, y_colour.y, y_colour.z);

	glBindVertexArray(y_axis_VAO);
	glDrawArrays(GL_LINES, 0, 2);

	//Z Axis
	glm::vec3 z_colour = glm::vec3(1.0f, 0.0f, 0.0f);
	glUniform3f(mycolour_location, z_colour.x, z_colour.y, z_colour.z);

	glBindVertexArray(z_axis_VAO);
	glDrawArrays(GL_LINES, 0, 2);

}

