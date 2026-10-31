#include "lab_m1/tema1/tema1.hpp"
#include "lab_m1/tema1/trans2D.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace m1;
Player::Player(int nrDices) {
	for (int i = 0; i < nrDices; i++) {
		Dice* dice = new Dice();
		dice->element = Omni;
		this->dices.push_back(dice);
	}
	this->dices[2]->element = Pyro;
	this->dices[3]->element = Cryo;
	this->dices[4]->element = Hydro;
}
Timer::Timer(float maxTime) {
	this->maxTime = maxTime;
	this->currentTime = this->maxTime;
	this->modelMatrix = glm::mat3(1);
}
Mesh* Timer::CreateTimerBar(const char* name, glm::vec3 color, float z = 0.0f) {
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
Mesh* CreatePlayArena(const char* name, glm::vec3 color, float z = 0.0f) {
	std::vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(0,0,z), color),
		VertexFormat(glm::vec3(1,0,z), color),
		VertexFormat(glm::vec3(1, 0.5f,z), color),
		VertexFormat(glm::vec3(0, 0.5f, z), color)
	};
	std::vector<unsigned int> indices = { 0,1,2,0,2,3 };
	Mesh* playArenaMesh = new Mesh(name);
	playArenaMesh->InitFromData(vertices, indices);
	return playArenaMesh;
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

void Tema1::CreateDiceMesh(const char* id, glm::vec3 color) {
	Mesh* mesh = new Mesh(id);
	mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "hexagon.obj");

	std::vector<VertexFormat> coloredVertices;
	for (int i = 0; i < mesh->positions.size(); i++) {
		coloredVertices.push_back(VertexFormat(mesh->positions[i], color));

	}
	mesh->InitFromData(coloredVertices,mesh->indices);
	meshes[mesh->GetMeshID()] = mesh;


}
Card::Card() {
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

	Mesh* playArenaMesh = CreatePlayArena("playerPlayArena", glm::vec3(0.5f, 0.1f, 0.1f));
	meshes[playArenaMesh->GetMeshID()] = playArenaMesh;

	card = new Card();
	CreateCardMesh(card, "card1", glm::vec3(0.7f,0.0f, 0.0f));
	card2 = new Card();
	CreateCardMesh(card2, "card2", glm::vec3(0.0f, 0.9f, 0.5f));

	timer = new Timer(30.0f);
	Mesh* timerBg = timer->CreateTimerBar("timer_bg", glm::vec3(0.2f),0.0f);
	meshes[timerBg->GetMeshID()] = timerBg;
	Mesh* timerFg = timer->CreateTimerBar("timer_fg", glm::vec3(0.0, 0.0f, 0.5f),0.01f);
	meshes[timerFg->GetMeshID()] = timerFg;

	Mesh* heart = new Mesh("heart");
	heart->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "heart.obj");
	meshes[heart->GetMeshID()] = heart;


	CreateDiceMesh("dice_omni", glm::vec3(0.5f, 0.5f, 0.5f));
	CreateDiceMesh("dice_pyro", glm::vec3(0.843f, 0.322f, 0.220f));
	CreateDiceMesh("dice_cryo", glm::vec3(0.773f, 0.984f, 0.984f));
	CreateDiceMesh("dice_hydro", glm::vec3(0.290f, 0.737f, 0.910f));

	player = new Player(5);
}

void Tema1::FrameStart() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);


}

void Tema1::RenderCardStacks() {
	for (int i = 0; i < 25; i += 5) {
		card->modelMatrix = glm::mat3(1);
		card->modelMatrix *= trans2D::Translate(1100 - i, 200 + i);
		card->modelMatrix *= trans2D::Rotate(glm::radians(90.0f));
		card->modelMatrix *= trans2D::Scale(100, 100);
		RenderMesh2D(card->mesh, shaders["VertexColor"], card->modelMatrix);
	}
	for (int i = 0; i < 25; i += 5) {
		card2->modelMatrix = glm::mat3(1);
		card2->modelMatrix *= trans2D::Translate(1100 - i, 600 + i);
		card2->modelMatrix *= trans2D::Rotate(glm::radians(90.0f));
		card2->modelMatrix *= trans2D::Scale(100, 100);
		RenderMesh2D(card2->mesh, shaders["VertexColor"], card2->modelMatrix);
	}
}

void Tema1::RenderTimer(float deltaTimeSeconds) {
	timer->currentTime -= deltaTimeSeconds;
	if (timer->currentTime < 0) {
		timer->currentTime = 0;
	}
	float scaleFactorY = timer->currentTime / timer->maxTime;

	glm::mat3 modelMatrix = glm::mat3(1);
	modelMatrix *= trans2D::Translate(950.0f, 300.0f);
	RenderMesh2D(meshes["timer_bg"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= trans2D::Translate(950.0f, 300.0f);
	modelMatrix *= trans2D::Scale(1, scaleFactorY);
	RenderMesh2D(meshes["timer_fg"], shaders["VertexColor"], modelMatrix);
}

void Tema1::RenderPlayArena() {
	glm::ivec2 resolution = window->GetResolution();
	float dynamicScaleX = resolution.x * 0.80f;
	float dynamicScaleY = resolution.y * 0.20f * 2.0f; //cuz the mesh height is 0.5f;
	float dynamicSpacing = dynamicScaleY * 2.1f;
	float offsetLeft = resolution.x * 0.1f;

	float posX = offsetLeft;
	float posY = resolution.y * 0.25f;

	glm::mat3 modelMatrix = glm::mat3(1);
	modelMatrix *= trans2D::Translate(posX, posY);
	modelMatrix *= trans2D::Scale(dynamicScaleX, dynamicScaleY);

	RenderMesh2D(meshes["playerPlayArena"], shaders["VertexColor"], modelMatrix);

	posY = resolution.y * 0.55f;
	modelMatrix = glm::mat3(1);
	modelMatrix *= trans2D::Translate(posX, posY);
	modelMatrix *= trans2D::Scale(dynamicScaleX, dynamicScaleY);
	RenderMesh2D(meshes["playerPlayArena"], shaders["VertexColor"], modelMatrix);

}

void Tema1::RenderDices() {
	glm::ivec2 resolution = window->GetResolution();
	float dynamicScale = resolution.y * 0.055f;
	float dynamicSpacing = dynamicScale * 2.25f;
	float offsetRight = resolution.x * 0.04f;
	glm::mat3 modelMatrix;
	for (int i = 0; i < player->dices.size(); i++) {
		modelMatrix = glm::mat3(1);

		float posX = (float)resolution.x - offsetRight;
		float posY = (float)resolution.y - dynamicSpacing * (i + 1);
		modelMatrix *= trans2D::Translate(posX, posY);
		modelMatrix *= trans2D::Scale(dynamicScale, dynamicScale);
		switch (player->dices[i]->element) {
		case Omni:
			RenderMesh2D(meshes["dice_omni"], shaders["VertexColor"], modelMatrix);
			break;
		case Pyro:
			RenderMesh2D(meshes["dice_pyro"], shaders["VertexColor"], modelMatrix);
			break;
		case Cryo:
			RenderMesh2D(meshes["dice_cryo"], shaders["VertexColor"], modelMatrix);
			break;
		case Hydro:
			RenderMesh2D(meshes["dice_hydro"], shaders["VertexColor"], modelMatrix);
			
		}
	}
}
void Tema1::Update(float deltaTimeSeconds) {

	glm::ivec2 resolution = window->GetResolution();
	glm::mat3 modelMatrix = glm::mat3(1);
	RenderDices();


	RenderPlayArena();
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
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)width, 0, (float)height, 0.01f, 400);
	camera->Update();
}