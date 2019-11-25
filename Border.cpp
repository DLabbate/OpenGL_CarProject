#include "pch.h"
#include "Border.h"


Border::Border()
{
	generateVAO();
}


Border::~Border()
{
}

void Border::generateVAO()
{
	float size = 100.0f;
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  size/4,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  size/4,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  size/4,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  size/4,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  size/4,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  size/4,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  size/4,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  size/4,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  size/4,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  size/4,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  size/4,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  size/4,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  size/4,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  size/4,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  size/4,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  size/4,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  size/4,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  size/4,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	GLuint VAO,VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //Positions
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //Normals
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //uv
	glEnableVertexAttribArray(2);

	this->vao = VAO;
}

void Border::DrawBorder(int shaderprogram, unsigned int texture)
{
	glUseProgram(shaderprogram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(vao);

	GLuint worldMatrixLocation = glGetUniformLocation(shaderprogram, "worldMatrix");
	glm::mat4 border_transform = glm::mat4(1.0f);
	glm::mat4 border_transform2 = glm::mat4(1.0f);
	glm::mat4 border_transform3 = glm::mat4(1.0f);
	glm::mat4 border_transform4 = glm::mat4(1.0f);

	GLuint material_ambient_location = glGetUniformLocation(shaderprogram, "material.ambient");
	glUniform3f(material_ambient_location, 0.75f, 0.75f, 0.75f);
	

	//Border1
	border_transform = glm::translate(border_transform, glm::vec3(0.0f, 0.5f, -52.5f +5));
	border_transform = glm::scale(border_transform, glm::vec3(100.0f, 5.0f, 5.0f));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &border_transform[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Border2
	border_transform2 = glm::translate(border_transform2, glm::vec3(0.0f, 0.5f, 52.5f -5));
	border_transform2 = glm::scale(border_transform2, glm::vec3(100.0f, 5.0f, 5.0f));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &border_transform2[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Border3
	border_transform3 = glm::rotate(border_transform3, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	border_transform3 = glm::translate(border_transform3, glm::vec3(0.0f, 0.5f, 52.5f-5));
	border_transform3 = glm::scale(border_transform3, glm::vec3(90.0f, 5.0f, 5.0f));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &border_transform3[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Border4
	border_transform4 = glm::rotate(border_transform4, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	border_transform4 = glm::translate(border_transform4, glm::vec3(0.0f, 0.5f, 52.5f-5));
	border_transform4 = glm::scale(border_transform4, glm::vec3(90.0f, 5.0f, 5.0f));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &border_transform4[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

}
