# Статични член-данни. Статични функции. Предефиниране на оператори.

## Статични член-данни
 - Принадлежат на самия клас, а не на конкретен обект от този клас. Тоест всички обекти от класа споделят една и съща стойност на тази променлива
 - Достъпва се чрез обект или чрез самия клас чрез `MyClass::променлива`
 - Трябва да се дефинира извън класа – дори да е декларирана в класа, тя задължително се дефинира извън него
 - Може да се използва за създаване на глобални броячи или споделени стойности
 
 **Пример:**
 ```
 #include <iostream>

class MyClass {
public:
    static int count; // декларация

     void showCount() {
        std::cout << "count = " << count << std::endl;
    }
};

// дефиниция извън класа
int MyClass::count = 0;

int main() {
    MyClass a;
    MyClass b;

    a.count = 5;  // достъп чрез обект

    std::cout << "b.count = " << b.count << std::endl;  // също ще е 5
    b.count = 10;

    std::cout << "a.count = " << a.count << std::endl;  // понеже count е общ, променяйки го чрез обекта b, то ще е 10 и за обекта а
    std::cout << "MyClass::count = " << MyClass::count << std::endl;  // достъп чрез класа, също е 10

    return 0;
}
 ```
 
## Константни статични член-данни
 Има два начина:
  - с `const`
  - с `constexpr`
  
  Ако е с `const`, може да инициализираме променлива в класа **само** за `int`
  Ако е с `constexpr`, може да инициализираме всякакъв вид променлива в класа

 > [!NOTE]
 > `constexpr` означава, че стойността е известна по време на компилация
 
 **Примери**
 ```
 #include <iostream>

 class MyClass {
 public:
     static const int maxSize = 100;  // инициализация директно в класа
 };

 int main() {
     std::cout << MyClass::maxSize << std::endl;  // изход: 100
     return 0;
 }
 ```
 
 Ако имаме различно от `int`
 ```
 class A {
public:
    static const double pi = 3.14; // Грешка! Не е позволено в класа
};
 ```
 
 Трябва да се направи така
 ```
 class A {
 public:
     static const double pi;  // само декларация
 };

 const double A::pi = 3.14;  // инициализация извън класа
 ```
 
 При `constexpr` можем да инициализираме променливите в класа
 ```
 #include <iostream>

 class MyClass {
 public:
     static constexpr int version = 2;  // инициализация в класа
	 static constexpr double pi = 3.14;  // валидно
 };

 int main() {
     int array[MyClass::version];  // работи, защото е compile-time стойност
     std::cout << "Array size: " << sizeof(array) / sizeof(int) << std::endl;
     return 0;
 }
 ```
 
 
 ## Статична функция
 Статичната функция е обикновена функция, декларирана със `static`, но **функцията е видима само в текущия .cpp файл**
 
  > [!NOTE]
  > Изполва се за скриване на функция от други файлове

**Пример**
```
// В some_file.cpp
static void helper() {
    // Тази функция не може да бъде извикана от други файлове
}
```

 ## Статична член-функция
 Това е функция вътре в клас, декларирана със `static`
 
  > [!NOTE]
  > Тя не може да достъпва **нестатични** член-данни или член-функции, защото не получава `this` указател
  
  > [!NOTE]
  > Не ни е нужен обект, за да я достъпим
  
 Изполва се за:
  - Работа със `static` член-данни
  - Помощни функции, които не зависят от обект
  - Фабрични методи, броячи и др.
  
 **Пример**
```
class MyClass {
public:
    static int counter;

    static void increment() {
        counter++;
    }
};

int MyClass::counter = 0;

int main() {
    MyClass::increment();  // извикване без обект
}
```

## Предефиниране на оператори
Предефинирането на оператори (operator overloading) позволява да дефинираме как да се държат стандартни оператори (+, -, ==, << и т.н.).
Това позволява обекти от класове да се използват по „естествен“ начин, както се използват вградените типове

Може да се предефинират почти всички оператори. Следните не може:
 - `::` (оператор за обхват)
 - `.` (достъп до член)
 - `sizeof`, `typeid`, `alignof`, `static_cast` и други compile-time оператори
 - `?:` (тернарен оператор)
 
  > [!NOTE]
  > Не може да се дефинират нови оператори, различни от съществуващите в езика
  
  > [!NOTE]
  > За да различим префиксен от постфиксен оператор `++`, се използва допълнителен параметър в декларацията на функцията при постфиксните оператори
  
**Пример**
```
#include <iostream>

class Counter {
    int value;

public:
    Counter(int v = 0) : value(v) {}

    // Префиксен ++
    Counter& operator++() {
        ++value;
        return *this;
    }

    // Постфиксен ++
    Counter operator++(int) { // int служи само за разграничение
        Counter temp = *this;
        ++value;
        return temp;
    }

    void print() const {
        std::cout << value << std::endl;
    }
};
```

Когато предефинираме един оператор, често има други оператори, които логически са свързани с него и те също трябва да бъдат предефинирани
| Ако предефинираш | Препоръчително е да предефинираш и |
|----------|-----------|
|`==`|	`!=`		|
|`<`|	`>`, `<=`, `>=`)	|
|`+`	|`+=`	|
|`-`|	`-=`	|
|`++`|	Префиксен и постфиксен	|
|`[]`|	Версия за const и не-const достъп|


**Пример с `==` и `!=`**
```
class Point {
    int x, y;

public:
    Point(int x = 0, int y = 0) : x(x), y(y) {}

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other); // използваме вече дефинирания ==
    }
};
```

  > [!NOTE]
  >  Така `!=` няма нужда да повтаря логиката – можем да я изградим върху вече дефинирания `==`
  
  
**Пример с `<`, `>`, `<=`, `>=`**
```
class Fraction {
    int numerator;
    int denominator;

public:
    Fraction(int n, int d) : numerator(n), denominator(d) {}

    bool operator<(const Fraction& other) const {
        return numerator * other.denominator < other.numerator * denominator;
    }

    bool operator>(const Fraction& other) const {
        return other < *this;
    }

    bool operator<=(const Fraction& other) const {
        return !(other < *this);
    }

    bool operator>=(const Fraction& other) const {
        return !(*this < other);
    }
};
```

**Обяснение за `operator<=`**
Тук `other < *this` проверява дали текущият обект е по-голям от другия (т.е. дали `*this > other`). Ако не е, тогава `this <= other`


### Предефиниране на оператори в класа, като friend или извън класа

#### Предефиниране на оператори в класа
Използваме ги, когато:
 - Те са тясно свързани с вътрешното състояние на обекта
 - работят върху текущия обект `this`
 
 **Пример**
 ```
 char& operator[](unsigned index);
 MyString& operator+=(const MyString& other);
 MyString substr(unsigned begin, unsigned howMany);
 ```
 
 Всички те работят с вътрешната памет (data, size, capacity) и манипулират текущия обект, затова са член-функции
 
#### Предефиниране на оператори като приятелски функции
Използваме ги, когато:
 - Лявият операнд не е текущият обект `this`
 - Когато искаме да използваме синтаксис, при който обектът не е непременно отляво
 
 **Пример**
 ```
friend MyString operator+(const MyString& lhs, const MyString& rhs);
friend std::ostream& operator<<(std::ostream& os, const MyString& str);
friend std::istream& operator>>(std::istream& is, MyString& str);
 ```
 
**Пояснение**
 - `operator+` трябва да достъпи `lhs` и `rhs`, но не искаме да модифицираме `lhs` ⇒ по-добре да е външна функция
 - `<<` и `>>` работят с потоци – първият аргумент е `std::ostream`/`std::istream`, а той не може да бъде член на нашия клас
 
 
#### Предефиниране на оператори като външни функции
Използваме ги, когато функцията няма нужда да достъпва `private` членове

**Пример**
```
bool operator==(const MyString& lhs, const MyString& rhs);
```

Ако `MyString` има публична функция `c_str()`, може да сравняваме чрез тях и така няма нужда операторът да е `friend`
Ако обаче няма публичен достъп до съдържанието, тогава `==` ще трябва да стане `friend`, за да има достъп до `data`


| Тип функция | Кога се използва |
|----------|-----------|
|Член-функция|	Работи с текущия обект `this`, ползва вътрешно състояние		|
|Приятелска функция|	Не е член, но трябва да има достъп до `private`, работи с два обекта (напр. `+`)	|
|Външна функция|Може да се реализира използвайки публични функции	|
