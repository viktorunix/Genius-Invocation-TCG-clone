#pragma once

#include "components/simple_scene.h"

namespace m1
{
	class Card {
	public:
		Card();
		float hp;
		glm::mat3 modelMatrix;
		Mesh* mesh;
		void Init();

	};
	class Timer {
	public:
		float maxTime;
		float currentTime;
		Timer(float maxTime);
		
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
		void RenderCardStacks();
		void ChangeCardColor(Card* card, glm::vec3 color);

	protected:
		Card* card;
		Card* card2;
		Timer* timer;
	};

	
}