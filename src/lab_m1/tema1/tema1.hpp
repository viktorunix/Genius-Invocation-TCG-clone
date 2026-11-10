#pragma once

#include "components/simple_scene.h"
#include <vector>

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
		GRAVEYARD
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
		Timer(float maxTime);
		glm::mat3 modelMatrix;
		Mesh* CreateTimerBar(const char* name, glm::vec3 color, float z);

		
	};

	class Player {
	public:
		std::vector<Dice*> dices;
		Player(int nrDices);
	};
	class Tema1 : public gfxc::SimpleScene
	{
	public:
		Tema1();
		~Tema1();
		
		void Init() override;
	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		void CreateCardMesh(Card* card, const char *id, glm::vec3 color);
		void CreateDiceMesh(const char* id, glm::vec3 color);
		void RenderCardStacks();
		void RenderTimer(float deltaTimeSeconds);
		void RenderPlayArena();
		void RenderDices();
		void ChangeCardColor(Card* card, glm::vec3 color);
		void OrganizeHand(std::vector<Card*>& handCards);

	protected:
		Card* card;
		Card* card2;
		Timer* timer;

		Player* player;
		std::vector<Card*> playerHand;
		std::vector<CharacterCard*> playerCharacters;
	};

	
}