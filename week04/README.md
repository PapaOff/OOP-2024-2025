# **Член-функции. Голяма четворка**

## **Какво е член-функция?**
Член-функцията (member function) е функция, която принадлежи на клас 
и има достъп до неговите член-данни (полета). Тя се дефинира вътре в 
класа и може да бъде извиквана чрез обекти от този клас

**Пример**
```
class Car {
private:
    int speed;
	
public:

	// Член-функция ( setter )
    void setSpeed(int s) { 
        speed = s;
    }

	// Член-функция ( getter )
    void showSpeed() const { 
        std::cout << "Speed: " << speed << " km/h\n";
    }
};

int main() {
    Car myCar;
    myCar.setSpeed(100);
    myCar.showSpeed();
    return 0;
}
```

> [!NOTE]
> Член-функциите могат да четат и променят данните на класа

> [!NOTE]
> Ако функция не трябва да променя членовете на класа, я декларираме с const. Това гарантира, че вътре в нея няма промени върху обекта. Винаги се слага при `гетъри`

> [!МНОГО ВАЖНО]
> Ако член-функция на клас е маркирана с `const`, тя може да бъде извиквана както от `нормални`, така и от `const обекти`

**Пример**

```
#include <iostream>

class Example {
private:
    int value;
public:
    Example(int v) : value(v) {}

    int getValue() const {
        return value;
    }

    void setValue(int v) {
        value = v;
    }
};

int main() {
    const Example obj(10); // Константен обект

    std::cout << obj.getValue() << std::endl; // Позволено (функцията е `const`)
    
    obj.setValue(20);                         // Грешка! Не може да се извика от `const` обект

    return 0;
}

```


## **Дефиниране на член-функции**
Член-функциите могат да бъдат дефинирани:
- В класа ( както в примера горе )
- Извън класа с помощта на `::`

**Пример**
```
#include <iostream>

class Car {
public:
    int speed;
    void setSpeed(int s); // Само декларация
    void showSpeed(); // Само декларация
};

// Дефиниция извън класа
void Car::setSpeed(int s) {
    speed = s;
}

// Дефиниция извън класа
void Car::showSpeed() {
    std::cout << "Speed: " << speed << " km/h\n";
}

int main() {
    Car myCar;
    myCar.setSpeed(120);
    myCar.showSpeed();
    return 0;
}

```

> [!NOTE]
> Подходящо за редактиране на текст


## **Тип на връщане на член-функции**

**Връщане на `char*`**

Ако функцията връща `char*` , това означава, че можем да променяме стойността

**Пример**
```
class Example {
private:
    char* name;
public:
    Example(const char* str) {
        name = new char[strlen(str) + 1];
        strcpy(name, str);
    }
    ~Example() { delete[] name; }

    char* getName() const { 
	return name; 
	}  
};


int main() {
    Example e("primer");
    char* temp = e.getName();
    std::cout << temp;   // Това изкарва на конзолата primer
    strcpy(temp, "neshto"); // Тук няма да гръмне, защото getName() връща само char*
    std::cout << temp;  //  Това изкарва на конзолата neshto
    
    return 0;
}

```

> [!NOTE]
> Подходящо за редактиране на текст


**Връщане на `const char*`**

Най-често използвано при връщане на низове от функции

**Пример**
```
class Example {
private:
    char* name;
public:
    Example(const char* str) {
        name = new char[strlen(str) + 1];
        strcpy(name, str);
    }
    ~Example() { delete[] name; }

	const char* getSafeName() const {     // Само за четене, но указателят може да се мести  
		return name; 
	}           
};


int main() {
    Example e("primer");
    const char* temp1 = e.getSafeName();
    std::cout << temp1;    // Това изкарва на конзолата primer
    strcpy(temp1, "neshto");    // Тук ще гръмне, защото връщаме const char* и не можем да променяме стойността
    char x = 'x';
    temp1 = &x;            // Променяме накъде сочи temp1
    std::cout << *temp1;   // Това изкарва на конзолата x
    
    return 0;
}

```

**Връщане на `const char* const`**

Когато искаш пълна защита (readonly)

**Пример**
```
class Example {
private:
    char* name;
public:
    Example(const char* str) {
        name = new char[strlen(str) + 1];
        strcpy(name, str);
    }
    ~Example() { delete[] name; }

    const char* const getFixedName() const { 
	return name; 
	}
};


int main() {
    Example e("primer");
    const char* const temp2 = e.getFixedName();
    char x = 'x';
    strcpy(temp2, "random"); // Тук гърми, защото имаме const char*
    temp2 = &x;              // Тук гърми, защото имаме const след char*
    
    return 0;
}

```

## **Ключовата дума `this`**
Ключовата дума `this` е указател към текущия обект на класа. Той е достъпен във всички член-функции (освен static функции) и позволява:
- Достъп до член-данните и методите на текущия обект
- Разграничаване между член-променливи и параметри с еднакви имена
- Връщане на референция към текущия обект `(*this)`, което позволява `method chaining`

### **Основна употреба**
```
class Example {
private:
    int value;
public:
    Example(int value) { // Конструктор
        this->value = value; // `this->value` сочи към член-данната, а `value` е параметър
    }

    void print() {
        std::cout << "Value: " << this->value << std::endl;
    }
};

int main() {
    Example obj(10);
    obj.print(); // Value: 10
    return 0;
}

```

### **Връщане на `*this` за Method Chaining**
Методите могат да връщат референция към текущия обект, което позволява викане на няколко метода последователно (method chaining)

```
class Example {
private:
    int value;
public:
    Example(int value) { this->value = value; }

    Example& add(int num) { 
        value += num;
        return *this; // Връща текущия обект
    }

    Example& multiply(int num) { 
        value *= num;
        return *this;
    }

    void print() {
        std::cout << "Value: " << value << std::endl;
    }
};

int main() {
    Example obj(5);
    obj.add(3).multiply(2).print(); // (5 + 3) * 2 = 16
    return 0;
}

```


## **Голяма четворка**
Голямата четворка (Big Four) са специални член-функции в класовете, които определят как се създават, копират, преместват и унищожават обекти

> [!МНОГО ВАЖНО]
> Ако класът има динамична памет (new), задължително трябва да дефинираме голямата 4ка

> [!МНОГО ВАЖНО]
> Ако класът има динамична памет (new), но не сме написали голяма 4ка, автоматично ще се създадат
> Проблемът е, че копирането ще бъде повърхностно (shallow copy), тоест копира само указателя, но не и съдържанието


## **Конструктори**
- Извиква се **веднъж** при създаването на обекта
- Името съвпада с името на класа

### **Конструктор по подразбиране (Default Constructor)**
Конструкторът по подразбиране е специален конструктор, който не приема параметри (или всички параметри имат стойности по подразбиране). 
Той се извиква автоматично, когато обект бъде създаден без аргументи

**Пример**
```
class Example {
private:
    int value;
public:
    Example() { // Конструктор по подразбиране
        value = 0;
        std::cout << "Default constructor called!" << std::endl;
    }

    void print() { std::cout << "Value: " << value << std::endl; }
};

int main() {
    Example obj; // Извиква се конструкторът по подразбиране
    obj.print(); // Value: 0
    return 0;
}

```

> [!ВАЖНО]
> Ако имаме дефиниран друг конструктор (например с параметри), компилаторът вече няма да генерира конструктор по подразбиране

#### **Конструктор по подразбиране, дефиниран с =default**
Ако искате изрично да укажете, че компилаторът трябва да генерира конструктора по подразбиране

```
class Example {
public:
    Example() = default;
};

```

#### **Забрана на конструктора по подразбиране (=delete)**
Ако не искате обектите да могат да се създават без аргументи, можете да забраните конструктора

```
class Example {
public:
    Example() = delete; // Забранява конструктора по подразбиране
    Example(int v) {}
};

int main() {
    Example obj1;    //  Грешка: Няма дефиниран конструктор по подразбиране
    Example obj2(10); // Работи
}

```

> [!NOTE]
> Използва се, когато искате да задължите потребителя винаги да подава параметри при създаване на обект


#### **Задаване на стойност на const член-данни**
В C++ `const` член-променливи трябва да бъдат инициализирани преди изпълнението на конструктора.
Единственият начин за инициализиране на const член-променливи е като използваме списък за инициализация

**Пример**
```
class Example {
private:
    const int value;
	int age;
public:
    Example() : value(v) {
		age = 5;
	}
};

int main() {
	Example obj(42); // value = 42, age = 5

}

```

> [!NOTE]
> Списъкът за инициализация задава стойност на value преди изпълнението на конструктора

> [!ВАЖНО]
> Член-данните на класа се инициализират в реда, в който са декларирани в класа, а не в реда на инициализиращия списък

**Пример**
```
#include <iostream>

class Example {
private:
    int id;
    double balance;

public:
    Example(int i, double b) 
        : balance(b), id(i) {
        std::cout << "Constructor called!\n";
    }
};

```

Въпреки че в списъка пишем balance(b), id(i), членовете ще се инициализират в този ред - id, balance

### **Конструктор с параметри**
```
class Example {
private:
    char* data;
public:
    Example(const char* str) {
        data = new char[strlen(str) + 1];
        strcpy(data, str);
    }
};

int main() {
	 Example obj1("Hello");
}
```

### **Конструктор за копиране (Copy Constructor))**
Конструкторът за копиране се използва, когато:
- Създаваме нов обект като копие на съществуващ обект (напр. `MyClass obj2(obj1);`)
- Подаваме обект по стойност в аргумент на функция
- Връщаме обект по стойност от функция

```
class Example {
private:
    char* data;
public:
    Example(const char* str) { // Конструктор с параметри
        data = new char[strlen(str) + 1];
        strcpy(data, str);
    }

    // Копиращ конструктор (прави deep copy)
    Example(const Example& other) {
        data = new char[strlen(other.data) + 1];
        strcpy(data, other.data);
    }

    void print() { std::cout << data << std::endl; }

    ~Example() { delete[] data; } // Деструктор
};

int main() {
    Example obj1("Hello");
    Example obj2(obj1); // Използва се копиращият конструктор

    obj1.print();
    obj2.print();
    return 0;
}

```

**Явно извикване на копиращия конструктор**
`Example obj2(obj1)`

**Невно извикване на копиращия конструктор**
`Example obj2 = obj1;`

> [!NOTE]
> Този синтаксис изглежда като присвояване, но в действителност пак извиква копиращия конструктор, защото `obj2` се създава при инициализация

### **Оператор за присвояване (operator=)**
`operator=` позволява присвояване на един обект на друг, като замества съществуващите му стойности.
- Използва се при вече съществуващи обекти (напр. obj1 = obj2;)
- Приема константна референция (const ClassName&), за да не се копира обектът излишно
- Връща *this, за да поддържа chaining

```
class Example {
private:
    char* data;
public:
	Example(const char* str = "") {
        data = new char[strlen(str) + 1];
        strcpy(data, str);
    }

    // Оператор за присвояване
    Example& operator=(const Example& other) {
        if (this != &other) { // Проверка за самоприсвояване
            delete[] data; // Освобождаваме старата памет
            data = new char[strlen(other.data) + 1];
            strcpy(data, other.data);
        }
        return *this; // Връщаме текущия обект, за да поддържаме `obj1 = obj2 = obj3;`
    }

    void print() { std::cout << data << std::endl; }

    ~Example() { delete[] data; } // Освобождаване на паметта
};

int main() {
    Example obj1("Hello");
    Example obj2;
	Example obj3("World");

    obj1 = obj2 = obj3; // Използва се `operator=`
    obj1.print(); // World

    return 0;
}

```

### **Деструктор (Destructor)**
- Извиква се автоматично **веднъж** при изтриването на обекта, когато
  - излезне извън scope-a, в който е деклариран
  - когато е извикан `delete[]`/`delete` оператор върху динамично заделени обекти
- Неговата основна роля е да освобождава ресурси (напр. динамично заделена памет, файлове)
- Започва с `~` (тилда), последвана от името на класа

```
class Example {
public:
    ~Example() { // Деструкторът няма аргументи и не връща стойност
        std::cout << "Destructor called!" << std::endl;
    }
};

```


**Ако клас използва new, трябва задължително да дефинираме деструктор, за да предотвратим изтичане на памет**
```
class Example {
private:
    char* data;
public:
    Example(const char* str) {
        data = new char[strlen(str) + 1];
        strcpy(data, str);
    }

    ~Example() { // Освобождаваме паметта
        delete[] data;
        std::cout << "Destructor called!\n";
    }
};

int main() {
    Example obj1("Hello"); // Обектът се унищожава автоматично
    Example* obj2 = new Example("World");

    delete obj2; // Без това ще имаме memory leak
    return 0;
}

```
