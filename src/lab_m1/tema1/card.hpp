#pragma once
#include "glm/glm.hpp"
#include "core/gpu/mesh.h"
namespace m1
{
	enum class EnemyMove {
		PASS = 0,
		NORMAL_ATTACK = 1,
		ELEMENTAL_SKILL,
		SWITCH_CHARACTER
	};
	enum class GameState {
		DICE_ROLL,
		PLAYER_ACTION,
		ENEMY_ACTION,
		ROUND_END
	};

	enum class CardZone {
		DECK,
		HAND,
		BOARD,
		USED
	};
	enum Element {
		Omni, Pyro, Cryo, Hydro
	};

	enum class ActionType {
		HEAL,
		ATTACK_BOOST,
		ADD_DICE
	};
	class Dice {
	public:
		Element element;
		Mesh* mesh;
	};

	class Card {
	public:
		Card();
		glm::mat3 modelMatrix;
		Mesh* mesh;
		
		CardZone currentZone = CardZone::DECK;
		float x, y;
		//for animations
		float targetX, targetY;
		bool isAnimating = false;


		float width = 0.0f;
		float height = 0.0f;
		bool isDragging = false;

		void Init();
	};

	class CharacterCard : public Card {
	public:
		int hp;
		Element element;
		bool isActive = false;
		glm::vec3 borderColor;
		Mesh* symbolMesh;
		std::vector<Mesh*> centerDrawingMeshes;
		bool hasAppliedElement = false;
		Element appliedElement;
		bool isFrozen = false;
	};

	class ActionCard : public Card {
	public:
		int costDice;
		ActionType type;
		Mesh* effectIconMesh;
	};

	class Timer {
	public:
		float maxTime;
		float currentTime;
		glm::mat3 modelMatrix;

		Timer(float maxTime);
		Mesh* CreateTimerBar(const char* name, glm::vec3 color, float z);
	};

	class Player {
	public:
		std::vector<Dice*> dices;
		std::vector<Card*> hand;
		std::vector<CharacterCard*> characters;

		CharacterCard* activeCharacter = nullptr;
		Player(int nrDices);
	};
}