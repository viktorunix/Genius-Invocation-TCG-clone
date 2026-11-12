#include "lab_m1/tema1/tema1.hpp"
#include "lab_m1/tema1/trans2D.hpp"
#include "lab_m1/tema1/card_factory.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

void Tema1::OrganizeBoard() {
	glm::ivec2 res = window->GetResolution();
	float dynamicWidth = res.x * 0.08f;
	float dynamicHeight = dynamicWidth * 1.5f;
	float slotSpacing = dynamicWidth * 1.2f;
	float boardCenterX = res.x / 2.0f;
	float startX = boardCenterX - slotSpacing - (dynamicWidth / 2.0f);


	float playerBoardY = (res.y * 0.25f) + (res.y * 0.05f);
	for (size_t i = 0; i < player->characters.size(); i++) {
		player->characters[i]->width = dynamicWidth;
		player->characters[i]->height = dynamicHeight;
		player->characters[i]->x = startX + (i * slotSpacing);
		player->characters[i]->y = player->characters[i]->isActive ? playerBoardY + (dynamicHeight * 0.2f) : playerBoardY;
	}

	
	float enemyBoardY = (res.y * 0.55f) + (res.y * 0.05f);
	for (size_t i = 0; i < enemy->characters.size(); i++) {
		enemy->characters[i]->width = dynamicWidth;
		enemy->characters[i]->height = dynamicHeight;
		enemy->characters[i]->x = startX + (i * slotSpacing);
		
		enemy->characters[i]->y = enemy->characters[i]->isActive ? enemyBoardY - (dynamicHeight * 0.2f) : enemyBoardY;
	}
}
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
	
	Card* cPyro = new Card();
	CreateCardMesh(cPyro, "card_pyro", glm::vec3(0.9f, 0.2f, 0.2f));

	Card* cHydro = new Card();
	CreateCardMesh(cHydro, "card_hydro", glm::vec3(0.1f, 0.3f, 0.8f)); 

	Card* cCryo = new Card();
	CreateCardMesh(cCryo, "card_cryo", glm::vec3(0.5f, 0.8f, 0.9f)); 
	player = new Player(5);
	Mesh* cardBg = new Mesh("card_bg");
	std::vector<VertexFormat> bgVertices = {
		VertexFormat(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)), // Gri închis
		VertexFormat(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)),
		VertexFormat(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)),
		VertexFormat(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f))
	};
	std::vector<unsigned int> bgIndices = { 0, 1, 2, 0, 2, 3 };
	cardBg->InitFromData(bgVertices, bgIndices);
	meshes["card_bg"] = cardBg;
	
	CardFactory factory(meshes);

	
	player->characters.push_back(factory.CreatePyroCharacter());
	player->characters.push_back(factory.CreateHydroCharacter());
	player->characters.push_back(factory.CreateCryoCharacter());
	player->characters[0]->isActive = true;

	


	player->hand.push_back(factory.CreateHealCard());
	player->hand.push_back(factory.CreateAttackBoostCard());
	player->hand.push_back(factory.CreateAddDiceCard());
	player->hand.push_back(factory.CreateHealCard());
	player->hand.push_back(factory.CreateAttackBoostCard());


	for (Card* c : player->hand) {
		c->width = dynamicWidth;
		c->height = dynamicHeight;
	}
	OrganizeHand(player->hand);


	enemy = new Player(5);
	enemy->characters.push_back(factory.CreateHydroCharacter());
	enemy->characters.push_back(factory.CreatePyroCharacter());
	enemy->characters.push_back(factory.CreateCryoCharacter());
	enemy->characters[0]->isActive = true;
	float enemyBoardY = (resolution.y * 0.55f) + (resolution.y * 0.05f);

	OrganizeBoard();
}

void Tema1::FrameStart() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);


}


void Tema1::Update(float deltaTimeSeconds) {

	RenderCharacters(player);
	RenderCharacters(enemy);
	RenderHand();
	
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
	if (player->characters.empty() || enemy->characters.empty()) {
		cout << "Game Ended" << endl;
		return;
	}

	CharacterCard* activePlayer = nullptr;
	for (CharacterCard* c : player->characters) {
		if (c->isActive) activePlayer = c;
	}
	CharacterCard* activeEnemy = nullptr;
	for (CharacterCard* c : enemy->characters) {
		if (c->isActive) activeEnemy = c;
	}
	if (!activePlayer || !activeEnemy) return;

	//combat input
	if (mouseX >= activeEnemy->x && mouseX <= activeEnemy->x + activeEnemy->width &&
		sceneY >= activeEnemy->y && sceneY <= activeEnemy->y + activeEnemy->height) {

		if (button == GLFW_MOUSE_BUTTON_2) {
			if (player->dices.size() >= 1) {
				player->dices.pop_back();
				activeEnemy->hp -= 2;
				cout << "COMBAT: " << activePlayer->element << " normal attacked (2DMG)" << endl;

			}
			else {
				cout << "COMBAT : Not enough dices for attacking" << endl;
			}
		}
		else if (button == GLFW_MOUSE_BUTTON_1) {
			if (player->dices.size() >= 3) {
				player->dices.pop_back();
				player->dices.pop_back();
				player->dices.pop_back();
				activeEnemy->hp -= 3;
				cout << "COMBAT: " << activePlayer->element << " element attack" << endl;
			}
			else {
				cout << "COMBAT: " << "Not enough dices for elemental attack" << endl;

			}
		}
		if (activeEnemy->hp <= 0) {
			cout << "COMBAT: Enemy card down" << endl;
			enemy->characters.erase(std::remove(enemy->characters.begin(), enemy->characters.end(), activeEnemy), enemy->characters.end());
			if (!enemy->characters.empty()) {
				enemy->characters[0]->isActive = true;
			}
			OrganizeBoard();
		}
		return;
	}
	//input on active characters
	if (button == GLFW_MOUSE_BUTTON_2) {
		for (size_t i = 0; i < player->characters.size(); i++) {
			CharacterCard* c = player->characters[i];
			if (!c->isActive && mouseX >= c->x && mouseX <= c->x + c->width && sceneY >= c->y && sceneY <= c->y + c->height) {
				
				//costs 1 dice to change characters
				if (player->dices.size() >= 1) {
					player->dices.pop_back();

					activePlayer->isActive = false;
					c->isActive = true;
					cout << "SYSTEM: " << "you changed active character" << endl;
					OrganizeBoard();
				}
				else {
					cout << "SYSTEM: you need at least 1 dice to change characters" << endl;
				}
				return;
			}
		}
	}
	//drag and drop input
	if (button == GLFW_MOUSE_BUTTON_2) {
		for (auto it = player->hand.rbegin(); it != player->hand.rend(); ++it) {
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
			player->hand.erase(std::remove(player->hand.begin(), player->hand.end(), selectedCard), player->hand.end());
			int cardsOnBoard = 0;
			for (Card* c : player->hand) {
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
		OrganizeHand(player->hand);
	}
}
void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {

}

void Tema1::OnWindowResize(int width, int height) {
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)width, 0, (float)height, 0.01f, 400);
	camera->Update();

	OrganizeHand(player->hand);
	OrganizeBoard();

}