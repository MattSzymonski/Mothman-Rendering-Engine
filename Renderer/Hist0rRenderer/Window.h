#pragma once

#include "stdio.h"
#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "CommonValues.h"

class Window
{
public:
	Window();

	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getKeys() { return keys; } //Get keys from this window, used by camera

	GLfloat getXChange(); //Get x change of mouse position, used by camera
	GLfloat getYChange(); //Get y change of mouse position, used by camera

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	GLFWwindow* GetWindow();

	~Window();

private:
	GLFWwindow * mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024]; //List where state of user inputs will be stored


	//To compare mouse movements between updates
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved; //To handle mouse flickering on the screen when placed at the beginning of the program

	void createCallbacks(); //Callbacks of input
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};