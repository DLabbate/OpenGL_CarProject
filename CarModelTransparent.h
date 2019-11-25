#pragma once
#include "CarModel.h"
class CarModelTransparent :
	public CarModel
{
public:
	CarModelTransparent();
	CarModelTransparent(glm::vec3);
	CarModelTransparent(CarModel&);
	~CarModelTransparent();
	void Update(float,int shaderpgroam);
	bool checkExpired();

protected:
	float lifetime = 0.35f;
	float age = 0.0f;
	bool isExpired = false;

	float opacity;
	float opacity_final = 0.0f;
	float opacity_initial = 0.015f;
};

