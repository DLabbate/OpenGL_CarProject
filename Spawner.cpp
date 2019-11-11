#include "pch.h"
#include "Spawner.h"


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
	for (std::list<Quad*>::iterator it = smokeparticles.begin(); it != smokeparticles.end(); ++it)
	{
		(*it)->Update(dt);
		if ((*it)->checkExpired() == true)
		{
			it = smokeparticles.erase(it);
			//Erase after life duration is reached
		}
	}
}

void Spawner::updateRotation(Camera camera)
{
	glm::vec3 cameraLookAt = camera.getTarget();
	glm::vec3 originalAxis(0, 0, -1);

	cameraLookAt.y = 0; //project onto xz plane
	cameraLookAt.x *= -1;
	cameraLookAt = normalize(cameraLookAt);

	float billboardRotation = glm::acos(-1 * dot(originalAxis, cameraLookAt)) * 360 / (2 * glm::pi<float>());

	if (cameraLookAt.x > 0) 
	{
		billboardRotation = 360 - billboardRotation;
	}


	for (std::list<Quad*>::iterator it = smokeparticles.begin(); it != smokeparticles.end(); ++it)
	{
		(*it)->updateRotation(billboardRotation);
	}
}

