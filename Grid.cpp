#include "pch.h"
#include "Grid.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



Grid::Grid()
{
}


Grid::~Grid()
{
}

GLuint Grid::generateGridHorizontal()
{
	glm::vec3 vertices[(101 * 2)];
	glm::vec3 colours[(101 * 2)];
	glm::vec3 position = glm::vec3(-50.0f, 0.0f, 50.0f);
	unsigned int index = 0;

	for (unsigned int i = 0; i <= 100; i++)
	{
		vertices[index++] = glm::vec3(position.x, 0.0f, position.z - i);
		vertices[index++] = glm::vec3(position.x + 100, 0.0f, position.z - i);
	}

	unsigned int index2 = 0;
	for (unsigned int i = 0; i <= 100; i++)
	{
		colours[index2++] = glm::vec3(1.0f, 1.0f, 1.0f);
		colours[index2++] = glm::vec3(1.0f, 1.0f, 1.0f);
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

	VAO_horizontal = VAO;

	return VAO;
}

GLuint Grid::generateGridVertical()
{
	glm::vec3 vertices[(101 * 2)];
	glm::vec3 colours[(101 * 2)];
	glm::vec3 position = glm::vec3(-50.0f, 0.0f, 50.0f);
	unsigned int index = 0;

	for (unsigned int i = 0; i <= 100; i++)
	{
		vertices[index++] = glm::vec3(position.x + i, 0.0f, position.z);
		vertices[index++] = glm::vec3(position.x + i, 0.0f, position.z - 100);
	}

	unsigned int index2 = 0;
	for (unsigned int i = 0; i <= 100; i++)
	{
		colours[index2++] = glm::vec3(1.0f, 1.0f, 1.0f);
		colours[index2++] = glm::vec3(1.0f, 1.0f, 1.0f);
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

	VAO_vertical = VAO;

	return VAO;
}

void Grid::drawGrid(int shaderprogram)
{
	glm::vec3 grid_colour = glm::vec3(1.0f, 1.0f, 1.0f);
	GLuint mycolour_location = glGetUniformLocation(shaderprogram, "mycolour");
	glUniform3f(mycolour_location, grid_colour.x, grid_colour.y, grid_colour.z);

	glBindVertexArray(VAO_horizontal);
	glDrawArrays(GL_LINES, 0, 101 * 2);

	glBindVertexArray(VAO_vertical);
	glDrawArrays(GL_LINES, 0, 101 * 2);
}

GLuint Grid::generateGroundGrid(int size,GLuint textureID)
{
	struct TexturedColoredVertex
	{
		TexturedColoredVertex(glm::vec3 _position, glm::vec3 _color, glm::vec2 _uv)
			: position(_position), color(_color), uv(_uv) {}

		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 uv;
	};
	
	glm::vec3 mycolour = glm::vec3(1.0f, 1.0f, 1.0f);

	TexturedColoredVertex vertices[]
	{
		TexturedColoredVertex(glm::vec3(-size / 2, 0.0f, -size/2), mycolour, glm::vec2(0.0f,0.0f)),
		TexturedColoredVertex(glm::vec3(size / 2, 0.0f, -size / 2), mycolour, glm::vec2(size/4,0.0f)),
		TexturedColoredVertex(glm::vec3(size / 2, 0.0f, size / 2), mycolour, glm::vec2(size/4,size/4)),
		TexturedColoredVertex(glm::vec3(-size / 2, 0.0f, size / 2), mycolour, glm::vec2(0.0f,size/4)),
	};

	unsigned int index[]
	{
		0,1,2,
		0,2,3
	};

	/*glm::vec2 texture_coordinates[]
	{
		glm::vec2(0.0f,0.0f),
		glm::vec2(1.0f,0.0f),
		glm::vec2(1.0f,1.0f),
		glm::vec2(0.0f,1.0f)
	};*/

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedColoredVertex), (void*)0);
	glEnableVertexAttribArray(0);

	glBindTexture(GL_TEXTURE_2D,textureID); //Bound to texture unit 0
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedColoredVertex), (void*)(2*sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);


	glm::vec3 normals[]
	{
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	};
	GLuint VBO_normals;
	glGenBuffers(1, &VBO_normals);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(0));
	glEnableVertexAttribArray(3);


	VAO_ground = VAO;

	return VAO;
}

void Grid::drawGround(int shaderprogram)
{
	glUseProgram(shaderprogram);
	glm::mat4 matrix = glm::mat4(1.0f);
	GLuint model_matrix_location = glGetUniformLocation(shaderprogram, "worldMatrix");
	glUniformMatrix4fv(model_matrix_location, 1, GL_FALSE, &matrix[0][0]);

	glBindVertexArray(VAO_ground);
	updateGroundMaterial(shaderprogram);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Grid::updateGroundMaterial(int shaderprogram)
{
	glUseProgram(shaderprogram);

	GLuint material_ambient_location = glGetUniformLocation(shaderprogram, "material.ambient");
	glUniform3f(material_ambient_location, 0.25f, 0.25f, 0.25f);

	GLuint material_diffuse_location = glGetUniformLocation(shaderprogram, "material.diffuse");
	glUniform3f(material_diffuse_location, 0.8f, 0.8f, 0.8f);

	GLuint material_specular_location = glGetUniformLocation(shaderprogram, "material.specular");
	glUniform3f(material_specular_location, 0.01f, 0.01f, 0.01f);

	GLuint material_shininess_location = glGetUniformLocation(shaderprogram, "material.shininess");
	glUniform1f(material_shininess_location, 0.01f * 128);
}

