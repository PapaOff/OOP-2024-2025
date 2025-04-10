# �����������. ������ � ���������� �������. ������������� �� �������-�������� �� ������� ���� � ����������� ����. ���������� �� ������������ � ����������� � ���������� �������. ����������� �����������. ��������� ������� � ������������.

## �����������
������������� � ��������� � �������-������������� ������������, ���� ����� ���� ���� (����������) ���� �� ������� ����-����� � ������� �� ���� ���� (�����)

**����� �������?**
 - �������� �������� �� ���� ������� ���
 - ��-����� ����������� �� ���� ���� ��������
 
 **������**
 ```
 class Animal {
public:
    void speak() {
        std::cout << "Some generic animal sound\n";
    }
};

class Dog : public Animal {
    // ��������� speak()
};

int main()
{
    Dog d;
    d.speak(); //Some generic animal sound
}
 ```
 
 > [!NOTE]
 > `public` � ����������� �� ����������� � ��� �������� ����� �� ����� ������ �� ������ �� ����������� �������
 
 **������������ �� �����������**
 | ����������� | Public ������� | Protected ������� | Private ������� |
 |----------|-----------|-----------|-----------|
 | `public` | ������� `public` | ������� `protected` | ���������� |
 | `protected` | ������ `protected`) | ������� `protected` | ���������� |
 | `private` | ������  `private`	| ������  `private` | ���������� |
 
 
## ������������� �� ������� � ���������� �������
 � ������� ���� ������ ���� ��������� ���������, � �������� default-����, ��������� �� ������� ����.
 ������ ������ ������������ ������� �� ���� �������� ���������, ������ �� ������������� (override) ��������� �� ������� ����
 
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
 
## ������������ � �����������
** ��� �� ��������� **
 - ��� ������������ ����� �� ������� ������������� �� ������� ����, � ���� ���� �� ������� ������������� �� ����������� ����
 - ��� ����������� ����� �� ������� ������������ �� ����������� ����, � ���� ���� �� ������� ����
 
** ������ **
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
**��������**
```
A constructor
B constructor
B destructor
A destructor
```

### ������������ ��� �����������
����� �� ����� �� ������� ���� ������ ���������� ���� �� �������, �������� ���� ������ �� ������������ �����, ���������� ���� ���������� ����������� �� ������������ ��� ���������������

 > [!NOTE]
 > ������ � ������������ �� ���������� ���� ������ �� �������������� ����-�����, ����� ����������� �� ������� ����, ���� ����� ���� ��������� �� ������������ �� ������� ����
 
 ** ������ **
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
 
 ** �������� **
 ```
Constructor A setting x = 5
Constructor B setting y = 10
 ```
 
 > [!NOTE]
 > ��� � ������������ �� ����������� ���� �� ��� ������� ��� ����������� �� ������� �� �� ������, �� �� ������ ������������ �� ������������. ��� ����� ����, �� ������� ������������� ������
 
 ** ���� ���� ������ **
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
 
  ** ���� �� ���� ������ **
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
 
### Copy ����������� � operator= ��� ����������� � ��������� �����

��� � ����������� ���� ����� copy ����������� � operator= **������** �� �������� copy ����������� � operator= �� �������

** ��� �� �� �������� **

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

    // ������� ����������� (�� ������ A(other))
    B(const B& other) {
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
    }

    // �������� �� ����������� (�� ������ A::operator=)
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
    B b2 = b1;         // ������� �����������
    B b3(1, "tmp");
    b3 = b1;           // �������� �� �����������

    b1.print();
    b2.print();
    b3.print();
}
```

** ����� **
```
A: 5
B: hello
A: 0
B: hello
A: 1
B: hello
```

 > [!NOTE]
 > ���������� � ������������� �� ������� ��������, ������ `�` �� � 5 ��� ����� �����



** ��� �� �������� **

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
    B b2 = b1;         // ������� �����������
    B b3(1, "tmp");
    b3 = b1;           // �������� �� �����������

    b1.print();
    b2.print();
    b3.print();
}
```

** ����� **
```
A: 5
B: hello
A: 5
B: hello
A: 5
B: hello
```

 > [!NOTE]
 > ��� ����������� ���� ���� ��������� ����� �� � ����� �� ����� ������ 4��, ���� �������� ���� �� ��� ��������� �����
 
 ������� ��� ������ ��������
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
    B b2 = b1;        // ������� �����������
    B b3(2, "temp");
    b3 = b1;          // ��������=

    b1.print();
    b2.print();
    b3.print();
}
 ```

### ����������
�� � ����� ����� �� ������ ����������� �� ������� ����. ���� ����� ����������� � ������������ �� ����� �� ����

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
    Animal a = d; // slicing � ������ �� ���� ������ �� Dog, ����� � Animal
    a.speak();   // ��������� Some generic animal sound
    //a.sniff(); // ������: sniff() �� ���������� � Animal
}
 ```
 
 ** ���� �� ���������� ����? **
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
        animals[i].speak(); // �� ������ Animal::speak() �� ����� �����
    }
}
```

## ����������� �����������
� �++ ���� ���� �� ��������� �� ������ �� ���� ����� ����

** ����� **
 - ���� �� ����������� �������� ��������������� �� ������ �� ���� ����
 
** ��������� **
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
    // ��������� � �� A, � �� B
};

int main() {
	C c;
	c.f(); // �� A
	c.g(); // �� B
}
``` 

** ��� ������������� ����������� ���� �� �������� ��������, ������ ��� ������ ����� ���� ������� � ���� � ���� ���**

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
	// c.f(); ������: ������ ��� `f()` �� �� ������
	c.A::f(); // ������� ���������
	c.B::f(); // ������� ���������
}
```

### ��� �� ������������ � ����������� ��� ����������� �����������
 - �������������� �� �������� � ���� �� �����������
 - ������������� �� �������� � �������� ���
 
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

** �������� **
```
A
B
C
```

### ����������� �������

� ������� ������ D ��������� A ��� ���� � ������ ���� B � ������ ���� C. ���� ������� �������������

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
	// d.say(); ������: D ��� ��� ����� �� A
}
```

## Virtual

### ������� �� ����������� �������

� ������� ���� �������, �� ����������� ������� �� ��������, ������ ����� ��� ����� �� ���� � ���� ����.
�� �� �������� ���� �������, ������ �� ������� ����� �� ����� ���� ���� �����

** ������� ** - ���������� `virtual` �����������
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
	d.who(); // ������ � ���� ���� ����� �� A
}
```

 > [!NOTE]
 > ���� ������, ������ ��� `virtual public A` �����������, `C++` �� ����� ������������ �� �������� ���� � ���� ����� �� `A` ( � ���� ������ `B` � `C` )
 
### ��������� �������
��������� ������� � �������-���� �� ����� ����, ����� ���� �� ���� ������������� � ���������� ���� � ��� ��������� ���� �������� ��� ���������� ��� ������� ����, C++ �� ������ ���������� ������ �� ����� �� ����������

**���������**
```
class Animal {
public:
    virtual void speak() {
        std::cout << "Some generic animal sound\n";
    }
};

class Dog : public Animal {
public:
    void speak() override { // override � ����� ��������, �� �� � ������������
        std::cout << "Bau bau\n";
    }
};
```

 > [!NOTE]
 > ������ ��������� ������� ���� ��������� ��� ���� ������� �� ����������� ����� ��� ������ �� ������� �� ����� �� ����������, �� ��� �����������

#### �������� � ��������� ���������

 1. �������� ���������
   - ������� �� ������� ����� �� ����� �� ����������
   - �������� ��, ������ ��������� ������� �� �������� ��� �������� ��� **�����������** ����� ��� ����� � �������� �� �����������
   
 2. ��������� ���������
   - ������� �� ������� ����� �� ����� �� ����������
   - �������� ��������� �������
   - ��������� ������������
   
**������ - �������� ���������**
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
    a->speak(); // �������� ��������� > ������� Animal::speak() � �� �������� Some generic animal sound
}
```

**������ - ��������� ���������**
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
    a->speak(); // ��������� ��������� > ������� Dog::speak() � �� �������� Bau Bau
}
```

** ������ �� ���� **
 - ��� �������� ���������, ������������ ������ ��������� ������ ���a �� ��������� ��� ������������
 - ��� ��������� ���������, ���������� ������ ��������� ������ ������� ��� �� ������, ��� ����� ���� ����������

** ����� ����� override?**
 - ��������� ���� �������� ������������� ������������ ��������� �������
 - ��� �������� ����������� � �� ������� ������������� ������
 
## ������������
������������ �������� ������ �����. � ��������� �� ��� ��������, �� ����� �� ���������� �������� ��� ���������� ��� ����� ����, �� �� ������� � �������� ������ �� ���������� �������

**������**
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

**�����**
```
Some generic animal sound
Bau bau
Meow
```

### ���������� ��� ������������
� `C++` ������������ �� �� ������ ����������, ����� ��� � ������ ���� `virtual`

����� ������� ��� �� ������ ������������� ���� �� ������� ����
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

**�����**
```
Some generic animal sound  
Bau bau  
Meow  
Animal destroyed  
Animal destroyed  
Animal destroyed  
```

**��������� � ��������� ���������� � ������� ����**

� ���� ������ **������������**, ����� ��� ��������� � �� ������ ������ `virtual` ���� ����������� �� ������� ����

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

**�����**
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

**���� ��� ��������� ���������� �� ��������� ��������?**
������ `delete` �������� ���� �� ���������, �� �� ������� ����� �� ������, � �� ���� �� ������� �����, ����� ��� ������������ � ���������

 > [!NOTE]
 > ��� �� ���������� ������ ���������� (� ���� ������ ���� Animal*), �� �������� ���� ������ ������������ �� ��� ��������� ����������, ����� ������������� �� ������������ ������� ���� �� �� �������