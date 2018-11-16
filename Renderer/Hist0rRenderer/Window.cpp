#include "Window.h"

using namespace std;

Window::Window()
{
	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; i++) //Initialize a list where state of user inputs will be stored
	{
		keys[i] = 0;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++) //Initialize a list where state of user inputs will be stored
	{
		keys[i] = 0;
	}
}

int Window::Initialise()
{
	//Initialise GLFW
	if (!glfwInit())
	{
		cout << "GLFW initization failed!" << endl;
		glfwTerminate();
		return 1;
	}

	//Setup GLFW window properties
	//OpenGL version, setting 3.3 version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Core profile= No backward compatibility, Throw an error if code we use is deprecated
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	mainWindow = glfwCreateWindow(width, height, "Hist0r Renderer", NULL, NULL);
	if (!mainWindow)
	{
		cout << "GLFW window creation failed!" << endl;
		glfwTerminate();
		return 1;
	}

	//Get buffer size information
	//Buffer is holding all the OpenGL data, it OpenGL viewport where everything will be rendered
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight); //Sets the buffer width and height based on mainWindow dimensions

	//Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	//Handle Key + Mouse Input
	createCallbacks();
	//glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Setting mode of cursor (Normal, Disabled, Hidden)

	//Allow modern extension features
	glewExperimental = GL_TRUE;

	//Initalize GLEW library
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		cout << "GLEW initialization failed!" << endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //Enabling disabled feature by default

	//Create viewport and setup size
	glViewport(0, 0, bufferWidth, bufferHeight);

	//Set this class to be user of this window
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window)); //This will get userPointer for window which is set 5 lines above, used to gain access to window

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) //This key has id: 256 and action has id: 1
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}


	

	if (key >= 0 && key < 1024) //If is a proper key, setting key state on list where state of user inputs are stored
	{
		
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			printf("Pressed: %d\n", key); //Debug
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;	
			printf("Relesed: %d\n", key); //Debug
		}
	}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window)); //This will get userPointer for window, used to gain access to window

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

	//printf("x:%.6f, y:%.6f\n", theWindow->xChange, theWindow->yChange); //Debug
}

void Window::createCallbacks() //Debug
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

//Get changes, used by camera (if mouse was changed and in the next frame it was not, camera still will get same change so we need to set changes to 0
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}