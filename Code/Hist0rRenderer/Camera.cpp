#include "Camera.h"

Camera::Camera()
{

}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	forward = glm::vec3(0.0f, 0.0f, -0.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	if (keys[GLFW_KEY_W])
	{
		position += forward * moveSpeed * deltaTime;
	}
	if (keys[GLFW_KEY_S])
	{
		position -= forward * moveSpeed * deltaTime;
	}
	if (keys[GLFW_KEY_A])
	{
		position -= right * moveSpeed * deltaTime;
	}
	if (keys[GLFW_KEY_D])
	{
		position += right * moveSpeed * deltaTime;
	}
	if (keys[GLFW_KEY_Q])
	{
		position -= worldUp * moveSpeed * deltaTime;
	}
	if (keys[GLFW_KEY_E])
	{
		position += worldUp * moveSpeed * deltaTime;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f) //Limiting maximal rotations
	{
		pitch = 89.0f; 
	}
	if (pitch < -89.0f) //Limiting maximal rotations
	{
		pitch = -89.0f;
	}

	update();
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + forward, up);
}

void Camera::update()
{
	//Calculating vectors
	forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(forward);

	right = glm::normalize(glm::cross(forward, worldUp)); //Right vector is a cross product of foward and worldUp
	up = glm::normalize(glm::cross(right, forward)); //Up vector is a cross product of right and forward
}


Camera::~Camera()
{


}
