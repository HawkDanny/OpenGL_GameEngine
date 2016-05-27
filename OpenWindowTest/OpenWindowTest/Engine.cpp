#include "Engine.h"

namespace {
	std::map<int, bool> keyIsDown;
	std::map<int, bool> keyWasDown;

	void mouseClick(GLFWwindow * windowPtr, int button, int action, int mods)
	{
		keyIsDown[button] = action;
	}

	void keyCallback(GLFWwindow *windowPtr, int key, int scancode, int action, int mods)
	{
		keyIsDown[key] = action;
	}
}


Engine::Engine()
{
}


Engine::~Engine()
{
	for (int i = 0; i < 3; i++)
		glDeleteTextures(1, &textureIDs[i]);
}

bool Engine::init()
{
	if (glfwInit() == GL_FALSE)
		return false;

	this->GLFWwindowPtr = glfwCreateWindow(800, 600, "Danny Hawk's DSA1 Engine", NULL, NULL);
	if (GLFWwindowPtr != nullptr)
		glfwMakeContextCurrent(GLFWwindowPtr);
	else
	{
		glfwTerminate();
		return false;
	}

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return false;
	}


	glfwSetMouseButtonCallback(GLFWwindowPtr, mouseClick);
	glfwSetKeyCallback(GLFWwindowPtr, keyCallback);


	//create camera
	cam = Camera();

	noBullet = true;
	noEnemy1 = false;
	noEnemy2 = false;
	shipListingCounter = 0;

	//enable wire-frame rendering
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//enable depth testing
	glEnable(GL_DEPTH_TEST);

	//disable cursor
	glfwSetInputMode(GLFWwindowPtr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	return true;
}

bool Engine::loadTextures(char* fileName, GLuint texID)
{

	//Transform tempTransform;
	Object tempObject;
	//tempObject.objectTransform = tempTransform;
	tempObject.filename = fileName;
	objects.push_back(tempObject);

	//add a model that corresponds to the loaded texture's object
	models.push_back(Model());


	FIBITMAP* image = FreeImage_Load(FreeImage_GetFileType(fileName, 0), fileName);
	if (image == nullptr)
		return false;

	FIBITMAP* image32Bit = FreeImage_ConvertTo32Bits(image); //Save as a 32 bit bmp
	FreeImage_Unload(image);

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	unsigned int imageWidth = FreeImage_GetWidth(image32Bit);
	unsigned int imageHeight = FreeImage_GetHeight(image32Bit);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32Bit));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	FreeImage_Unload(image32Bit);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

//Temporary method
void Engine::hardCodeObjectValues()
{
	//enemy 1
	objects[0].objFile = "models/enemy1.obj";
	models[0].tag = "models/enemy1.obj";
	objects[0].objectTransform.location = glm::vec3(0, 0.5, 0);
	objects[0].objectTransform.rotation = glm::vec3(glm::half_pi<float>(), 0, 0);
	objects[0].objectTransform.size = glm::vec3(0.03, 0.03, 0.03);
	objects[0].rb.mass = 8;
	objects[0].rb.velocity = glm::vec3(0, 0, 0);
	objects[0].rb.force = glm::vec3(0, 0, 0);
	objects[0].collider = sphere;
	objects[0].rb.radius = 0.03;

	//enemy 2
	objects[1].objFile = "models/enemy2.obj";
	models[1].tag = "models/enemy2.obj";
	objects[1].objectTransform.location = glm::vec3(0, 0, 0);
	objects[1].objectTransform.rotation = glm::vec3(glm::half_pi<float>(), 0, 0);
	objects[1].objectTransform.size = glm::vec3(0.03, 0.03, 0.03);
	objects[1].rb.mass = 8;
	objects[1].rb.velocity = glm::vec3(0, 0, 0);
	objects[1].rb.force = glm::vec3(0, 0, 0);
	objects[1].collider = sphere;
	objects[1].rb.radius = 0.03;

	objects[2].objFile = "models/player.obj";
	models[2].tag = "models/player.obj";
	objects[2].objectTransform.location = glm::vec3(0, -0.75, 0);
	//objects[2].objectTransform.rotation = glm::vec3(glm::half_pi<float>(), 0, 0);
	objects[2].objectTransform.size = glm::vec3(0.15, 0.15, 0.03);
	objects[2].rb.mass = 8;
	objects[2].rb.velocity = glm::vec3(0, 0, 0);
	objects[2].rb.force = glm::vec3(0, 0, 0);
	objects[2].collider = sphere;

	objects[3].objFile = "models/bullet.obj";
	models[3].tag = "models/bullet.obj";
	objects[3].objectTransform.location = glm::vec3(0, -1, 0);
	objects[3].objectTransform.rotation = glm::vec3(glm::half_pi<float>(), 0, 0);
	objects[3].objectTransform.size = glm::vec3(0.05, 0.05, 0.05);
	objects[3].rb.mass = 8;
	objects[3].rb.velocity = glm::vec3(0, 0, 0);
	objects[3].rb.force = glm::vec3(0, 0, 0);
	objects[3].collider = sphere;
	objects[3].rb.radius = 0.03;

	//objects[0].objectTransform.location = glm::vec3(0.5, -0.5, 0);
	//objects[0].objectTransform.rotation = glm::vec3(glm::half_pi<float>(), 0, 0);
	//objects[0].objectTransform.size = glm::vec3(0.2, 0.2, 0.2);
	//objects[0].rb.mass = 8;
	//objects[0].rb.velocity = glm::vec3(0, 0, 0);
	//objects[0].rb.force = glm::vec3(0, 0, 0);
	//objects[0].collider = aabb;
	//
	////grass
	//objects[1].objectTransform.location = glm::vec3(-1, 0, 0);
	//objects[1].objectTransform.rotation = glm::vec3(0, 0, 0);
	//objects[1].objectTransform.size = glm::vec3(0.2, 0.2, 0.2);
	//objects[1].rb.mass = 4;
	//objects[1].rb.velocity = glm::vec3(0, 0, 0);
	//objects[1].rb.force = glm::vec3(0, 0, 0);
	//objects[1].collider = colliderless;
	//
	////goat
	//objects[2].objectTransform.location = glm::vec3(0, -0.75, 0);
	//objects[2].objectTransform.rotation = glm::vec3(0, 0, 0);
	//objects[2].objectTransform.size = glm::vec3(0.2, 0.2, 0.2);
	//objects[2].rb.mass = 2;
	//objects[2].rb.radius = 0.27;
	//objects[2].rb.velocity = glm::vec3(0, 0, 0);
	//objects[2].rb.force = glm::vec3(0, 0, 0);
	//objects[2].collider = aabb;
}

void Engine::populateEnemies(unsigned int numEnemies)
{
	//if (numEnemies > MAX_VALUE_OF_ENEMIES)
	//numEnemies = MAX_VALUE_OF_ENEMIES;


	//Each loop creates one enemy
	for (int i = 0; i < numEnemies; i++)
	{

	}
}

void Engine::updateTime()
{
	//Update the time values here
	previousFrameTime = currentFrameTime;
	currentFrameTime = glfwGetTime();
	deltaTime = currentFrameTime - previousFrameTime;
}

void Engine::calcPhysics(Object& o)
{
	o.objectTransform.location += o.rb.velocity;
}

void Engine::applyDrag(Object& o)
{
	glm::vec3 dragAccelerationVec = glm::vec3(0.75, 0.75, 0.75);
	o.rb.velocity *= dragAccelerationVec;
}

bool Engine::detectCollision(const Object& o1, const Object& o2)
{
	//Both spheres
	if ((o1.collider == sphere) && (o2.collider == sphere))
	{
		float sum = o1.rb.radius + o2.rb.radius;
		if ((sum * sum) < ((o1.objectTransform.location.x - o2.objectTransform.location.x) * (o1.objectTransform.location.x - o2.objectTransform.location.x) + (o1.objectTransform.location.y - o2.objectTransform.location.y) * (o1.objectTransform.location.y - o2.objectTransform.location.y) + (o1.objectTransform.location.z - o2.objectTransform.location.z) * (o1.objectTransform.location.z - o2.objectTransform.location.z)))
			return true;

		return false;
	}

	//Both aabb
	else if ((o1.collider == aabb) && (o2.collider == aabb))
	{
		float o1_minX, o1_minY, o1_minZ, o1_maxX, o1_maxY, o1_maxZ, o2_minX, o2_minY, o2_minZ, o2_maxX, o2_maxY, o2_maxZ;
		float o1_halfWidth, o1_halfHeight, o1_halfDepth, o2_halfWidth, o2_halfHeight, o2_halfDepth;
		o1_halfWidth = o1.objectTransform.size.x / 2;
		o1_halfHeight = o1.objectTransform.size.y / 2;
		o1_halfDepth = o1.objectTransform.size.z / 2;
		o2_halfWidth = o2.objectTransform.size.x / 2;
		o2_halfHeight = o2.objectTransform.size.y / 2;
		o2_halfDepth = o2.objectTransform.size.z / 2;

		o1_minX = o1.objectTransform.location.x - o1.objectTransform.size.x;
		o1_maxX = o1.objectTransform.location.x + o1.objectTransform.size.x;
		o1_minY = o1.objectTransform.location.y - o1.objectTransform.size.y;
		o1_maxY = o1.objectTransform.location.y + o1.objectTransform.size.y;
		o1_minZ = o1.objectTransform.location.z - o1.objectTransform.size.z;
		o1_maxZ = o1.objectTransform.location.z + o1.objectTransform.size.z;

		o2_minX = o2.objectTransform.location.x - o2.objectTransform.size.x;
		o2_maxX = o2.objectTransform.location.x + o2.objectTransform.size.x;
		o2_minY = o2.objectTransform.location.y - o2.objectTransform.size.y;
		o2_maxY = o2.objectTransform.location.y + o2.objectTransform.size.y;
		o2_minZ = o2.objectTransform.location.z - o2.objectTransform.size.z;
		o2_maxZ = o2.objectTransform.location.z + o2.objectTransform.size.z;


		if (o2_minX > o1_maxX)
			return false;
		else if (o2_maxX < o1_minX)
			return false;
		else if (o2_minY > o1_maxY)
			return false;
		else if (o2_maxY < o1_minY)
			return false;
		else if (o2_minZ > o1_maxZ)
			return false;
		else if (o2_maxZ < o1_minZ)
			return false;

		return true;
	}

	//aabb and sphere
	else if ((o1.collider == aabb) && (o2.collider == sphere))
	{
		float distanceSquared = 0;
		float o1_minX, o1_minY, o1_minZ, o1_maxX, o1_maxY, o1_maxZ;
		float o1_halfWidth, o1_halfHeight, o1_halfDepth;
		o1_halfWidth = o1.objectTransform.size.x / 2;
		o1_halfHeight = o1.objectTransform.size.y / 2;
		o1_halfDepth = o1.objectTransform.size.z / 2;

		o1_minX = o1.objectTransform.location.x - o1_halfWidth;
		o1_maxX = o1.objectTransform.location.x + o1_halfWidth;
		o1_minY = o1.objectTransform.location.y - o1_halfHeight;
		o1_maxY = o1.objectTransform.location.y + o1_halfHeight;
		o1_minZ = o1.objectTransform.location.z - o1_halfDepth;
		o1_maxZ = o1.objectTransform.location.z + o1_halfDepth;


		//Calculate distance squared first, in order to use it in checks
		if (o2.objectTransform.location.x < o1_minX)
			distanceSquared += (o1_minX - o2.objectTransform.location.x) * (o1_minX - o2.objectTransform.location.x);
		else if (o2.objectTransform.location.x > o1_maxX)
			distanceSquared += (o2.objectTransform.location.x - o1_maxX) * (o2.objectTransform.location.x - o1_maxX);
		if (o2.objectTransform.location.y < o1_minY)
			distanceSquared += (o1_minY - o2.objectTransform.location.y) * (o1_minY - o2.objectTransform.location.y);
		else if (o2.objectTransform.location.y > o1_maxY)
			distanceSquared += (o2.objectTransform.location.y - o1_maxY) * (o2.objectTransform.location.y - o1_maxY);
		if (o2.objectTransform.location.z < o1_minZ)
			distanceSquared += (o1_minZ - o2.objectTransform.location.z) * (o1_minZ - o2.objectTransform.location.z);
		else if(o2.objectTransform.location.z > o1_maxZ)
			distanceSquared += (o2.objectTransform.location.z - o1_maxZ) * (o2.objectTransform.location.z - o1_maxZ);


		if (distanceSquared < o2.rb.radius * o2.rb.radius)
			return true;

		return false;
	}
	else if ((o1.collider == sphere) && (o2.collider == aabb))
	{
		float distanceSquared = 0;
		float o2_minX, o2_minY, o2_minZ, o2_maxX, o2_maxY, o2_maxZ;
		float o2_halfWidth, o2_halfHeight, o2_halfDepth;
		o2_halfWidth = o2.objectTransform.size.x / 2;
		o2_halfHeight = o2.objectTransform.size.y / 2;
		o2_halfDepth = o2.objectTransform.size.z / 2;

		o2_minX = o2.objectTransform.location.x - o2_halfWidth;
		o2_maxX = o2.objectTransform.location.x + o2_halfWidth;
		o2_minY = o2.objectTransform.location.y - o2_halfHeight;
		o2_maxY = o2.objectTransform.location.y + o2_halfHeight;
		o2_minZ = o2.objectTransform.location.z - o2_halfDepth;
		o2_maxZ = o2.objectTransform.location.z + o2_halfDepth;


		//Calculate distance squared first, in order to use it in checks
		if (o1.objectTransform.location.x < o2_minX)
			distanceSquared += (o2_minX - o1.objectTransform.location.x) * (o2_minX - o1.objectTransform.location.x);
		else if (o1.objectTransform.location.x > o2_maxX)
			distanceSquared += (o1.objectTransform.location.x - o2_maxX) * (o1.objectTransform.location.x - o2_maxX);
		if (o1.objectTransform.location.y < o2_minY)
			distanceSquared += (o2_minY - o1.objectTransform.location.y) * (o2_minY - o1.objectTransform.location.y);
		else if (o1.objectTransform.location.y > o2_maxY)
			distanceSquared += (o1.objectTransform.location.y - o2_maxY) * (o1.objectTransform.location.y - o2_maxY);
		if (o1.objectTransform.location.z < o2_minZ)
			distanceSquared += (o2_minZ - o1.objectTransform.location.z) * (o2_minZ - o1.objectTransform.location.z);
		else if (o1.objectTransform.location.z > o2_maxZ)
			distanceSquared += (o1.objectTransform.location.z - o2_maxZ) * (o1.objectTransform.location.z - o2_maxZ);


		if (distanceSquared < o1.rb.radius * o1.rb.radius)
			return true;

		return false;
	}

	else
		return false;
}

bool Engine::bufferModels()
{
	for (int i = 0; i < models.size(); i++)
	{
		models[i].buffer(objects[i].objFile);
	}
	return true;
}

bool Engine::gameLoop()
{
	glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int textureIDindex = 1;

	currentFrameTime = glfwGetTime();

	lightLocation = {-1, 0, 2};


	while (!glfwWindowShouldClose(this->GLFWwindowPtr))
	{
		glClear(GL_COLOR_BUFFER_BIT); //call before every draw
		//glBindVertexArray(this->vertArr);
		glClear(GL_DEPTH_BUFFER_BIT);

		//TODO: separate into update and draw
		updateTime();

		//calcPhysics(objects[2]);

		//Assign transformation matrix
		for (int i = 0; i < objects.size(); i++)
		{
			//glClear(GL_COLOR_BUFFER_BIT);
			objects[i].objectTransform.objectWorldTransform = glm::translate(objects[i].objectTransform.location) *
															  glm::yawPitchRoll(objects[i].objectTransform.rotation.y, objects[i].objectTransform.rotation.x, objects[i].objectTransform.rotation.z) *
															  glm::scale(objects[i].objectTransform.size);
			
		
		
		
			
			glBindTexture(GL_TEXTURE_2D, i + 1); //textures don't start at 0, they start at 1
			glUniformMatrix4fv(2, 1, GL_FALSE, &objects[i].objectTransform.objectWorldTransform[0][0]);
			glUniform3f(5, lightLocation.x, lightLocation.y, lightLocation.z);
			glUniform3f(6, cam.camLoc.x, cam.camLoc.y, cam.camLoc.z);
			
			//if (!(models[i].tag == "models/bullet.obj") || !noBullet)
			//	models[i].render();
			//if (models[i].tag == "models/enemy1.obj" && !noEnemy1)
			//	models[i].render();
			//if (models[i].tag == "models/enemy2.obj" && !noEnemy2)
			//	models[i].render();

			if (models[i].tag == "models/bullet.obj" && !noBullet)
				models[i].render();
			else if (models[i].tag == "models/enemy1.obj" && !noEnemy1)
				models[i].render();
			else if (models[i].tag == "models/enemy2.obj" && !noEnemy2)
				models[i].render();
			else if (models[i].tag == "models/player.obj")
				models[i].render();
			
		}
		glfwSwapBuffers(this->GLFWwindowPtr); // call after all drawing is done
		

		cam.update(this->GLFWwindowPtr, keyIsDown, keyWasDown);

		++shipListingCounter;
		if (shipListingCounter > 480)
			shipListingCounter = 0;

		if (shipListingCounter < 120)
			objects[2].objectTransform.rotation += glm::vec3(0, glm::quarter_pi<float>() / 90, 0);
		else if (shipListingCounter >= 120 && shipListingCounter < 360)
			objects[2].objectTransform.rotation += glm::vec3(0, -glm::quarter_pi<float>() / 90, 0);
		else
			objects[2].objectTransform.rotation += glm::vec3(0, glm::quarter_pi<float>() / 90, 0);


		objects[0].objectTransform.rotation += glm::vec3(0, glm::quarter_pi<float>() / 45, 0);
		objects[1].objectTransform.rotation += glm::vec3(0, -glm::quarter_pi<float>() / 45, 0);
		objects[3].objectTransform.rotation += glm::vec3(glm::quarter_pi<float>() / 45, glm::quarter_pi<float>() / 35, -glm::quarter_pi<float>() / 40);
		

		//check collision between goat and door
		if (!detectCollision(objects[0], objects[3])) //enemy1
		{
			noEnemy1 = true;
			noBullet = true;
		}
		if (!detectCollision(objects[1], objects[3]) && !noEnemy2) //enemy2
		{
			noEnemy2 = true;
			noBullet = true;
		}

		//glBindTexture(GL_TEXTURE_2D, textureIDindex);
		//glfwSwapBuffers(this->GLFWwindowPtr);


		//binds
		if (keyIsDown[GLFW_KEY_ESCAPE])
			glfwSetWindowShouldClose(GLFWwindowPtr, GL_TRUE);

		//if (keyIsDown[GLFW_MOUSE_BUTTON_1] && !keyWasDown[GLFW_MOUSE_BUTTON_1])
		//	textureIDindex = ((textureIDindex++) % 3);

		if (keyIsDown[GLFW_KEY_RIGHT])
			objects[2].rb.velocity = glm::vec3(.05, 0, 0);
		else if (keyIsDown[GLFW_KEY_LEFT])
			objects[2].rb.velocity = glm::vec3(-.05, 0, 0);
		else
		{
			//make an enum for these
			//applyDrag(objects[2]);
		}

		if (keyIsDown[GLFW_KEY_SPACE] && !keyWasDown[GLFW_KEY_SPACE])
		{
			noBullet = false;
			objects[3].objectTransform.location = objects[2].objectTransform.location + glm::vec3(0, 0.1, 0);
		}
		else
		{
			if (!noBullet)
				objects[3].objectTransform.location += glm::vec3(0, 0.1, 0) * deltaTime;
		}

		//update input
		keyWasDown = keyIsDown;

		glfwPollEvents();
	}

	glfwTerminate();
	return true;
}

bool Engine::useShaders()
{
	if (sm.loadShaders("shaders/vShader.glsl", "shaders/fShader.glsl"))
	{
		glUseProgram(sm.getProgram());
		return true;
	}
	return false;
}
