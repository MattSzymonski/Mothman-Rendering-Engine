//OpenGL project that draws a tetrahedron on black background and rotates it using matrix muliplied by position vector of tetrahedron in geometry shader, this matrix is being updated in main loop
//Additionally tetrahedron is colorful due to interpolation based on vertex average of vertex positions
//Camera movement and rotation is implemented

//Texture units:
//1 - diffuse texture
//3 - directional shadow map
//2 - normal texture
//2-8 - shadow maps 

//Standard libraries
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>

//OpenGL must-have libraries
#include <GL\glew.h>
#include <GLFW\glfw3.h>

//OpenGL math libraries
#include <glm\mat4x4.hpp>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

//Project headers
#include "CommonValues.h"
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "Lights/SpotLight.h"

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "Material.h"
#include "Model.h"
#include "Terrain.h"
#include "VertexOperations.h"

using namespace std;

const float toRadians = 3.14159265f / 180.0f;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0; //Initialize projection matrices
GLuint uniformCameraPosition = 0; //Initialize camera position matrix
GLuint uniformSpecularIntensity = 0, uniformShininess = 0; //Initialize specular light matrices
GLuint uniformOmniLightPos = 0, uniformFarPlane = 0; //Initialize matrices for omnidirectional light shadows


GLuint uniformAmbientIntensity = 0, uniformAmbientColour = 0; //Initialize ambient light matrices
GLuint uniformDiffuseDirection = 0, uniformDiffuseIntensity = 0; //Initialize diffuse light matrices



Window mainWindow;

vector<Mesh*> meshList; //List of Mesh pointers

vector<Shader> shaderList; //List of Shader 
Shader directionalShadowShader;
Shader omniShadowShader;

Camera camera; //Camera class

Model floorModel;
Model testModel_Human;
Model testModel_Cube;
Model testModel_TanTest;
Model testModel_CubeTan;
Terrain terrain;

Texture brickTexture;
Texture dirtTexture;

Material shinyMaterial;
Material dullMaterial;

DirectionalLight directionalLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

//To maintain same speed on every CPU
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

//---------------------Moving Mesh--------------------------------
float movementCurrentTranslation_Human = 0;
float movementDirection_Human = true;

float currentAngle = 0.0f;
float angleIncrement = 1.0f;

bool sizeDirection = true;
float currentSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

bool wireFrameState = false;


glm::vec3 initialPos_Light;
float movementCurrentTranslation_Light = 0;
float movementDirection_Light = true;



//---------------------------------------------------------------

void CreateObjects()
{
	//Create tetrahedron
	std::vector<GLfloat> vertices;
	vertices.insert(vertices.end(), { -1.0f, -1.0f,  0.0f,     0.0f, 0.0f,    0.0f, 0.0f, 0.0f,	 0.0f, 0.0f, 0.0f });
	vertices.insert(vertices.end(), { 0.0f,  -1.0f,  1.0f,     0.5f, 0.0f,    0.0f, 0.0f, 0.0f,	 0.0f, 0.0f, 0.0f });
	vertices.insert(vertices.end(), { 1.0f,  -1.0f,  0.0f,     1.0f, 0.0f,    0.0f, 0.0f, 0.0f,	 0.0f, 0.0f, 0.0f });
	vertices.insert(vertices.end(), { 0.0f,   1.0f,  0.0f,     0.5f, 1.0f,    0.0f, 0.0f, 0.0f,	 0.0f, 0.0f, 0.0f });

	std::vector<unsigned int> indices;
	indices.insert(indices.end(), { 0, 3, 1 });
	indices.insert(indices.end(), { 1, 3, 2 });
	indices.insert(indices.end(), { 2, 3, 0 });
	indices.insert(indices.end(), { 0, 1, 2 });

	VertexOperations::CalcAverageNormals(indices, indices.size(), vertices, vertices.size(), 11, 5); //Calculate average normal vectors for each vertex (Phong shading)
	VertexOperations::CalculateTangents(indices, indices.size(), vertices, vertices.size(), 11, 3, 8);

	Mesh *obj1 = new Mesh(); //From mesh.h
	obj1->CreateMesh(&vertices[0], &indices[0], 44, 12);
	meshList.push_back(obj1); //Add to the end of the list




	//Create floor
	std::vector<GLfloat> verticesFloor;
	verticesFloor.insert(verticesFloor.end(), { -10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,  0.0f, 0.0f, 0.0f });
	verticesFloor.insert(verticesFloor.end(), { 10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,  0.0f, 0.0f, 0.0f });
	verticesFloor.insert(verticesFloor.end(), { -10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,  0.0f, 0.0f, 0.0f });
	verticesFloor.insert(verticesFloor.end(), { 10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f,  0.0f, 0.0f, 0.0f });

	std::vector<unsigned int> indicesFloor;
	//indicesFloor.insert(indicesFloor.end(), { 1, 0, 3 });
	//indicesFloor.insert(indicesFloor.end(), { 0, 3, 2 });

	indicesFloor.insert(indicesFloor.end(), { 0, 1, 2 });
	indicesFloor.insert(indicesFloor.end(), { 2, 1, 3 });

	VertexOperations::CalcAverageNormals(indicesFloor, indicesFloor.size(), verticesFloor, verticesFloor.size(), 11, 5); //Calculate average normal vectors for each vertex (Phong shading)
	VertexOperations::CalculateTangents(indicesFloor, indicesFloor.size(), verticesFloor, verticesFloor.size(), 11, 3, 8);

	Mesh *obj2 = new Mesh(); //From mesh.h
	obj2->CreateMesh(&verticesFloor[0], &indicesFloor[0], 44, 5);
	meshList.push_back(obj2); //Add to the end of the list


	//Terrain
	terrain = Terrain("Textures/Terrain/TerrainHeight1.png", "Textures/Terrain/TerrainTex1.png", 1.0f);
	terrain.CreateTerrain();

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles("Shaders/vertex.shader", "Shaders/fragment.shader");
	shaderList.push_back(*shader1);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map_vertex.shader", "Shaders/directional_shadow_fragment.shader");

	omniShadowShader = Shader();
	omniShadowShader.CreateFromFiles("Shaders/omni_directional_shadow_map_vertex.shader", "Shaders/omni_directional_shadow_map_geometry.shader", "Shaders/omni_directional_shadow_map_fragment.shader");
}

void RenderScene()
{
	//SPECIFIC MODEL OPERATIONS
	glm::mat4 model;

	//Terrain
	
	model = glm::mat4();

	model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	//model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //Set uniform variable value in shader (uniform variable in shader, count, transpose?, pointer to matrix)	
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	//brickTexture.UseTexture();
	terrain.RenderTerrain();
	

	//Tetrahedron
	model = glm::mat4(); //Creating identity matrix
	model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f)); //Transforming identity matrix into translation matrix
	model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Tranforming matrix of translation into matrix of translation and rotation
	model = glm::scale(model, glm::vec3(currentSize, currentSize, 1.0f)); //Tranforming matrix of translation and rotation into matrix of translation, rotation and scale 
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //Set uniform variable value in shader (uniform variable in shader, count, transpose?, pointer to matrix)	
	brickTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[0]->RenderMesh();

	//Floor
	model = glm::mat4(); //Creating identity matrix
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.0f)); //Transforming identity matrix into translation matrix													
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //Set uniform variable value in shader (uniform variable in shader, count, transpose?, pointer to matrix)	
	dirtTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	floorModel.RenderModel();
	//meshList[1]->RenderMesh(); //Old verices floor


	//TanTest
	model = glm::mat4(); //Creating identity matrix
	model = glm::translate(model, glm::vec3(6.0f, 1.0f, -0.0f)); //Transforming identity matrix into translation matrix
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //Set uniform variable value in shader (uniform variable in shader, count, transpose?, pointer to matrix)	
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	testModel_TanTest.RenderModel();
	
	//CubeTest
	model = glm::mat4(); //Creating identity matrix
	model = glm::translate(model, glm::vec3(0.0f, 0.5f, 1.0f)); //Transforming identity matrix into translation matrix
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //Set uniform variable value in shader (uniform variable in shader, count, transpose?, pointer to matrix)	
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	testModel_CubeTan.RenderModel();

	//Test Human
	model = glm::mat4(); //Creating identity matrix
	model = glm::translate(model, glm::vec3(0.0f + movementCurrentTranslation_Human, 0.0f, -0.0f)); //Transforming identity matrix into translation matrix
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //Set uniform variable value in shader (uniform variable in shader, count, transpose?, pointer to matrix)	
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	testModel_Human.RenderModel();

	//Cube
	model = glm::mat4(); //Creating identity matrix
	model = glm::translate(model, glm::vec3(0.0f, 0.5f, 3.0f)); //Transforming identity matrix into translation matrix
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //Set uniform variable value in shader (uniform variable in shader, count, transpose?, pointer to matrix)	
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	testModel_Cube.RenderModel();

	//Cube2
	model = glm::mat4(); //Creating identity matrix
	model = glm::translate(model, glm::vec3(1.0f, 0.5f, -4.0f)); //Transforming identity matrix into translation matrix
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //Set uniform variable value in shader (uniform variable in shader, count, transpose?, pointer to matrix)	
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	testModel_Cube.RenderModel();

	//Cube3
	model = glm::mat4(); //Creating identity matrix
	model = glm::translate(model, glm::vec3(-7.0f, 0.5f, -5.0f)); //Transforming identity matrix into translation matrix
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //Set uniform variable value in shader (uniform variable in shader, count, transpose?, pointer to matrix)	
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	testModel_Cube.RenderModel();

	//Cube4
	model = glm::mat4(); //Creating identity matrix
	model = glm::translate(model, glm::vec3(-5.0f, 0.5f, -8.0f)); //Transforming identity matrix into translation matrix
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //Set uniform variable value in shader (uniform variable in shader, count, transpose?, pointer to matrix)	
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	testModel_Cube.RenderModel();

	//Cube5
	model = glm::mat4(); //Creating identity matrix
	model = glm::translate(model, glm::vec3(-3.0f, 0.5f, -3.0f)); //Transforming identity matrix into translation matrix
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //Set uniform variable value in shader (uniform variable in shader, count, transpose?, pointer to matrix)	
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	testModel_Cube.RenderModel();
}

void DirectionalShadowMapPass(DirectionalLight* light) //Pass that renders directional shadow map
{
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight()); //Set viewport size to the same dimensions as framebuffer

	directionalShadowShader.UseShader(); //Set directional shadow shader active
	uniformModel = directionalShadowShader.GetModelLocation(); //Get uniform variables in shader
	directionalShadowShader.SetDirectionalLightTransform(&light->CalculateLightTransform()); //Set uniform variable in shader to view matrix from light source point of view

	light->GetShadowMap()->Write(); //Bind texture to the active framebuffer

	glClear(GL_DEPTH_BUFFER_BIT);

	directionalShadowShader.Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0); //Bind default framebuffer that will draw scene
}

void OmniShadowMapPass(PointLight* light) //Pass that renders omni-directional shadow map
{
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight()); //Set viewport size to the same dimensions as framebuffer

	omniShadowShader.UseShader(); //Set directional shadow shader active
	uniformModel = omniShadowShader.GetModelLocation(); //Get uniform variables in shader
	uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation(); //Get uniform variables in shader
	uniformFarPlane = omniShadowShader.GetFarPlaneLocation(); //Get uniform variables in shader

	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z); //Set uniform variable in shader
	glUniform1f(uniformFarPlane, light->GetFarPlane()); //Set uniform variable in shader
	omniShadowShader.SetOmniLightMatrices(light->CalculateLightTransform()); //Set uniform variable in shader to view matrix from light source point of view

	light->GetShadowMap()->Write(); //Bind texture to the active framebuffer

	glClear(GL_DEPTH_BUFFER_BIT);

	omniShadowShader.Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	shaderList[0].UseShader(); //Set default shader active

	//Get uniform variables in shader
	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformCameraPosition = shaderList[0].GetCameraPositionLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();

	glViewport(0, 0, 800, 600);

	//Clear window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Clears all frame and sets color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear colors and depth (Each pixel stores more information than only color (depth, stencil, alpha, etc))

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix)); //Set uniform variable value in shader
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix)); //Set uniform variable value in shader
	glUniform3f(uniformCameraPosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z); //Set uniform variable value in shader

	//Set lights (set light properties to shader uniform variables)
	shaderList[0].SetDirectionalLight(&directionalLight);
	shaderList[0].SetPointLights(pointLights, pointLightCount, OMNIDIR_SHADOWMAP_TEXUNIT, 0); 
	shaderList[0].SetSpotLights(spotLights, spotLightCount, OMNIDIR_SHADOWMAP_TEXUNIT + pointLightCount, pointLightCount); //OMNIDIR_SHADOWMAP_TEXUNIT + pointLightCount  - each light have own buffer
	shaderList[0].SetDirectionalLightTransform(&directionalLight.CalculateLightTransform());

	directionalLight.GetShadowMap()->Read(GL_TEXTURE3); //Make texture unit 3(third) active and bind shadow map texture to it

	shaderList[0].SetTextureDiffuse(DIFFUSE_TEXUNIT); //Set uniform variable in shader that holds texture for mesh to 0(first) texture unit (Diffuse map)
	shaderList[0].SetTextureNormal(NORMAL_TEXUNIT); //Set uniform variable in shader that holds texture for mesh to 0(first) texture unit (Normal map)
	shaderList[0].SetDirectionalShadowMap(DIR_SHADOWMAP_TEXUNIT); //Set uniform variable in shader that holds shadow map texture 1(second) texture unit


	//glm::vec3 lowerLight = camera.getCameraPosition();
	//lowerLight.y -= 0.3f;
	//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection()); //Flashlight

	//We should validate shader after assigning uniform variables, otherwise when we have two different types of texture(samplerCube and sampler2d) assigned to
	//the default texture unit(before assigning uniform variables it is 0) will generate an error
	shaderList[0].Validate();
	RenderScene();
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(5.0f, 2.0f, -5.0f), glm::vec3(0.0f, 1.0f, 0.0f), 135.0f, 0.0f, 5.0f, 0.4f); //Creating camera

	//Loading models

	floorModel = Model();
	floorModel.LoadModel("Models/Floor.fbx");
	testModel_Human = Model();
	testModel_Human.LoadModel("Models/Gary.fbx");
	testModel_Cube = Model();
	testModel_Cube.LoadModel("Models/Cube.fbx");
	testModel_TanTest = Model();
	testModel_TanTest.LoadModel("Models/Tan.fbx");
	testModel_CubeTan = Model();
	testModel_CubeTan.LoadModel("Models/CubeTan.fbx");


	//Loading textures
	brickTexture = Texture("Textures/brick.png", TexType::Diffuse);
	brickTexture.LoadTexture();
	dirtTexture = Texture("Textures/dirt.png", TexType::Diffuse);
	dirtTexture.LoadTexture();

	//brickTexture.UseTexture(); //This texture will be used for the rest of the code

	//Creating materials
	shinyMaterial = Material(1.0f, 32);
	dullMaterial = Material(0.3f, 4);

	//Create directional light
	directionalLight = DirectionalLight(2048, 2048,
		1.0f, 1.0f, 1.0f,
		0.2f, 0.3f,
		-10.0f, -15.0f, -10.0f);

	//Create pointlights
	pointLights[0] = PointLight(1024, 1024,
		0.1f, 100.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		-5.0f, 1.0f, -5.0f,
		0.3f, 0.1f, 0.0f);
	pointLightCount++;
	initialPos_Light = pointLights[0].GetPosition();

	pointLights[1] = PointLight(1024, 1024,
		0.1f, 100.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f,
		-9.0f, 2.0f, -2.0f,
		0.3f, 0.1f, 0.1f);
	pointLightCount++;

	//Create spotlights
	spotLights[0] = SpotLight(1024, 1024,
		0.1f, 100.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 5.0f,
		-8.0f, 1.0f, -4.0f,
		45.0f, 0.0f, 0.0f,
		0.50f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(1024, 1024,
		0.1f, 100.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 5.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.40f, 0.10f, 0.10f,
		15.0f);
	spotLightCount++;


	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f); //(field of view, aspect ratio, draw distance min, draw distance max)

	//Loop until window closed
	while (!mainWindow.getShouldClose()) //If will loop until this function detects that the window should be closed based on variable hidden inside GLFW 
	{
		GLfloat now = glfwGetTime(); // = SDL_GetPerformanceCounter(); - if using STL instead of GLFW
		deltaTime = now - lastTime; // = (now - lastTime) * 1000/SDL_GetPerformanceFrequency(); - if using STL instead of GLFW
		lastTime = now;

		//Get and handle user input events
		glfwPollEvents();
		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//---------------------Moving Mesh---------------------------


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


		

		//Toggle wireframe mode
		if (mainWindow.getKeys()[GLFW_KEY_M] == true)
		{
			wireFrameState = !wireFrameState;
			if (wireFrameState)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			mainWindow.getKeys()[GLFW_KEY_M] = false;
		}




		//-------------------------DRAW---------------------------
		DirectionalShadowMapPass(&directionalLight); //Shadow pass for directional shadows (once because we have only one light)
		for (size_t i = 0; i < pointLightCount; i++) { OmniShadowMapPass(&pointLights[i]); } //Shadow pass for omni-directional point light shadows (as many times as we have point lights)
		for (size_t i = 0; i < spotLightCount; i++) { OmniShadowMapPass(&spotLights[i]); } //Shadow pass for omni-directional spot light shadows (as many times as we have spot lights)
		RenderPass(projection, camera.calculateViewMatrix());

		glUseProgram(0);

		//---------------------------------------------------------

		mainWindow.swapBuffers();//Swap back buffer and front buffer
	}

	return 0;
}

