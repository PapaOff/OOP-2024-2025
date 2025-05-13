#pragma once
#include "Hero.h"
#include "Mag.h"

class Knight : public Hero {
 private:
	int armor;
 protected:
    void takeDamage(Hero* attacker) override;
 public:
	Knight(int health, int attack, int armor);
	virtual ~Knight();
	void attackHero(Hero* target) override;
	void print() const override;
	Hero* clone() override;
}