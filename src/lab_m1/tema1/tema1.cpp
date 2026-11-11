#include "lab_m1/tema1/tema1.hpp"
#include "lab_m1/tema1/trans2D.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

void Tema1::OrganizeHand(std::vector<Card*>& handCards) {
	glm::ivec2 res = window->GetResolution();
	float cardWidth = 100.0f;

	float dynamicWidth = res.x * 0.08f;
	float dynamicHeight = dynamicWidth * 1.5f;
	float spacing = res.x * 0.015f;

	int cardsInHand = 0;
	for (Card* c : handCards) {
		if (c->currentZone == CardZone::HAND) {
			cardsInHand++;

			c->width = dynamicWidth;
			c->height = dynamicHeight;
		}
	}
	if (cardsInHand == 0) return;




	//starting x to be in the center
	float totalWidth = (cardsInHand * dynamicWidth) + ((cardsInHand - 1) * spacing);
	float startX = (res.x - totalWidth) / 2.0f;
	float fixedY = res.y * 0.05f;

	int displayIndex = 0;
	for (size_t i = 0; i < handCards.size(); i++) {
		if (handCards[i]->currentZone == CardZone::HAND && !handCards[i]->isDragging) {
			handCards[i]->x = startX + displayIndex * (dynamicWidth + spacing);
			handCards[i]->y = fixedY;
			displayIndex++;
		}
	}

}
Player::Player(int nrDices) {
	for (int i = 0; i < nrDices; i++) {
		Dice* dice = new Dice();
		dice->element = Omni;
		this->dices.push_back(dice);
	}
	this->dices[2]->element = Hydro;
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
	float dynamicWidth = resolution.x * 0.08f;
	float dynamicHeight = dynamicWidth * 1.5f;

	float slotSpacing = dynamicWidth * 1.2f; 
	float boardCenterX = resolution.x / 2.0f;
	float startX = boardCenterX - slotSpacing - (dynamicWidth / 2.0f);
	float boardY = (resolution.y * 0.25f) + (resolution.y * 0.05f); 
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	Mesh* playArenaMesh = CreatePlayArena("playerPlayArena", glm::vec3(0.5f, 0.1f, 0.1f));
	meshes[playArenaMesh->GetMeshID()] = playArenaMesh;

	Mesh* playerArena = CreatePlayArena("playerPlayArena", glm::vec3(0.15f, 0.25f, 0.35f)); // Deep Blue
	meshes[playerArena->GetMeshID()] = playerArena;

	Mesh* enemyArena = CreatePlayArena("enemyPlayArena", glm::vec3(0.35f, 0.15f, 0.15f)); // Deep Red
	meshes[enemyArena->GetMeshID()] = enemyArena;

	timer = new Timer(30.0f);
	Mesh* timerBg = timer->CreateTimerBar("timer_bg", glm::vec3(0.2f),0.0f);
	meshes[timerBg->GetMeshID()] = timerBg;
	Mesh* timerFg = timer->CreateTimerBar("timer_fg", glm::vec3(0.0, 0.0f, 0.5f),0.01f);
	meshes[timerFg->GetMeshID()] = timerFg;


	Mesh* heart = new Mesh("heart");
	heart->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "heart.obj");
	std::vector<VertexFormat> coloredHeartVertices;
	for (int i = 0; i < heart->positions.size(); i++) {
		coloredHeartVertices.push_back(VertexFormat(heart->positions[i], glm::vec3(1.0f, 0.1f, 0.2f)));

	}
	heart->InitFromData(coloredHeartVertices, heart->indices);
	meshes[heart->GetMeshID()] = heart;


	CreateDiceMesh("dice_omni", glm::vec3(0.5f, 0.5f, 0.5f));
	CreateDiceMesh("dice_pyro", glm::vec3(0.843f, 0.322f, 0.220f));
	CreateDiceMesh("dice_cryo", glm::vec3(0.773f, 0.984f, 0.984f));
	CreateDiceMesh("dice_hydro", glm::vec3(0.290f, 0.737f, 0.910f));

	card = new Card();
	CreateCardMesh(card, "card1", glm::vec3(0.7f, 0.0f, 0.0f));
	card2 = new Card();
	CreateCardMesh(card2, "card2", glm::vec3(0.0f, 0.9f, 0.5f)); 
	player = new Player(5);
	

	for (int i = 0; i < 3; i++) {
		CharacterCard* charCard = new CharacterCard();
		charCard->currentZone = CardZone::BOARD;
		charCard->mesh = meshes["card2"];
		charCard->width = dynamicWidth;
		charCard->height = dynamicHeight;
		charCard->x = startX + (i * slotSpacing);

		charCard->y = (i == 0) ? boardY + (dynamicHeight * 0.2f) : boardY; // Active character offset
		charCard->hp = 10;

		playerCharacters.push_back(charCard);
	}

	
	for (int i = 0; i < 5; i++) {
		ActionCard* actionCard = new ActionCard();
		actionCard->currentZone = CardZone::HAND;
		actionCard->mesh = meshes["card1"]; 
		actionCard->costDice = 3;
		playerHand.push_back(actionCard);
	}

	OrganizeHand(playerHand);
}

void Tema1::FrameStart() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);


}

void Tema1::RenderCardStacks() {
	glm::ivec2 resolution = window->GetResolution();
	float leftMargin = resolution.x * 0.05f;

	for (int i = 0; i < 25; i += 5) {
		card->modelMatrix = glm::mat3(1);
		card->modelMatrix *= trans2D::Translate(leftMargin - i, resolution.y * 0.30f + i);
		card->modelMatrix *= trans2D::Rotate(glm::radians(90.0f));
		card->modelMatrix *= trans2D::Scale(100, 100);
		RenderMesh2D(card->mesh, shaders["VertexColor"], card->modelMatrix);
	}
	for (int i = 0; i < 25; i += 5) {
		card2->modelMatrix = glm::mat3(1);
		card2->modelMatrix *= trans2D::Translate(leftMargin - i, resolution.y * 0.60f + i);
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
	float boardWidth = resolution.x * 0.70f; 
	float boardScaleY = resolution.y * 0.60f; 

	float posX = (resolution.x - boardWidth) / 2.0f;

	float posY_Player = resolution.y * 0.25f;
	glm::mat3 modelMatrix = glm::mat3(1);
	modelMatrix *= trans2D::Translate(posX, posY_Player);
	modelMatrix *= trans2D::Scale(boardWidth, boardScaleY);
	RenderMesh2D(meshes["playerPlayArena"], shaders["VertexColor"], modelMatrix);


	float posY_Enemy = resolution.y * 0.55f; 
	modelMatrix = glm::mat3(1);
	modelMatrix *= trans2D::Translate(posX, posY_Enemy);
	modelMatrix *= trans2D::Scale(boardWidth, boardScaleY);
	RenderMesh2D(meshes["enemyPlayArena"], shaders["VertexColor"], modelMatrix);
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

	for (CharacterCard* c : playerCharacters) {
		for (int i = 0; i < c->hp; i++) {
			glm::mat3 hpMatrix = glm::mat3(1);
			float heartX = c->x + (c->width * 0.1f) + (i % 5) * (c->width * 0.2f);
			float heartY = c->y + (c->height * 0.08f) + (i / 5) * (c->width * 0.2f);

			hpMatrix *= trans2D::Translate(heartX, heartY);
			hpMatrix *= trans2D::Scale(c->width * 0.04f, c->width * 0.04f);
			RenderMesh2D(meshes["heart"], shaders["VertexColor"], hpMatrix);
		}

		glm::mat3 modelMatrix = glm::mat3(1);
		modelMatrix *= trans2D::Translate(c->x + (c->width / 2.0f), c->y + (c->height / 2.0f));
		modelMatrix *= trans2D::Rotate(glm::radians(90.0f));
		modelMatrix *= trans2D::Scale(c->width, c->width); 
		RenderMesh2D(c->mesh, shaders["VertexColor"], modelMatrix);
		
	}
	for (Card* baseCard : playerHand) {
		ActionCard* c = static_cast<ActionCard*>(baseCard);
		for (int i = 0; i < c->costDice; i++) {
			glm::mat3 costMatrix = glm::mat3(1);
			float hexX = c->x + (c->width * 0.2f);
			float hexY = c->y + c->height - (c->height * 0.15f) - (i * (c->width * 0.22f));

			costMatrix *= trans2D::Translate(hexX, hexY);
			costMatrix *= trans2D::Scale(c->width * 0.1f, c->width * 0.1f);
			RenderMesh2D(meshes["dice_omni"], shaders["VertexColor"], costMatrix);
		}

		glm::mat3 modelMatrix = glm::mat3(1);
		modelMatrix *= trans2D::Translate(c->x + (c->width / 2.0f), c->y + (c->height / 2.0f));
		modelMatrix *= trans2D::Rotate(glm::radians(90.0f));
		modelMatrix *= trans2D::Scale(c->width, c->width);
		RenderMesh2D(c->mesh, shaders["VertexColor"], modelMatrix);
	}
	RenderDices();


	RenderPlayArena();
	RenderCardStacks();
	
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
	if (selectedCard && selectedCard->isDragging) {
		glm::ivec2 res = window->GetResolution();
		int sceneY = res.y - mouseY;

		selectedCard->x = mouseX - dragOffsetX;
		selectedCard->y = sceneY - dragOffsetY;
	}
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
	glm::ivec2 res = window->GetResolution();
	int sceneY = res.y - mouseY; //matching y to camera;
	if (button == GLFW_MOUSE_BUTTON_2) {
		for (auto it = playerHand.rbegin(); it != playerHand.rend(); ++it) {
			Card* c = *it;
			if (c->currentZone == CardZone::HAND && mouseX >= c->x && mouseX <= c->x + c->width && sceneY >= c->y && sceneY <= c->y + c->height) {
				c->isDragging = true;
				selectedCard = c;
				
				dragOffsetX = mouseX - c->x;
				dragOffsetY = sceneY - c->y;
				break;
			}
		}
	}
}
void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
	if(button == GLFW_MOUSE_BUTTON_2 && selectedCard) {
		glm::ivec2 res = window->GetResolution();
		int sceneY = res.y - mouseY;

		float arenaMinX = res.x * 0.1f;
		float arenaMaxX = arenaMinX + (res.x * 0.80f);
		float arenaMinY = res.y * 0.25f;
		float arenaMaxY = arenaMinY + (res.y * 0.4f);
		
		if (mouseX >= arenaMinX && mouseX <= arenaMaxX &&
			sceneY >= arenaMinY && sceneY <= arenaMaxY) {
			//valid drop 
			selectedCard->currentZone = CardZone::USED;
			playerHand.erase(std::remove(playerHand.begin(), playerHand.end(), selectedCard), playerHand.end());
			int cardsOnBoard = 0;
			for (Card* c : playerHand) {
				if (c->currentZone == CardZone::BOARD && c != selectedCard) {
					cardsOnBoard++;
				}

				if (cardsOnBoard < 3) {

				}
			}
			selectedCard->currentZone = CardZone::BOARD;

			//TODO: Snapping the active card slot
		}
		selectedCard->isDragging = false;
		selectedCard = nullptr;
		OrganizeHand(playerHand);
	}
}
void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {

}

void Tema1::OnWindowResize(int width, int height) {
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)width, 0, (float)height, 0.01f, 400);
	camera->Update();

	OrganizeHand(playerHand);


	float dynamicWidth = width * 0.08f;
	float dynamicHeight = dynamicWidth * 1.5f;

	float slotSpacing = dynamicWidth * 1.2f;
	float boardCenterX = width / 2.0f;
	float startX = boardCenterX - slotSpacing - (dynamicWidth / 2.0f);
	float boardY = (height * 0.25f) + (height * 0.05f);

	for (size_t i = 0; i < playerCharacters.size(); i++) {
		CharacterCard* charCard = playerCharacters[i];
		charCard->width = dynamicWidth;
		charCard->height = dynamicHeight;

		charCard->x = startX + (i * slotSpacing);
		charCard->y = (i == 0) ? boardY + (dynamicHeight * 0.2f) : boardY; 
	}
}