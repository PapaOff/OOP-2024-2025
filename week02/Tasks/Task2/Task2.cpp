// Археолог е открил няколко съкровища и записва техните координати в бинарен файл
// treasure.dat. Създайте структура Treasure, която да има име ( до 20 символа),
// координата Х и координата У. Запишете няколко структури в бинарния файл и после ги
// прочетете.

#include <iostream>
#include <fstream>

struct Treasure
{
    char name[20];
    int x, y;
};

void writeTreasures(const char *filePath)
{
    std::ofstream file(filePath, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Can't open file!" << std::endl;
        return;
    }

    Treasure treasures[] = {
        {"Golden Idol", 120, 450},
        {"Silver Sword", 300, 600},
        {"Ancient Coin", 50, 90}};

    for (size_t i = 0; i < 3; i++)
    {
        file.write(reinterpret_cast<const char *>(&treasures[i]), sizeof(Treasure));
    }


	// Можем и директно да запишем целия масив
    // file.write(reinterpret_cast<const char *>(treasures), sizeof(treasures)); // sizeof(tresures) е същото като sizeof(Treasure)*3
    file.close();
    std::cout << "Treasures successfully saved!\n";
}

void readTreasures(const char *filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Can't open file!" << std::endl;
        return;
    }

    Treasure t;
    std::cout << "Treasures:\n";
    while (file.read(reinterpret_cast<char *>(&t), sizeof(Treasure)))
    {
        std::cout << "Name: " << t.name << ", Coordinates: (" << t.x << ", " << t.y << ")\n";
    }

    file.close();
}

int main()
{
    const char *filePath = "treasure.dat";

    writeTreasures(filePath);
    readTreasures(filePath);

    return 0;
}
