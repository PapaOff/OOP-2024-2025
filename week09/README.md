# Абстрактни класове. Изключения. Шаблонни функции

## Абстрактни класове
Базов клас, който не може да бъде инстанциран и който съдържа поне една чисто виртуална функция

** Какво е чисто виртуална функция **
Функция, която няма тяло и служи само като интерфейс

Синтаксис:
```
virtual void draw() = 0;
```

> [!NOTE]
> `= 0` не означава, че връща `0`, а че няма дефиниция. Това казва на компилатора "оставям на наследниците да реализират това поведение"

Ползваме абстрактни класове, когато:
 - Искаме да наложим интерфейс, който наследниците задължително трябва да следват
 - Описваме общо поведение, без да знаем или дефинираме конкретна реализация
 
 
** Един абстрактен клас може да има както чисто виртуални, така и обикновени виртуални или дори напълно реализирани функции **

 
> [!IMPORTANT]
> Не можем да създаваме обекти от абстрактни класове

```
Shape s;     // Компилационна грешка
Shape* ps;   // Разрешено (може да сочи към обект на наследник)
```

** Пример **
```
class Shape {
public:
    virtual void draw() = 0; // Чисто виртуална функция
};


class Circle : public Shape {
public:
    void draw() {
        std::cout << "Drawing a circle\n";
    }
};

class Square : public Shape {
public:
    void draw() {
        std::cout << "Drawing a square\n";
    }
};

int main(){
Shape* s1 = new Circle();
Shape* s2 = new Square();

s1->draw();  // "Drawing a circle"
s2->draw();  // "Drawing a square"
return 0;
}
```

> [!CAUTION]
> Понеже не можем да създаваме обекти от клас `Shape` следното не е валидно

```
int main(){
Shape* arr = new Shape[2];
arr[0] = new Circle();
arr[1] = new Square();

arr[0]->draw();  // "Drawing a circle"
arr[1]->draw();  // "Drawing a square"
return 0;
}
```

Ако искаме обаче да имаме масив от наследници на абстрактен клас, може да направим следното:
```
int main(){
Shape** arr = new Shape*[2];
arr[0] = new Circle();
arr[1] = new Square();

arr[0]->draw();  // "Drawing a circle"
arr[1]->draw();  // "Drawing a square"
return 0;
}
```

> [!NOTE]
> Не забравяйте да изчистите паметта

```
for (int i = 0; i < 2; i++) {
    delete arr[i];
}
delete[] arr;
```

> [!WARNING]
> Виртуалният деструктор не се създава по подразбиране, дори ако имаме виртуални функции в класа, затова трябва изрично да го дефинираме

** Пример **
```
class Animal {
public:
    virtual void makeSound() {
        cout << "Generic animal sound\n";
    }
    // Без виртуален деструктор
    ~Animal() {
        cout << "Animal destroyed\n";
    }
};

class Dog : public Animal {
public:
    ~Dog() {
        cout << "Dog destroyed\n";
    }
};

int main() {
    Animal* a = new Dog();
    delete a;  // "Dog destroyed" няма да се извика, само "Animal destroyed"
}
```

Има ли грешка следния код?
```
class Shape {
public:
    virtual void draw() = 0;
    virtual ~Shape() {}
};

class Square : public Shape {
public:
    void draw() const {
        cout << "Drawing a square\n";
    }
};

void render(const Shape& s) {
    s.draw();
}

int main() {
    Square sq;
    render(sq);  // -> "Drawing a square"
}
```

## Изключения
Механизъм за обработка на грешки, който позволява на програмата да реагира на неочаквани събития (като деление на нула, невалиден вход, липсващ файл и др.), без да навлезе в непредвидено състояние

** Основни понятия **
 - `throw` - използва се, когато искаме да генерираме изключение (тоест да сигнализираме, че е възникнал проблем)
 - `try` - ** блокът ** `try` съдържа код, който може да предизвика изключение
 - `catch` - ** блокът ** `catch` улавя изключенията, хвърлени от `try`. Може да има няколко блока `catch` за различни типове изключения
 
** Пример **
```
#include <iostream>

int main() {
    int a = 10;
    int b = 0;

    try {
        if (b == 0) {
            throw "Деление на нула!";
        }
        std::cout << a / b;
    }
    catch (const char* msg) {
        std::cout << "Грешка: " << msg << std::endl;
    }

    return 0;
}
```

> [!IMPORTANT]
> Aко в `try` блока има локални обекти, когато се хвърли изключение автоматично се извикват техните деструктори, за да се освободят ресурсите им преди да се премине към `catch`

** Пример **
```
#include <iostream>

class Test {
public:
    Test() {
        std::cout << "Конструктор\n";
    }
    ~Test() {
        std::cout << "Деструктор\n";
    }
};

int main() {
    try {
        Test t;
        throw "Грешка!";
        std::cout << "Този ред няма да се изпълни\n";
    }
    catch (const char* msg) {
        std::cout << "Хванато изключение: " << msg << std::endl;
    }
    return 0;
}
```

** Изход **
```
Конструктор
Деструктор
Хванато изключение: Грешка!
```

Какво се случва в примера горе?
 - Програмата напуска текущата функция или блок, заради `throw`
 - Всички локални обекти в тази рамка (или в текущия блок), създадени до момента на изключението, се унищожават в обратен ред на създаването им
 - Ако няма `catch` блок на същото ниво, процесът продължава нагоре по стека
 - Ако никъде няма `catch`, се извиква `std::terminate()` → програмата приключва
 
** Този процес се нарича stack unwinding **


Обяснете stack unwinding-a за следния код
```
#include <iostream>

class X {
public:
    X(const char* name) : name(name) {
        std::cout << "Създаден: " << name << std::endl;
    }
    ~X() {
        std::cout << "Унищожен: " << name << std::endl;
    }
private:
    const char* name;
};

void f() {
    X a("a");
    X b("b");
    throw "Грешка!";
    X c("c");
}

int main() {
    try {
        f();
    }
    catch (const char* e) {
        std::cout << "Хванато изключение: " << e << std::endl;
    }
}

```

### Видове изключения
1. Примитивни типове
Можем да хвърляме стойности като int, char, double, const char* и т.н.
```
throw 42;
throw 3.14;
throw "Грешка!";
```
Те работят, но не носят много информация — затова не се препоръчват

2. Стандартни изключения – от `<stdexcept>`
`C++` има вградени класове за най-честите грешки – всички наследяват от `std::exception`

** Често използвани грешки ** 
 | Клас | Предназначение |
 |----------|-----------|
 | `std::exception` | Базов клас за всички стандартни изключения` |
 | `std::runtime_error` | Грешки по време на изпълнение (вход, логика, и т.н.) |
 | `std::logic_error` | Грешки в логиката на програмата |
 | `std::out_of_range` | Достъп до индекс извън граници |
 | `std::invalid_argument` | Подаден невалиден аргумент |
 | `std::bad_alloc` | Недостатъчно памет (от new)	|
 
 
### Изключения в конструктори
Ако в конструктора на обект възникне грешка и се хвърли изключение `throw`, тогава:
 - Обектът не се създава (счита се за "недовършен")
 - Деструкторът на този обект не се извиква, защото обектът никога не е бил успешно създаден
 - Деструкторите на вече създадени членове или базови класове се извикват
 
> [!WARNING]
> Ако в конструктора сме заделили динамична памет, трябва да я освободим преди да стигнем `throw`

** Пример **
```
Person(const char* name, int age) {
    this->name = new char[strlen(name)+1];
	strcpy(this->name, name);
    if (age < 18) 
        {
            delete[] name;
            throw std::invalid_argument("Age of person must be at least 18");
        }
    }
}
```

### `Catch` блокове
Може да имаме повече от един `catch` блок. Важно е да ги подреждаме от най-специфичен към най-общ
Добра практика е да ползваме `const` и `&` при хващането на изключения

** Пример **
```
try {
    throw std::out_of_range("Грешка");
}
catch (const std::invalid_argument& e) {
	std::cout << e.what() << std::endl;
    // няма да се изпълни
}
catch (const std::out_of_range& e) {
	std::cout << e.what() << std::endl;
    // този ще се изпълни
}
catch (const std::exception& e) {
	std::cout << e.what() << std::endl;
    // щеше да се изпълни, ако горното не се беше изпълнилно
}
catch (...) {
    // хваща всякакви грешки, включително и от примитивен тип
    // не можем да извикаме what()
}
```

### `nothrow`
 - не хвърля изключение, а `nullptr`
 
```
int* p = new (std::nothrow) int[1000000000];
if (!p) {
    std::cout << "Allocation failed\n";
}
```

### Създаване на собствени изключения
1. Наследяване от `std::exception`
```
class MyException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Custom exception";
    }
};
```
> [!NOTE]
> Задължително пренапишете `what()`, за да контролирате какво ще се изписва като грешка 


2. Наследяване от `std::logic_error`
```
#include <iostream>
#include <stdexcept>


class InvalidRevenueInput : public std::logic_error {
public:
    InvalidRevenueInput(const char* msg)
        : std::logic_error(msg) {}
};

double calculateGrowthRate(double oldRevenue, double newRevenue) {
    if (oldRevenue < 0 || newRevenue < 0) {
        throw InvalidRevenueInput("Приходите не могат да са отрицателни.");
    }
    if (oldRevenue == 0) {
        throw InvalidRevenueInput("Началният приход не може да е 0.");
    }

    return (newRevenue - oldRevenue) / oldRevenue * 100.0;
}

int main() {
    try {
        double growth = calculateGrowthRate(-1000, 2000);
        std::cout << "Ръст: " << growth << "%\n";
    }
    catch (const InvalidRevenueInput& e) {
        std::cout << "Грешка в логиката на входа: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Обща грешка: " << e.what() << std::endl;
    }

    return 0;
}
```

** Изход **
```
Грешка в логиката на входа: Невалиден вход: Приходите не могат да са отрицателни.
```

## Шаблонни функции
 - функция с параметър тип, който се определя при извикване, а не при дефиниция
 - позволяват ни да пишем универсален код, който работи с различни типове, без да го дублираме
 
Синтаксис
 ```
template <typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}


int main() {
	int a = max(4, 7);         // T = int
	double d = max(3.5, 2.1);  // T = double
}
 ```
 
Шаблони с два различни типа
```
template <typename T1, typename T2>
void printSum(T1 a, T2 b) {
    std::cout << "Sum is: " << (a + b) << std::endl;
}
```


### Function pointer
```
void map(int* arr, int size, int (*mappingFunction)(int))
{
    for (int i = 0; i < size; i++)
    {
        arr[i] = mappingFunction(arr[i]);
    }
}

int timesTwo(int x)
{
    return x * 2;
}

int main()
{
    int a[] = {1, 2, 3, 4, 5};

    map(a, 5, timesTwo);

    for (int i = 0; i < 5; i++)
    {
        cout << a[i] << " "; // 2 4 6 8 10
    }

    return 0;
}
```
