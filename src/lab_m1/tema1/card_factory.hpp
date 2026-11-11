#pragma once
#include "lab_m1/tema1/card.hpp"
#include <unordered_map>
#include <string>

namespace m1 {
	class CardFactory {
	public:
		//passing the dictionary of meshes for reusing
		CardFactory(std::unordered_map<std::string, Mesh*>& sceneMeshes);

		CharacterCard* CreatePyroCharacter();
		CharacterCard* CreateHydroCharacter();
		CharacterCard* CreateCryoCharacter();

		ActionCard* CreateHealCard();
		ActionCard* CreateAttackBoostCard();
		ActionCard* CreateAddDiceCard();
	private:
		std::unordered_map<std::string, Mesh*>& meshes;

		Mesh* CreateCrossMesh(const char* name, glm::vec3 color);
		Mesh* CreateSwordMesh(const char* name, glm::vec3 color);
		Mesh* CreateTriangleMesh(const char* name, glm::vec3 color);
		Mesh* CreateRhombusMesh(const char* name, glm::vec3 color);
		Mesh* CreateMageDrawing(const char* name);
		Mesh* CreateKnightDrawing(const char* name);
		Mesh* CreateRogueDrawing(const char* name);
	};
}