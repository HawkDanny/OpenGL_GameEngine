#include "ShaderManager.h"

enum Collider { colliderless, aabb, sphere };

struct Vertex
{
	glm::vec3 location;
	glm::vec2 uv;
	glm::vec3 normal;

	Vertex(glm::vec3 loc, glm::vec2 uv, glm::vec3 norm)
	{
		location = loc;
		this->uv = uv;
		normal = norm;
	}

	Vertex() {}
};

struct Transform
{
	glm::vec3 location;
	glm::vec3 rotation;
	glm::vec3 size;
	glm::mat4 objectWorldTransform;
};

struct RigidBody
{
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 force;
	float mass;
	float radius;
};

struct Object
{
	std::string objFile; //filepath for the model
	std::string textureFile; //filepath to the texture used for the model
	Transform objectTransform;
	char* filename;
	RigidBody rb;
	Collider collider;
};

class PhysicsManager
{
private:
	//Apply the given force to the object
	void applyForce(Object* o, glm::vec3 force);
	void applyDrag(Object* o, float dragCoefficient = 0.1);
	//void applyGravity(Object* o);

public:
	PhysicsManager();
	~PhysicsManager();

	//Time
	float previousFrameTime;
	float currentFrameTime;
	float deltaTime;

	//collections of different objects
	std::map<const std::string, Object*> collection_players;
	std::map<const std::string, Object*> collection_scenery;

	//Iterates through all objects and updates physics where appropriate
	void updatePerpetualPhysics(std::string key);
};

