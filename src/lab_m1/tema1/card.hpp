#pragma once
#include "glm/glm.hpp"
#include "core/gpu/mesh.h"
namespace m1
{
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
		float width = 100.0f;
		float height = 150.0f;
		bool isDragging = false;

		void Init();
	};

	class CharacterCard : public Card {
	public:
		int hp;
		Element element;

	};

	class ActionCard : public Card {
	public:
		int costDice;
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
		Player(int nrDices);
	};
}