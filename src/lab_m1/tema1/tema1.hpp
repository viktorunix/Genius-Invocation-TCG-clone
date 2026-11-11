#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema1/card.hpp"
#include <vector>

namespace m1
{
	
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
		void RenderHand();
		void RenderCharacters();
		void ChangeCardColor(Card* card, glm::vec3 color);
		void OrganizeHand(std::vector<Card*>& handCards);

	protected:
		Card* card;
		Card* card2;
		Timer* timer;

		Player* player;
		std::vector<Card*> playerHand;
		std::vector<CharacterCard*> playerCharacters;

		Card* selectedCard = nullptr;
		float dragOffsetX = 0;
		float dragOffsetY = 0;
	};

	
}