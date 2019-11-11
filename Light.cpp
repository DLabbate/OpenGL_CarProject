#include "pch.h"
#include <iostream>
#include "Light.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


Light::Light()
{
	lightcolour = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::vec3 vertices[] = {
		glm::vec3(-0.5f,-0.5f,0.5f),	//Position1
		lightcolour,					//Colour1

		glm::vec3(0.5f,-0.5f,0.5f),		//Position2
		lightcolour,					//Colour2...

		glm::vec3(0.5f,0.5f,0.5f),
		lightcolour,

		glm::vec3(-0.5f,0.5f,0.5f),
		lightcolour,

		glm::vec3(-0.5f,-0.5f,-0.5f),
		lightcolour,

		glm::vec3(0.5f,-0.5f,-0.5f),
		lightcolour,

		glm::vec3(0.5f,0.5f,-0.5f),
		lightcolour,

		glm::vec3(-0.5f,0.5f,-0.5f),
		lightcolour
	};


	GLuint index[] = {
		0,1,2,0,2,3, //Face 1

		4,5,6,4,6,7, //Back

		4,0,3,4,3,7, //Left

		1,5,2,5,6,2, //Right

		3,2,6,3,6,7, //Top

		0,1,5,0,5,4

	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

	vao = VAO;
}


Light::~Light()
{
}

void Light::draw(int shaderprogram)
{
	glBindVertexArray(vao);

	glm::mat4 light_transform = glm::mat4(1.0);
	light_transform = glm::translate(light_transform, position);
	light_transform = glm::scale(light_transform, glm::vec3(0.4));

	GLuint model_matrix_location = glGetUniformLocation(shaderprogram, "worldMatrix");
	glUniformMatrix4fv(model_matrix_location, 1, GL_FALSE, &light_transform[0][0]);

	GLuint lightcolour_location = glGetUniformLocation(shaderprogram, "lightcolour");
	glUniform3f(lightcolour_location,lightcolour.x,lightcolour.y,lightcolour.z);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

glm::vec3 Light::getLightColour() const
{
	return lightcolour;
}

glm::vec3 Light::getPosition() const
{
	return position;
}

void Light::updateShader(int shaderprogram)
{
	glUseProgram(shaderprogram);
	GLuint light_position_location = glGetUniformLocation(shaderprogram, "light.position");
	glUniform3f(light_position_location, position.x, position.y, position.z);

	GLuint light_ambient_location = glGetUniformLocation(shaderprogram, "light.ambient");
	glUniform3f(light_ambient_location, ambient.x,ambient.y,ambient.z);
	//std::cout << "Ambient Light: " << ambient.x<<ambient.y<<ambient.z<<std::endl;

	GLuint light_diffuse_location = glGetUniformLocation(shaderprogram, "light.diffuse");
	glUniform3f(light_diffuse_location, diffuse.x,diffuse.y,diffuse.z);
	//std::cout << "Diffuse Light: " << diffuse.x << diffuse.y << diffuse.z << std::endl;

	GLuint light_specular_location = glGetUniformLocation(shaderprogram, "light.specular");
	glUniform3f(light_specular_location, specular.x,specular.y,specular.z);
	//std::cout << "Specular Light: " << specular.x << specular.y << specular.z << std::endl;

	/*GLuint light_constant_location = glGetUniformLocation(shaderprogram, "light.constant");
	glUniform1f(light_constant_location, 1.0f);

	GLuint light_linear_location = glGetUniformLocation(shaderprogram, "light.linear");
	glUniform1f(light_linear_location, 0.09f);

	GLuint light_quadratic_location = glGetUniformLocation(shaderprogram, "light.quadratic");
	glUniform1f(light_quadratic_location, 0.032f);*/

	GLuint light_terms_location = glGetUniformLocation(shaderprogram, "light.terms");
	glUniform3f(light_terms_location, 1.0f,0.0014f, 0.000007f);
}

void Light::processInput(GLFWwindow* w,float dt)
{
	if (glfwGetKey(w, GLFW_KEY_5) == GLFW_PRESS)
	{
		position.y -= 30.0f * dt;
	}

	if (glfwGetKey(w, GLFW_KEY_6) == GLFW_PRESS)
	{
		position.y += 30.0f * dt;
	}
}

void Light::setPosition(glm::vec3 pos)
{
	this->position = pos;
}

void Light::updateHeadLight_1(int shaderprogram)
{
	glUseProgram(shaderprogram);

	GLuint light_position_location = glGetUniformLocation(shaderprogram, "headlight1.position");
	glUniform3f(light_position_location, position.x, position.y, position.z);

	GLuint light_ambient_location = glGetUniformLocation(shaderprogram, "headlight1.ambient");
	glUniform3f(light_ambient_location, 0.5f, 0.5f, 0.5f);
	//std::cout << "Ambient Light: " << ambient.x<<ambient.y<<ambient.z<<std::endl;

	GLuint light_diffuse_location = glGetUniformLocation(shaderprogram, "headlight1.diffuse");
	glUniform3f(light_diffuse_location, diffuse.x, diffuse.y, diffuse.z);
	//std::cout << "Diffuse Light: " << diffuse.x << diffuse.y << diffuse.z << std::endl;

	GLuint light_specular_location = glGetUniformLocation(shaderprogram, "headlight1.specular");
	glUniform3f(light_specular_location, specular.x, specular.y, specular.z);
	//std::cout << "Specular Light: " << specular.x << specular.y << specular.z << std::endl;

	/*GLuint light_constant_location = glGetUniformLocation(shaderprogram, "light.constant");
	glUniform1f(light_constant_location, 1.0f);

	GLuint light_linear_location = glGetUniformLocation(shaderprogram, "light.linear");
	glUniform1f(light_linear_location, 0.09f);

	GLuint light_quadratic_location = glGetUniformLocation(shaderprogram, "light.quadratic");
	glUniform1f(light_quadratic_location, 0.032f);*/

	GLuint light_terms_location = glGetUniformLocation(shaderprogram, "headlight1.terms");
	glUniform3f(light_terms_location, 1.0f, 0.14f, 0.07f);

}

void Light::updateHeadLight_2(int shaderprogram)
{
	glUseProgram(shaderprogram);

	GLuint light_position_location = glGetUniformLocation(shaderprogram, "headlight2.position");
	glUniform3f(light_position_location, position.x, position.y, position.z);

	GLuint light_ambient_location = glGetUniformLocation(shaderprogram, "headlight2.ambient");
	glUniform3f(light_ambient_location, 0.5f, 0.5f, 0.5f);
	//std::cout << "Ambient Light: " << ambient.x<<ambient.y<<ambient.z<<std::endl;

	GLuint light_diffuse_location = glGetUniformLocation(shaderprogram, "headlight2.diffuse");
	glUniform3f(light_diffuse_location, diffuse.x, diffuse.y, diffuse.z);
	//std::cout << "Diffuse Light: " << diffuse.x << diffuse.y << diffuse.z << std::endl;

	GLuint light_specular_location = glGetUniformLocation(shaderprogram, "headlight2.specular");
	glUniform3f(light_specular_location, specular.x, specular.y, specular.z);
	//std::cout << "Specular Light: " << specular.x << specular.y << specular.z << std::endl;

	/*GLuint light_constant_location = glGetUniformLocation(shaderprogram, "light.constant");
	glUniform1f(light_constant_location, 1.0f);

	GLuint light_linear_location = glGetUniformLocation(shaderprogram, "light.linear");
	glUniform1f(light_linear_location, 0.09f);

	GLuint light_quadratic_location = glGetUniformLocation(shaderprogram, "light.quadratic");
	glUniform1f(light_quadratic_location, 0.032f);*/

	GLuint light_terms_location = glGetUniformLocation(shaderprogram, "headlight2.terms");
	glUniform3f(light_terms_location, 1.0f, 0.14f, 0.07f);

}

void Light::updateTailLight_1(int shaderprogram)
{
	glUseProgram(shaderprogram);

	GLuint light_position_location = glGetUniformLocation(shaderprogram, "taillight1.position");
	glUniform3f(light_position_location, position.x, position.y, position.z);

	GLuint light_ambient_location = glGetUniformLocation(shaderprogram, "taillight1.ambient");
	glUniform3f(light_ambient_location, 0.5f, 0.0f, 0.0f); //RED
	//std::cout << "Ambient Light: " << ambient.x<<ambient.y<<ambient.z<<std::endl;

	GLuint light_diffuse_location = glGetUniformLocation(shaderprogram, "taillight1.diffuse");
	glUniform3f(light_diffuse_location, diffuse.x, diffuse.y, diffuse.z);
	//std::cout << "Diffuse Light: " << diffuse.x << diffuse.y << diffuse.z << std::endl;

	GLuint light_specular_location = glGetUniformLocation(shaderprogram, "taillight1.specular");
	glUniform3f(light_specular_location, specular.x, specular.y, specular.z);
	//std::cout << "Specular Light: " << specular.x << specular.y << specular.z << std::endl;

	/*GLuint light_constant_location = glGetUniformLocation(shaderprogram, "light.constant");
	glUniform1f(light_constant_location, 1.0f);

	GLuint light_linear_location = glGetUniformLocation(shaderprogram, "light.linear");
	glUniform1f(light_linear_location, 0.09f);

	GLuint light_quadratic_location = glGetUniformLocation(shaderprogram, "light.quadratic");
	glUniform1f(light_quadratic_location, 0.032f);*/

	GLuint light_terms_location = glGetUniformLocation(shaderprogram, "taillight1.terms");
	glUniform3f(light_terms_location, 1.0f, 0.14f, 0.07f);

}

void Light::updateTailLight_2(int shaderprogram)
{
	glUseProgram(shaderprogram);

	GLuint light_position_location = glGetUniformLocation(shaderprogram, "taillight2.position");
	glUniform3f(light_position_location, position.x, position.y, position.z);

	GLuint light_ambient_location = glGetUniformLocation(shaderprogram, "taillight2.ambient");
	glUniform3f(light_ambient_location, 0.5f, 0.0f, 0.0f); //RED
	//std::cout << "Ambient Light: " << ambient.x<<ambient.y<<ambient.z<<std::endl;

	GLuint light_diffuse_location = glGetUniformLocation(shaderprogram, "taillight2.diffuse");
	glUniform3f(light_diffuse_location, diffuse.x, diffuse.y, diffuse.z);
	//std::cout << "Diffuse Light: " << diffuse.x << diffuse.y << diffuse.z << std::endl;

	GLuint light_specular_location = glGetUniformLocation(shaderprogram, "taillight2.specular");
	glUniform3f(light_specular_location, specular.x, specular.y, specular.z);
	//std::cout << "Specular Light: " << specular.x << specular.y << specular.z << std::endl;

	/*GLuint light_constant_location = glGetUniformLocation(shaderprogram, "light.constant");
	glUniform1f(light_constant_location, 1.0f);

	GLuint light_linear_location = glGetUniformLocation(shaderprogram, "light.linear");
	glUniform1f(light_linear_location, 0.09f);

	GLuint light_quadratic_location = glGetUniformLocation(shaderprogram, "light.quadratic");
	glUniform1f(light_quadratic_location, 0.032f);*/

	GLuint light_terms_location = glGetUniformLocation(shaderprogram, "taillight2.terms");
	glUniform3f(light_terms_location, 1.0f, 0.14f, 0.07f);

}

void Light::setDiffuse(glm::vec3 diff)
{
	diffuse = diff;
}

