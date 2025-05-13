#pragma once
#include "Hero.h"

class Mag : public Hero {
 protected:
    void takeDamage(Hero* attacker) override;
 public:
    Mag(int health, int attack);

    void attackHero(Hero* target) override;
    void print() const override;
    Hero* clone() override;
    virtual ~Mag();
};
