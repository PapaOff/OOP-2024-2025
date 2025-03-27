# Абстракция. Константни обекти. Приятелски функции и приятелски класове.

## Абстракция
Абстракцията е процес на извличане на съществените характеристики на обектите и скриване на детайлите, които не са от значение за външния потребител. 
Казано просто: показваме какво прави един обект, без да разкриваме как го прави.

**В C++ има два основни начина да се постигне абстракция:**

 1. Чрез класове - класът сам по себе си е абстракция.Той описва поведението и характеристиките на обекти, без да навлизаме в технически подробности.
 
 ```
 class BankAccount {
    double balance;
public:
    void deposit(double amount);
    void withdraw(double amount);
    double getBalance() const;
};
 ```
 
Тук потребителят не го интересува как точно работят deposit() и withdraw(). За него е важно да знае, че работят и може да ги ползва.

 2. Чрез абстрактни класове/интерфейси - това е материал от следващите семинари
 
 **Цели на абстракцията**
  - Скриване на ненужната сложност
  - Подобрена четимост и поддръжка на кода
  - Насърчава преизползваемост на кода
  - Позволява заместване на конкретна реализация, без да се променя останалия код ( част от SOLID принципите, постига се с абстрактни класове/интерфейси)
  
> [!NOTE]
> Eнкапсулацията скрива данни, абстракцията скрива сложността

## Константни обекти
 - Тези обекти могат да използват само константни функции, защото не могат да променят нищо в себе си.
 - В константни методи `this` е от тип `const MyClass* const`
 
 Предните пъти споменахме, че е добра практика да подаваме обекти по const&, когато са по-обемни.
 Нека например имаме клас `Person`, който има име, възраст и височина. Една имплементация може да изглежда по следния начин:
 
 ```
 class Person
{
private:
    char name[32];
    short age;
    float height;

public:
    Person(const char* personName, short personAge, float personHeight)
    {
        setName(personName);
        setAge(personAge);
        setHeight(personHeight);
    }

    void setName(const char* newName)
    {
        if (newName == nullptr)
            return;

        strcpy(name, newName);
    }

    void setAge(short newAge)
    {
        if (newAge <= 0)
            return;

        age = newAge;
    }

    void setHeight(float newHeight)
    {
        if (newHeight <= 0)
            return;

        height = newHeight;
    }

    char* getName()
    {
        return name;
    }

    short getAge()
    {
        return age;
    }

    float getHeight()
    {
        return height;
    }
};
 ```
 
 Този обект е сравнително обемен, за това бихме го подали като `const&` на някоя функция, например за принтиране:
 
 ```
 void printPerson(const Person& person)
{
    std::cout << person.getName() << ' '
              << person.getAge() << ' '
              << person.getHeight() << '\n';
}
 ```
 
 Получаваме грешка, защото подаваме `Person` като `const`, но не сме направили функциите `const`. След като добавим `const` на фунцкиите няма да имаме проблем.
 
  ```
class Person
{
private:
    // ...
public:
    //...

    char* getName() const
    {
        return name;
    }

    short getAge() const
    {
        return age;
    }

    float getHeight() const
    {
        return height;
    }
};       << person.getHeight() << '\n';
}
 ```
 
## Приятелски функции и приятелски класове
 - реализират се чрез ключовата дума `friend`
 
 **Кога не се използва**
 - Когато можеш да използваш getter или член-функция
 - Когато можеш да постигнеш същото чрез интерфейс
 
### Приятелски класове
Клас, който има достъп до всички членове (вкл. private и protected) на друг клас

```
class Engine;

class Car {
    int speed;
    friend class Engine; // Engine има достъп до Car::speed
};

class Engine {
public:
    void boost(Car& c) {
        c.speed += 50; // OK: има достъп
    }
};
```

> [!NOTE]
> Приятелството не е взаимно. Ако A е приятел на B, това не значи, че B е приятел на A

> [!NOTE]
> Приятелството не се наследява

> [!NOTE]
> Приятелският достъп се декларира вътре в класа, който "споделя" достъп


### Приятелска функция
Функция, която **не е** член на класа, но има **пълен достъп** до неговите private и protected членове
 
 
 **Пример**
 ```
 class Point {
    int x, y;
    friend std::ostream& operator<<(std::ostream& os, const Point& p); // декларация
public:
    Point(int x, int y) : x(x), y(y) {}
};

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}
 ```
 
 > [!NOTE]
 > `Операторът <<` не може да бъде член на класа `Point`, защото лявата страна на операцията е обект от тип `std::ostream`, който е външен за нашия клас и не можем да го променяме. Казано по-подробно, понеже `operator<<` е бинарен оператор, той очаква два операнда. В случая лявата страна е `std::ostream`, а дясната е обект от нашия клас. Ако направим `operator<<` член на класа `Point`, ще се изисква лявата страна на операцията да е обект от тип `Point`, което не съответства на начина, по който операторът се използва в практиката. Затова дефинираме `operator<<` като външна функция, която при нужда правим приятелска, за да получим достъп до вътрешните данни на класа.
 
### Приятелски клас при разделна компилация
 `Car.h`
 
 ```
#pragma once

class Engine; // предварителна декларация

class Car {
    int speed;
    friend class Engine; // Engine има достъп до speed
public:
    Car();
};
 ```
 
`Car.cpp`

```
#include "Car.h"

Car::Car() : speed(100) {}
```

`Engine.h`
```
#pragma once

#include "Car.h"

class Engine {
public:
    void boost(Car& c);
};
```

`Engine.cpp`

```
#include "Engine.h"
#include <iostream>

void Engine::boost(Car& c) {
    c.speed += 50;
    std::cout << "Boosted speed: " << c.speed << std::endl;
}
```

`main.cpp`

```
#include "Engine.h"
#include "Car.h"

int main() {
    Car car;
    Engine e;
    e.boost(car);
}
```


### Приятелска функция при разделна компилация

`Point.h`

```
#pragma once

#include <iostream>

class Point {
    int x, y;

    friend std::ostream& operator<<(std::ostream& os, const Point& p); // само декларация
public:
    Point(int x, int y);
};
```

`Point.cpp`

```
#include "Point.h"

Point::Point(int x, int y) : x(x), y(y) {}

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}
```

`main.cpp`

```
#include "Point.h"

int main() {
    Point p(1, 2);
    std::cout << p << std::endl;
}
```
