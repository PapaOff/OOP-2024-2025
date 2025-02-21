#  **Преговор**


##  **Типове памет**
*	**Stack** - всички променливи, които са нестатични и **не са** заделени с ключовата дума <em>new</em>
*	**Heap** - всички променливи, които **са** заделени с <em>new</em>
*	**Статична** - статични и глобални променливи


##  **Указател и Референция**
 ```
int main()
{
    int n = 5; 			 			   // заделя се памет в stack
    int* ptr = new int[n]; 				   // заделя се памет в stack за ptr и памет в heap за n на брой int

    std::cout << ptr << std::endl;   			   // Адреса на първия елемент от заделения масив ( адрес, намиращ се в heap )
    std::cout << *ptr << std::endl;  		   	   // Стойността, която се намира на адреса, към който сочи ( в този случай първия елемент )
    std::cout << &ptr << std::endl;  			   // Адреса на самия ptr ( адрес, намиращ се в stack )

    int& y = n; 			 		   // y е референция към n, тоест споделят една и съща памет
    std::cout << "n: " << n << ", y: " << y << std::endl;  // Изход: n: 5, y: 5
    y++;
    std::cout << n; 				           // n = 6
    n++;
    std::cout << y; 				          // y = 7

    delete ptr; 					 // Изтрива само първия елемент
    delete[] ptr; 					 // Изтрива всички елементи
}
 ```


##  **Масиви, Масив от char, Символен низ**
###  **Масиви**
 ```
int main()
{
    int n;
    std::cin >> n;
    int* ptr = new int[n];

    ptr[1] 			// взима втория елемент на масив
    *(ptr + 1)		       // взима втория елемент на масив ( нарича се pointer arithmetic )
}
 ```

###  **Масив от char**
 ```
int main()
{
    char str[5] = {'H', 'e', 'l', 'l', 'o'};	        // Няма '\0' в края
    std::cout << str;      				// Непредвидим резултат (може да изведе случайни символи), защото липсва '\0'
}
 ```

###  **Символен низ**
 ```
int main()
{
    char str[] = "Hello"; 				// Еквивалентно на {'H', 'e', 'l', 'l', 'o', '\0'}
    std::cout << str;      				// Гарантирано ще изведе "Hello"
}
 ```

##  **Подаване на параметри във фунцкия**
###  **По копие**

 ```
int f(int x, int y)				// прави се копие на х и у
{
    x++;					// увеличава се копието на х
    y++;					// увеличава се копието на у
    return x + y;
}

int main()
{
    int x = 1;
    int y = 2;
    std::cout << f(x,y);				       // Изход: 5
    std::cout << "x: " << x << ", y: " << y << std::endl;     // Изход: x: 1, y: 2
	
}
 ```

###  **По референция**

 ```
int f(int& x, int& y)				// прави се копие на х и у
{
    x++;					// увеличава се стойността на х
    y++;					// увеличава се стойността на у
    return x + y;
}

int main()
{
    int x = 1;
    int y = 2;
    std::cout << f(x,y);				       // Изход: 5
    std::cout << "x: " << x << ", y: " << y << std::endl;     // Изход: x: 2, y: 3
	
}
 ```

###  **С указател**

 ```
void printArrayVersion1(int* arr, int size) 
{
	for (int i = 0; i < size; i++) {
	    std::cout << arr[i] << " ";
	}

	std::cout << std::endl;
}

void printArrayVersion2(int arr[], int size) 
{
	for (int i = 0; i < size; i++) {
	    std::cout << arr[i] << " ";
	}

	std::cout << std::endl;
}

int main() 
{
    int numbers[] = {1, 2, 3, 4, 5};
    int size = 5;
	
    printArrayVersion1(numbers, size); // 1 2 3 4 5
    printArrayVersion2(numbers, size); // 1 2 3 4 5
}

 ```

###  **Двумерен масив**
####  **Статично заделен**

 ```
void printMatrix(int arr[][3], int rows) 		// Броят на колоните трябва да бъде фиксиран, защото компилаторът трябва да знае 
							// как да разпредели паметта
{
	for (int i = 0; i < rows; i++) {
	    for (int j = 0; j < 3; j++) {
		 std::cout << arr[i][j] << " ";
        }

        std::cout << std::endl;
    }
}

int main() 
{
    int matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};

    printMatrix(matrix, 2);
    return 0;
}

 ```

####  **Динамично заделен**

 ```
#include <iostream>

void printMatrix(int** arr, int rows, int cols) 
{
	for (int i = 0; i < rows; i++) {
	    for (int j = 0; j < cols; j++) {
                std::cout << arr[i][j] << " ";
        }

        std::cout << std::endl;
    }
}

int main() 
{
    int rows = 2, cols = 3;

    int** matrix = new int*[rows];
    for (int i = 0; i < rows; i++) {
	    for (int j = 0; j < cols; j++) {
                std::cin >> matrix[i][j];
        }

    printMatrix(matrix, rows, cols);


    // Освобождаване на паметта
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

 ```

###  *Разлика между * и *& като аргумент на фунцкия**
####  **Подаване с * (по копие)**


 ```
void modifyValue(int* ptr) // прави се копие на ptr
{
    *ptr = 20; 						      // променяме стойността на адреса, където сочи указателя
}

void changePointer(int* ptr) 
{
    int temp = 30;
    ptr = &temp; 					      // Променяме локалното копие на ptr
}


int main() 
{
    int a = 10;
    int* p = &a;

    modifyValue(p);
    std::cout << "After modifyValue: " << a << std::endl;     // a става 20

    changePointer(p);
    std::cout << "After changePointer: " << *p << std::endl;  // Все още е 20
}

 ```

####  **Подаване с *& (по референция)**


 ```
void modifyValue(int*& ptr) // не се прави копие на ptr
{
    *ptr = 20; 						      // променяме стойността на адреса, където сочи указателя
}

void changePointer(int*& ptr) // не се прави копие на ptr
{
    static int temp = 30; 				     // Трябва да е статично или динамично заделена. Защо? - Dangling pointer (висящ указател)
    ptr = &temp; 					     // Променяме накъде сочи на ptr
}


int main() 
{
    int a = 10;
    int* p = &a;

    modifyValue(p);
    std::cout << "After modifyValue: " << a << std::endl;    // a става 20

    changePointer(p);
    std::cout << "After changePointer: " << *p << std::endl; // Вече е 30, защото p сочи към temp
}

 ```

###  **Enum и Enum Class**
####  **Enum**
*	Елементите на enum са в глобалното пространство
*	Стойностите му могат автоматично да се преобразуват в int
*	Може да предизвика конфликти
<br />
 ```
#include <iostream>

enum Color { Red, Green, Blue }; 		         // Елементите са в глобалния scope

enum TrafficLight { Red, Yellow, Green }; 	         // Има конфликт с "Red" и "Green" от Color

int main() 
{
    Color c = Red;  				         // Разрешено, Red = 0
    int value = Green; 				         // Разрешено, Green = 1 (неявно преобразуване)
    
    // TrafficLight t = Red;  			         // Грешка! Не е ясно дали е Red или RedLight

    std::cout << "Green as int: " << value << std::endl; // Извежда "1"

}


 ```

####  **Enum Class**
*	Елементите са в обхвата на enum class (не създава конфликти дори да има друг enum )
*	Не се преобразуват автоматично в int (по-безопасно)
*	Изисква се static_cast<int> за числова стойност
<br />
 ```
enum class Color { Red, Green, Blue }; 			 // Членовете са вътре в Color
enum class TrafficLight { Red, Yellow, Green }; 	 // Няма конфликт с Color

int main() {
    Color c = Color::Red; 				 // Трябва да се уточни името на елемента
    TrafficLight t = TrafficLight::Green; 		 // Няма конфликт

    // int value = c; 					 // Грешка! Няма неявно преобразуване

    int value = static_cast<int>(Color::Green); 	 // Явно преобразуване

    std::cout << "Green as int: " << value << std::endl; // Извежда "1"
}

 ```

###  **Union**
*	Елементите споделят една и съща памет
*	Има размер толкова byte, колкото най-големия елемент
<br />
 ```
union student { 
    int age; 
    float GPA; 
}; 
  
int main() 
{
    union student student1; 
  
    student1.age = 25; 
    std::cout << "Age : " << student1.age << std::endl;   	          // Age : 25
  
    student1.GPA = 4.5; 
    std::cout << "GPA : " << student1.GPA << std::endl;		          // GPA : 4.5

    std::cout << "Memory address of age: " << &student1.age << std::endl; // Memory address of age: 0x7ffcfc68fc20

    std::cout << "Memory address of GPA: " << &student1.GPA << std::endl; // Memory address of GPA: 0x7ffcfc68fc20

    std::cout << "Size of a union: " << sizeof(student1) << std::endl;	  // Size of a union: 4 -> защото int е най-големия елемент
}

 ```

###  **Основни git команди**

 ```
git clone <URL>

 ```
#### Клонира (копира) съществуващо хранилище
<br />

 ```
git status

 ```
#### Показва статуса на файловете (променени, нови, готови за commit и т.н.)
<br />

 ```
git log

 ```
#### Показва историята на комитите
<br />

 ```
git add <име_на_файл>
git add .


 ```
#### Добавя файлове за commit (. означава "всички файлове")
<br />

 ```
git commit -m "Съобщение за commit"

 ```
#### Запазва промените в локалното хранилище
<br />

 ```
git branch

 ```
#### Показва всички локални клонове
<br />

 ```
git branch <име_на_клон>

 ```
#### Създава нов клон
<br />

 ```
git checkout <име_на_клон>
git switch <име_на_клон>

 ```
#### Превключва към друг клон. Switch е алтернативен и по-модерен начин за смяна
<br />

 ```
git pull origin <име_на_клон>

 ```
#### Изтегля и слива последните промени. Така актуализираме локалния клон в нашето repo с последните промени по клона в remote repo-то
<br />

 ```
git push origin <име_на_клон>

 ```
#### Качва локални промени към remote repo
<br />

