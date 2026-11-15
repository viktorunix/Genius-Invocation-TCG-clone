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
	CharacterCard* activeEnemy = nullptr;
	for (CharacterCard* c : enemy->characters)
		if (c->isActive) activeEnemy = c;
	if (activeEnemy) {
		activeEnemy->hp -= baseDmg;
		HandleDeath(enemy);
		EndPlayerTurn();
	}
}
void Tema1::EnemyTurn() {
	if (enemyPassed ||!enemy->activeCharacter || !player->activeCharacter) return;

	
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