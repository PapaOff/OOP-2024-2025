# Шаблони

Функция или клас, който работи с общ тип, дефиниран чрез шаблонен параметър `template<typename T>`, вместо с конкретен тип като `int`, `double` и т.н

```
#include <iostream>
using namespace std;

template <class T>
T sum(const T& a, const T& b) {
    return a + b;
}

int main() {
    int a = 4;
    int b = 9;
    cout << sum<int>(a, b) << endl;

    double c = 3.14;
    double d = 4.5;
    cout << sum<double>(c,d) << endl;

    return 0;
}
```

Компилаторът генерира т. нар. шаблонна функция, като замества параметрите на шаблона с типовете на съответните фактически параметри.

> [!NOTE]
> Шаблоните в C++ се инстанцират по време на компилация

### Темплейтна специализация

Различно поведение на темплейтния клас/функция за определени типове
Използваме `template<>` за специализация на шаблон — изключение от универсалния случай
В примера имаме шаблонна `print`, използвайки `template<typename T>` и шаблонна `print` само за `char*`, използвайки `template<>`

> [!NOTE]
> Когато извикаш шаблон с конкретен тип, ако има специализация, тя има приоритет

```
#include <iostream>

template<typename T>
void print(T* arr, size_t size) {
	for (size_t i = 0; i < size; i++) {
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;
}

template<> // !!!
void print(char* arr, size_t size) {
	for (size_t i = 0; i < size; i++) {
		std::cout << arr[i] << " ---- ";
	}
	std::cout << std::endl;
}

int main() {
	int arr1[] = { 1, 2 };
	print<int>(arr1, 2);
	char arr2[] = { 'a', 'b' };
	print<char>(arr2, 2);
}
```

**Изход**
```
1 2
a ---- b ----
```

### Необходима функция

Необходима функция (required function) при шаблоните в C++ е функция, която не е част от интерфейса на шаблона, но шаблонът очаква да съществува в подадения тип. Тя не е дефинирана в самия шаблон, но е извикана вътре в него — което означава, че типът, с който се инстанцира шаблонът, трябва да я предоставя.

```
template <class T>
void HeterogeneousContainer<T>::copyFrom(const HeterogeneousContainer& other) {
    data = new T*[other.capacity];
    for (size_t i = 0; i < other.capacity; i++) {
        data[i] = other.data[i]->clone(); // the compiler expects the class that is used as T to have the clone() function
    }
    size = other.size;
    capacity = other.capacity;
}
```

Тук `clone()` е необходима функция – компилаторът няма да даде грешка при дефиниция на шаблона, но ще даде при инстанциране, ако подаденият тип няма такава функция