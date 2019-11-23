#include "pch.h"
#include "ColliderBox.h"


ColliderBox::ColliderBox()
{
	for (unsigned int i = 0; i < 8; i++)
	{
		points.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	}
		
	//points[0] = glm::vec3(0.5f, 0.5f, 0.5f);
	//std::cout << (double)points[0].x;
}


ColliderBox::~ColliderBox()
{
}

void ColliderBox::UpdatePoints()
{
	points[0] = glm::vec3(transform * glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	points[1] = glm::vec3(transform * glm::vec4(0.5f, 0.5f, -0.5f, 1.0f));
	points[2] = glm::vec3(transform * glm::vec4(0.5f, -0.5f, 0.5f, 1.0f));
	points[3] = glm::vec3(transform * glm::vec4(0.5f, -0.5f, -0.5f, 1.0f));
	points[4] = glm::vec3(transform * glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f));
	points[5] = glm::vec3(transform * glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f));
	points[6] = glm::vec3(transform * glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f));
	points[7] = glm::vec3(transform * glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f));
}

void ColliderBox::Update(glm::mat4 newtransform)
{
	transform = newtransform;
	UpdatePoints();
	UpdateCentre();
}

glm::mat4 ColliderBox::getTransform()
{
	return transform;
}

std::vector<glm::vec3> ColliderBox::getPoints()
{
	return points;
}

void ColliderBox::UpdateCentre()
{
	centre = glm::vec3(transform * glm::vec4(0.0f,0.0f,0.0f,1.0f));
}

glm::vec3 ColliderBox::getCentre()
{
	return centre;
}