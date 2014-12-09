#include <GL/glew.h>
//maths headers
#include <glm/glm.hpp>
using glm::mat4;
using glm::vec4;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <gl/GLU.h>

#include <vector>

#ifdef _DEBUG && WIN32
const std::string ASSET_PATH = "assets/";
const std::string SHADER_PATH = "shaders/";
const std::string TEXTURE_PATH = "textures/";
const std::string FONT_PATH = "fonts/";
const std::string MODEL_PATH = "models/";
#endif

//Our headers
#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "Light.h"
#include "FBXLoader.h"
#include "SkyboxMaterial.h"


//SDL Window
SDL_Window * window = NULL;
//SDL GL Context
SDL_GLContext glcontext = NULL;

//Window Width
const int WINDOW_WIDTH = 1000;
//Window Height
const int WINDOW_HEIGHT = 600;

bool running = true;

vec4 ambientLightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);

std::vector<GameObject*> displayList;
GameObject * mainCamera;
GameObject * mainLight;
GameObject * skyBox = NULL;

//Rotation floats -Earth
<<<<<<< HEAD
float earthX = -45.0f;
float earthY = -0.002f;
=======
float earthX = -60.0f;
float earthY = -60.0f;
>>>>>>> origin/master
float earthZ = 0.0f;

//Camera Transform -Position
float cameraX;
float cameraY;
float cameraZ;
<<<<<<< HEAD
=======

>>>>>>> origin/master

void CheckForErrors()
{
	GLenum error;
	do{
		error = glGetError();
	} while (error != GL_NO_ERROR);
}

//Global functions
void InitWindow(int width, int height, bool fullscreen)
{
	//Create a window
	window = SDL_CreateWindow(
		"Lab 6",             // window title
		SDL_WINDOWPOS_CENTERED,     // x position, centered
		SDL_WINDOWPOS_CENTERED,     // y position, centered
		width,                        // width, in pixels
		height,                        // height, in pixels
		SDL_WINDOW_OPENGL           // flags
		);
}



void CleanUp()
{
	auto iter = displayList.begin();
	while (iter != displayList.end())
	{
		(*iter)->destroy();
		if ((*iter))
		{
			delete (*iter);
			(*iter) = NULL;
			iter = displayList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	displayList.clear();

	// clean up, reverse order!!!
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}



//Function to initialise OpenGL
void initOpenGL()
{
	//Ask for version 3.2 of OpenGL

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Create OpenGL Context
	glcontext = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
	}

	//Smooth shading
	glShadeModel(GL_SMOOTH);

	//clear the background to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Clear the depth buffer
	glClearDepth(1.0f);

	//Enable depth testing
	glEnable(GL_DEPTH_TEST);

	//The depth test to go
	glDepthFunc(GL_LEQUAL);

	//Turn on best perspective correction
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

//Function to set/reset viewport
void setViewport(int width, int height)
{

	//make sure height is always above 1
	if (height == 0) {
		height = 1;
	}


	//Setup viewport
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void createSkyBox()
{
	Vertex triangleData[] = {
			{ vec3(-10.0f, 10.0f, 10.0f) },// Top Left
			{ vec3(-10.0f, -10.0f, 10.0f) },// Bottom Left
			{ vec3(10.0f, -10.0f, 10.0f) }, //Bottom Right
			{ vec3(10.0f, 10.0f, 10.0f) },// Top Right


			//back
			{ vec3(-10.0f, 10.0f, -10.0f) },// Top Left
			{ vec3(-10.0f, -10.0f, -10.0f) },// Bottom Left
			{ vec3(10.0, -10.0f, -10.0f) }, //Bottom Right
			{ vec3(10.0f, 10.0f, -10.0f) }// Top Right
	};


	GLuint indices[] = {
		//front
		0, 1, 2,
		0, 3, 2,

		//left
		4, 5, 1,
		4, 1, 0,

		//right
		3, 7, 2,
		7, 6, 2,

		//bottom
		1, 5, 2,
		6, 2, 1,

		//top
		5, 0, 7,
		5, 7, 3,

		//back
		4, 5, 6,
		4, 7, 6
	};

	//creat mesh and copy in

	Mesh * pMesh = new Mesh();
	pMesh->init();

	pMesh->copyVertexData(8, sizeof(Vertex), (void**)triangleData);
	pMesh->copyIndexData(36, sizeof(int), (void**)indices);

	Transform *t = new Transform();
	t->setPosition(0.0f, 0.0f, 0.0f);
	//load textures and skybox material + Shaders
	SkyBoxMaterial *material = new SkyBoxMaterial();
	material->init();

	std::string vsPath = ASSET_PATH + SHADER_PATH + "/skyVS.glsl";
	std::string fsPath = ASSET_PATH + SHADER_PATH + "/skyFS.glsl";
	material->loadShader(vsPath, fsPath);

	std::string posZTexturename = ASSET_PATH + TEXTURE_PATH + "StarField2048.png";
	std::string negZTexturename = ASSET_PATH + TEXTURE_PATH + "StarField2048.png";
	std::string posXTexturename = ASSET_PATH + TEXTURE_PATH + "StarField2048.png";
	std::string negXTexturename = ASSET_PATH + TEXTURE_PATH + "StarField2048.png";
	std::string posYTexturename = ASSET_PATH + TEXTURE_PATH + "StarField2048.png";
	std::string negYTexturename = ASSET_PATH + TEXTURE_PATH + "StarField2048.png";

	material->loadCubeTexture(posZTexturename, negZTexturename, posXTexturename, negXTexturename, posYTexturename, negYTexturename);
	//create gameobject but don't add to queue!
	skyBox = new GameObject();
	skyBox->setMaterial(material);
	skyBox->setTransform(t);
	skyBox->setMesh(pMesh);
}
	
void Initialise()
{
	std::string vsPath = ASSET_PATH + SHADER_PATH + "/DirectionalLightTextureVS.glsl";
	std::string fsPath = ASSET_PATH + SHADER_PATH + "/DirectionalLightTextureFS.glsl";
	createSkyBox();


	mainCamera = new GameObject();
	mainCamera->setName("MainCamera");

	Transform *t = new Transform();
	t->setPosition(0.0f, 0.0f, 2.0f);
	mainCamera->setTransform(t);

	Camera * c = new Camera();
	c->setAspectRatio((float)(WINDOW_WIDTH / WINDOW_HEIGHT));
	c->setFOV(45.0f);
	c->setNearClip(0.1f);
	c->setFarClip(1000.0f);

	mainCamera->setCamera(c);
	displayList.push_back(mainCamera);

	mainLight = new GameObject();
	mainLight->setName("MainLight");

	t = new Transform();
	t->setPosition(0.0f, 0.0f, 0.0f);
	mainLight->setTransform(t);

	Light * light = new Light();
	mainLight->setLight(light);
	displayList.push_back(mainLight);

	//alternative sytanx
	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		(*iter)->init();
	}

	//List of models being used in the scene
	//Will want to put all of these into an array for convience
	std::string stationModel = ASSET_PATH + MODEL_PATH + "station.fbx";
	std::string sunModel = ASSET_PATH + MODEL_PATH + "sphere.fbx";
	std::string earthModel = ASSET_PATH + MODEL_PATH + "sphere.fbx";
	std::string moonModel = ASSET_PATH + MODEL_PATH + "sphere.fbx";
	std::string shipModel = ASSET_PATH + MODEL_PATH + "newShip.fbx";
	std::string gateModel = ASSET_PATH + MODEL_PATH + "gate.fbx";
	std::string satelliteModel = ASSET_PATH + MODEL_PATH + "satellite.fbx";

	//Model and Texture for the space station
	GameObject * go = loadFBXFromFile(stationModel);
	for (int i = 0; i < go->getChildCount(); i++)
	{
		Material * material = new Material();
		material->init();
		material->loadShader(vsPath, fsPath);
		std::string diffTexturePath = ASSET_PATH + TEXTURE_PATH + "/station_diff.png";
		material->loadDiffuseMap(diffTexturePath);
		std::string bumpTexturePath = ASSET_PATH + TEXTURE_PATH + "/station_norm.png";
		material->loadBumpMap(bumpTexturePath);
		go->getChild(i)->setMaterial(material);
	}
	go->getTransform()->setPosition(0.0f, 0.0f, -600.0f);
	displayList.push_back(go);

	//Model and texture for Gate 1
	go = loadFBXFromFile(gateModel);
	for (int i = 0; i < go->getChildCount(); i++)
	{
		Material * material = new Material();
		material->init();
		material->loadShader(vsPath, fsPath);
		std::string diffTexturePath = ASSET_PATH + TEXTURE_PATH + "/gate_diff.png";
		material->loadDiffuseMap(diffTexturePath);
		std::string bumpTexturePath = ASSET_PATH + TEXTURE_PATH + "/gate_norm.png";
		material->loadBumpMap(bumpTexturePath);
		go->getChild(i)->setMaterial(material);
	}
	go->getTransform()->setPosition(80.0f, 0.0f, -300.0f);
	go->getTransform()->setRotation(0.0f, 0.0f, 0.0f);
	displayList.push_back(go);

	//Model and texture for Gate 2
	go = loadFBXFromFile(gateModel);
	for (int i = 0; i < go->getChildCount(); i++)
	{
		Material * material = new Material();
		material->init();
		material->loadShader(vsPath, fsPath);
		std::string diffTexturePath = ASSET_PATH + TEXTURE_PATH + "/gate_diff.png";
		material->loadDiffuseMap(diffTexturePath);
		std::string bumpTexturePath = ASSET_PATH + TEXTURE_PATH + "/gate_norm.png";
		material->loadBumpMap(bumpTexturePath);
		go->getChild(i)->setMaterial(material);
	}
	go->getTransform()->setPosition(-80.0f, 0.0f, -300.0f);
	go->getTransform()->setRotation(0.0f, 0.0f, 0.0f);
	displayList.push_back(go);

	//Model and texture for the satellite
	go = loadFBXFromFile(satelliteModel);
	for (int i = 0; i < go->getChildCount(); i++)
	{
		Material * material = new Material();
		material->init();
		material->loadShader(vsPath, fsPath);
		std::string diffTexturePath = ASSET_PATH + TEXTURE_PATH + "/satellite_diff.png";
		material->loadDiffuseMap(diffTexturePath);
		std::string bumpTexturePath = ASSET_PATH + TEXTURE_PATH + "/satellite_norm.png";
		material->loadBumpMap(bumpTexturePath);
		go->getChild(i)->setMaterial(material);
	}
	go->getTransform()->setPosition(5.0f, 0.0f, -50.0f);
	go->getTransform()->setRotation(0.0f, 45.0f, 0.0f);
	go->getTransform()->setScale(0.01f, 0.01f, 0.01f);
	displayList.push_back(go);

	//Model and texture for the space ship1
	go = loadFBXFromFile(shipModel);
	for (int i = 0; i < go->getChildCount(); i++)
	{
		Material * material = new Material();
		material->init();
		material->loadShader(vsPath, fsPath);
		std::string diffTexturePath = ASSET_PATH + TEXTURE_PATH + "/ship5_diff.png";
		material->loadDiffuseMap(diffTexturePath);
		go->getChild(i)->setMaterial(material);
	}
	go->getTransform()->setPosition(0.0f, -5.0f, -70.0f);
	go->getTransform()->setRotation(90.0f, 0.0f, 0.0f);
	go->getTransform()->setScale(0.002f, 0.002f, 0.002f);
	displayList.push_back(go);

	//Model and Texture for sun model
	go = loadFBXFromFile(sunModel);
	for (int i = 0; i < go->getChildCount(); i++)
	{
		Material * material = new Material();
		material->init();
		material->loadShader(vsPath, fsPath);
		std::string diffTexturePath = ASSET_PATH + TEXTURE_PATH + "/sun1_diff.png";
		material->loadDiffuseMap(diffTexturePath);
		go->getChild(i)->setMaterial(material);
	}
	go->getTransform()->setPosition(0.0f, 200.0f, -1000.0f);
	go->getTransform()->setRotation(0.0f, 90.0f, 0.0f);
	go->getTransform()->setScale(0.5f, 0.5f, 0.5f);
	displayList.push_back(go);

	//Model and texture for the earth model
	go = loadFBXFromFile(earthModel);
	for (int i = 0; i < go->getChildCount(); i++)
	{
		Material * material = new Material();
		material->init();
		material->loadShader(vsPath, fsPath);
		std::string diffTexturePath = ASSET_PATH + TEXTURE_PATH + "/earth_diff.png";
		material->loadDiffuseMap(diffTexturePath);
		std::string bumpTexturePath = ASSET_PATH + TEXTURE_PATH + "/earth_norm.png";
		material->loadBumpMap(bumpTexturePath);
		go->getChild(i)->setMaterial(material);
		go->setName("earth");
	}
	go->getTransform()->setPosition(-300.0f, -300.0f, -1000.0f);
<<<<<<< HEAD
=======
	go->getTransform()->setRotation(earthX, earthY, earthZ);
>>>>>>> origin/master
	go->getTransform()->setScale(0.6f, 0.6f, 0.6f);
	displayList.push_back(go);

	//Model and texture for the moon model
	go = loadFBXFromFile(moonModel);
	for (int i = 0; i < go->getChildCount(); i++)
	{
		Material * material = new Material();
		material->init();
		material->loadShader(vsPath, fsPath);
		std::string diffTexturePath = ASSET_PATH + TEXTURE_PATH + "/moon_diff.png";
		material->loadDiffuseMap(diffTexturePath);
		std::string bumpTexturePath = ASSET_PATH + TEXTURE_PATH + "/moon_norm.png";
		material->loadBumpMap(bumpTexturePath);
		go->getChild(i)->setMaterial(material);
	}
	go->getTransform()->setPosition(-150.0f, -200.0f, -1000.0f);
	go->getTransform()->setRotation(40.0f, 0.0f, 0.0f);
	go->getTransform()->setScale(0.08f, 0.08f, 0.08f);
	displayList.push_back(go);
}




//Function to update the game state
void update()
{
	skyBox->update();

	//alternative sytanx
	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		(*iter)->update();
	}
	
	//Update camera position
	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		if ((*iter)->getName == "MainCamera")
		{
			(*iter)->getTransform()->setPosition(cameraX, cameraY, cameraZ);
		}
	}
}

void renderGameObject(GameObject * pObject)
{
	if (!pObject)
		return;

	pObject->render();

	Mesh * currentMesh = pObject->getMesh();
	Transform * currentTransform = pObject->getTransform();
	Material * currentMaterial = (Material*)pObject->getMaterial();

	if (currentMesh && currentMaterial && currentTransform)
	{
		currentMaterial->bind();
		currentMesh->bind();

		GLint MVPLocation = currentMaterial->getUniformLocation("MVP");
		GLint ModelLocation = currentMaterial->getUniformLocation("Model");
		GLint ambientMatLocation = currentMaterial->getUniformLocation("ambientMaterialColour");
		GLint ambientLightLocation = currentMaterial->getUniformLocation("ambientLightColour");
		GLint diffuseMatLocation = currentMaterial->getUniformLocation("diffuseMaterialColour");
		GLint diffuseLightLocation = currentMaterial->getUniformLocation("diffuseLightColour");
		GLint lightDirectionLocation = currentMaterial->getUniformLocation("lightDirection");
		GLint specularMatLocation = currentMaterial->getUniformLocation("specularMaterialColour");
		GLint specularLightLocation = currentMaterial->getUniformLocation("specularLightColour");
		GLint specularpowerLocation = currentMaterial->getUniformLocation("specularPower");
		GLint cameraPositionLocation = currentMaterial->getUniformLocation("cameraPosition");
		GLint diffuseTextureLocation = currentMaterial->getUniformLocation("diffuseMap");
		GLint specTextureLocation = currentMaterial->getUniformLocation("specMap");
		GLint bumpTextureLocation = currentMaterial->getUniformLocation("bumpMap");
		Camera * cam = mainCamera->getCamera();
		Light* light = mainLight->getLight();


		mat4 MVP = cam->getProjection()*cam->getView()*currentTransform->getModel();
		mat4 Model = currentTransform->getModel();

		vec4 ambientMaterialColour = currentMaterial->getAmbientColour();
		vec4 diffuseMaterialColour = currentMaterial->getDiffuseColour();
		vec4 specularMaterialColour = currentMaterial->getSpecularColour();
		float specularPower = currentMaterial->getSpecularPower();

		vec4 diffuseLightColour = light->getDiffuseColour();
		vec4 specularLightColour = light->getSpecularColour();
		vec3 lightDirection = light->getDirection();

		vec3 cameraPosition = mainCamera->getTransform()->getPosition();

		glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(Model));
		glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));
		glUniform4fv(ambientMatLocation, 1, glm::value_ptr(ambientMaterialColour));
		glUniform4fv(ambientLightLocation, 1, glm::value_ptr(ambientLightColour));

		glUniform4fv(diffuseMatLocation, 1, glm::value_ptr(diffuseMaterialColour));
		glUniform4fv(diffuseLightLocation, 1, glm::value_ptr(diffuseLightColour));
		glUniform3fv(lightDirectionLocation, 1, glm::value_ptr(lightDirection));

		glUniform4fv(specularMatLocation, 1, glm::value_ptr(specularMaterialColour));
		glUniform4fv(specularLightLocation, 1, glm::value_ptr(specularLightColour));

		glUniform3fv(cameraPositionLocation, 1, glm::value_ptr(cameraPosition));
		glUniform1f(specularpowerLocation, specularPower);

		glUniform1i(diffuseTextureLocation, 0);
		glUniform1i(specTextureLocation, 1);
		glUniform1i(bumpTextureLocation, 2);

		glDrawElements(GL_TRIANGLES, currentMesh->getIndexCount(), GL_UNSIGNED_INT, 0);
	}

	for (int i = 0; i < pObject->getChildCount(); i++)
	{
		renderGameObject(pObject->getChild(i));
	}
}

void renderSkyBox()
{
	skyBox->render();

	Mesh * currentMesh = skyBox->getMesh();
	SkyBoxMaterial * currentMaterial = (SkyBoxMaterial*)skyBox->getMaterial();
	if (currentMesh && currentMaterial)
	{
		Camera * cam = mainCamera->getCamera();

		currentMaterial->bind();
		currentMesh->bind();

		GLint cameraLocation = currentMaterial->getUniformLocation("cameraPos");
		GLint viewLocation = currentMaterial->getUniformLocation("view");
		GLint projectionLocation = currentMaterial->getUniformLocation("projection");
		GLint cubeTextureLocation = currentMaterial->getUniformLocation("cubeTexture");

		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(cam->getProjection()));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(cam->getView()));
		glUniform4fv(cameraLocation, 1, glm::value_ptr(mainCamera->getTransform()->getPosition()));
		glUniform1i(cubeTextureLocation, 0);

		glDrawElements(GL_TRIANGLES, currentMesh->getIndexCount(), GL_UNSIGNED_INT, 0);

		currentMaterial->unbind();
	}
	CheckForErrors();
}

//Function to render(aka draw)
void render()
{
	//old imediate mode!
	//Set the clear colour(background)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	//clear the colour and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderSkyBox();

	//alternative sytanx
	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		renderGameObject((*iter));
	}

	SDL_GL_SwapWindow(window);
}

void rotate()
{
<<<<<<< HEAD
	if (earthY < 360.0f)
	{
		earthY++;
	}
	else
	{
		earthY = 0.0f;
	}

	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		if ((*iter)->getName() == "earth")
		{
			(*iter)->getTransform()->setRotation(earthX, earthY, earthZ);
		}
	}
}
=======
	if (earthX < 360.0f)
	{
		earthX++;
	}
	else
	{
		earthX = 0.0f;
	}
	
	if (earthY < 360.0f)
	{
		earthY++;

	}
	else
	{
		earthY = 0.0f;
	}
>>>>>>> origin/master

	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		if ((*iter)->getName() == "earth")
		{
			(*iter)->getTransform()->setRotation(earthX, earthY, earthZ);
		}
	}
}
	
//Main Method
int main(int argc, char * arg[])
{
	// init everyting - SDL, if it is nonzero we have a problem
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "ERROR SDL_Init " << SDL_GetError() << std::endl;

		return -1;
	}

	int imageInitFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	int returnInitFlags = IMG_Init(imageInitFlags);
	if (((returnInitFlags)& (imageInitFlags)) != imageInitFlags) {
		std::cout << "ERROR SDL_Image Init " << IMG_GetError() << std::endl;
		// handle error
	}

	if (TTF_Init() == -1) {
		std::cout << "TTF_Init: " << TTF_GetError();
	}

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false);
	//Call our InitOpenGL Function
	initOpenGL();
	CheckForErrors();
	//Set our viewport
	setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);

	Initialise();

	//Value to hold the event generated by SDL
	SDL_Event event;
	//Game Loop
	while (running)
	{
		//While we still have events in the queue
		while (SDL_PollEvent(&event)) {
			//Get event type
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				//set our boolean which controls the loop to false
				running = false;
			}
		}
		
		
		rotate();	
		update();
		rotate();
		//render
		render();


	}


	CleanUp();

	return 0;
}