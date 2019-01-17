#pragma once

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <GLFW\glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMovementSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange, GLfloat deltaTime);

	glm::vec3 GetCameraPosition();

	void CalculateViewMatrix();
	glm::mat4 GetViewMatrix();

	void CalculateProjectionMatrix(float FOV, GLfloat windowBufferWidth, GLfloat windowBufferHeight, float nearPlane, float farPlane);
	glm::mat4 GetProjectionMatrix();

	glm::vec3 GetForward();

	~Camera();

private:

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;;

	GLfloat yaw;
	GLfloat pitch;
	

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};

