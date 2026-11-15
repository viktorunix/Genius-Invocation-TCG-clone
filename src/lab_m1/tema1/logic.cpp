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
	CheckGameOver();
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
		bool reactionOccurred = false;
		//melt;
		if ((atkElement == Pyro && (targetElement == Hydro || targetElement == Cryo)) ||
			((atkElement == Hydro || atkElement == Cryo) && targetElement == Pyro)) {
			triggerMeltVape = true;
			reactionOccurred = true;
		}
		//freeze
		else if ((atkElement == Hydro && targetElement == Cryo) || (atkElement == Cryo && targetElement == Hydro)) {
			triggerFreeze = true;
			reactionOccurred = true;
		}
		if (reactionOccurred)
			target->hasAppliedElement = false;
		else
			target->appliedElement = atkElement;
	}
	else {
		target->hasAppliedElement = true;
		target->appliedElement = atkElement;
	}
	//dmg calculation
	int finalDmg = baseDmg + player->activeCharacter->bonusDmg;
	if (triggerMeltVape) {
		finalDmg += 2;
		cout << "REACTION: MELT/VAPE" << endl;

	}
	if (triggerFreeze) {
		target->isFrozen = true;
		cout << "REACTION: FREEZE" << endl;
	}
	target->hp -= finalDmg;
	target->isHit = true;
	target->hitAnimationTimer = 0.3f;

	player->activeCharacter->isAttacking = true;
	player->activeCharacter->attackAnimationTimer = 0.2f;
	
	//starting the animation for projectile
	isProjectileActive = true;
	projX = player->activeCharacter->x + (player->activeCharacter->width / 2.0f);
	projY = player->activeCharacter->y + (player->activeCharacter->height / 2.0f);
	projTargetX = target->x + (target->width / 2.0f);
	projTargetY = target->y + (target->height / 2.0f);
	projElement = atkElement;
	HandleDeath(enemy);
	EndPlayerTurn();
	
}
void Tema1::EnemyTurn() {
	if (enemyPassed ||!enemy->activeCharacter || !player->activeCharacter) return;

	//verify freeze
	if (enemy->activeCharacter->isFrozen) {
		cout << "ENEMY: character card frozen, skip their turn" << endl;
		//enemy->activeCharacter->isFrozen = false;
		enemyPassed = true;
		if (firstToPass == 0) firstToPass = 2;

		currentState = GameState::PLAYER_ACTION;
		return;
	}
	std::vector<EnemyMove> validMoves;
	

	if (enemy->dices.size() >= 1) validMoves.push_back(EnemyMove::NORMAL_ATTACK);
	if (enemy->dices.size() >= 3) validMoves.push_back(EnemyMove::ELEMENTAL_SKILL); 
	//varifying if enemy can play action card
	int playableCardIndex = -1;
	for (size_t i = 0; i < enemy->hand.size(); i++) {
		ActionCard* ac = static_cast<ActionCard*>(enemy->hand[i]);
		if (enemy->dices.size() >= ac->costDice) {
			playableCardIndex = i;
			break;
		}
	}
	if (playableCardIndex != -1) {
		validMoves.push_back(EnemyMove::USE_ACTION_CARD);
		validMoves.push_back(EnemyMove::USE_ACTION_CARD);
		validMoves.push_back(EnemyMove::USE_ACTION_CARD);
	}
	//verifying if enemy can switch
	bool canSwitch = false;
	for (CharacterCard* c : enemy->characters) {
		if (!c->isActive && c->hp > 0) canSwitch = true;
	}
	if (canSwitch && ConsumeDices(enemy, 1, Omni)) validMoves.push_back(EnemyMove::SWITCH_CHARACTER);


	EnemyMove move;
	if (validMoves.empty())
		move = EnemyMove::PASS;
	else
		move = validMoves[rand() % validMoves.size()];
	switch (move) {
	case EnemyMove::PASS:
		cout << "INAMIC: A declarat End Round (Pass)!" << endl;
		enemyPassed = true;
		if (firstToPass == 0) firstToPass = 2;
		break;

	case EnemyMove::NORMAL_ATTACK:
		if (ConsumeDices(enemy, 1, Omni)) {
			int damage = 2 + enemy->activeCharacter->bonusDmg;
			player->activeCharacter->hp -= damage;
			player->activeCharacter->isHit = true;
			player->activeCharacter->hitAnimationTimer = 0.3f;

			enemy->activeCharacter->isAttacking = true;
			enemy->activeCharacter->attackAnimationTimer = 0.2f;
			cout << "INAMIC: Atac Normal (2 DMG)" << endl;
			
			isProjectileActive = true;
			projX = enemy->activeCharacter->x + (enemy->activeCharacter->width / 2.0f);
			projY = enemy->activeCharacter->y + (enemy->activeCharacter->height / 2.0f);
			projTargetX = player->activeCharacter->x + (player->activeCharacter->width / 2.0f);
			projTargetY = player->activeCharacter->y + (player->activeCharacter->height / 2.0f);
			projElement = Omni;
		}
		break;

	case EnemyMove::ELEMENTAL_SKILL:
		if (ConsumeDices(enemy, 3, enemy->activeCharacter->element)) {
			int damage = 3 + enemy->activeCharacter->bonusDmg;
			player->activeCharacter->hp -= damage;
			player->activeCharacter->isHit = true;
			player->activeCharacter->hitAnimationTimer = 0.3f;

			enemy->activeCharacter->isAttacking = true;
			enemy->activeCharacter->attackAnimationTimer = 0.2f;

			isProjectileActive = true;
			projX = enemy->activeCharacter->x + (enemy->activeCharacter->width / 2.0f);
			projY = enemy->activeCharacter->y + (enemy->activeCharacter->height / 2.0f);
			projTargetX = player->activeCharacter->x + (player->activeCharacter->width / 2.0f);
			projTargetY = player->activeCharacter->y + (player->activeCharacter->height / 2.0f);
			projElement = enemy->activeCharacter->element;
			cout << "INAMIC: Skill Elemental (3 DMG)" << endl;
		}
		break;

	case EnemyMove::SWITCH_CHARACTER:
		if (ConsumeDices(enemy, 1, Omni)) {
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
		}
		break;
	case EnemyMove::USE_ACTION_CARD:
		ActionCard* cardToPlay = static_cast<ActionCard*>(enemy->hand[playableCardIndex]);

		
		if (ConsumeDices(enemy, cardToPlay->costDice, Omni)) {

			if (cardToPlay->type == ActionType::HEAL) {
				enemy->activeCharacter->hp += 3;
				if (enemy->activeCharacter->hp > 10) enemy->activeCharacter->hp = 10;
				cout << "ENEMY: used heal card" << endl;
			}
			else if (cardToPlay->type == ActionType::ADD_DICE) {
				for (int i = 0; i < 3; i++) {
					Dice* d = new Dice(); d->element = Omni; enemy->dices.push_back(d);
				}
				cout << "ENEMY: used dice card" << endl;
			}
			else if (cardToPlay->type == ActionType::HEAL_ALL) {
				for (CharacterCard* c : enemy->characters) {
					if (c->hp > 0) { c->hp += 2; if (c->hp > 10) c->hp = 10; }
				}
				cout << "ENEMY: used AOE HEAL" << endl;
			}
			else if (cardToPlay->type == ActionType::APPLY_STATUS_ALL) {
				if (enemy->activeCharacter) {
					Element eElem = enemy->activeCharacter->element;
					for (CharacterCard* c : player->characters) {
						if (c->hp > 0) { c->hasAppliedElement = true; c->appliedElement = eElem; }
					}
					cout << "ENEMY: used AOE STATUS card" << endl;
				}
			}
			else {
				enemy->activeCharacter->bonusDmg += 2;
				cout << "ENEMY: used attack card" << endl;
			}


			enemy->hand.erase(enemy->hand.begin() + playableCardIndex);
			OrganizeHand(enemy, enemy->hand);
		}
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

	for (CharacterCard* c : player->characters) c->isFrozen = false;
	for (CharacterCard* c : enemy->characters) c->isFrozen = false;
	for (Dice* d : player->dices)
		delete d;
	player->dices.clear();

	for (Dice* d : enemy->dices)
		delete d;
	enemy->dices.clear();


	for (int i = 0; i < 10; i++) {
		Dice* d1 = new Dice();
		d1->element = static_cast<Element>(rand() % 4); 
		player->dices.push_back(d1);

		Dice* d2 = new Dice();
		d2->element = static_cast<Element>(rand() % 4);
		enemy->dices.push_back(d2);
	}
	for (int i = 0; i < 2; ++i) {
		DrawRandomActionCard(player);
		DrawRandomActionCard(enemy);
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

	OrganizeHand(p, p->hand);
}

void Tema1::DrawRandomActionCard(Player* p) {
	int r = rand() % 5;
	Card* newCard = nullptr;

	if (r == 0) {
		newCard = factory->CreateHealCard();

	}
	else if (r == 1) {
		newCard = factory->CreateAttackBoostCard();
	}
	else if (r == 2) {
		newCard = factory->CreateAddDiceCard();
	}
	else if (r == 3) {
		newCard = factory->CreateHealAllCard();
	}
	else {
		newCard = factory->CreateStatusAllCard();
	}

	DrawCardFromDeck(p, newCard);
}
void Tema1::CheckGameOver() {
	bool playerAlive = false;
	for (CharacterCard* c : player->characters) if (c->hp > 0) playerAlive = true;

	bool enemyAlive = false;
	for (CharacterCard* c : enemy->characters) if (c->hp > 0) enemyAlive = true;

	if (!playerAlive || !enemyAlive) {
		if (!playerAlive) cout << "SYSTEM: enemy won" << endl;
		if (!enemyAlive) cout << "SYSTEM: player won" << endl;

		
		for (CharacterCard* c : player->characters) {
			c->hp = 10; c->isFrozen = false; 
			c->hasAppliedElement = false; 
			c->isActive = false;
			c->bonusDmg = 0;
		}
		for (CharacterCard* c : enemy->characters) {
			c->hp = 10;
			c->isFrozen = false;
			c->hasAppliedElement = false;
			c->isActive = false;
			c->bonusDmg = 0;
		}

		player->characters[0]->isActive = true; player->activeCharacter = player->characters[0];
		enemy->characters[0]->isActive = true; enemy->activeCharacter = enemy->characters[0];

		OrganizeBoard();
		StartNewRound();
	}
}

bool Tema1::ConsumeDices(Player* p, int amount, Element reqElement) {
	//verifying if we have enough dices
	int totalAvailable = 0;
	for (Dice* d : p->dices) {
		if (reqElement == Omni || d->element == reqElement || d->element == Omni) {
			totalAvailable++;
		}
	}
	if (totalAvailable < amount) return false;

	int needed = amount;

	//we first consume the dices tied to that element

	if (reqElement != Omni) {
		for (auto it = p->dices.begin(); it != p->dices.end() && needed > 0;) {
			if ((*it)->element == reqElement) {
				delete* it;
				it = p->dices.erase(it);
				needed--;
			}
			else {
				++it;
			}
		}
	}
	//consuming omni dices
	for (auto it = p->dices.begin(); it != p->dices.end() && needed > 0; ) {
		if (reqElement == Omni || (*it)->element == Omni) {
			delete* it;
			it = p->dices.erase(it);
			needed--;
		}
		else {
			++it;
		}
	}
	return true;
}