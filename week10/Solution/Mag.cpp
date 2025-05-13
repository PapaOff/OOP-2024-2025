#include "Mag.h"
#include <iostream>

Mag::Mag(int health, int attack): Hero(health, attack) {}

void Mag::takeDamage(Hero* attacker) {
    setHealth(getHealth() - hero->getAttack());
}

void Mag::attackHero(Hero* target) {
    target->takeDamage(this);
}

void Mag::print() const {
    std::cout << "Mag: health " << getHealth() << " attack " << getAttack() << "\n";
}

Hero* Mag::clone() {
    return new Mag(*this);
}

Mag::~Mag() {}
