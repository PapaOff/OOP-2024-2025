# Smart Pointers
Умните указатели в `C++` са класове-шаблони, които управляват автоматично динамично заделена памет. 
Те се грижат за освобождаването ѝ, когато вече не е нужна, като по този начин предотвратяват `memory leak-ове` и грешки с `delete`

**Умни указатели** 
 | Указател | Собственост | Копиране | Автоматично освобождаване на памет |  | 
 |----------|-----------| -----------| -----------| -----------|
 | `unique_ptr<T>` | Да (единствена) | не директно | да | най-ефективен|
 | `shared_ptr<T>` | Да (споделена) | да | при  ref count = 0 | полезен при дървета, свързани списъци|
 | `weak_ptr<T>` | Не | | да | не | избягване на цикли при `shared_ptr<T>` |
 
 
## `unique_ptr`
 - Собствеността над обекта принадлежи **само на един** `unique_ptr`
 - При излизане от обхват автоматично се извиква `delete`
 - Не може да се копира, но може да се премества с `std::move`
 
 ```c++
 #include <memory>

 std::unique_ptr<int> ptr = std::make_unique<int>(42);     // *ptr == 42
 std::unique_ptr<int> b = a;                               // Грешка: копиране не е позволено
 std::unique_ptr<int> b = std::move(a);                    // a вече не притежава обекта
 ```
### Функции на `unique_ptr` 
 - `get()` - > Връща суровия (обикновен) указател `T*`, без да дава собственост
 - `release()` - >	Освобождава собствеността и връща суровия указател. Не трие обекта!
 - `reset()` - > Триe текущия обект (ако има) и може да приеме нов указател
 - `*` / `->` - > Позволяват достъп до обекта чрез `*` и `->`
 - `operator bool()` - > Проверява дали `unique_ptr` сочи към нещо (`nullptr` или не)
 - `swap()` - > Разменя стойностите на два `unique_ptr`
 
```c++
#include <iostream>
#include <memory>

struct Demo {
    Demo(int v) : value(v) {
        std::cout << "Constructed Demo(" << value << ")\n";
    }
    ~Demo() {
        std::cout << "Destructed Demo(" << value << ")\n";
    }
    void sayHi() const {
        std::cout << "Hello from " << value << "\n";
    }
private:
    int value;
};

int main() {
    std::unique_ptr<Demo> ptr1 = std::make_unique<Demo>(10);

    (*ptr1).sayHi();
    ptr1->sayHi();

    // get() -> връща суров указател (без да дава собственост)
    Demo* raw = ptr1.get();
    std::cout << "Raw pointer: " << raw << "\n";

    // operator bool()
    if (ptr1) {
        std::cout << "ptr1 is not null\n";
    }

    Demo* raw2 = ptr1.release(); // ptr1 вече не притежава обекта
    std::cout << "After release, ptr1 is " << (ptr1 ? "valid" : "null") << "\n";

    delete raw2; // Трябва ръчно да изтрием, защото unique_ptr вече не го управлява

    // reset() -> слага нов обект и трие стария (ако има)
    ptr1.reset(new Demo(20)); // ptr1 вече сочи към нов обект

    std::unique_ptr<Demo> ptr2 = std::make_unique<Demo>(30);
    std::cout << "Before swap:\n";
    ptr1->sayHi(); // 20
    ptr2->sayHi(); // 30

    ptr1.swap(ptr2);

    std::cout << "After swap:\n";
    ptr1->sayHi(); // 30
    ptr2->sayHi(); // 20

    // reset() без аргумент -> трие текущия обект
    ptr1.reset(); // унищожава обекта
    if (!ptr1) std::cout << "ptr1 is now null\n";
}

``` 
## `shared_ptr`
 - Поддържа `reference count`
 - Паметта се освобождава автоматично, когато всички `shared_ptr` към нея бъдат унищожени
 - Може да се копира и присвоява, но се увеличава `reference count` за всяко копиране/присвояване
 
```c++
#include <iostream>
#include <memory>

class Demo {
public:
    Demo(int v) : value(v) {
        std::cout << "Constructed Demo(" << value << ")\n";
    }
    ~Demo() {
        std::cout << "Destructed Demo(" << value << ")\n";
    }
    void show() const {
        std::cout << "Value: " << value << "\n";
    }
private:
    int value;
};

int main() {
    std::shared_ptr<Demo> ptr1 = std::make_shared<Demo>(10); // reference count = 1
    std::cout << "ptr1 use_count: " << ptr1.use_count() << "\n";

    {
        std::shared_ptr<Demo> ptr2 = ptr1; // копиране -> ref count = 2
        std::cout << "ptr2 use_count: " << ptr2.use_count() << "\n";

        std::shared_ptr<Demo> ptr3;
        ptr3 = ptr2; // присвояване -> ref count = 3
        std::cout << "ptr3 use_count: " << ptr3.use_count() << "\n";

        ptr3->show();
    } // ptr2 и ptr3 излизат от обхват -> ref count = 1

    std::cout << "ptr1 use_count after block: " << ptr1.use_count() << "\n";

    // ptr1 излиза от обхват и обектът се унищожава автоматично
}
```

**Основен проблем**
Проблемът при циклична зависимост с `shared_ptr` възниква, когато два (или повече) обекта се сочат взаимно чрез `shared_ptr`. 
Така техните `reference count` никога не стават 0, което означава, че паметта никога не се освобождава, т.е. имаме `memory leak`

```c++
#include <iostream>
#include <memory>

struct B;

struct A {
    std::shared_ptr<B> b;
    ~A() { std::cout << "A destroyed\n"; }
};

struct B {
    std::shared_ptr<A> a;
    ~B() { std::cout << "B destroyed\n"; }
};

int main() {
    std::shared_ptr<A> a = std::make_shared<A>();
    std::shared_ptr<B> b = std::make_shared<B>();
    a->b = b;
    b->a = a;
}
```
**Какво се случва**
 - `a` и `b` сочат един към друг чрез `shared_ptr`
 - `reference count` за всеки обект никога не става 0
 - деструкторите не се извикват, а паметта не се освобождава
 
### Функции на `shared_ptr` 
 - `get()` -> Връща суровия указател без да променя брояча
 - `use_count()` -> Връща текущия брой `shared_ptr`, които сочат към обекта
 - `unique()` -> Връща `true`, ако този `shared_ptr` е единственият притежател
 - `reset()` -> Освобождава обекта (ако броячът стане 0) или сочи към нов
 - `*` / `->` -> Позволява достъп до обекта
 - `operator bool()` -> Проверка за nullptr
 - `swap()` -> Разменя съдържанието с друг shared_ptr
 
 
```c++
#include <iostream>
#include <memory>

struct Demo {
    Demo(int v) : value(v) {
        std::cout << "Constructed Demo(" << value << ")\n";
    }
    ~Demo() {
        std::cout << "Destructed Demo(" << value << ")\n";
    }
    void sayHi() const {
        std::cout << "Hello from " << value << "\n";
    }
private:
    int value;
};

int main() {
    std::shared_ptr<Demo> ptr1 = std::make_shared<Demo>(10);

    //get() -> връща суров указател без да променя брояча
    Demo* raw = ptr1.get();
    std::cout << "Raw pointer: " << raw << "\n";

    std::cout << "use_count: " << ptr1.use_count() << "\n"; // 1

    std::cout << "Is unique? " << std::boolalpha << ptr1.unique() << "\n"; // true

    // Копиране
    std::shared_ptr<Demo> ptr2 = ptr1;
    std::cout << "use_count after copy: " << ptr1.use_count() << "\n"; // 2
    std::cout << "Is ptr1 unique? " << ptr1.unique() << "\n"; // false

    (*ptr1).sayHi();
    ptr2->sayHi();

    //operator bool() -> проверка за nullptr
    if (ptr1) {
        std::cout << "ptr1 is valid\n";
    }

    // reset() -> освобождава или пренасочва
    ptr2.reset(); // намалява брояча
    std::cout << "After ptr2.reset(), use_count: " << ptr1.use_count() << "\n"; // 1

    // reset с нов обект
    ptr1.reset(new Demo(20)); // старият се унищожава
    std::cout << "After ptr1.reset(new ...), use_count: " << ptr1.use_count() << "\n";

    std::shared_ptr<Demo> ptr3 = std::make_shared<Demo>(30);
    std::cout << "Before swap:\n";
    ptr1->sayHi(); // 20
    ptr3->sayHi(); // 30

    ptr1.swap(ptr3);

    std::cout << "After swap:\n";
    ptr1->sayHi(); // 30
    ptr3->sayHi(); // 20
}

```
 
## `weak_ptr`
 - Използва се при `shared_ptr`
 - Не притежава обекта и не увеличава брояча на `shared_ptr`
 - Служи само за наблюдение на обекта
 - Подходящ за прекъсване на цикли при `shared_ptr`
 
 **Пример**
```c++
 std::shared_ptr<int> s = std::make_shared<int>(99);
 std::weak_ptr<int> w = s; // ref count не се променя
```

**Още един пример**
```c++
struct Node {
	int value;
    std::shared_ptr<Node> next;
};

void createCycle() {
    auto a = std::make_shared<Node>();
    auto b = std::make_shared<Node>();
    a->next = b;
    b->next = a; // Цикъл! Никога не се освобождават
}
```

**Решение**
```c++
struct Node {
	int value;
    std::weak_ptr<Node> next;
};
```

### Функции на `weak_ptr` 
 - `lock()` -> Опитва се да създаде `shared_ptr` от `weak_ptr`. Ако обектът е изтрит връща `nullptr`
 - `expired()` -> Връща `true`, ако обектът вече е изтрит
 - `use_count()` -> Връща броя на `shared_ptr`, които сочат към обекта
 - `reset()` -> Изтрива наблюдението (но не освобождава обекта)
 - `swap()` -> Разменя съдържанието с друг `weak_ptr`
 
```c++
#include <iostream>
#include <memory>

struct Demo {
    Demo(int v) : value(v) {
        std::cout << "Constructed Demo(" << value << ")\n";
    }
    ~Demo() {
        std::cout << "Destructed Demo(" << value << ")\n";
    }
    void sayHi() const {
        std::cout << "Hello from " << value << "\n";
    }
private:
    int value;
};

int main() {
    std::shared_ptr<Demo> shared = std::make_shared<Demo>(100);
    std::weak_ptr<Demo> weak1 = shared;

    std::cout << "use_count: " << weak1.use_count() << "\n"; // 1

    // expired() - false, защото shared_ptr все още съществува
    std::cout << "expired? " << std::boolalpha << weak1.expired() << "\n"; // false

    // lock() -> създава нов shared_ptr ако обектът все още съществува
    std::shared_ptr<Demo> maybe = weak1.lock();
    if (maybe) {
        maybe->sayHi(); // Hello from 100
    }

    // reset() -> прекратява наблюдението (не влияе на оригиналния обект)
    weak1.reset();
    std::cout << "After reset, expired? " << weak1.expired() << "\n"; // true

    std::weak_ptr<Demo> weak2;
    weak2 = shared;
    std::cout << "Before swap: weak2 expired? " << weak2.expired() << "\n"; // false

    weak1.swap(weak2); // weak1 отново гледа към обекта, weak2 вече е празен
    std::cout << "After swap: weak1 expired? " << weak1.expired() << "\n"; // false
    std::cout << "After swap: weak2 expired? " << weak2.expired() << "\n"; // true

    // Унищожаваме shared_ptr
    shared.reset();
    std::cout << "After shared.reset(): weak1 expired? " << weak1.expired() << "\n"; // true

    // Проверка с lock след унищожение
    std::shared_ptr<Demo> check = weak1.lock();
    if (!check) {
        std::cout << "Object no longer exists (lock returned nullptr)\n";
    }
}
```
