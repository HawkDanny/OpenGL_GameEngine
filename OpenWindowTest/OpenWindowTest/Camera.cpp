#include "Camera.h"



Camera::Camera()
{
	//perspective matrix
	float zoom = 1.f;
	int width = 800;
	int height = 600;

	float fovy = 3.14159f * .4f / zoom;
	float aspect = (float)width / (float)height;
	float zNear = .01f;
	float zFar = 1000.f;

	perspectiveMat = glm::perspective(fovy, aspect, zNear, zFar);
	
	//view matrix
	camLoc = { 0, 0, 2 };
	camRot = { 0, 0, 0 };

	glm::mat3 rotMat = (glm::mat3) glm::yawPitchRoll(camRot.y, camRot.x, camRot.z);

	glm::vec3 eye = camLoc;
	glm::vec3 center = eye + rotMat * glm::vec3(0, 0, -1);
	glm::vec3 up = rotMat * glm::vec3(0, 1, 0);

	glm::mat4 viewMat = glm::lookAt(eye, center, up);

	cameraMat = perspectiveMat * viewMat;

	identity = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };

	mouseSensitivity = .005;
	windowWidth = 800;
	windowHeight = 600;
}

Camera::Camera(glm::mat4 &perspectiveMat, glm::mat4 &viewMat)
{
	cameraMat = perspectiveMat * viewMat;
	identity = { { 1, 0, 0, 0 },{ 0, 1, 0, 0 },{ 0, 0, 1, 0 },{ 0, 0, 0, 1 } };
	mouseSensitivity = .005;
	windowWidth = 800;
	windowHeight = 600;
}

Camera::~Camera()
{

}


void Camera::update(GLFWwindow *window, std::map<int, bool> keyIsDown, std::map<int, bool> keyWasDown)
{
	glfwGetCursorPos(window, &mouseX, &mouseY);

	//camRot.y -= mouseSensitivity * (mouseX - windowWidth * .5f); //Yaw
	//camRot.x -= mouseSensitivity * (mouseY - windowHeight * .5f); //Pitch
	//camRot.x = glm::clamp(camRot.x, -.5f * glm::pi<float>(), .5f * glm::pi<float>());

	glfwSetCursorPos(window, windowWidth * .5f, windowHeight * .5f);


	glm::mat3 R = (glm::mat3)glm::yawPitchRoll(camRot.y, camRot.x, camRot.z);
	if (keyIsDown[GLFW_KEY_A])
		camVel += R * glm::vec3(-1, 0, 0);
	if (keyIsDown[GLFW_KEY_D])
		camVel += R * glm::vec3(1, 0, 0);
	if (keyIsDown[GLFW_KEY_W])
		camVel += R * glm::vec3(0, 0, 1);
	if (keyIsDown[GLFW_KEY_S])
		camVel += R * glm::vec3(0, 0, -1);

	if (!(keyIsDown[GLFW_KEY_A] || keyIsDown[GLFW_KEY_D]))
		camVel.x = 0;
	if (!(keyIsDown[GLFW_KEY_S] || keyIsDown[GLFW_KEY_W]))
		camVel.y = 0;
	camVel.z = 0;

	float speed = 0.01f;
	if (camVel != glm::vec3())
		camVel = glm::normalize(camVel) * speed;

	camLoc += camVel;

	//Temp
	glm::mat3 rotMat = (glm::mat3) glm::yawPitchRoll(camRot.y, camRot.x, camRot.z);

	glm::vec3 eye = camLoc;
	glm::vec3 center = eye + rotMat * glm::vec3(0, 0, -1);
	glm::vec3 up = rotMat * glm::vec3(0, 1, 0);

	glm::mat4 viewMat = glm::lookAt(eye, center, up);

	cameraMat = perspectiveMat * viewMat;
	//End Temp


	glUniformMatrix4fv(3, 1, GL_FALSE, &cameraMat[0][0]);
}