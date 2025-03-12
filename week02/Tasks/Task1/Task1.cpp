// Древен свитък с важни записки е намерен, но мастилото е избледняло и някои думи са
// изгубени. За щастие, текстът е записан в scroll.txt, но ти трябва да го възстановиш,
// като прочетеш и отпечаташ съдържанието му. Измежду думите са скрити и цифри.
// Тези цифри образуват тайна парола, която трябва да бъде записана в бинарен файл,
// за да не може да се прочете с просто око

#include <iostream>
#include <fstream>

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

// Това е лоша практика. Функцията трябва да прави само 1 нещо
void printTextAndExtractPassword(const char *inputFile, const char *outputFile)
{
    std::ifstream file(inputFile);
    if (!file.is_open())
    {
        std::cout << "Oh, no! I can't open the file... the script is lost!" << std::endl;
        return;
    }

    char line[256];    // Буфер за ред от файла
    char password[21]; // Буфер за паролата (максимално 20 цифри и 1 за терминираща 0)
    int passIndex = 0;

    std::cout << "Script content:\n";
    while (file.getline(line, sizeof(line)))
    {
        std::cout << line << std::endl;

        for (int i = 0; line[i] != '\0'; i++)
        {
            if (isDigit(line[i]) && passIndex <= 20)
            {
                password[passIndex++] = line[i];
            }
        }
    }

    file.close();
    password[passIndex] = '\0';

    std::ofstream binFile(outputFile, std::ios::binary);
    if (!binFile.is_open())
    {
        std::cout << "Can't open file!" << std::endl;
        return;
    }

    // Тук password не го подаваме по &, защото е указател и те сочат към &
    binFile.write(reinterpret_cast<const char *>(password), passIndex);
    binFile.close();

    std::cout << "\nPassword saved!\n";
}

void readPassword(const char *outputFile)
{
    std::ifstream binFile(outputFile, std::ios::binary);
    if (!binFile.is_open())
    {
        std::cout << "Can't open file!" << std::endl;
        return;
    }

    char password[20];
    binFile.read(reinterpret_cast<char *>(password), sizeof(password));
    binFile.close();

    std::cout << "Password from binary file: " << password << std::endl;
}

int main()
{
    const char *inputFile = "scroll.txt";
    const char *outputFile = "password.dat";

    printTextAndExtractPassword(inputFile, outputFile);
    readPassword(outputFile);

    return 0;
}
