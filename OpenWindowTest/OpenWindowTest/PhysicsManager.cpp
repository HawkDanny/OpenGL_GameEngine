#include "PhysicsManager.h"



PhysicsManager::PhysicsManager()
{
}


PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::updatePerpetualPhysics(std::string key)
{
	if (collection_players[key] != nullptr)
	{
		applyDrag(collection_players[key]);
	}
	else if (collection_scenery[key] != nullptr)
	{
		//call functions that pertain to scenery
	}
}


#pragma region PrivatePhysicsMethods

void PhysicsManager::applyForce(Object* o, glm::vec3 force)
{
	//multiply force by deltaTime?
	o->rb.acceleration += (force / o->rb.mass);
}

void PhysicsManager::applyDrag(Object* o, float dragCoefficient)
{
	float speed = glm::distance(o->rb.velocity, glm::vec3(0, 0, 0)); //Set the speed to the magnitude
	float dragMagnitude = dragCoefficient * speed * speed;
	glm::vec3 drag = { o->rb.velocity.x, o->rb.velocity.y, o->rb.velocity.z };
	drag *= -1;
	glm::normalize(drag);
	drag *= dragMagnitude;

	applyForce(o, drag);
}

//call only when the player is airborn
//void PhysicsManager::applyGravity(Object* o)
//{

//}

#pragma endregion

