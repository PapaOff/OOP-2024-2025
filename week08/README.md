# Наследяване. Базови и производни класове. Предефиниране на функции-елементи от базовия клас в производния клас. Използване на конструктори и деструктори в производни класове. Множествено наследяване. Виртуални функции и полиморфизъм.

## Наследяване
Наследяването е механизъм в обектно-ориентираното програмиране, чрез който един клас (производен) може да наследи член-данни и функции от друг клас (базов)

**Какво печелим?**
 - Повторна употреба на вече написан код
 - По-добра организация на кода чрез йерархии
 
 **Пример**
 ```
 class Animal {
public:
    void speak() {
        std::cout << "Some generic animal sound\n";
    }
};

class Dog : public Animal {
    // Наследява speak()
};

int main()
{
    Dog d;
    d.speak(); //Some generic animal sound
}
 ```
 
 > [!NOTE]
 > `public` е модификатор на наследяване – той определя какви ще бъдат нивата на достъп до наследените членове
 
 **Модификатори на наследяване**
 | Модификатор | Public членове | Protected членове | Private членове |
 |----------|-----------|-----------|-----------|
 | `public` | остават `public` | остават `protected` | недостъпни |
 | `protected` | стават `protected`) | остават `protected` | недостъпни |
 | `private` | стават  `private`	| стават  `private` | недостъпни |
 
 
## Предефиниране на функции в производни класове
 В примера горе кучето няма собствено поведение, а използва default-ното, наследено от базовия клас.
 Когато искаме производните класове да имат различно поведение, трябва да предефинираме (override) функциите от базовия клас
 
 ```
class Animal {
public:
    void speak() {
        std::cout << "Some generic animal sound\n";
    }
};

class Dog : public Animal {
	public:
    void speak() {
        std::cout << "Bau bau\n";
    }
};

int main() {
    Dog d;
    d.speak();
}
 ```
 
## Конструктори и Деструктори
** Ред на извикване **
 - При конструктори първо се извиква конструкторът на базовия клас, а след това се извиква конструкторът на производния клас
 - При дестурктори първо се извиква деструкторът на производния клас, а след това на базовия клас
 
** Пример **
```
class A {
public:
    A() { std::cout << "A constructor\n"; }
    ~A() { std::cout << "A destructor\n"; }
};

class B : public A {
public:
    B() { std::cout << "B constructor\n"; }
    ~B() { std::cout << "B destructor\n"; }
};

int main() {
    B obj;
}
```
**Резултат**
```
A constructor
B constructor
B destructor
A destructor
```

### Конструктори при наследяване
Както се вижда от примера горе когато производен клас се създава, базовият клас винаги се инициализира първи, независимо дали използваме конструктор по подразбиране или параметризирани

 > [!NOTE]
 > Когато в конструктора на производен клас трябва да инициализираме член-данни, които принадлежат на базовия клас, това става чрез извикване на конструктора на базовия клас
 
 ** Пример **
 ```
class A {
    int x;
public:
    A(int val) {
        x = val;
        std::cout << "Constructor A setting x = " << x << "\n";
    }
};

class B : public A {
    int y;
public:
    B(int x, int y) : A(x), y(y) {
        std::cout << "Constructor B setting y = " << y << "\n";
    }
};

int main() {
    B obj(5, 10);  
}
 ```
 
 ** Резултат **
 ```
Constructor A setting x = 5
Constructor B setting y = 10
 ```
 
 > [!NOTE]
 > Ако в конструктора на производния клас не сме оказали кой конструктор на базовия да се извика, ще се извика конструктора по подразбиране. Ако такъв няма, ще получим компилационна грешка
 
 ** Така дава грешка **
 ```
class A {
    int x;
public:
    A(int val) {
        x = val;
        std::cout << "Constructor A setting x = " << x << "\n";
    }
};

class B : public A {
    int y;
public:
    B(int x, int y) : y(y) {
        std::cout << "Constructor B setting y = " << y << "\n";
    }
};

int main() {
    B obj(5, 10);
}
 ```
 
  ** Така НЕ дава грешка **
 ```
class A {
    int x;
public:
	A() {
        x =0;
        std::cout << "Constructor A setting x = " << x << "\n";
    }
    A(int val) {
        x = val;
        std::cout << "Constructor A setting x = " << x << "\n";
    }
};

class B : public A {
    int y;
public:
    B(int x, int y) : y(y) {
        std::cout << "Constructor B setting y = " << y << "\n";
    }
};

int main() {
    B obj(5, 10);
}
 ```
 
### Copy конструктор и operator= при наследяване и динамична памет

Ако в производния клас пишем copy конструктор и operator= **трябва** да извикаме copy конструктор и operator= на базовия

** Ако не ги извикаме **

```
#include <iostream>
#include <cstring>

class A {
    int x;
public:
    A(int val = 0) : x(val) {}

    void print() const {
        std::cout << "A: " << x << "\n";
    }
};

class B : public A {
    char* name;
public:
    B(int val, const char* n) : A(val) {
        name = new char[strlen(n) + 1];
        strcpy(name, n);
    }

    // Копиращ конструктор (не викаме A(other))
    B(const B& other) {
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
    }

    // Оператор за присвояване (не викаме A::operator=)
    B& operator=(const B& other) {
        if (this != &other) {
            delete[] name;
            name = new char[strlen(other.name) + 1];
            strcpy(name, other.name);
        }
        return *this;
    }

    ~B() {
        delete[] name;
    }

    void print() const {
        A::print();
        std::cout << "B: " << name << "\n";
    }
};

int main() {
    B b1(5, "hello");
    B b2 = b1;         // Копиращ конструктор
    B b3(1, "tmp");
    b3 = b1;           // Оператор за присвояване

    b1.print();
    b2.print();
    b3.print();
}
```

** Изход **
```
A: 5
B: hello
A: 0
B: hello
A: 1
B: hello
```

 > [!NOTE]
 > Копирането и присвояването не работят правилно, защото `х` не е 5 при всеки обект



** Ако ги извикаме **

```
#include <iostream>
#include <cstring>

class A {
    int x;
public:
    A(int val = 0) : x(val) {}

    void print() const {
        std::cout << "A: " << x << "\n";
    }
};

class B : public A {
    char* name;
public:
    B(int val, const char* n) : A(val) {
        name = new char[strlen(n) + 1];
        strcpy(name, n);
    }

    B(const B& other) : A(other){
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
    }

    B& operator=(const B& other) {
        if (this != &other) {
			A::operator=(other);
            delete[] name;
            name = new char[strlen(other.name) + 1];
            strcpy(name, other.name);
        }
        return *this;
    }

    ~B() {
        delete[] name;
    }

    void print() const {
        A::print();
        std::cout << "B: " << name << "\n";
    }
};

int main() {
    B b1(5, "hello");
    B b2 = b1;         // Копиращ конструктор
    B b3(1, "tmp");
    b3 = b1;           // Оператор за присвояване

    b1.print();
    b2.print();
    b3.print();
}
```

** Изход **
```
A: 5
B: hello
A: 5
B: hello
A: 5
B: hello
```

 > [!NOTE]
 > Ако производния клас няма динамична памет не е нужно да пишем голяма 4ка, дори базовият клас да има динамична памет
 
 Следния код работи правилно
 ```
#include <iostream>
#include <cstring>

class A {
    char* text;
public:
    A(const char* t = "") {
        text = new char[strlen(t) + 1];
        strcpy(text, t);
    }

    A(const A& other) {
        text = new char[strlen(other.text) + 1];
        strcpy(text, other.text);
    }

    A& operator=(const A& other) {
        if (this != &other) {
            delete[] text;
            text = new char[strlen(other.text) + 1];
            strcpy(text, other.text);
        }
        return *this;
    }

    ~A() {
        delete[] text;
    }

    void print() const {
        std::cout << "A: " << text << "\n";
    }
};

class B : public A {
private:
 int x;
public:
    B(int x,const char* t) : A(t) {
        this->x = x;
    }
    
    void print() const {
        A::print();
        std::cout << "X =  " << x << "\n";
    }
};

int main() {
    B b1(1, "hello");
    B b2 = b1;        // копиращ конструктор
    B b3(2, "temp");
    b3 = b1;          // оператор=

    b1.print();
    b2.print();
    b3.print();
}
 ```

### Деструктор
Не е нужно ръчно да викаме деструктора на базовия клас. Това става автоматично – компилаторът се грижи за реда

## Object slicing
 ```
class Animal {
public:
    void speak() {
        std::cout << "Some generic animal sound\n";
    }
};

class Dog : public Animal {
	public:
    void speak() {
        std::cout << "Bau bau\n";
    }
	
	void sniff() {
		std::cout << "Sniffing\n";
	}
};

int main() {
    Dog d;
    Animal a = d; // slicing — копира се само частта от Dog, която е Animal
    a.speak();   // отпечатва Some generic animal sound
    //a.sniff(); // Грешка: sniff() не съществува в Animal
}
 ```
 
 ** Защо ни интересува това? **
```
#include <iostream>

class Animal {
public:
    void speak() {
        std::cout << "Some generic animal sound\n";
    }
};

class Dog : public Animal {
public:
    void speak() {
        std::cout << "Bau bau\n";
    }

    void sniff() {
        std::cout << "Sniffing\n";
    }
};

class Cat : public Animal {
public:
    void speak() {
        std::cout << "Meow\n";
    }
};

int main() {
    Dog d;
    Cat c;

    Animal animals[2];
    animals[0] = d;
    animals[1] = c;

    for (int i = 0; i < 2; ++i) {
        animals[i].speak(); // Ще извика Animal::speak() за всеки обект
    }
}
```

## Множествено наследяване
В С++ клас може да наследява от повече от един базов клас

** Ползи **
 - Може да комбинираме различни функционалности от повече от един клас
 
** Синтаксис **
```
class A {
public:
    void f() { std::cout << "A::f()\n"; }
};

class B {
public:
    void g() { std::cout << "B::g()\n"; }
};

class C : public A, public B {
    // Наследява и от A, и от B
};

int main() {
	C c;
	c.f(); // от A
	c.g(); // от B
}
``` 

** При множественото наследяване може да възникне конфликт, когато два базови класа имат функция с едно и също име**

```
class A {
public:
    void f() { std::cout << "A::f()\n"; }
};

class B {
public:
    void f() { std::cout << "B::f()\n"; }
};

class C : public A, public B {};


int main() {
	C c;
	// c.f(); Грешка: неясно коя `f()` да се извика
	c.A::f(); // Изрично посочваме
	c.B::f(); // Изрично посочваме
}
```

### Ред на конструктори и деструктори при множествено наследяване
 - Конструкторите се извикват в реда на наследяване
 - Деструкторите се извикват в обратния ред
 
```
class A { public: A() { std::cout << "A\n"; } };
class B { public: B() { std::cout << "B\n"; } };
class C : public A, public B {
public:
    C() { std::cout << "C\n"; }
};

int main() {
	C c;
}
```

** Резултат **
```
A
B
C
```

### Диамантения проблем

В следния пример D наследява A два пъти — веднъж чрез B и веднъж чрез C. Това създава двусмисленост

```
class A {
public:
    void say() { std::cout << "A\n"; }
};

class B : public A {};
class C : public A {};
class D : public B, public C {};

int main() {
	D d;
	// d.say(); Грешка: D има две копия на A
}
```

## Virtual

### Решение на диамантения проблем

В примера горе видяхме, че диамантения проблем се получава, когато имаме две копия на клас в друг клас.
За да разрешим този проблем, трябва по някакъв начин да имаме само едно копие

** Решение ** - използваме `virtual` наследяване
```
class A {
public:
    void who() { std::cout << "I am A\n"; }
};

class B : virtual public A {};
class C : virtual public A {};
class D : public B, public C {};

int main() {
	D d;
	d.who(); // Работи – само едно копие на A
}
```

 > [!NOTE]
 > Това работи, защото при `virtual public A` наследяване, `C++` се грижи наследниците да споделят едно и също копие на `A` ( в този случай `B` и `C` )
 
### Виртуални функции
Виртуална функция е функция-член на базов клас, която може да бъде предефинирана в производен клас и при извикване през указател или референция към базовия клас, C++ ще избере правилната версия по време на изпълнение

**Синтаксис**
```
class Animal {
public:
    virtual void speak() {
        std::cout << "Some generic animal sound\n";
    }
};

class Dog : public Animal {
public:
    void speak() override { // override е добра практика, но не е задължително
        std::cout << "Bau bau\n";
    }
};
```

 > [!NOTE]
 > Когато маркираме функция като виртуална все едно казваме на компилатора „реши коя версия да извикаш по време на изпълнение, не при компилация“

#### Статично и динамично свързване

 1. Статично свързване
   - Изборът на функция става по време на компилация
   - Използва се, когато извикваме функция по стойност или указател към **НЕвиртуален** метод или типът е известен на компилатора
   
 2. Динамично свързване
   - Изборът на функция става по време на изпълнение
   - Използва виртуални функции
   - Позволява полиморфизъм
   
**Пример - статично свързване**
```
class Animal {
public:
    void speak() {
        std::cout << "Some generic animal sound\n";
    }
};

class Dog : public Animal {
public:
    void speak() {
        std::cout << "Bau bau\n";
    }
};

int main() {
    Animal* a = new Dog();
    a->speak(); // Статично свързване > извиква Animal::speak() и ще отпечата Some generic animal sound
}
```

**Пример - динамично свързване**
```
class Animal {
public:
    virtual void speak() {
        std::cout << "Some generic animal sound\n";
    }
};

class Dog : public Animal {
public:
    void speak() override {
        std::cout << "Bau Bau\n";
    }
};

int main() {
    Animal* a = new Dog();
    a->speak(); // Динамично свързване > извиква Dog::speak() и ще отпечата Bau Bau
}
```

** Казано по друг **
 - При статично свързване, компилаторът избира функцията според типa на указателя или променливата
 - При динамично свързване, програмата избира функцията според реалния тип на обекта, към който сочи указателят

** Какво прави override?**
 - Проверява дали наистина предефинираме съществуваща виртуална функция
 - Ако сбъркаме сигнатурата – ще получим компилационна грешка
 
## Полиморфизъм
Полиморфизъм означава „много форми“. В контекста на ООП означава, че можем да използваме указател или референция към базов клас, за да работим с различни обекти от производни класове

**Пример**
```
#include <iostream>

class Animal {
public:
    virtual void speak() const {
        std::cout << "Some generic animal sound\n";
    }
};

class Dog : public Animal {
public:
    void speak() const override {
        std::cout << "Bau bau\n";
    }
};

class Cat : public Animal {
public:
    void speak() const override {
        std::cout << "Meow\n";
    }
};

int main() {
    Animal* animals[3];
    animals[0] = new Animal();
    animals[1] = new Dog();
    animals[2] = new Cat();

    for (int i = 0; i < 3; ++i) {
        animals[i]->speak();
    }
	
	for (int i = 0; i < 3; ++i) {
        delete animals[i];
    }
}
```

**Изход**
```
Some generic animal sound
Bau bau
Meow
```

### Деструктор при полиморфизъм
В `C++` деструкторът не се избира полиморфно, освен ако е обявен като `virtual`

Тоест следния код ще извика деструкторите само на базовия клас
```
#include <iostream>

class Animal {
public:
    virtual void speak() const {
        std::cout << "Some generic animal sound\n";
    }
    
    ~Animal() {
        std::cout << "Animal destroyed\n";
    }
};

class Dog : public Animal {
public:
    void speak() const override {
        std::cout << "Bau bau\n";
    }
    
    ~Dog() {
        std::cout << "Dog destroyed\n";
    }
};

class Cat : public Animal {
public:
    void speak() const override {
        std::cout << "Meow\n";
    }
    
    ~Cat() {
        std::cout << "Cat destroyed\n";
    }
};

int main() {
    Animal* animals[3];
    animals[0] = new Animal();
    animals[1] = new Dog();
    animals[2] = new Cat();

    for (int i = 0; i < 3; ++i) {
        animals[i]->speak();
    }
    
    for (int i = 0; i < 3; ++i) {
        delete animals[i];
    }
}
```

**Изход**
```
Some generic animal sound  
Bau bau  
Meow  
Animal destroyed  
Animal destroyed  
Animal destroyed  
```

**Решението е виртуален деструктор в базовия клас**

В този пример **единственото**, което сме променили е да сложим думата `virtual` пред деструктора на базовия клас

```
#include <iostream>

class Animal {
public:
    virtual void speak() const {
        std::cout << "Some generic animal sound\n";
    }
    
    virtual ~Animal() {
        std::cout << "Animal destroyed\n";
    }
};

class Dog : public Animal {
public:
    void speak() const override {
        std::cout << "Bau bau\n";
    }
    
    ~Dog() {
        std::cout << "Dog destroyed\n";
    }
};

class Cat : public Animal {
public:
    void speak() const override {
        std::cout << "Meow\n";
    }
    
    ~Cat() {
        std::cout << "Cat destroyed\n";
    }
};

int main() {
    Animal* animals[3];
    animals[0] = new Animal();
    animals[1] = new Dog();
    animals[2] = new Cat();

    for (int i = 0; i < 3; ++i) {
        animals[i]->speak();
    }
    
    for (int i = 0; i < 3; ++i) {
        delete animals[i];
    }
}
```

**Изход**
```
Some generic animal sound  
Bau bau  
Meow  
Animal destroyed  
Dog destroyed  
Animal destroyed  
Cat destroyed  
Animal destroyed  
```

**Защо без виртуален деструктор не изтриваме правилно?**
Защото `delete` използва типа на указателя, за да разбере какво да изтрие, а не типа на реалния обект, освен ако деструкторът е виртуален

 > [!NOTE]
 > Ако ще използваме обекти полиморфно (в този случай през Animal*), то базовият клас трябва задължително да има виртуален деструктор, иначе деструкторите на производните класове няма да се извикат
