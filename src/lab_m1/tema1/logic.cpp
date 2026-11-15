#include "lab_m1/tema1/tema1.hpp"
#include <iostream>

using namespace std;
using namespace m1;

void Tema1::HandleDeath(Player* p) {
	for (CharacterCard* c : p->characters) {
		if (c->isActive && c->hp <= 0) {
			cout << "SYSTEM: Active character card down!" << endl;
			c->hp = 0;
			c->isActive = false;

			for (CharacterCard* next : p->characters) {
				if (next->hp > 0) {
					next->isActive = true;
					p->activeCharacter = next;
					break;
				}
			}
			OrganizeBoard();
			return;
		}
	}
}
void Tema1::EndPlayerTurn() {
	if (enemyPassed) {
		currentState = GameState::PLAYER_ACTION;

	}
	else {
		currentState = GameState::ENEMY_ACTION;
		EnemyTurn();
	}
}

void Tema1::PerformPlayerAttack(int baseDmg) {
	if (!enemy->activeCharacter || !player->activeCharacter) return;
	CharacterCard* target = enemy->activeCharacter;
	Element atkElement = player->activeCharacter->element;
	bool triggerMeltVape = false;
	bool triggerFreeze = false;

	if (target->hasAppliedElement) {
		Element targetElement = target->appliedElement;

		//melt;
		if ((atkElement == Pyro && (targetElement == Hydro || targetElement == Cryo)) ||
			((targetElement == Hydro || atkElement == Cryo) && targetElement == Pyro)) {
			triggerMeltVape = true;
		}
		//freeze
		else if ((atkElement == Hydro && targetElement == Cryo) || (atkElement == Cryo && targetElement == Hydro)) {
			triggerFreeze = true;
		}
		target->hasAppliedElement = false;
	}
	else {
		target->hasAppliedElement = true;
		target->appliedElement = atkElement;
	}
	//dmg calculation
	int finalDmg = baseDmg;
	if (triggerMeltVape) {
		finalDmg += 2;
		cout << "REACTION: MELT/VAPE" << endl;

	}
	if (triggerFreeze) {
		target->isFrozen = true;
		cout << "REACTION: FREEZE" << endl;
	}
	target->hp -= finalDmg;
	HandleDeath(enemy);
	EndPlayerTurn();
	
}
void Tema1::EnemyTurn() {
	if (enemyPassed ||!enemy->activeCharacter || !player->activeCharacter) return;

	//verify freeze
	if (enemy->activeCharacter->isFrozen) {
		cout << "ENEMY: character card frozen, skip their turn" << endl;
		enemy->activeCharacter->isFrozen = false;
		enemyPassed = true;
		if (firstToPass == 0) firstToPass = 2;

		currentState = GameState::PLAYER_ACTION;
		return;
	}
	std::vector<EnemyMove> validMoves;
	validMoves.push_back(EnemyMove::PASS);

	if (enemy->dices.size() >= 1) validMoves.push_back(EnemyMove::NORMAL_ATTACK);
	if (enemy->dices.size() >= 3) validMoves.push_back(EnemyMove::ELEMENTAL_SKILL); 

	//verifying if enemy can switch
	bool canSwitch = false;
	for (CharacterCard* c : enemy->characters) {
		if (!c->isActive && c->hp > 0) canSwitch = true;
	}
	if (canSwitch && enemy->dices.size() >= 1) validMoves.push_back(EnemyMove::SWITCH_CHARACTER);


	EnemyMove move = validMoves[rand() % validMoves.size()];
	switch (move) {
	case EnemyMove::PASS:
		cout << "INAMIC: A declarat End Round (Pass)!" << endl;
		enemyPassed = true;
		if (firstToPass == 0) firstToPass = 2;
		break;

	case EnemyMove::NORMAL_ATTACK:
		enemy->dices.pop_back();
		player->activeCharacter->hp -= 2;
		cout << "INAMIC: Atac Normal (2 DMG)" << endl;
		break;

	case EnemyMove::ELEMENTAL_SKILL:
		enemy->dices.pop_back(); enemy->dices.pop_back(); enemy->dices.pop_back();
		player->activeCharacter->hp -= 3;
		cout << "INAMIC: Skill Elemental (3 DMG)" << endl;
		break;

	case EnemyMove::SWITCH_CHARACTER:
		enemy->dices.pop_back();
		enemy->activeCharacter->isActive = false;
		for (CharacterCard* c : enemy->characters) {
			if (!c->isActive && c->hp > 0) {
				c->isActive = true;
				enemy->activeCharacter = c;
				break;
			}
		}
		cout << "INAMIC: A schimbat personajul activ!" << endl;
		OrganizeBoard();
		break;
	}
	

	HandleDeath(player);


	if (playerPassed && enemyPassed) {
		StartNewRound();
	}
	else if (playerPassed && !enemyPassed) {
		EnemyTurn();
	}
	else {
		currentState = GameState::PLAYER_ACTION;
	}
}
void Tema1::StartNewRound() {
	playerPassed = false;
	enemyPassed = false;


	for (Dice* d : player->dices)
		delete d;
	player->dices.clear();

	for (Dice* d : enemy->dices)
		delete d;
	enemy->dices.clear();


	for (int i = 0; i < 10; i++) {
		Dice* d1 = new Dice();
		d1->element = Omni;
		player->dices.push_back(d1);

		Dice* d2 = new Dice();
		d2->element = Omni;
		enemy->dices.push_back(d2);
	}
	for (int i = 0; i < 2; ++i) {
		DrawRandomActionCard(player);
	}

	if (firstToPass == 2) {
		cout << "NEW ROUND: Enemy starts" << endl;
		firstToPass = 0;
		currentState = GameState::ENEMY_ACTION;
		EnemyTurn();
	}
	else {
		cout << "NEW ROUND: Player starts" << endl;
		firstToPass = 0;
		currentState = GameState::PLAYER_ACTION;
	}
}
void Tema1::DrawCardFromDeck(Player* p, Card* newCard) {
	glm::ivec2 resolution = window->GetResolution();

	float leftMargin = resolution.x * 0.05f;
	float deckY = (p == player) ? resolution.y * 0.30f : resolution.y * 0.60f;

	newCard->x = leftMargin;
	newCard->y = deckY;

	newCard->width = resolution.x * 0.07f;
	newCard->height = newCard->width * 1.35f;

	newCard->currentZone = CardZone::HAND;
	p->hand.push_back(newCard);

	OrganizeHand(p->hand);
}

void Tema1::DrawRandomActionCard(Player* p) {
	int r = rand() % 3;
	Card* newCard = nullptr;

	if (r == 0) {
		newCard = factory->CreateHealCard();

	}
	else if (r == 1) {
		newCard = factory->CreateAttackBoostCard();
	}
	else {
		newCard = factory->CreateAddDiceCard();
	}
	DrawCardFromDeck(p, newCard);
}
