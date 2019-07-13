void ControlWireframeMode()
{
	//Toggle wireframe mode
	if (mainWindow.GetKeys()[GLFW_KEY_V] == true)
	{
		wireFrameState = !wireFrameState;
		cout << "Wireframe mode: " << wireFrameState << endl;
		mainWindow.GetKeys()[GLFW_KEY_V] = false;
	}
}

//----------------------------------------------------------------------------------------------------------

//Changing edge detection post effect offset variable
float edgeDetection = postProcessingEffectsSettings.edgeDetection.GetOffsetValue();
if (mainWindow.GetKeys()[GLFW_KEY_Y])
{
	postProcessingEffectsSettings.edgeDetection.SetOffsetValue(edgeDetection += 2 * deltaTime);
}
if (mainWindow.GetKeys()[GLFW_KEY_U])
{
	postProcessingEffectsSettings.edgeDetection.SetOffsetValue(edgeDetection -= 2 * deltaTime);
}

//----------------------------------------------------------------------------------------------------------

float movementCurrentTranslation_Human = 0;
float movementDirection_Human = true;

//Human movement
if (movementCurrentTranslation_Human >= 3 || movementCurrentTranslation_Human <= -3)
{
	movementDirection_Human = !movementDirection_Human;
}
if (movementDirection_Human)
{
	movementCurrentTranslation_Human += 1.1f * deltaTime;
}
else
{
	movementCurrentTranslation_Human -= 1.1f * deltaTime;
}

//----------------------------------------------------------------------------------------------------------

glm::vec3 initialPos_Light;
float movementCurrentTranslation_Light = 0;
float movementDirection_Light = true;

//Light movement
if (movementCurrentTranslation_Light >= 2 || movementCurrentTranslation_Light <= -2)
{
	movementDirection_Light = !movementDirection_Light;
}
if (movementDirection_Light)
{
	movementCurrentTranslation_Light += 1.0f * deltaTime;
}
else
{
	movementCurrentTranslation_Light -= 1.0f * deltaTime;
}
pointLights[0].SetPosition(initialPos_Light + glm::vec3(0, 0, movementCurrentTranslation_Light));

//----------------------------------------------------------------------------------------------------------
float currentAngle = 0.0f;
float angleIncrement = 1.0f;

bool sizeDirection = true;
float currentSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

//Rotating tetrahedron
currentAngle += angleIncrement / 2;
if (currentAngle >= 360.0f)
{
	currentAngle -= 360.0f;
}
if (currentSize >= maxSize || currentSize <= minSize)
{
	sizeDirection = !sizeDirection;
}
if (sizeDirection)
{
	currentSize += 0.1f  * deltaTime;
}
else
{
	currentSize -= 0.1f  * deltaTime;
}

//In RenderScene
model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Tranforming matrix of translation into matrix of translation and rotation
model = glm::scale(model, glm::vec3(currentSize, currentSize, 1.0f)); //Tranforming matrix of translation and rotation into matrix of translation, rotation and scale 

//----------------------------------------------------------------------------------------------------------

void ControlTessellationSettings()
{
	if (mainWindow.GetKeys()[GLFW_KEY_1])
	{
		tfa += 2 * deltaTime;
	}
	if (mainWindow.GetKeys()[GLFW_KEY_2])
	{
		tfa -= 2 * deltaTime;
	}
	terrainController.GetTerrain("Terrain1")->GetTerrainSettings()->SetTessellationFactor(tfa);

	if (mainWindow.GetKeys()[GLFW_KEY_3])
	{
		tsl += 2 * deltaTime;
	}
	if (mainWindow.GetKeys()[GLFW_KEY_4])
	{
		tsl -= 2 * deltaTime;
	}
	terrainController.GetTerrain("Terrain1")->GetTerrainSettings()->SetTessellationSlope(tsl);

	if (mainWindow.GetKeys()[GLFW_KEY_5])
	{
		tsh += 2 * deltaTime;
	}
	if (mainWindow.GetKeys()[GLFW_KEY_6])
	{
		tsh -= 2 * deltaTime;
	}
	terrainController.GetTerrain("Terrain1")->GetTerrainSettings()->SetTessellationShift(tsh);
}

//----------------------------------------------------------------------------------------------------------

int FPSCounterframesPassed = 0;
double FPSCounterStartTime = 0;
float fps = 0.0f;

void FPSCounter()
{
	double timePassed = glfwGetTime();

	FPSCounterframesPassed++;

	if (timePassed - FPSCounterStartTime > 0.75 && FPSCounterframesPassed > 30)
	{
		fps = (double)FPSCounterframesPassed / (timePassed - FPSCounterStartTime);
		FPSCounterStartTime = timePassed;
		FPSCounterframesPassed = 0;
		std::cout << "FPS: " << fps << std::endl;
	}
}

//----------------------------------------------------------------------------------------------------------

//Function pointers
void Window::CheckWindowSize(void(*CreateRenderBuffersPtr)(), void(*DeleteRenderBuffersPtr)())
{
	DeleteRenderBuffersPtr();
	CreateRenderBuffersPtr();
}