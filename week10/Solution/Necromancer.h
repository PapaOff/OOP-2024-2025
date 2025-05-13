#pragma once
#include "Hero.h"

class Necromancer : public Hero {
private:
    Hero** killed;
    int size;
    int capacity;

    void copy(const Necromancer& other);
    void free();
    void resize();
protected:
    void takeDamage(Hero* attacker) override;
public:
    Necromancer(int health, int attack);
    Necromancer(const Necromancer& other);
    Necromancer& operator=(const Necromancer& other);
    virtual ~Necromancer();

    void attackHero(Hero* target) override;
    void print() const override;
    Hero* clone() override;
};
