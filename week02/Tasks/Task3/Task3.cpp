// В древна библиотека се съхраняват тайни ръкописи в текстов файл library.txt.
// Археологът иска да провери дали дадена дума съществува в ръкописите.
// Проверете дали думата, въведена от археологът се съдържа в текста и изведете подходящо
// съобщение на конзолата.

/ Функция за проверка дали символът е разделител (разделя думите)
bool isSeparator(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\0' || c == '.' || c == ',' || c == '!' || c == '?';
}

bool containsWord(const char *line, const char *word)
{
    int textLen = 0, wordLen = 0;

    // Изчисляваме дължините. Може и strlen
    while (line[textLen] != '\0')
        textLen++;
    while (word[wordLen] != '\0')
        wordLen++;

    for (int i = 0; i <= textLen - wordLen; i++)
    {
        int j;
        for (j = 0; j < wordLen; j++)
        {
            if (line[i + j] != word[j])
                break;
        }

        // Трябва да се уверим, че това което сме намерили е дума, а не част от дума.
        // Например ако търсим думата "кон", а в текста има "конюшна".
        // Затова проверяваме символите преди и след думата.
        if (j == wordLen)
        {
            bool atStart = (i == 0 || isSeparator(line[i - 1])); // Символа преди думата
            bool atEnd = isSeparator(line[i + j]);               // Символът след думата

            return atStart && atEnd;
        }
    }

    return false;
}

void searchWordInLibrary(const char *filePath, const char *searchWord)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cout << "Can't open file!" << std::endl;
        return;
    }

    char line[256];
    bool found = false;

    while (file.getline(line, sizeof(line)))
    {
        if (containsWord(line, searchWord))
        {
            found = true;
            break;
        }
    }

    file.close();

    if (found)
    {
        std::cout << "Word \"" << searchWord << "\" found in library!\n";
    }
    else
    {
        std::cout << "Word \"" << searchWord << "\" not found in library.\n";
    }
}

int main()
{
    const char *filePath = "library.txt";

    char searchWord[50];
    std::cout << "Enter word to find: ";
    std::cin >> searchWord;

    searchWordInLibrary(filePath, searchWord);

    return 0;
}
