#include "Window.h"


using namespace std;

Window::Window()
{

}

Window::Window(GLint windowWidthStart, GLint windowHeightStart)
{
	this->windowWidth = windowWidthStart;
	this->windowHeight = windowHeightStart;
	this->windowWidthLast = windowWidthStart;
	this->windowHeightLast = windowHeightStart;
	framebufferWidth= 0;
	framebufferHeight = 0;
	framebufferWidthLast = 0;
	framebufferHeightLast = 0;


	for (size_t i = 0; i < 1024; i++) //Initialize a list where state of user inputs will be stored
	{
		keys[i] = 0;
	}

	for (size_t i = 0; i < 64; i++) //Initialize a list where state of user inputs will be stored
	{
		mouseButtons[i] = 0;
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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Setup GLFW window properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //OpenGL version, setting 3.3 version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Core profile= No backward compatibility, Throw an error if code we use is deprecated
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Allow forward compatibility

	mainWindow = glfwCreateWindow(windowWidth, windowHeight, "Mothman Rendering Engine", NULL, NULL); //Create the window
	if (!mainWindow)
	{
		cout << "GLFW window creation failed!" << endl;
		glfwTerminate();
		return 1;
	}

	//Get buffer size information
	//Buffer is holding all the OpenGL data, it OpenGL viewport where everything will be rendered
	glfwGetFramebufferSize(mainWindow, &framebufferWidth, &framebufferHeight); //Sets the buffer width and height based on mainWindow dimensions
	framebufferWidthLast = framebufferWidth;
	framebufferHeightLast = framebufferHeightLast;
	glfwGetWindowSize(mainWindow, &windowWidth, &windowHeight); //Sets the window width and height based on mainWindow dimensions

	glfwMakeContextCurrent(mainWindow); //Set context for GLEW to use
	CreateCallbacks(); //Handle Key + Mouse Input
	//glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Setting mode of cursor (Normal, Disabled, Hidden)
	glewExperimental = GL_TRUE; //Allow modern extension features

	GLenum error = glewInit(); //Initalize GLEW library
	if (error != GLEW_OK)
	{
		cout << "GLEW initialization failed!" << endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //Enabling disabled feature by default
	glViewport(0, 0, framebufferWidth, framebufferWidth); //Create viewport and setup size
	glfwSetWindowUserPointer(mainWindow, this); //Set this class to be user of this window
	glfwSwapInterval(0); //Disable Vsync on NVIDIA cards
}


void Window::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
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
			printf("Pressed: %d key\n", key); //Debug
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;	
			printf("Relesed: %d key\n", key); //Debug
		}
	}
}

void Window::HandleMouseButtons(GLFWwindow* window, int button, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window)); //This will get userPointer for window which is set 5 lines above, used to gain access to window

	if (button >= 0 && button < 1024) 
	{
		if (action == GLFW_PRESS)
		{
			theWindow->mouseButtons[button] = true;
			printf("Pressed: %d mouse button\n", button); //Debug
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->mouseButtons[button] = false;
			printf("Relesed: %d  mouse button\n", button); //Debug
		}
	}
}

void Window::HandleMousePosition(GLFWwindow* window, double xPos, double yPos)
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

void Window::CreateCallbacks() //Debug
{
	glfwSetKeyCallback(mainWindow, HandleKeys);
	glfwSetCursorPosCallback(mainWindow, HandleMousePosition);
	glfwSetMouseButtonCallback(mainWindow, HandleMouseButtons);
}

//Get changes, used by camera (if mouse was changed and in the next frame it was not, camera still will get same change so we need to set changes to 0
GLfloat Window::GetXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::GetYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

bool Window::CheckWindowSize()
{
	glfwGetWindowSize(mainWindow, &windowWidth, &windowHeight);
	if (windowWidth != windowWidthLast || windowHeight != windowHeightLast) //If detect window size change
	{
		glfwGetFramebufferSize(mainWindow, &framebufferWidth, &framebufferHeight);
		glViewport(0, 0, framebufferWidth, framebufferWidth);
		cout << "Resizing: " << "(Window - from: " << windowWidthLast << "x" << windowHeightLast << " to " << windowWidth << "x" << windowHeight << ")" << "(Framebuffer - from: " << framebufferWidthLast << "x" << framebufferHeightLast << " to " << framebufferWidth << "x" << framebufferHeight << ")" << endl;
		windowWidthLast = windowWidth;
		windowHeightLast = windowHeight;
		framebufferWidthLast = framebufferWidth;
		framebufferHeightLast = framebufferHeight;
		return true;
	}
	return false;
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}