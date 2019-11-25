#include "pch.h"
#include "Spawner.h"
#include "CarModel.h"
#include "CarModelTransparent.h"


Spawner::Spawner()
{
}


Spawner::~Spawner()
{
}

void Spawner::spawnSmoke(float dt)
{
	spawn_timer += dt;

	if (spawn_timer > spawn_interval)
	{
		smokeparticles.push_back(new Quad());
		spawn_timer -= spawn_interval; //Reset to 0seconds
	}
}


void Spawner::spawnSmoke(float dt,glm::vec3 position)
{
	spawn_timer += dt;

	if (spawn_timer > spawn_interval)
	{
		smokeparticles.push_back(new Quad(position));
		spawn_timer -= spawn_interval; //Reset to 0seconds
	}

}

void Spawner::drawSmoke(int shaderprogram,float dt)
{
	for (std::list<Quad*>::iterator it = smokeparticles.begin(); it != smokeparticles.end(); ++it)
	{
		(*it)->Draw(shaderprogram);
	}
}

void Spawner::updateSmoke(float dt)
{
	for (std::list<Quad*>::iterator it = smokeparticles.begin(); it != smokeparticles.end(); )
	{
		(*it)->Update(dt);
		if ((*it)->checkExpired() == true)
		{
			it = smokeparticles.erase(it);
			//Erase after life duration is reached
		}
		else
		{
			++it;
		}
	}
}

void Spawner::updateRotation(Camera camera)
{
	glm::vec3 cameraLookAt = camera.getTarget();
	glm::vec3 originalAxis(0, 0, 1);

	cameraLookAt.y = 0; //project onto xz plane
	//cameraLookAt.x *= -1;
	cameraLookAt = normalize(cameraLookAt);

	float billboardRotation = glm::acos(-dot(originalAxis, cameraLookAt)) * 360 / (2 * glm::pi<float>());
	billboardRotation += 180;

	if (cameraLookAt.x > 0) 
	{
		billboardRotation = 360 - billboardRotation;
	}


	for (std::list<Quad*>::iterator it = smokeparticles.begin(); it != smokeparticles.end(); ++it)
	{
		(*it)->updateRotation(billboardRotation);
	}
}


void Spawner::drawCars(int shaderprogram_car,int shaderprogram_wheels,float deltatime,unsigned int wheel_texture)
{
	//if (cars.size() >= 4)
	{
		for (std::list<CarModelTransparent*>::iterator it = cars.begin(); it != cars.end(); ++it)
		{
			(*it)->DrawCar(shaderprogram_car, shaderprogram_wheels, deltatime, wheel_texture);
		}
	}
}

void Spawner::updateCars(float dt,int shaderprogram)
{
	//if (cars.size() >= 4)
	{
		for (std::list<CarModelTransparent*>::iterator it = cars.begin(); it != cars.end(); )
		{
			(*it)->Update(dt, shaderprogram);
			if ((*it)->checkExpired() == true)
			{
				it = cars.erase(it);
				//Erase after life duration is reached
			}

			else
			{
				++it; //we don't want to increment when we erase, because it already increments
			}
		}
	}
}


void Spawner::spawnCars(float dt, CarModel& car)
{
	//CarModel c(car);
	spawn_timer_car += dt;
	if ((spawn_timer_car > spawn_interval_car) && (car.getCollisionVelocity() != glm::vec3(0.0f, 0.0f, 0.0f)))
	{
		cars.push_back(new CarModelTransparent(car));
		spawn_timer_car -= spawn_interval_car; //Reset to 0seconds
	}

}

