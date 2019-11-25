#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "ColliderBox.h"
class CarModel
{
public:
	CarModel();
	CarModel(float, float, float);
	CarModel(glm::vec3);
	~CarModel();
	void DrawCar(int, int,float,unsigned int);
	void ProcessInputs(GLFWwindow*, float);
	void Reset();
	glm::vec3 getPosition() const; //Centred of the car
	glm::vec3 getPos() const; //x,y,z
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
	float getCarAngle();
	
	void TranslateRandom(float);
	
	void UpdateTransformations();
	void UpdateCollisionBoxes();
	std::vector<ColliderBox> getCollisionBoxes();

	void Translate(glm::vec3,float);
	void GetsHit(glm::vec3);
	void setRandomMovement(bool);
	glm::vec3 getCollisionVelocity() const;

protected:
	GLuint vao_body;
	GLuint vao_wheel;
	float x_pos;
	float y_pos;
	float z_pos;
	float movementspeed = 12.0f;
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


	float translation_timer = (static_cast<float> (rand()) / static_cast<float>(RAND_MAX)) *2.0f+1; //Random Timer
	float rotation_timer = (static_cast<float> (rand()) / static_cast<float>(RAND_MAX)) *2.0f+1;
	//float translation_random = static_cast<float>(rand() % 21+5);
	float rotation_random = rand() % 2;
	enum MovementType {translate,rotate,hit,manual};
	MovementType movement = translate;
	float collision_timer = 0.0f;
	glm::vec3 collision_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	bool move_random = true;

	glm::mat4 wheel1_transform;
	glm::mat4 wheel2_transform;
	glm::mat4 wheel3_transform;
	glm::mat4 wheel4_transform;
	glm::mat4 trunk_transform;
	glm::mat4 bonnet_transform;
	glm::mat4 body_transform;

	std::vector<ColliderBox> collision_boxes;
};

