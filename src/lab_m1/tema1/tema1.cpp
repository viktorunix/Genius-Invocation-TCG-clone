#include "lab_m1/tema1/tema1.hpp"
#include "lab_m1/tema1/trans2D.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace m1;
Timer::Timer(float maxTime) {
	this->maxTime = maxTime;
	this->currentTime = this->maxTime;
}
Mesh* CreateTimerBar(const char* name, glm::vec3 color, float z = 0.0f) {
	std::vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(0,0,z), color),
		VertexFormat(glm::vec3(40,0,z), color),
		VertexFormat(glm::vec3(40, 250, z), color),
		VertexFormat(glm::vec3(0, 250, z), color)
	};
	std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3 };
	Mesh* bar = new Mesh(name);
	bar->InitFromData(vertices, indices);
	return bar;
}
Tema1::Tema1() {

}

Tema1::~Tema1() {

}
void Tema1::CreateCardMesh(Card* card,const char* id, glm::vec3 color) {
	card->mesh = new Mesh(id);
	card->mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "card.obj");


	std::vector<VertexFormat> coloredVertices;
	for (int i = 0; i < card->mesh->positions.size(); i++) {
		coloredVertices.push_back(VertexFormat(card->mesh->positions[i], color));
	}

	card->mesh->InitFromData(coloredVertices, card->mesh->indices);

	meshes[card->mesh->GetMeshID()] = card->mesh;
}
Card::Card() {
	this->hp = 10;
	this->modelMatrix = glm::mat3(1);
}

void Tema1::Init() {
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);


	/*Mesh* card = new Mesh("card");
	card->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "card.obj");
	meshes[card->GetMeshID()] = card;
	*/
	//card = new Card();
	card = new Card();
	CreateCardMesh(card, "card1", glm::vec3(0.7f,0.0f, 0.0f));
	card2 = new Card();
	CreateCardMesh(card2, "card2", glm::vec3(0.0f, 0.9f, 0.5f));

	timer = new Timer(30.0f);
	//timer->maxTime = 30.0f;
	//timer->currentTime = timer->maxTime;
	Mesh* timerBg = CreateTimerBar("timer_bg", glm::vec3(0.2f),0.0f);
	meshes[timerBg->GetMeshID()] = timerBg;
	Mesh* timerFg = CreateTimerBar("timer_fg", glm::vec3(0.0, 0.0f, 0.5f),0.01f);
	meshes[timerFg->GetMeshID()] = timerFg;
}

void Tema1::FrameStart() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);


}
void Tema1::RenderCardStacks() {
	//ChangeCardColor(card, glm::vec3(0.8f, 0, 0));
	for (int i = 0; i < 25; i += 5) {
		card->modelMatrix = glm::mat3(1);
		card->modelMatrix *= trans2D::Translate(1100 - i, 200 + i);
		card->modelMatrix *= trans2D::Rotate(glm::radians(90.0f));
		card->modelMatrix *= trans2D::Scale(100, 100);
		RenderMesh2D(card->mesh, shaders["VertexColor"], card->modelMatrix);
	}
	//ChangeCardColor(card, glm::vec3(0.0f, 0.7f, 0.0f));
	for (int i = 0; i < 25; i += 5) {
		card2->modelMatrix = glm::mat3(1);
		card2->modelMatrix *= trans2D::Translate(1100 - i, 600 + i);
		card2->modelMatrix *= trans2D::Rotate(glm::radians(90.0f));
		card2->modelMatrix *= trans2D::Scale(100, 100);
		RenderMesh2D(card2->mesh, shaders["VertexColor"], card2->modelMatrix);
	}
}
void Tema1::Update(float deltaTimeSeconds) {
	//glm::mat3 modelMatrix = glm::mat3(1);

	//modelMatrix *= trans2D::Translate(300, 300);
	//modelMatrix *= trans2D::Rotate(glm::radians(90.0f));
	//modelMatrix *= trans2D::Scale(100, 100);

	//RenderMesh2D(meshes["card"],shaders["Simple"], modelMatrix);
	/*card->modelMatrix = glm::mat3(1);
	card->modelMatrix *= trans2D::Translate(300, 300);
	card->modelMatrix *= trans2D::Rotate(glm::radians(90.0f));
	card->modelMatrix *= trans2D::Scale(100, 100);
	RenderMesh2D(card->mesh, shaders["Simple"], card->modelMatrix);
	card2->modelMatrix = glm::mat3(1);
	card2->modelMatrix *= trans2D::Translate(280, 320);
	card2->modelMatrix *= trans2D::Rotate(glm::radians(90.0f));
	card2->modelMatrix *= trans2D::Scale(100, 100);
	//RenderMesh2D(card2->mesh, shaders["Simple"], card2->modelMatrix);
	RenderMesh2D(card->mesh, shaders["Simple"], card2->modelMatrix);
	*/
	RenderCardStacks();

	timer->currentTime -= deltaTimeSeconds;
	if (timer->currentTime < 0) {
		timer->currentTime = 0;
	}
	float scaleFactorY = timer->currentTime / timer->maxTime;

	glm::mat3 modelMatrix = glm::mat3(1);
	modelMatrix *= trans2D::Translate(800, 300.0f);
	RenderMesh2D(meshes["timer_bg"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= trans2D::Translate(800, 300);
	modelMatrix *= trans2D::Scale(1, scaleFactorY);
	RenderMesh2D(meshes["timer_fg"], shaders["VertexColor"], modelMatrix);
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