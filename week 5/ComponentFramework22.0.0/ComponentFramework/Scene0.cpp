#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene0.h"
#include "MMath.h"
#include "Debug.h"
#include "Actor.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

Scene0::Scene0() :sphere(nullptr), shader{ nullptr }, mesh{ nullptr }, earthTexture{ nullptr }, moonTexture{ nullptr }  {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene0::~Scene0() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
}

bool Scene0::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);
	sphere = new Actor(nullptr);
	sphere->OnCreate();

	mesh = new Mesh(nullptr, "meshes/Sphere.obj");
	mesh->OnCreate();

	shader = new Shader(nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	shader->OnCreate();
	if (shader->OnCreate() == false){
		std::cout << "Shader Failed..";
	}

	earthTexture = new Texture();
	earthTexture->LoadImage("textures/earthclouds.jpg");


	moonTexture = new Texture();
	moonTexture->LoadImage("textures/moon.jpg");

	projectionMatrix = MMath::perspective(45.0f, (16.0f/9.0f), 0.5f, 100.f );
	viewMatrix = MMath::lookAt(Vec3(3.0f, -2.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	//modelMatrix.loadIdentity();
	lightPos = Vec3(20.0f, 0.0f, -5.0f);
	return true;
}

void Scene0::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);
	
	sphere->OnDestroy();
	delete sphere;

	shader->OnDestroy();
	delete shader;

	mesh->OnDestroy();
	delete mesh;

	delete earthTexture;
	delete moonTexture;

}

void Scene0::HandleEvents(const SDL_Event &sdlEvent) {
	switch( sdlEvent.type ) {
    case SDL_KEYDOWN:
		break;

	case SDL_MOUSEMOTION:          
		break;

	case SDL_MOUSEBUTTONDOWN:              
		break; 

	case SDL_MOUSEBUTTONUP:            
	break;

	default:
		break;
    }
}

void Scene0::Update(const float deltaTime) {
	static float totalTime = 0.0f;
	totalTime += deltaTime;
	earthMatrix = MMath::rotate(totalTime * 4.0f, Vec3(0.0, 1.0f, 0.0f)) *
				  MMath::rotate(-90.0f, Vec3(1.0f, 0.0f, 0.0f));

	moonMatrix =	MMath::rotate(totalTime * 4.0f, Vec3(0.0, 1.0f, 0.0f)) *
					MMath::translate(Vec3(0.0f, 0.0f, 6.0f)) *
					MMath::scale(Vec3(0.27f, 0.27f , 0.27f)) *
					MMath::rotate(totalTime * 50.0f  + 90.0f, Vec3(0.0f, 1.0f, 0.0f));
	
}

void Scene0::Render() const {
	glEnable(GL_DEPTH_TEST);
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
	glUniform3fv(shader->GetUniformID("lightPos"), 1, lightPos);
	

	glBindTexture(GL_TEXTURE_2D, earthTexture->getTextureID());
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, earthMatrix);
	mesh->Render(GL_TRIANGLES);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, moonTexture->getTextureID());
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, moonMatrix);
	mesh->Render(GL_TRIANGLES);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}



	
