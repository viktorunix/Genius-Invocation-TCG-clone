#include "lab_m1/tema1/card_factory.hpp"
#include <iostream>

using namespace m1;

CardFactory::CardFactory(std::unordered_map<std::string, Mesh*>& sceneMeshes) :  meshes(sceneMeshes) {

}

Mesh* CardFactory::CreateCrossMesh(const char* name, glm::vec3 color) {
	std::vector<VertexFormat> vertices{
		VertexFormat(glm::vec3(-0.15f, -0.4f, 0.0f), color),
		VertexFormat(glm::vec3(0.15f, -0.4f, 0.0f), color),
		VertexFormat(glm::vec3(0.15f,  0.4f, 0.0f), color),
		VertexFormat(glm::vec3(-0.15f,  0.4f, 0.0f), color),

		VertexFormat(glm::vec3(-0.4f, -0.15f, 0.0f), color),
		VertexFormat(glm::vec3(0.4f, -0.15f, 0.0f), color),
		VertexFormat(glm::vec3(0.4f,  0.15f, 0.0f), color),
		VertexFormat(glm::vec3(-0.4f,  0.15f, 0.0f), color)
	};

	std::vector<unsigned int> indices = {
		0, 1, 2, 0, 2, 3, // vertical
		4, 5, 6, 4, 6, 7  // orizontal
	};
	Mesh* mesh = new Mesh(name);
	mesh->InitFromData(vertices, indices);
	return mesh;
}
Mesh* CardFactory::CreateSwordMesh(const char* name, glm::vec3 color) {
	std::vector<VertexFormat> vertices = {
		//blade
		VertexFormat(glm::vec3(-0.1f,  0.1f, 0.0f), color),
		VertexFormat(glm::vec3(0.1f,  0.1f, 0.0f), color),
		VertexFormat(glm::vec3(0.1f,  0.6f, 0.0f), color),
		VertexFormat(glm::vec3(0.0f,  0.8f, 0.0f), color), // Vârful
		VertexFormat(glm::vec3(-0.1f,  0.6f, 0.0f), color),
		// the guard
		VertexFormat(glm::vec3(-0.3f,  0.0f, 0.0f), color),
		VertexFormat(glm::vec3(0.3f,  0.0f, 0.0f), color),
		VertexFormat(glm::vec3(0.3f,  0.1f, 0.0f), color),
		VertexFormat(glm::vec3(-0.3f,  0.1f, 0.0f), color),
		// the handle
		VertexFormat(glm::vec3(-0.1f, -0.3f, 0.0f), color),
		VertexFormat(glm::vec3(0.1f, -0.3f, 0.0f), color),
		VertexFormat(glm::vec3(0.1f,  0.0f, 0.0f), color),
		VertexFormat(glm::vec3(-0.1f,  0.0f, 0.0f), color)
	};
	std::vector<unsigned int> indices = {
		0, 1, 2, 0, 2, 4, 2, 3, 4, 
		5, 6, 7, 5, 7, 8,          
		9, 10, 11, 9, 11, 12       
	};
	Mesh* mesh = new Mesh(name);
	mesh->InitFromData(vertices, indices);
	return mesh;
}

ActionCard* CardFactory::CreateHealCard() {
	if (meshes.find("icon_cross") == meshes.end()) {
		meshes["icon_cross"] = CreateCrossMesh("icon_cross", glm::vec3(0.2f, 0.8f, 0.2f));

	}

	ActionCard* card = new ActionCard();
	card->mesh = meshes["card1"];
	card->currentZone = CardZone::HAND;
	card->costDice = 2;
	card->type = ActionType::HEAL;
	card->effectIconMesh = meshes["icon_cross"];

	return card;
}

ActionCard* CardFactory::CreateAttackBoostCard() {
	if (meshes.find("icon_sword") == meshes.end()) {
		meshes["icon_sword"] = CreateSwordMesh("icon_sword", glm::vec3(0.8f, 0.8f, 0.8f));
	}

	ActionCard* card = new ActionCard();
	card->mesh = meshes["card1"];
	card->currentZone = CardZone::HAND;
	card->costDice = 3;
	card->type = ActionType::ATTACK_BOOST;
	card->effectIconMesh = meshes["icon_sword"];

	return card;
}

ActionCard* CardFactory::CreateAddDiceCard() {
	ActionCard* card = new ActionCard();
	card->mesh = meshes["card1"];
	card->currentZone = CardZone::HAND;
	card->costDice = 1;
	card->type = ActionType::ADD_DICE;
	card->effectIconMesh = meshes["dice_omni"];

	return card;
}

Mesh* CardFactory::CreateTriangleMesh(const char* name, glm::vec3 color) {
	std::vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(-0.3f, -0.3f, 0.0f), color),
		VertexFormat(glm::vec3(0.3f, -0.3f, 0.0f), color),
		VertexFormat(glm::vec3(0.0f,  0.4f, 0.0f), color)
	};
	std::vector<unsigned int> indices = { 0, 1, 2 };
	Mesh* mesh = new Mesh(name);
	mesh->InitFromData(vertices, indices);
	return mesh;
}
Mesh* CardFactory::CreateRhombusMesh(const char* name, glm::vec3 color) {
	std::vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(0.0f, -0.4f, 0.0f), color),
		VertexFormat(glm::vec3(0.3f,  0.0f, 0.0f), color),
		VertexFormat(glm::vec3(0.0f,  0.4f, 0.0f), color),
		VertexFormat(glm::vec3(-0.3f,  0.0f, 0.0f), color)
	};
	std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3 };
	Mesh* mesh = new Mesh(name);
	mesh->InitFromData(vertices, indices);
	return mesh;
}

Mesh* CardFactory::CreateMageDrawing(const char* name) {
	//pyro character
	glm::vec3 skinColor = glm::vec3(0.9f, 0.8f, 0.7f);
	glm::vec3 robeColor = glm::vec3(0.8f, 0.2f, 0.2f);
	glm::vec3 staffColor = glm::vec3(0.4f, 0.2f, 0.0f);

	std::vector<VertexFormat> vertices = {
		//square head
		VertexFormat(glm::vec3(-0.1f, 0.2f, 0.0f), skinColor), VertexFormat(glm::vec3(0.1f, 0.2f, 0.0f), skinColor),
		VertexFormat(glm::vec3(0.1f, 0.4f, 0.0f), skinColor), VertexFormat(glm::vec3(-0.1f, 0.4f, 0.0f), skinColor),
		//triangle body
		VertexFormat(glm::vec3(-0.25f, -0.3f, 0.0f), robeColor), VertexFormat(glm::vec3(0.25f, -0.3f, 0.0f), robeColor),
		VertexFormat(glm::vec3(0.0f, 0.2f, 0.0f), robeColor),
		//rectangle staff
		VertexFormat(glm::vec3(0.2f, -0.4f, 0.0f), staffColor), VertexFormat(glm::vec3(0.25f, -0.4f, 0.0f), staffColor),
		VertexFormat(glm::vec3(0.25f, 0.5f, 0.0f), staffColor), VertexFormat(glm::vec3(0.2f, 0.5f, 0.0f), staffColor)
	};
	std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3,  4, 5, 6,  7, 8, 9, 7, 9, 10 };
	Mesh* mesh = new Mesh(name);
	mesh->InitFromData(vertices, indices);
	return mesh;
}

Mesh* CardFactory::CreateKnightDrawing(const char* name) {
	// hydro character
	glm::vec3 skinColor = glm::vec3(0.9f, 0.8f, 0.7f);
	glm::vec3 armorColor = glm::vec3(0.2f, 0.4f, 0.8f);
	glm::vec3 shieldColor = glm::vec3(0.7f, 0.7f, 0.8f);
	std::vector<VertexFormat> vertices = {
		//square head
		VertexFormat(glm::vec3(-0.1f, 0.2f, 0.0f), skinColor), VertexFormat(glm::vec3(0.1f, 0.2f, 0.0f), skinColor),
		VertexFormat(glm::vec3(0.1f, 0.4f, 0.0f), skinColor), VertexFormat(glm::vec3(-0.1f, 0.4f, 0.0f), skinColor),
		//rectangle body
		VertexFormat(glm::vec3(-0.2f, -0.3f, 0.0f), armorColor), VertexFormat(glm::vec3(0.2f, -0.3f, 0.0f), armorColor),
		VertexFormat(glm::vec3(0.2f, 0.2f, 0.0f), armorColor), VertexFormat(glm::vec3(-0.2f, 0.2f, 0.0f), armorColor),
		//rhombus shield
		VertexFormat(glm::vec3(-0.2f, -0.2f, 0.0f), shieldColor), VertexFormat(glm::vec3(0.0f, 0.0f, 0.0f), shieldColor),
		VertexFormat(glm::vec3(-0.2f, 0.2f, 0.0f), shieldColor), VertexFormat(glm::vec3(-0.4f, 0.0f, 0.0f), shieldColor)
	};
	std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3,  4, 5, 6, 4, 6, 7,  8, 9, 10, 8, 10, 11 };
	Mesh* mesh = new Mesh(name);
	mesh->InitFromData(vertices, indices);
	return mesh;
}

Mesh* CardFactory::CreateRogueDrawing(const char* name) {
	//cryo character
	glm::vec3 hoodColor = glm::vec3(0.3f, 0.8f, 0.8f);
	glm::vec3 bodyColor = glm::vec3(0.1f, 0.2f, 0.3f);
	glm::vec3 bladeColor = glm::vec3(0.8f, 0.9f, 0.9f);
	std::vector<VertexFormat> vertices = {
		// triangle hood
		VertexFormat(glm::vec3(-0.15f, 0.4f, 0.0f), hoodColor), VertexFormat(glm::vec3(0.15f, 0.4f, 0.0f), hoodColor), VertexFormat(glm::vec3(0.0f, 0.1f, 0.0f), hoodColor),
		// trapeze body
		VertexFormat(glm::vec3(-0.2f, -0.3f, 0.0f), bodyColor), VertexFormat(glm::vec3(0.2f, -0.3f, 0.0f), bodyColor),
		VertexFormat(glm::vec3(0.1f, 0.1f, 0.0f), bodyColor), VertexFormat(glm::vec3(-0.1f, 0.1f, 0.0f), bodyColor),
		//left blade
		VertexFormat(glm::vec3(-0.35f, -0.1f, 0.0f), bladeColor), VertexFormat(glm::vec3(-0.25f, -0.1f, 0.0f), bladeColor),
		VertexFormat(glm::vec3(-0.25f, 0.3f, 0.0f), bladeColor), VertexFormat(glm::vec3(-0.35f, 0.3f, 0.0f), bladeColor),
		// right blade
		VertexFormat(glm::vec3(0.25f, -0.1f, 0.0f), bladeColor), VertexFormat(glm::vec3(0.35f, -0.1f, 0.0f), bladeColor),
		VertexFormat(glm::vec3(0.35f, 0.3f, 0.0f), bladeColor), VertexFormat(glm::vec3(0.25f, 0.3f, 0.0f), bladeColor)
	};
	std::vector<unsigned int> indices = { 0, 1, 2,  3, 4, 5, 3, 5, 6,  7, 8, 9, 7, 9, 10,  11, 12, 13, 11, 13, 14 };
	Mesh* mesh = new Mesh(name);
	mesh->InitFromData(vertices, indices);
	return mesh;
}

CharacterCard* CardFactory::CreatePyroCharacter() {
	if (meshes.find("symbol_pyro") == meshes.end()) {
		meshes["symbol_pyro"] = CreateTriangleMesh("symbol_pyro", glm::vec3(1.0f, 0.3f, 0.1f));
		meshes["draw_mage"] = CreateMageDrawing("draw_mage");
	}

	CharacterCard* card = new CharacterCard();
	card->mesh = meshes["card1"];
	card->borderColor = glm::vec3(0.9f, 0.2f, 0.2f);
	card->element = Pyro;
	card->hp = 10;

	card->symbolMesh = meshes["symbol_pyro"];
	card->centerDrawingMeshes.push_back(meshes["draw_mage"]);

	return card;
}

CharacterCard* CardFactory::CreateHydroCharacter() {
	if (meshes.find("symbol_hydro") == meshes.end()) {
		meshes["symbol_hydro"] = CreateRhombusMesh("symbol_hydro", glm::vec3(0.2f, 0.5f, 1.0f));
		meshes["draw_knight"] = CreateKnightDrawing("draw_knight");
	}
	CharacterCard* card = new CharacterCard();
	card->mesh = meshes["card1"];
	card->borderColor = glm::vec3(0.1f, 0.3f, 0.8f);
	card->hp = 10;

	card->symbolMesh = meshes["symbol_hydro"];
	card->centerDrawingMeshes.push_back(meshes["draw_knight"]);

	return card;
}
CharacterCard* CardFactory::CreateCryoCharacter() {
	if (meshes.find("symbol_cryo") == meshes.end()) {
		meshes["symbol_cryo"] = CreateRhombusMesh("symbol_cryo", glm::vec3(0.6f, 0.9f, 1.0f));
		meshes["draw_rogue"] = CreateRogueDrawing("draw_rogue");
	}

	CharacterCard* card = new CharacterCard();
	card->mesh = meshes["card1"];
	card->borderColor = glm::vec3(0.5f, 0.8f, 0.9f); 
	card->element = Cryo;
	card->hp = 10;

	card->symbolMesh = meshes["symbol_cryo"];
	card->centerDrawingMeshes.push_back(meshes["draw_rogue"]);

	return card;
}