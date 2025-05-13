#include "Necromancer.h"
#include <iostream>

Necromancer::Necromancer(int health, int attack): Hero(health, attack), size(0), capacity(4) {
    killed = new Hero*[capacity];
}

Necromancer::Necromancer(const Necromancer& other): Hero(other) {
    copy(other);
}

Necromancer& Necromancer::operator=(const Necromancer& other) {
    if (this != &other) {
        Hero::operator=(other);
        free();
        copy(other);
    }
    return *this;
}

Necromancer::~Necromancer() {
    free();
}

void Necromancer::copy(const Necromancer& other) {
    size = other.size;
    capacity = other.capacity;
    killed = new Hero*[capacity];
    for (int i = 0; i < size; ++i) {
        killed[i] = other.killed[i]->clone();
    }
}

void Necromancer::free() {
    for (int i = 0; i < size; ++i) {
        delete killed[i];
    }
    delete[] killed;
    killed = nullptr;
    size = 0;
    capacity = 0;
}

void Necromancer::resize() {
    capacity *= 2;
    Hero** newArr = new Hero*[capacity];
    for (int i = 0; i < size; ++i) {
        newArr[i] = killed[i];
    }
    delete[] killed;
    killed = newArr;
}

void Necromancer::attackHero(Hero* target) {
    if (!target || target->getHealth() <= 0) return;

    target->takeDamage(this);
    if (target->getHealth() <= 0) {
        if (size >= capacity) resize();
        killed[size++] = target->clone();
        return;
    }

    for (int i = 0; i < size && target->getHealth() > 0; ++i) {
        target->takeDamage(killed[i]);
        if (target->getHealth() <= 0) {
            if (size >= capacity) resize();
            killed[size++] = target->clone();
            break;
        }
    }
}

void Necromancer::takeDamage(Hero* attacker) {
    health -= attacker->getAttack();
    if (health < 0) health = 0;
}

void Necromancer::print() const {
    std::cout << "Necromancer: health " << getHealth() << " attack " << getAttack() << "\n";
    std::cout << "Killed souls:\n";
    for (int i = 0; i < size; ++i) {
        killed[i]->print();
    }
}

Hero* Necromancer::clone() {
    return new Necromancer(*this);
}
