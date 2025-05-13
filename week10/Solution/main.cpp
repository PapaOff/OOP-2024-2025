#include "Knight.h"
#include "Mag.h"
#include "Necromancer.h"

int main() {
    Hero* knight = new Knight(100, 15, 30);
    Hero* mag = new Mag(60, 12);
    Hero* necro = new Necromancer(80, 8);

    std::cout << "\n--- Necromancer атакува Knight ---\n";
    necro->attackHero(knight);
    knight->print();
    necro->print();

    std::cout << "\n--- Mag атакува Knight ---\n";
    mag->attackHero(knight);
    knight->print();

    std::cout << "\n--- Necromancer атакува Mag ---\n";
    necro->attackHero(mag);
    mag->print();
    necro->print();

    delete knight;
    delete mag;
    delete necro;

    return 0;
}
