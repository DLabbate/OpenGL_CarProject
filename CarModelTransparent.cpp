#include "pch.h"
#include "CarModelTransparent.h"


CarModelTransparent::CarModelTransparent()
{
}

CarModelTransparent::CarModelTransparent(CarModel& car): CarModel(car)
{
	//calls copy constructor
	//this->x_pos = car.getPos().x;
	//this->y_pos = car.getPos().y;
	////std::cout << y_pos<<std::endl;
	//this->z_pos = car.getPos().z;
	//this->car_angle = car.getCarAngle();
}


CarModelTransparent::~CarModelTransparent()
{
}

CarModelTransparent::CarModelTransparent(glm::vec3 pos) : CarModel(pos.x, pos.y, pos.z)
{

}

bool CarModelTransparent::checkExpired()
{
	return isExpired;
}

void CarModelTransparent::Update(float dt, int shaderprogram)
{
	age += dt;

	if (age > lifetime)
	{
		isExpired = true;
	}

	float interpolation_parameter = (age / lifetime); //linear
	opacity = opacity_initial * (1 - interpolation_parameter) + opacity_final * interpolation_parameter;

	glUseProgram(shaderprogram);
	GLuint opacity_location = glGetUniformLocation(shaderprogram, "opacity");
	glUniform1f(opacity_location, opacity);
}
