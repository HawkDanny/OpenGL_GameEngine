#include "Camera.h"

using std::cout;
using std::cin;
using std::endl;

class Engine
{
private:
	GLFWwindow* GLFWwindowPtr;
	ShaderManager sm;
	PhysicsManager pm;
	Camera cam;
	glm::vec3 lightLocation;
	bool noBullet;
	bool noEnemy1;
	bool noEnemy2;
	int shipListingCounter;

public:
	Engine();
	~Engine();
	GLuint textureIDs[3];
	std::vector<Object> objects;
	std::vector<Model> models;
	float previousFrameTime;
	float currentFrameTime;
	float deltaTime;
	

	bool init();
	bool bufferModels();
	bool gameLoop();
	bool useShaders();
	bool loadTextures(char* fileName, GLuint texID);
	void hardCodeObjectValues();
	void populateEnemies(unsigned int numEnemies);
	void updateTime();
	void calcPhysics(Object& o);
	void applyDrag(Object& o);
	bool detectCollision(const Object& o1, const Object& o2);
};