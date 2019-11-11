#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
class CarModel
{
public:
	CarModel();
	CarModel(float, float, float);
	~CarModel();
	void DrawCar(int, int,float,unsigned int);
	void ProcessInputs(GLFWwindow*, float);
	void Reset();
	glm::vec3 getPosition() const;
	glm::vec3 getFront() const;
	void generateBodyVAO();
	void generateWheelVAO();
	void updateMaterial(int);
	void updateWheelMaterial(int);
	glm::vec3 getHeadLight1();
	glm::vec3 getHeadLight2();
	glm::vec3 getTailLight1();
	glm::vec3 getTailLight2();
	glm::vec3 getExhaust();
	void updateVectors();

	
	void TranslateRandom(float);

private:
	GLuint vao_body;
	GLuint vao_wheel;
	float x_pos;
	float y_pos;
	float z_pos;
	float movementspeed = 20.0f;
	float car_angle = -90.0f; //With respect to y-axis
	float scale = 1.0f;
	float wheel_angle = 0.0f;
	float wheel_angle_lateral = 0.0f;
	float wheel_speed = 55.0f;
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

	int keystate_space = GLFW_RELEASE;
	int keystate_PgUp = GLFW_RELEASE;
	bool texture;


	float translation_timer = 1.0f;
	float rotation_timer = 1.0f;
	float translation_random = static_cast<float>(rand() % 21+5);
	float rotation_random = static_cast<float>(rand() % 61 - 30);
	enum MovementType {translate,rotate};
	MovementType movement = translate;
};

