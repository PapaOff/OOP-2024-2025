#include "Hero.h"

void Hero::setHealth(int amount){
	if(amount < 0){
		return;
	}
	
	health = amount;
}

Hero::Hero(int health, int attack) : health(health), attack(attack){}

Hero& Hero::operator=(const Hero& other) {
	if(this != &other){
		health = other.health;
		attack = other.attack;
	}
	
	return *this;
}

Hero::~Hero(){
	health = 0;
	attack = 0;
}

int Hero::getHealth() const
{
	return health;
}

int Hero::getAttack() const
{
	return attack;
}