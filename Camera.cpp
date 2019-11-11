#include "pch.h"
#include <iostream>
#include <algorithm>
#include "Camera.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

Camera::Camera(float shaderprogram)
{
	eye = glm::vec3(0.0f, 0.0f, 3.0f);
	target = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	/*glm::mat4 view = glm::lookAt(eye, target, up);
	GLuint view_matrix_location = glGetUniformLocation(shaderprogram, "viewMatrix");
	glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, &view[0][0]);*/
}

Camera::Camera(float shaderprogram, glm::vec3 eye, glm::vec3 target, glm::vec3 up, GLFWwindow* w)
{
	this->eye = eye;
	this->target = target;
	this->up = up;

	/*glm::mat4 view = glm::lookAt(this->eye, this->target + this->eye, this->up);
	GLuint view_matrix_location = glGetUniformLocation(shaderprogram, "viewMatrix");
	glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, &view[0][0]);*/

	glfwGetCursorPos(w, &lastMousePosX, &lastMousePosY);

	eye_backup = eye;
	target_backup = target;
	up_backup = up;
}



Camera::~Camera()
{
}

void Camera::processInputs(GLFWwindow* w, float deltatime, int shaderprogram)
{

	camera_side = glm::normalize(glm::cross(target, up));
	dt = glfwGetTime() - lastframe;
	lastframe += dt;

	if ((glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_LEFT)==GLFW_RELEASE) && (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE))
	{
		glfwGetCursorPos(w, &lastMousePosX, &lastMousePosY);
	}

	if (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_LEFT)) //Pan
	{
		double mousePosX, mousePosY;
		glfwGetCursorPos(w, &mousePosX, &mousePosY);

		double dx = mousePosX - lastMousePosX;
		//double dy = mousePosY - lastMousePosY;

		lastMousePosX = mousePosX;
		lastMousePosY = mousePosY;

		// Convert to spherical coordinates
		//const float x = 60.0f;
		cameraHorizontalAngle -= dx * dt * 5;
		
		//cameraVerticalAngle -= dy * dt * 5;
		std::cout << deltatime << std::endl;

		// Clamp vertical angle to [-85, 85] degrees
		cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));

		if (cameraHorizontalAngle > 360)
		{
			cameraHorizontalAngle -= 360;
		}
		else if (cameraHorizontalAngle < -360)
		{
			cameraHorizontalAngle += 360;
		}

		float theta = glm::radians(cameraHorizontalAngle);
		float phi = glm::radians(cameraVerticalAngle);

		target = glm::vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
		camera_side = glm::cross(target, glm::vec3(0.0f, 1.0f, 0.0f));

		glm::normalize(camera_side);

		/*glm::mat4 view = glm::lookAt(eye, eye + target, up);
		GLuint view_matrix_location = glGetUniformLocation(shaderprogram, "viewMatrix");
		glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, &view[0][0]);*/
	}

	if (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_RIGHT)) //Tilt
	{
		double mousePosX, mousePosY;
		glfwGetCursorPos(w, &mousePosX, &mousePosY);

		//double dx = mousePosX - lastMousePosX;
		double dy = mousePosY - lastMousePosY;

		lastMousePosX = mousePosX;
		lastMousePosY = mousePosY;

		// Convert to spherical coordinates
		//const float x = 60.0f;
		//cameraHorizontalAngle -= dx * dt * 5;

		cameraVerticalAngle -= dy * dt * 5;
		std::cout << deltatime << std::endl;

		// Clamp vertical angle to [-85, 85] degrees
		cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));

		if (cameraHorizontalAngle > 360)
		{
			cameraHorizontalAngle -= 360;
		}
		else if (cameraHorizontalAngle < -360)
		{
			cameraHorizontalAngle += 360;
		}

		float theta = glm::radians(cameraHorizontalAngle);
		float phi = glm::radians(cameraVerticalAngle);

		target = glm::vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
		camera_side = glm::cross(target, glm::vec3(0.0f, 1.0f, 0.0f));

		glm::normalize(camera_side);

	/*	glm::mat4 view = glm::lookAt(eye, eye + target, up);
		GLuint view_matrix_location = glGetUniformLocation(shaderprogram, "viewMatrix");
		glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, &view[0][0]);*/
	}

	if (glfwGetKey(w, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		eye -= camera_side * 10.0f * deltatime;
	}

	if (glfwGetKey(w, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		eye += camera_side * 10.0f * deltatime;
	}

	if (glfwGetKey(w, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		eye -= up * 10.0f * deltatime;
	}

	if (glfwGetKey(w, GLFW_KEY_UP) == GLFW_PRESS)
	{
		eye += up * 10.0f * deltatime;
	}

	if (glfwGetKey(w, GLFW_KEY_1) == GLFW_PRESS)
	{
		eye += target * 10.0f * deltatime;
	}

	if (glfwGetKey(w, GLFW_KEY_2) == GLFW_PRESS)
	{
		eye -= target * 10.0f * deltatime;
	}

	if (glfwGetKey(w, GLFW_KEY_R) == GLFW_PRESS)
	{
		target.y += 2.0f * deltatime;
	}

	if (glfwGetKey(w, GLFW_KEY_F) == GLFW_PRESS)
	{
		target.y -= 2.0f * deltatime;
	}


	if ((glfwGetKey(w, GLFW_KEY_4) == GLFW_PRESS) && (keystate_4 == GLFW_RELEASE))
	{
		//followcar = !followcar;
		switchCameraPerspective();
	}
	keystate_4 = glfwGetKey(w, GLFW_KEY_4);

	if (glfwGetKey(w, GLFW_KEY_TAB))
	{
		//followcar = !followcar;
		cameramode = regular;
	}

	/*glm::mat4 view = glm::lookAt(eye, eye + target, up);
	GLuint view_matrix_location = glGetUniformLocation(shaderprogram, "viewMatrix");
	glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, &view[0][0]);*/
}

void Camera::Reset()
{
	eye = eye_backup;
	target = target_backup;
	up = up_backup;

	cameraHorizontalAngle = 90.0f;
	cameraVerticalAngle = 0.0f;

	followcar = false;
}

void Camera::FollowCar(CarModel car)
{
	if (cameramode == firstperson)
	{
		eye = car.getPosition() - 9.0f * car.getFront() + glm::vec3(0.0f,4.0f,0.0f);
		target = car.getFront() - glm::vec3(0.0f,0.5f,0.0f);
	}

	else if (cameramode == thirdperson)
	{
		eye = car.getPosition() + 4.0f * car.getFront() + glm::vec3(0.0f, 1.0f, 0.0f);
		target = car.getFront();
	}
}

glm::mat4 Camera::getviewMatrix() const
{
	return glm::lookAt(eye, eye + target, up);
}

glm::vec3 Camera::getPosition() const
{
	return eye;
}

void Camera::switchCameraPerspective()
{
	cameramode = (cameramode == firstperson) ? thirdperson : firstperson;
}

glm::vec3 Camera::getTarget() const
{
	return target;
}
