/*
Framebuffer size is not always equal to window size.
On Windows it is but Retina displays have 2 times smaller pixels (window has 1600x800 while the screen has 800x400)

glViewport - should be feed with dimensions in pixels (framebuffer dimensions)
glfwGetFramebufferSize - return framebuffer size in pixels
glfwGetScreenSize - return screen size (not always in pixels)

glfwSetWindowSize - should be feed with screen dimensions
*/


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

	GLint GetWindowWidth() { return windowWidth; }
	GLint GetWindowHeight() { return windowHeight; }
	GLint GetFramebufferWidth() { return framebufferWidth; }
	GLint GetFramebufferHeight() { return framebufferHeight; }

	bool GetShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* GetKeys() { return keys; } //Get keys from this window, used by camera
	bool* GetMouseButtons() { return mouseButtons; }

	GLfloat GetXChange(); //Get x change of mouse position, used by camera
	GLfloat GetYChange(); //Get y change of mouse position, used by camera

	void SwapBuffers() { glfwSwapBuffers(mainWindow); }

	GLFWwindow* GetWindow() { return mainWindow; }

	bool CheckWindowSize();


	~Window();

private:
	GLFWwindow * mainWindow;

	GLint windowWidthLast, windowHeightLast;
	GLint windowWidth, windowHeight;
	GLint framebufferWidthLast, framebufferHeightLast;
	GLint framebufferWidth, framebufferHeight;


	bool keys[1024]; //List where state of user inputs will be stored
	bool mouseButtons[64];

	//To compare mouse movements between updates
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved; //To handle mouse flickering on the screen when placed at the beginning of the program

	//void framebuffer_size_callback(GLFWwindow* window, int width, int height);


	void FramebufferSizeCallback(GLFWwindow* window, int newFramebufferWidth, int newFramebufferHeight);
	void CreateCallbacks(); //Callbacks of input
	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouseButtons(GLFWwindow* window, int button, int action, int mode);
	static void HandleMousePosition(GLFWwindow* window, double xPos, double yPos);
};