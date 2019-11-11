#include "pch.h"
#include <iostream>
#include "CarModel.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

CarModel::CarModel()
{
	x_pos = 0;
	y_pos = 0;
	z_pos = 0;
	generateBodyVAO();
	generateWheelVAO();
}

CarModel::CarModel(float x, float y, float z)
{
	this->x_pos = x;
	this->y_pos = y;
	this->z_pos = z;
	generateBodyVAO();
	generateWheelVAO();
}

void CarModel::generateBodyVAO()
{
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	GLuint VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //Positions
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float))); //Normals
	glEnableVertexAttribArray(2);

	vao_body = VAO;
}

void CarModel::generateWheelVAO()
{

	float pi = glm::pi<float>();
	glm::vec3 vertices[4 * 3 * (72 + 1)];

	glm::vec3 normals[4 * 3 * (72 + 1)];
	glm::vec2 uv[4 * 3 * (72 + 1)];

	int index = 0;

	//for (float x = -0.5f; x <= 0.5f; x += 0.5f)
	{
		for (float theta = 0.0f; theta < 2 * pi; theta = theta + pi / 36)
		{
			std::cout << index << std::endl;
			vertices[index] = glm::vec3(0.5f, 0.0f, 0.0f);
			vertices[++index] = glm::vec3(0.5f, 0.5*sin(theta), 0.5*cos(theta));
			vertices[++index] = glm::vec3(0.5f, 0.5*sin(theta + pi / 36), 0.5*cos(theta + pi / 36));
			index++;
		}
	}

	{
		for (float theta = 0.0f; theta < 2 * pi; theta = theta + pi / 36)
		{
			std::cout << index << std::endl;
			vertices[index] = glm::vec3(-0.5f, 0.0f, 0.0f);
			vertices[++index] = glm::vec3(-0.5f, 0.5*sin(theta), 0.5*cos(theta));
			vertices[++index] = glm::vec3(-0.5f, 0.5*sin(theta + pi / 36), 0.5*cos(theta + pi / 36));
			index++;
		}
	}

	for (float theta = 0.0f; theta < 2 * pi; theta = theta + pi / 36) //Executes 73 times
	{
		//std::cout << index << std::endl;
		vertices[index] = glm::vec3(-0.5f, 0.5*sin(theta), 0.5*cos(theta));
		vertices[++index] = glm::vec3(-0.5f, 0.5*sin(theta + pi / 36), 0.5*cos(theta + pi / 36));
		vertices[++index] = glm::vec3(-0.5f + 1, 0.5*sin(theta + pi / 36), 0.5*cos(theta + pi / 36));
		index++;
	}

	for (float theta = 0.0f; theta < 2 * pi; theta = theta + pi / 36) //Executes 73 times
	{
		//std::cout << index << std::endl;
		vertices[index] = glm::vec3(0.5f, 0.5*sin(theta), 0.5*cos(theta));
		vertices[++index] = glm::vec3(0.5f, 0.5*sin(theta + pi / 36), 0.5*cos(theta + pi / 36));
		vertices[++index] = glm::vec3(-0.5f, 0.5*sin(theta), 0.5*cos(theta));
		index++;
	}

	for (int i = 0; i < sizeof(vertices) / sizeof(glm::vec3); i++)
	{
		normals[i] = glm::vec3(0.0f,vertices[i].y,vertices[i].z); //Since it is centered at (0,0,0)
		//uv[i] = glm::vec2(vertices[i].z, vertices[i].y);
		uv[i] = glm::vec2(atan(vertices[i].y / vertices[i].z), vertices[i].x);
	}

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	GLuint VBO_uv;
	glGenBuffers(1, &VBO_uv);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(2);

	GLuint VBO_normals;
	glGenBuffers(1, &VBO_normals);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(3);

	vao_wheel = VAO;

}


void CarModel::DrawCar(int shaderprogram, int shaderprogram_wheels,float deltatime, unsigned int wheel_texture)
{
	//glBindVertexArray(vao_body); //Unit cube VAO
	updateVectors();

	right = glm::vec3(cos(glm::radians(car_angle)), 0.0f, -sin(glm::radians(car_angle))); //Vector pointing to the side of the car
	glm::mat4 rotation_matrix = glm::mat4(1.0); //Rotation applied to all parts of the car

	//Matrix to rotate and translate car. This matrix first translates to the origin, applies scaling and rotation, followed by a translation back to original position
	//Note that rotation is performed about the CENTRE of the car
	rotation_matrix = glm::translate(rotation_matrix, glm::vec3(x_pos + 2.0f, 0.0f, z_pos + 2.0f));
	rotation_matrix = glm::rotate(rotation_matrix, glm::radians(car_angle), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation_matrix = glm::scale(rotation_matrix, glm::vec3(scale, scale, scale));
	rotation_matrix = glm::translate(rotation_matrix, glm::vec3(x_pos + 2.0f, 0.0f, z_pos + 2.0f)*-1.0f);

	//Matrix to make wheels spin
	glm::mat4 wheel_rotation = glm::mat4(1.0);
	wheel_rotation = glm::rotate(wheel_rotation, -glm::radians(wheel_angle), glm::vec3(1.0f, 0.0f, 0.0f));

	//Change Wheel Colour
	glm::vec3 wheel_colour = glm::vec3(0.0f, 0.0f, 0.0f);
	GLuint mycolour_location = glGetUniformLocation(shaderprogram, "mycolour");
	glUniform3f(mycolour_location, wheel_colour.x,wheel_colour.y,wheel_colour.z);

	glm::mat4 wheel_rotation_lateral = glm::mat4(1.0);
	wheel_rotation_lateral = glm::rotate(wheel_rotation_lateral, glm::radians(wheel_angle_lateral), glm::vec3(0.0f, 1.0f, 0.0f));
	
	//UNIFORM LOCATIONS
	GLuint model_matrix_location_wheel = glGetUniformLocation(shaderprogram_wheels, "worldMatrix");
	GLuint model_matrix_location_body = glGetUniformLocation(shaderprogram, "worldMatrix");

	//Bind Texture Shader (for wheels) before drawing, and update wheel material in the shader
	glUseProgram(shaderprogram_wheels);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,wheel_texture);
	glBindVertexArray(vao_wheel);
	updateWheelMaterial(shaderprogram_wheels);

	//***TOP LEFT WHEEL***
	//Notes: For the wheels, rotation (along wheels origin) is applied first, following a translate, and subsequently a rotation of the car
	//Rotation of the car is applied Last
	glm::mat4 wheel1_transform = glm::mat4(1.0f);
	wheel1_transform = rotation_matrix * glm::translate(wheel1_transform, glm::vec3(x_pos + 0.0f, y_pos + 0.0f, z_pos + 0.0f)) *wheel_rotation_lateral * wheel_rotation;
	glUniformMatrix4fv(model_matrix_location_wheel, 1, GL_FALSE, &wheel1_transform[0][0]);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	glDrawArrays(GL_TRIANGLES, 0, 4 * 3 * 73);

	//***TOP RIGHT WHEEL***
	glm::mat4 wheel2_transform = glm::mat4(1.0);
	wheel2_transform = rotation_matrix * glm::translate(wheel2_transform, glm::vec3(x_pos + 4.0f, y_pos + 0.0f, z_pos + 0.0f)) * wheel_rotation_lateral * wheel_rotation;
	glUniformMatrix4fv(model_matrix_location_wheel, 1, GL_FALSE, &wheel2_transform[0][0]);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	glDrawArrays(GL_TRIANGLES, 0, 4 * 3 * 73);

	//***BOTTOM LEFT WHEEL***
	glm::mat4 wheel3_transform = glm::mat4(1.0);
	wheel3_transform = rotation_matrix * glm::translate(wheel3_transform, glm::vec3(x_pos + 0.0f, y_pos + 0.0f, z_pos + 4.0f)) * wheel_rotation;
	glUniformMatrix4fv(model_matrix_location_wheel, 1, GL_FALSE, &wheel3_transform[0][0]);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	glDrawArrays(GL_TRIANGLES, 0, 4 * 3 * 73);

	//***BOTTOM RIGHT WHEEL***
	glm::mat4 wheel4_transform = glm::mat4(1.0);
	wheel4_transform = rotation_matrix * glm::translate(wheel4_transform, glm::vec3(x_pos + 4.0f, y_pos + 0.0f, z_pos + 4.0f)) * wheel_rotation;
	glUniformMatrix4fv(model_matrix_location_wheel, 1, GL_FALSE, &wheel4_transform[0][0]);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	glDrawArrays(GL_TRIANGLES, 0, 4 * 3 * 73);

	
	//Bind Shader for car body, and update material in shader
	glUseProgram(shaderprogram);
	glBindVertexArray(vao_body);
	updateMaterial(shaderprogram);
	
	//***BODY***
	glm::vec3 body_colour = glm::vec3(0.0f, 100.0f/255, 255.0f/255);
	glUniform3f(mycolour_location, body_colour.x, body_colour.y, body_colour.z);

	glm::mat4 body_transform = glm::mat4(1.0);
	body_transform = rotation_matrix * glm::translate(body_transform, glm::vec3(x_pos + 2.0f, y_pos + 0.75f, z_pos + 2.0f));
	body_transform = glm::scale(body_transform, glm::vec3(3.0f, 2.5f, 5.0f));
	glUniformMatrix4fv(model_matrix_location_body, 1, GL_FALSE, &body_transform[0][0]);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//***TRUNK***
	glm::vec3 trunk_colour = glm::vec3(51.0f/255, 153.0f/255, 255.0f/255);
	glUniform3f(mycolour_location, trunk_colour.x, trunk_colour.y, trunk_colour.z);

	glm::mat4 trunk_transform = glm::mat4(1.0);
	trunk_transform = rotation_matrix * glm::translate(trunk_transform, glm::vec3(x_pos + 2.0f, y_pos + 0.15f, z_pos + 5.5f));
	trunk_transform = glm::scale(trunk_transform, glm::vec3(2.5f, 1.15f, 2.0f));
	glUniformMatrix4fv(model_matrix_location_body, 1, GL_FALSE, &trunk_transform[0][0]);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//***BONNET***
	glm::mat4 bonnet_transform = glm::mat4(1.0);
	bonnet_transform = rotation_matrix * glm::translate(bonnet_transform, glm::vec3(x_pos + 2.0f, y_pos + 0.15f, z_pos + -1.5f));
	bonnet_transform = glm::scale(bonnet_transform, glm::vec3(2.5f, 1.15f, 2.0f));
	glUniformMatrix4fv(model_matrix_location_body, 1, GL_FALSE, &bonnet_transform[0][0]);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//***ROOF***
	glm::mat4 roof_transform = glm::mat4(1.0);
	roof_transform = rotation_matrix * glm::translate(roof_transform, glm::vec3(x_pos + 2.0f, y_pos + 2.375f, z_pos + 2.0f));
	roof_transform = glm::scale(roof_transform, glm::vec3(3.0f, 0.75f, 3.0f));
	glUniformMatrix4fv(model_matrix_location_body, 1, GL_FALSE, &roof_transform[0][0]);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glm::mat4 spotlight1_transform = glm::mat4(1.0);
	spotlight1_transform = rotation_matrix * glm::translate(spotlight1_transform, glm::vec3(x_pos + 1.0f, y_pos+0.5, z_pos - 2.50f));
	spotlight1_transform = glm::scale(spotlight1_transform, glm::vec3(0.25f, 0.25f, 0.25f));
	glUniformMatrix4fv(model_matrix_location_body, 1, GL_FALSE, &spotlight1_transform[0][0]);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glm::mat4 spotlight2_transform = glm::mat4(1.0);
	spotlight2_transform = rotation_matrix * glm::translate(spotlight2_transform, glm::vec3(x_pos + 3.0f, y_pos + 0.5, z_pos - 2.50f));
	spotlight2_transform = glm::scale(spotlight2_transform, glm::vec3(0.25f, 0.25f, 0.25f));
	glUniformMatrix4fv(model_matrix_location_body, 1, GL_FALSE, &spotlight2_transform[0][0]);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void CarModel::updateVectors()
{
	right = glm::normalize(glm::vec3(cos(glm::radians(car_angle)), 0.0f, -sin(glm::radians(car_angle))));
	front = glm::normalize(glm::vec3(-sin(glm::radians(car_angle)), 0.0f, -cos(glm::radians(car_angle))));
}

void CarModel::ProcessInputs(GLFWwindow* w, float deltatime)
{
	right = glm::normalize(glm::vec3(cos(glm::radians(car_angle)), 0.0f, -sin(glm::radians(car_angle))));
	front = glm::normalize(glm::vec3(-sin(glm::radians(car_angle)), 0.0f, -cos(glm::radians(car_angle))));
	wheel_angle += wheel_speed * deltatime;

	if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS)
	{
		//z_pos -= movementspeed * deltatime;
		x_pos += front.x * movementspeed * deltatime;
		y_pos += front.y * movementspeed * deltatime;
		z_pos += front.z * movementspeed * deltatime;
		car_angle += wheel_angle_lateral *5.0f* deltatime;
	}

	if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS)
	{
		//z_pos += movementspeed * deltatime;
		x_pos -= front.x * movementspeed * deltatime;
		y_pos -= front.y * movementspeed * deltatime;
		z_pos -= front.z * movementspeed * deltatime;
		car_angle -= wheel_angle_lateral * 5.0f* deltatime;
	}

	if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS)
	{
		//x_pos -= movementspeed * deltatime;
		x_pos -= right.x * movementspeed * deltatime;
		y_pos -= right.y * movementspeed * deltatime;
		z_pos -= right.z * movementspeed * deltatime;
	}

	if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS)
	{
		//x_pos += movementspeed * deltatime;
		x_pos += right.x * movementspeed * deltatime;
		y_pos += right.y * movementspeed * deltatime;
		z_pos += right.z * movementspeed * deltatime;
	}

	if (glfwGetKey(w, GLFW_KEY_Q) == GLFW_PRESS)
	{
		car_angle += 50 * deltatime;
		right = glm::vec3(cos(glm::radians(car_angle)), 0.0f, -sin(glm::radians(car_angle)));
	}

	if (glfwGetKey(w, GLFW_KEY_E) == GLFW_PRESS)
	{
		car_angle -= 50 * deltatime;
		right = glm::vec3(cos(glm::radians(car_angle)), 0.0f, -sin(glm::radians(car_angle)));
	}

	if (glfwGetKey(w, GLFW_KEY_U) == GLFW_PRESS)
	{
		scale += 1.5 * deltatime;
	}

	if (glfwGetKey(w, GLFW_KEY_J) == GLFW_PRESS && scale >= 0.25)
	{
		scale -= 1.5 * deltatime;
	}

	if (glfwGetKey(w, GLFW_KEY_9) == GLFW_PRESS && scale >= 0.25)
	{
		wheel_speed += deltatime * 200;
	}

	if (glfwGetKey(w, GLFW_KEY_0) == GLFW_PRESS && scale >= 0.25)
	{
		wheel_speed -= deltatime * 200;
	}

	if (glfwGetKey(w, GLFW_KEY_7) && wheel_angle_lateral <= 30.0f)
	{
		wheel_angle_lateral += deltatime * 50.0f;
	}

	if (glfwGetKey(w, GLFW_KEY_8) && wheel_angle_lateral >= -30.0f)
	{
		wheel_angle_lateral -= deltatime * 50.0f;
	}

	if (keystate_space == GLFW_RELEASE && glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		x_pos = rand() % 90 - 50; //These values are smaller than the size of the grid so that the car is not "off" the grid
		z_pos = rand() % 90 - 50;
	}
	keystate_space = glfwGetKey(w, GLFW_KEY_SPACE);

	if (glfwGetKey(w, GLFW_KEY_3) == GLFW_PRESS)
	{
		Reset();
	}

}

CarModel::~CarModel()
{
}

void CarModel::Reset()
{
	x_pos = -2.0f;
	y_pos = 0.5f;
	z_pos = -2.0f;
	scale = 1.0f;
	car_angle = -90.0f;
	wheel_angle_lateral = 0.0f;
}

glm::vec3 CarModel::getPosition() const
{
	return glm::vec3(x_pos + 2.0f, y_pos + 0.75f, z_pos + 2.0f);
}

glm::vec3 CarModel::getFront() const
{
	return front;
}

void CarModel::updateMaterial(int shaderprogram)
{
	GLuint material_ambient_location = glGetUniformLocation(shaderprogram, "material.ambient");
	glUniform3f(material_ambient_location, 0.25f, 0.25f, 0.25f);

	GLuint material_diffuse_location = glGetUniformLocation(shaderprogram, "material.diffuse");
	glUniform3f(material_diffuse_location, 0.4f, 0.4f, 0.4f);

	GLuint material_specular_location = glGetUniformLocation(shaderprogram, "material.specular");
	glUniform3f(material_specular_location, 0.774597f, 0.774597f, 0.774597f);

	GLuint material_shininess_location = glGetUniformLocation(shaderprogram, "material.shininess");
	glUniform1f(material_shininess_location, 0.6f*128);
}

void CarModel::updateWheelMaterial(int shaderprogram)
{
	GLuint material_ambient_location = glGetUniformLocation(shaderprogram, "material.ambient");
	glUniform3f(material_ambient_location, 0.12f, 0.12f, 0.12f);

	GLuint material_diffuse_location = glGetUniformLocation(shaderprogram, "material.diffuse");
	glUniform3f(material_diffuse_location, 0.01f, 0.01f, 0.01f);

	GLuint material_specular_location = glGetUniformLocation(shaderprogram, "material.specular");
	glUniform3f(material_specular_location, 0.1f, 0.1f, 0.1f);

	GLuint material_shininess_location = glGetUniformLocation(shaderprogram, "material.shininess");
	glUniform1f(material_shininess_location, 1.0f);
}

glm::vec3 CarModel::getHeadLight1()
{
	glm::mat4 rotation_matrix = glm::mat4(1.0); //Rotation applied to all parts of the car

	//Matrix to rotate and translate car. This matrix first translates to the origin, applies scaling and rotation, followed by a translation back to original position
	//Note that rotation is performed about the CENTRE of the car
	rotation_matrix = glm::translate(rotation_matrix, glm::vec3(x_pos + 2.0f, 0.0f, z_pos + 2.0f));
	rotation_matrix = glm::rotate(rotation_matrix, glm::radians(car_angle), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation_matrix = glm::scale(rotation_matrix, glm::vec3(scale, scale, scale));
	rotation_matrix = glm::translate(rotation_matrix, glm::vec3(x_pos + 2.0f, 0.0f, z_pos + 2.0f)*-1.0f);

	return glm::vec3(rotation_matrix * glm::vec4(x_pos + 1.0f, y_pos + 0.5, z_pos - 2.50f,1.0f));
}

glm::vec3 CarModel::getHeadLight2()
{
	glm::mat4 rotation_matrix = glm::mat4(1.0); //Rotation applied to all parts of the car

	//Matrix to rotate and translate car. This matrix first translates to the origin, applies scaling and rotation, followed by a translation back to original position
	//Note that rotation is performed about the CENTRE of the car
	rotation_matrix = glm::translate(rotation_matrix, glm::vec3(x_pos + 2.0f, 0.0f, z_pos + 2.0f));
	rotation_matrix = glm::rotate(rotation_matrix, glm::radians(car_angle), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation_matrix = glm::scale(rotation_matrix, glm::vec3(scale, scale, scale));
	rotation_matrix = glm::translate(rotation_matrix, glm::vec3(x_pos + 2.0f, 0.0f, z_pos + 2.0f)*-1.0f);

	return glm::vec3(rotation_matrix* glm::vec4(x_pos + 3.0f, y_pos + 0.5, z_pos - 2.50f,1.0));
}

glm::vec3 CarModel::getTailLight1()
{
	glm::mat4 rotation_matrix = glm::mat4(1.0); //Rotation applied to all parts of the car

	//Matrix to rotate and translate car. This matrix first translates to the origin, applies scaling and rotation, followed by a translation back to original position
	//Note that rotation is performed about the CENTRE of the car
	rotation_matrix = glm::translate(rotation_matrix, glm::vec3(x_pos + 2.0f, 0.0f, z_pos + 2.0f));
	rotation_matrix = glm::rotate(rotation_matrix, glm::radians(car_angle), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation_matrix = glm::scale(rotation_matrix, glm::vec3(scale, scale, scale));
	rotation_matrix = glm::translate(rotation_matrix, glm::vec3(x_pos + 2.0f, 0.0f, z_pos + 2.0f)*-1.0f);

	return glm::vec3(rotation_matrix* glm::vec4(x_pos + 1.0f, y_pos + 0.5, z_pos + 6.5f, 1.0));
}

glm::vec3 CarModel::getTailLight2()
{
	glm::mat4 rotation_matrix = glm::mat4(1.0); //Rotation applied to all parts of the car

	//Matrix to rotate and translate car. This matrix first translates to the origin, applies scaling and rotation, followed by a translation back to original position
	//Note that rotation is performed about the CENTRE of the car
	rotation_matrix = glm::translate(rotation_matrix, glm::vec3(x_pos + 2.0f, 0.0f, z_pos + 2.0f));
	rotation_matrix = glm::rotate(rotation_matrix, glm::radians(car_angle), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation_matrix = glm::scale(rotation_matrix, glm::vec3(scale, scale, scale));
	rotation_matrix = glm::translate(rotation_matrix, glm::vec3(x_pos + 2.0f, 0.0f, z_pos + 2.0f)*-1.0f);

	return glm::vec3(rotation_matrix* glm::vec4(x_pos + 3.0f, y_pos + 0.5, z_pos + 6.5f, 1.0));
}

glm::vec3 CarModel::getExhaust()
{
	glm::mat4 rotation_matrix = glm::mat4(1.0); //Rotation applied to all parts of the car

	//Matrix to rotate and translate car. This matrix first translates to the origin, applies scaling and rotation, followed by a translation back to original position
	//Note that rotation is performed about the CENTRE of the car
	rotation_matrix = glm::translate(rotation_matrix, glm::vec3(x_pos + 2.0f, 0.0f, z_pos + 2.0f));
	rotation_matrix = glm::rotate(rotation_matrix, glm::radians(car_angle), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation_matrix = glm::scale(rotation_matrix, glm::vec3(scale, scale, scale));
	rotation_matrix = glm::translate(rotation_matrix, glm::vec3(x_pos + 2.0f, 0.0f, z_pos + 2.0f)*-1.0f);

	return glm::vec3(rotation_matrix* glm::vec4(x_pos + 2.0f, y_pos, z_pos + 6.0f + 0.5f, 1.0));
}

void CarModel::TranslateRandom(float dt)
{
	if (movement == translate)
	{
		translation_timer -= dt;
		if (translation_timer > 0.0f)
		{
			float translation_x = front.x * translation_random * dt;
			float translation_z = front.z * translation_random * dt;
			x_pos = ((x_pos + translation_x) < 40.0f && (x_pos + translation_x) > -40.0f) ? x_pos + translation_x : x_pos;
			z_pos = ((z_pos + translation_z) < 40.0f && (z_pos + translation_z) > -40.0f) ? z_pos + translation_z : z_pos;
			/*x_pos = (-40.0f < x_pos < 40.0f) ? x_pos + front.x * translation_random * dt : x_pos;
			z_pos = (-40.0f < z_pos < 40.0f) ? z_pos + front.z * translation_random * dt : z_pos;*/
		}

		else
		{
			translation_timer = 1.0f; //Reset timer
			translation_random = static_cast<float>(rand() % 21+5);
			movement = rotate;
		}
	}

	else if(movement == rotate)
	{
		rotation_timer -= dt;
		if (rotation_timer > 0.0f)
		{
			car_angle += 5.0f * rotation_random * dt;
		}

		else
		{
			rotation_timer = 1.0f; //Reset timer
			rotation_random = static_cast<float>(rand() % 61 - 30);
			movement = translate;
		}

	}
	
}
