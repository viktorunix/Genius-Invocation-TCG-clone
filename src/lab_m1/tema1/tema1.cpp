#include "lab_m1/tema1/tema1.hpp"
#include "lab_m1/tema1/trans2D.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

Tema1::Tema1() {

}

Tema1::~Tema1() {

}

void Tema1::Init() {
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);


	Mesh* card = new Mesh("card");
	card->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "card.obj");
	meshes[card->GetMeshID()] = card;
}

void Tema1::FrameStart() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);


}

void Tema1::Update(float deltaTimeSeconds) {
	glm::mat3 modelMatrix = glm::mat3(1);

	modelMatrix *= trans2D::Translate(500, 500);
	modelMatrix *= trans2D::Rotate(glm::radians(90.0f));
	modelMatrix *= trans2D::Scale(200, 200);

	RenderMesh2D(meshes["card"],shaders["Simple"], modelMatrix);
	
}

void Tema1::FrameEnd() {
	//DrawCoordinateSystem();
}

void Tema1::OnInputUpdate(float deltaTime, int mods) {

}
void Tema1::OnKeyPress(int key, int mods) {

}
void Tema1::OnKeyRelease(int key, int mods) {

}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {

}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {

}
void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {

}
void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {

}

void Tema1::OnWindowResize(int width, int height) {

}