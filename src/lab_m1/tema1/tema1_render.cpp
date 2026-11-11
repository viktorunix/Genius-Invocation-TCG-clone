#include "lab_m1/tema1/tema1.hpp"
#include "lab_m1/tema1/trans2D.hpp"

using namespace std;
using namespace m1;

void Tema1::RenderCharacters() {
	for (CharacterCard* c : playerCharacters) {
		//drawing hp
		for (int i = 0; i < c->hp; i++) {
			glm::mat3 hpMatrix = glm::mat3(1);
			float heartX = c->x + (c->width * 0.1f) + (i % 5) * (c->width * 0.2f);
			float heartY = c->y + (c->height * 0.08f) + (i / 5) * (c->width * 0.2f);
			hpMatrix *= trans2D::Translate(heartX, heartY);
			hpMatrix *= trans2D::Scale(c->width * 0.04f, c->width * 0.04f);
			RenderMesh2D(meshes["heart"], shaders["VertexColor"], hpMatrix);
		}
		//drawing element
		if (c->symbolMesh) {
			glm::mat3 symMatrix = glm::mat3(1);
			symMatrix *= trans2D::Translate(c->x + (c->width * 0.2f), c->y + c->height - (c->height * 0.15f));
			symMatrix *= trans2D::Scale(c->width * 0.15f, c->width * 0.15f);
			RenderMesh2D(c->symbolMesh, shaders["VertexColor"], symMatrix);
		}

		//drawing the main drawing
		for (Mesh* drawingPart : c->centerDrawingMeshes) {
			glm::mat3 drawMatrix = glm::mat3(1);
			drawMatrix *= trans2D::Translate(c->x + (c->width / 2.0f), c->y + (c->height / 2.0f));
			drawMatrix *= trans2D::Scale(c->width * 0.5f, c->width * 0.5f);
			RenderMesh2D(drawingPart, shaders["VertexColor"], drawMatrix);
		}
		//drawing the background of the card
		glm::mat3 bgMatrix = glm::mat3(1);
		bgMatrix *= trans2D::Translate(c->x + (c->width / 2.0f), c->y + (c->height / 2.0f));
		bgMatrix *= trans2D::Rotate(glm::radians(90.0f));
		bgMatrix *= trans2D::Scale(c->height * 0.95f, c->width * 0.95f);
		RenderMesh2D(meshes["card_bg"], shaders["VertexColor"], bgMatrix);


		//drawing the contour
		glm::mat3 borderMatrix = glm::mat3(1);
		borderMatrix *= trans2D::Translate(c->x + (c->width / 2.0f), c->y + (c->height / 2.0f));
		borderMatrix *= trans2D::Rotate(glm::radians(90.0f));
		borderMatrix *= trans2D::Scale(c->height * 1.05f, c->width * 1.05f);
		RenderMesh2D(c->mesh, shaders["VertexColor"], borderMatrix);





	}
}
void Tema1::RenderHand() {
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

		if (c->effectIconMesh) {
			glm::mat3 iconMatrix = glm::mat3(1);
			iconMatrix *= trans2D::Translate(c->x + (c->width / 2.0f), c->y + (c->height / 2.0f));
			iconMatrix *= trans2D::Scale(c->width * 0.4f, c->width * 0.4f);
			RenderMesh2D(c->effectIconMesh, shaders["VertexColor"], iconMatrix);

		}
		glm::mat3 modelMatrix = glm::mat3(1);
		modelMatrix *= trans2D::Translate(c->x + (c->width / 2.0f), c->y + (c->height / 2.0f));
		modelMatrix *= trans2D::Rotate(glm::radians(90.0f));
		modelMatrix *= trans2D::Scale(c->height, c->width);
		RenderMesh2D(c->mesh, shaders["VertexColor"], modelMatrix);


	}
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
