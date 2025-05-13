#pragma once

class Hero{
 private:
	int health;
	int attack;
 protected:
	setHealth(int amount);
	virtual void takeDamage(Hero* attacker) = 0;
 public:
	Hero(int health, int attack);
	Hero& operator=(const Hero& other);
	virtual ~Hero();
	virtual void attackHero(Hero* target) = 0;
	virtual void print() const = 0;
	virtual Hero* clone() = 0;
	int getHealth() const;
	int getAttack() const;
};