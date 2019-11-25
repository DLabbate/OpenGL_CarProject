#include "pch.h"
#include "Collider.h"


Collider::Collider()
{
}


Collider::~Collider()
{
}

glm::vec3 Collider::checkCollision(ColliderBox box1, ColliderBox box2)
{
	glm::mat4 T1 = box1.getTransform();
	std::vector<glm::vec3> pointlist_1 = box1.getPoints();
	std::vector<glm::vec3> pointlist_1_cubespace;

	glm::mat4 T2 = box2.getTransform();
	std::vector<glm::vec3> pointlist_2 = box2.getPoints();
	std::vector<glm::vec3> pointlist_2_cubespace;


	//Get points of second box in Space 1
	for (std::vector<glm::vec3>::iterator it = pointlist_2.begin(); it != pointlist_2.end(); ++it)
	{
		pointlist_2_cubespace.push_back( inverse(T1) * glm::vec4((*it),1.0f) );
	}

	for (std::vector<glm::vec3>::iterator it = pointlist_2_cubespace.begin(); it != pointlist_2_cubespace.end(); ++it)
	{
		glm::vec3 point = (*it);

		if ( (point.x > -0.5f) && (point.x < 0.5f) && (point.y > -0.5f) && (point.y < 0.5f) && (point.z > -0.5f) && (point.z < 0.5f))
		{
			std::cout << "Collision!" << std::endl;
			//return true;
			//std::cout << glm::vec3(T1*glm::vec4(point, 1.0f)).x;
			//glm::vec3 translation = (T1*glm::vec4(point, 1.0f));
			//return glm::vec3(-translation.x, 0.0f, -translation.z);
			//std::cout << box1.getCentre().x;
			return glm::vec3(box1.getCentre().x - box2.getCentre().x,0.0f, box1.getCentre().z - box2.getCentre().z);
		}

		else
		{
			continue;
		}
	}

	//Get points of first box in Space 2
	for (std::vector<glm::vec3>::iterator it = pointlist_1.begin(); it != pointlist_1.end(); ++it)
	{
		pointlist_1_cubespace.push_back(inverse(T2) * glm::vec4((*it), 1.0f));
	}

	for (std::vector<glm::vec3>::iterator it = pointlist_1_cubespace.begin(); it != pointlist_1_cubespace.end(); ++it)
	{
		glm::vec3 point = (*it);

		if ((point.x > -0.5f) && (point.x < 0.5f) && (point.y > -0.5f) && (point.y < 0.5f) && (point.z > -0.5f) && (point.z < 0.5f))
		{
			std::cout << "Collision!" << std::endl;
			//return true;
			//glm::vec3 translation = (T2*glm::vec4(point, 1.0f));
			//return glm::vec3(translation.x, 0.0f, translation.z);
			return glm::vec3(box1.getCentre().x - box2.getCentre().x, 0.0f, box1.getCentre().z - box2.getCentre().z);
		}

		else
		{
			continue;
		}
	}

	
	return glm::vec3(0.0f, 0.0f, 0.0f);


}

void Collider::checkCollisionCar(CarModel& car1, CarModel& car2,float dt)
{
	std::vector<ColliderBox> collisionboxes_1 = car1.getCollisionBoxes();
	std::vector<ColliderBox> collisionboxes_2 = car2.getCollisionBoxes();

	//collisionboxes_1 = car1.getCollisionBoxes();
	//collisionboxes_2 = car2.getCollisionBoxes();

	/*for (std::vector<ColliderBox>::iterator it1 = collisionboxes_1.begin(); it1 != collisionboxes_1.end(); ++it1)
		for (std::vector<ColliderBox>::iterator it2 = collisionboxes_2.begin(); it2 != collisionboxes_2.end(); ++it2)
		{
			if ( *it1 != *it2)
			{
				checkCollision(*it1, *it2);
			}
		}*/

	for (unsigned int i = 0; i<collisionboxes_1.size(); i++)
		for (unsigned int j = 0; j < collisionboxes_2.size(); j++)
		{
			glm::vec3 trans = (checkCollision(collisionboxes_1[i],collisionboxes_2[j]));
			if (trans != glm::vec3(0.0f,0.0f,0.0f))
			{
				int x = rand() % 2;
				//car1.Translate(trans,dt);
				if (x == 0)
				{
					car1.GetsHit(glm::normalize(trans));
				}

				else
				{
					car2.GetsHit(glm::normalize(-trans));
				}
				
				return;
				//std::cout << trans.x << std::endl;
			}
		}


}

