#include "Model.h"





class Camera
{
private:
	float fov;
	float aspectRatio;
	float nearDrawDistance;
	float farDrawDistance;
	Transform cameraTransform;
	RigidBody cameraRB;
	//Object camObj;
	float mouseSensitivity;
	int windowWidth;
	int windowHeight;
	double mouseX;
	double mouseY;
	glm::vec3 camRot;
	glm::vec3 camVel;
	glm::mat4 perspectiveMat;

public:
	glm::vec3 camLoc;
	glm::mat4x4 cameraMat;
	glm::mat4x4 identity;

	//Camera(float fov, float aspectRatio, float nearDrawDistance, float farDrawDistance);
	Camera();
	Camera(glm::mat4 &perspectiveMat, glm::mat4 &viewMat);
	~Camera();

	void update(GLFWwindow *window, std::map<int, bool> keyIsDown, std::map<int, bool> keyWasDown);
};

