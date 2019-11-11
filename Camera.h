#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "CarModel.h"
class Camera
{
public:
	Camera(float);
	Camera(float, glm::vec3, glm::vec3, glm::vec3, GLFWwindow*);
	~Camera();
	void processInputs(GLFWwindow*, float, int);
	void Reset();
	void FollowCar(CarModel);
	glm::mat4 getviewMatrix() const;
	glm::vec3 getPosition() const;
	void switchCameraPerspective();
	void getCameraPerspective();
	glm::vec3 getTarget() const;


private:
	glm::vec3 eye;		//Camera Position
	glm::vec3 target;	//What camera is looking at
	glm::vec3 up;		//Up vector

	glm::vec3 camera_side;
	float zoom;

	float cameraHorizontalAngle = 90.0f;
	float cameraVerticalAngle = 0.0f;

	double lastMousePosX;
	double lastMousePosY;

	glm::vec3 eye_backup; //These values keep the original settings
	glm::vec3 target_backup;
	glm::vec3 up_backup;

	double lastframe = 0;
	double dt;

	bool followcar = false;
	int keystate_4 = GLFW_RELEASE;
	enum CameraMode { firstperson, thirdperson, regular };
	CameraMode cameramode = firstperson;
};

