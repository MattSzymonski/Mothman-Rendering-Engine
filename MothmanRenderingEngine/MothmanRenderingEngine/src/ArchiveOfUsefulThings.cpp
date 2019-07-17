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

//----------------------------------------------------------------------------------------------------------

//SPECIFIC MODEL OPERATIONS
glm::mat4 model;

//Tetrahedron
model = glm::mat4(); //Creating identity matrix
model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f)); //Transforming identity matrix into translation matrix
//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //Set uniform variable value in shader (uniform variable in shader, count, transpose?, pointer to matrix)
shader->SetUniform("u_model", model);

brickTexture.UseTexture();
if (shader == &mainShader) { shinyMaterial.UseMaterial(&mainShader, "u_material.specularIntensity", "u_material.shininess"); }
meshList[0]->RenderMesh();

//Floor
model = glm::mat4(); //Creating identity matrix
model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.0f)); //Transforming identity matrix into translation matrix
shader->SetUniform("u_model", model);
dirtTexture.UseTexture();
if (shader == &mainShader) { shinyMaterial.UseMaterial(&mainShader, "u_material.specularIntensity", "u_material.shininess"); }
floorModel.RenderModel();
//meshList[1]->RenderMesh(); //Old verices floor

//TanTest
model = glm::mat4(); //Creating identity matrix
model = glm::translate(model, glm::vec3(6.0f, 1.0f, -0.0f)); //Transforming identity matrix into translation matrix
shader->SetUniform("u_model", model);
if (shader == &mainShader) { shinyMaterial.UseMaterial(&mainShader, "u_material.specularIntensity", "u_material.shininess"); }
testModel_TanTest.RenderModel();

//CubeTest
model = glm::mat4(); //Creating identity matrix
model = glm::translate(model, glm::vec3(0.0f, 0.5f, 1.0f)); //Transforming identity matrix into translation matrix
shader->SetUniform("u_model", model);
if (shader == &mainShader) { shinyMaterial.UseMaterial(&mainShader, "u_material.specularIntensity", "u_material.shininess"); }
testModel_CubeTan.RenderModel();

//Test Human
model = glm::mat4(); //Creating identity matrix
//model = glm::translate(model, glm::vec3(0.0f + movementCurrentTranslation_Human, 0.0f, -0.0f)); //Transforming identity matrix into translation matrix
model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.0f)); //Transforming identity matrix into translation matrix
shader->SetUniform("u_model", model);
if (shader == &mainShader) { shinyMaterial.UseMaterial(&mainShader, "u_material.specularIntensity", "u_material.shininess"); }
testModel_Human.RenderModel();

//Cube
model = glm::mat4(); //Creating identity matrix
model = glm::translate(model, glm::vec3(0.0f, 0.5f, 3.0f)); //Transforming identity matrix into translation matrix
shader->SetUniform("u_model", model);
if (shader == &mainShader) { shinyMaterial.UseMaterial(&mainShader, "u_material.specularIntensity", "u_material.shininess"); }
testModel_Cube.RenderModel();

//Cube2
model = glm::mat4(); //Creating identity matrix
model = glm::translate(model, glm::vec3(1.0f, 0.5f, -4.0f)); //Transforming identity matrix into translation matrix
shader->SetUniform("u_model", model);
if (shader == &mainShader) { shinyMaterial.UseMaterial(&mainShader, "u_material.specularIntensity", "u_material.shininess"); }
testModel_Cube.RenderModel();

//Cube3
model = glm::mat4(); //Creating identity matrix
model = glm::translate(model, glm::vec3(-7.0f, 0.5f, -5.0f)); //Transforming identity matrix into translation matrix
shader->SetUniform("u_model", model);
if (shader == &mainShader) { shinyMaterial.UseMaterial(&mainShader, "u_material.specularIntensity", "u_material.shininess"); }
testModel_Cube.RenderModel();

//Cube4
model = glm::mat4(); //Creating identity matrix
model = glm::translate(model, glm::vec3(-5.0f, 0.5f, -8.0f)); //Transforming identity matrix into translation matrix
shader->SetUniform("u_model", model);
if (shader == &mainShader) { shinyMaterial.UseMaterial(&mainShader, "u_material.specularIntensity", "u_material.shininess"); }
testModel_Cube.RenderModel();

//Cube5
model = glm::mat4(); //Creating identity matrix
model = glm::translate(model, glm::vec3(-3.0f, 0.5f, -3.0f)); //Transforming identity matrix into translation matrix
shader->SetUniform("u_model", model);
if (shader == &mainShader) { shinyMaterial.UseMaterial(&mainShader, "u_material.specularIntensity", "u_material.shininess"); }
testModel_Cube.RenderModel();

//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------