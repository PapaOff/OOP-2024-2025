#include "Knight.h"

Knight::Knight(int health, int attack, int armor) : Hero(health, attack) {
	this->armor = armor;
}
virtual Knight::~Knight() {
	armor = 0;
}

void Knight::attackHero(Hero* target) {
    target->takeDamage(this);
}

void Knight::takeDamage(Hero* attacker) {
    if (dynamic_cast<Mag*>(attacker)) {
        health -= attacker->getAttack();
		return;
    }
	
	int dmg = attacker->getAttack();
    if (armor >= dmg) {
        armor -= dmg;
    } else {
        int leftover = dmg - armor;
        armor = 0;
        health -= leftover;
    }

    if (health < 0) health = 0;
}

void Knight::print() const {
	std::cout << "Knight: health " << getHealth() << " armor " << armor << " attack " << getAttack();
}
Hero* Knight::clone() {
	return new Knight(*this);
}