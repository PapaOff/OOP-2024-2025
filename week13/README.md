# SOLID
| Буква | Име на принцип | Какво означава | Пояснение | 
|----------|-----------| -----------| -----------|
| `S` | Single Responsibility | Всеки клас трябва да има само една отговорност | Клас `Invoice` трябва само да изчислява сумата, а не и да я пише във файл |
| `O` | Open/Closed | Класовете трябва да са отворени за разширение, но затворени за промяна | Вместо да редактираш съществуващ клас при ново поведение, наследяваш или добавяш стратегия, без да пипаш оригинала |
| `L` | Liskov Substitution | Всеки наследник трябва да може да замести обект от базов клас | Тоест наследника трябва да има валидно поведение за всеки метод, който `override`-ва от базовия |
| `I` | Interface Segregation | По-добре много малки интерфейси, отколкото един голям | Един клас не трябва да бъде принуден да имплементира методи, които не му трябват |
| `D` | Dependency Inversion | Зависимостите трябва да са към абстракции, не конкретики | Тоест да зависиш от интерфейс, а не от конкретна имплементация |

# Desing Patterns
Представляват повторно използваеми, доказали се във времето решения на типични проблеми, възникващи при проектирането на софтуер.
Те не са готов код, а по-скоро схеми и подходи, които можем да следваме

Разделят се на 3 вида:
 - Creational
 - Structural
 - Behavioral
 
## Creational
Вместо директно да се създаде обект, тези pattern-и ни дават повече гъвкавост за това как да ги създадем

### Singleton
Използва се, когато искаме една единствена инстанция, която да бъде достъпна от много места в кода

В много приложения е необходимо да се осъществи връзка с база данни – за да четем, записваме или актуализираме данни.
Но създаването на такава връзка всеки път, когато ни потрябва, е неефективно и заема ресурс.
Освен това, ако позволим различни части от кода да създават собствени връзки, можем да стигнем до проблеми като:
 - Несъгласувани данни (дна част от кода може да чете стари данни, докато друга вече ги е променила, защото използват различни сесии)
 - Трудности при управление на транзакции (ако всяка операция върви по своя връзка, не можем да обвържем действията в една обща транзакция и да гарантираме целостта на данните)
 - Грешки, ако две части от системата правят промени паралелно, без координация (едната актуализира запис, докато другата го изтрива, без да знаят една за друга)
 
**Пример**
`DatabaseConnection.h`
```c++
#pragma once

#include <string>
#include <pqxx/pqxx>

class DatabaseConnection {
private:
    static DatabaseConnection* instance;
	pqxx::connection* connection;
    bool connected;

    // Private е, защото искаме ние да контролираме създаването
    DatabaseConnection();

public:
    // Забраняваме копиране и присвояване
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

   
    ~DatabaseConnection();

    // Статичен метод за достъп до инстанцията
    static DatabaseConnection* getInstance();

    static void destroyInstance(); // за освобождаване
    void query(const std::string& sql) const;
    bool isConnected() const;
};
```

`DatabaseConnection.cpp`
```c++
#include "DatabaseConnection.h"
#include <iostream>

// Инициализация на статичния член
DatabaseConnection* DatabaseConnection::instance = nullptr;

DatabaseConnection::DatabaseConnection() {
    try {
        std::string connStr = "dbname=mydb user=myuser password=mypass host=localhost port=5432";
        connection = new pqxx::connection(connStr);
        if (connection->is_open()) {
            connected = true;
            std::cout << "Успешна връзка с PostgreSQL.\n";
        } else {
            connected = false;
            std::cerr << "Връзката не можа да се отвори.\n";
        }
    } catch (const std::exception& e) {
        connected = false;
        std::cerr << "Грешка при свързване: " << e.what() << "\n";
    }
}

DatabaseConnection::~DatabaseConnection() {
    if (connection != nullptr) {
        if (connection->is_open()) {
            std::cout << "[DatabaseConnection] Затваряне на връзката...\n";
        }
        delete connection;
        connection = nullptr;
    }
    std::cout << "[DatabaseConnection] Обектът беше унищожен.\n";
}

DatabaseConnection* DatabaseConnection::getInstance() {
    if (instance == nullptr) {
        instance = new DatabaseConnection();
    }
    return instance;
}

void DatabaseConnection::destroyInstance() {
    delete instance;
    instance = nullptr;
}

void DatabaseConnection::query(const std::string& sql) const {
    if (connected && connection != nullptr && connection->is_open()) {
        try {
            pqxx::work txn(*connection);
            pqxx::result res = txn.exec(sql);
            txn.commit();
            std::cout << "[DatabaseConnection] Успешно изпълнена заявка.\n";
        } catch (const std::exception& e) {
            std::cerr << "[DatabaseConnection] Грешка при изпълнение на заявка: " << e.what() << "\n";
        }
    } else {
        std::cout << "[DatabaseConnection] Няма активна връзка с базата.\n";
    }
}

bool DatabaseConnection::isConnected() const {
    return connected;
}
```

`main.cpp`
```c++
#include "DatabaseConnection.h"

int main() {
    DatabaseConnection* db = DatabaseConnection::getInstance(); // Създаване на обекта

    if (db->isConnected()) {
        db->query("SELECT * FROM users");  // Изпълняване на заявка: SELECT * FROM users
    }


    DatabaseConnection* db2 = DatabaseConnection::getInstance(); // Вземаме пак същата инстанция, не се създава нова
    db2->query("SELECT * FROM orders"); // Изпълняване на заявка: SELECT * FROM orders

	DatabaseConnection::destroyInstance(); // важно за освобождаване на паметта
    return 0;
}
```


### Builder
Представете си, че имате клас с 15 полета, от които само няколко са задължителни.
Ако се опитате да съберете всичко в един конструктор с 15 аргумента – успех на този, който ще се опита да разбере какво се случва

Вместо това можем да направим следното:
 - да отделим задължителните параметри в конструктора
 - да задаваме незадължителните стойности чрез ясно именовани методи
 - и накрая да създадем обекта по по-четим и контролиран начин

Това води до по-разбираем, гъвкав и поддържан код, особено когато се налага да създаваме обекти с много конфигурации

Нека да разгледаме пример с HTTP заяка. Тя има:
 - задължителни части: метод (GET, POST), URL
 - незадължителни части: headers, body, timeout, cookies и т.н

**Пример**
`HttpRequest.hpp`
```c++
#pragma once

#include <string>
#include <map>

class HttpRequest {
private:
    std::string method;
    std::string url;
    std::map<std::string, std::string> headers;
    std::string body;

    // Частен конструктор
    HttpRequest(const std::string& method, const std::string& url)
        : method(method), url(url) {}

public:
    class Builder {
    private:
        std::string method;
        std::string url;
        std::map<std::string, std::string> headers;
        std::string body;

    public:
        Builder(const std::string& method, const std::string& url)
            : method(method), url(url) {}

        Builder& setHeader(const std::string& key, const std::string& value) {
            headers[key] = value;
            return *this;
        }

        Builder& setBody(const std::string& data) {
            body = data;
            return *this;
        }

        HttpRequest build() {
            HttpRequest request(method, url);
            request.headers = headers;
            request.body = body;
            return request;
        }
    };

    void send() const {
        std::cout << "=== Sending HTTP " << method << " request ===\n";
        std::cout << "URL: " << url << "\n";
        if (!headers.empty()) {
            std::cout << "Headers:\n";
            for (const auto& h : headers) {
                std::cout << "  " << h.first << ": " << h.second << "\n";
            }
        }
        if (!body.empty()) {
            std::cout << "Body: " << body << "\n";
        }
    }
};
```

`main.cpp`
```c++
#include "HttpRequest.h"
#include <iostream>

int main() {
    HttpRequest request = HttpRequest::Builder("POST", "https://api.example.com/data")
        .setHeader("Content-Type", "application/json")
        .setHeader("Authorization", "Bearer token123")
        .setBody(R"({"name": "John", "age": 30})")
        .build();

    request.send();

    return 0;
}
```

### Factory
Позволява да създаваме обекти, без да знаем техния точен клас, като вместо това използваме общ интерфейс и фабричен метод,
който решава кой точно тип обект да върне

Представете си, че имаме система, в която могат да се регистрират:
 - `Гост (Guest)` – няма профил, има ограничен достъп;
 - `Регистриран потребител (RegisteredUser)` – има профил, вижда повече;
 - `Администратор (Admin)` – има пълен достъп
 
Когато потребителят влезе, не искаме ръчно да пишем
```c++
if (role == "guest") user = new Guest();
else if (role == "admin") user = new Admin();
else if (role == "user") user = new RegisteredUser();
```
Това е неудобно, защото може да ни трябва на много места и ще имаме повтарящ се код. 
Също така, ако добавим/махнем роли, ще трябва да променяме кода на всички места

Вместо това имаме `UserFactory`, която знае какъв обект да създаде, на базата на подадения тип

**Пример**
`User.hpp`
```c++
#pragma once
#include <string>

class User {
public:
    virtual std::string getPermissions() const = 0;
    virtual ~User() = default;
};
```

`Guest.hpp`
```c++
#pragma once
#include "User.hpp"

class Guest : public User {
public:
    std::string getPermissions() const override {
        return "Read-only access";
    }
};
```

`Admin.hpp`
```c++
#pragma once
#include "User.hpp"

class Admin : public User {
public:
    std::string getPermissions() const override {
        return "Full access";
    }
};
```

`RegisteredUser.h`
```c++
#pragma once
#include "User.hpp"

class RegisteredUser : public User {
public:
    std::string getPermissions() const override {
        return "Read and write access";
    }
};
```

`UserFactory.hpp`
```c++
#pragma once
#include "User.hpp"
#include "Guest.hpp"
#include "Admin.hpp"
#include "RegisteredUser.hpp"
#include <string>

class UserFactory {
public:
    static User* createUser(const std::string& type) {
        if (type == "guest") return new Guest();
        if (type == "admin") return new Admin();
        if (type == "user") return new RegisteredUser();
        return nullptr;
    }
};
```

`main.cpp`
```c++
#include "UserFactory.hpp"
#include <iostream>

int main() {
    User* user = UserFactory::createUser("admin");

    if (user) {
        std::cout << "Permissions: " << user->getPermissions() << "\n";
        delete user;
    } else {
        std::cout << "Unknown user type.\n";
    }

    return 0;
}
```
## Structural
Определят начина, по който обектите се свързват помежду си и как са организирани - как са "подредени"

### Facade
Предоставя опростен интерфейс към сложна система от класове, обекти или подсистеми

В реални системи често имаме:
 - много класове
 - сложна логика (инициализация, зависимости)
 - повтарящ се код при всяко извикване
 
Facade решава това, като обединява често използвана функционалност в един централен клас

**Пример**
`OrderManager.hpp`
```c++
class OrderManager {
public:
    void createOrder() {
        std::cout << "Генериране на ID и създаване на поръчка...\n";
    }

    void updateStatus() {
        std::cout << "Обновяване на статус на поръчката...\n";
    }
};
```

`PaymentProcessor.hpp`
```c++
class PaymentProcessor {
public:
    void validateCard() {
        std::cout << "Проверка на кредитна карта...\n";
    }

    void processPayment() {
        std::cout << "Обработка на плащането...\n";
    }
};
```

`InventoryService.hpp`
```c++
class InventoryService {
public:
    void reserveItems() {
        std::cout << "Резервиране на артикули...\n";
    }

    void updateInventory() {
        std::cout << "Актуализиране на инвентара...\n";
    }
};
```

`ShippingService.hpp`
```c++
class ShippingService {
public:
    void calculateCost() {
        std::cout << "Изчисляване на цена за доставка...\n";
    }

    void scheduleShipment() {
        std::cout << "Избор на куриер и планиране на доставка...\n";
    }
};
```

`BuyNowFacade.hpp`
```c++
class BuyNowFacade {
private:
    OrderManager order;
    PaymentProcessor payment;
    InventoryService inventory;
    ShippingService shipping;

public:
    void buy() {
        std::cout << "=== [Buy Now Button Clicked] ===\n";
        order.createOrder();
        inventory.reserveItems();
        payment.validateCard();
        payment.processPayment();
        inventory.updateInventory();
        order.updateStatus();
        shipping.calculateCost();
        shipping.scheduleShipment();
        std::cout << "=== Поръчката е завършена! ===\n";
    }
};
```

`main.cpp без Facade`
```
int main() {
    std::cout << "=== [Buy Now Button Clicked] ===\n";

    OrderManager order;
    order.createOrder();

    InventoryService inventory;
    inventory.reserveItems();

    PaymentProcessor payment;
    payment.validateCard();
    payment.processPayment();

    inventory.updateInventory();

    order.updateStatus();

    ShippingService shipping;
    shipping.calculateCost();
    shipping.scheduleShipment();

    std::cout << "=== Поръчката е завършена! ===\n";

    return 0;
}
```

`main.cpp с Facade`
```
int main() {
    BuyNowFacade shop;
    shop.buy();

    return 0;
}
```

### Adapter
Позволява обекти с несъвместими интерфейси да работят заедно, като превежда интерфейса на един клас към друг, който се очаква

Представете си следната ситуация:
Вие разработвате нова онлайн платформа, която трябва да приема плащания. Решили сте, че всички платежни модули ще имплементират общ интерфейс 
`PaymentProcessor`, за да може в бъдеще лесно да добавяте нови доставчици – например `Stripe`, `Apple Pay` и др
Вашият интерфейс изглежда така:
```c++
class PaymentProcessor {
public:
    virtual void pay(double amount, const std::string& currency) = 0;
};
```

Но имате проблем - компанията вече използва стара библиотека на `PayPal`, написана преди години, която не следва този интерфейс. 
Тя изисква отделно логване и приема само `float` стойности в `USD`, чрез метод `sendPayment(float)`. Вместо да пренаписвате старата библиотека (което не е позволено, тя е в use от години), 
решавате да използвате `Adapter` шаблона – ще създадете клас, който "превежда" новия интерфейс към стария

`LegacyPayPal.hpp`
```c++
#pragma once
#include <iostream>

class LegacyPayPal {
public:
    void login(const std::string& email, const std::string& password) {
        std::cout << "[LegacyPayPal] Влизане с " << email << "\n";
    }

    void sendPayment(float amountInUSD) {
        std::cout << "[LegacyPayPal] Извършване на плащане: $" << amountInUSD << " USD\n";
    }
};
```

**Новия интерфейс**
`PaymentProcessor.hpp`
```c++
#pragma once
#include <string>

class PaymentProcessor {
public:
    virtual void pay(double amount, const std::string& currency) = 0;
    virtual ~PaymentProcessor() = default;
};
```
`PayPalAdapter.hpp`
```c++
#pragma once
#include "PaymentProcessor.hpp"
#include "LegacyPayPal.hpp"
#include <map>

class PayPalAdapter : public PaymentProcessor {
private:
    LegacyPayPal* legacy;
    std::string email;
    std::string password;

    float convertToUSD(double amount, const std::string& currency) {
        std::map<std::string, float> rates = {
            {"USD", 1.0f},
            {"EUR", 1.1f},
            {"BGN", 0.55f}
        };
        return static_cast<float>(amount * rates[currency]);
    }

public:
    PayPalAdapter(LegacyPayPal* legacy, const std::string& email, const std::string& password)
        : legacy(legacy), email(email), password(password) {}

    void pay(double amount, const std::string& currency) override {
        legacy->login(email, password);
        float usdAmount = convertToUSD(amount, currency);
        legacy->sendPayment(usdAmount);
    }
};
```

`main.cpp`
```c++
#include "PayPalAdapter.h"
#include <iostream>

int main() {
    LegacyPayPal* oldPayPal = new LegacyPayPal();

    // Системата очаква PaymentProcessor, но получава адаптер
    PaymentProcessor* processor = new PayPalAdapter(oldPayPal, "john@example.com", "securePass");

    processor->pay(200, "BGN");

    delete processor;
    delete oldPayPal;

    return 0;
}
```
## Behavioral
Свързани с начина, по който обектите взаимодействат

### Strategy
Позволява да дефинираме различни алгоритми (поведения), които са взаимозаменяеми, и да делегираме избора на алгоритъм по време на изпълнение, вместо да пишем if-else

Представете си, че разработвате приложение за онлайн магазин, който предлага различни начини за изчисляване на отстъпки:
 - за нови клиенти – 10%
 - за лоялни клиенти – 20%
 - за корпоративни клиенти – фиксирана сума
 
Ако това го напишете с `if` ще изглежда така:
```c++
if (clientType == "new") { ... }
else if (clientType == "loyal") { ... }
else if (clientType == "corporate") { ... }
```
Лоша идея – трудно за поддръжка, невъзможно за разширение без промяна на кода


Вместо това използвате `Strategy`
 - създавате интерфейс `DiscountStrategy`, който дефинира метода `calculateDiscount()`
 - всеки тип отстъпка се реализира като отделен клас
 - флавният обект (например `Order`) приема конкретна стратегия и я използва
 
**Пример**

```c++
class DiscountStrategy {
public:
    virtual double calculateDiscount(double total) const = 0;
    virtual ~DiscountStrategy() = default;
};
```

```
class NewCustomerDiscount : public DiscountStrategy {
public:
    double calculateDiscount(double total) const override {
        return total * 0.10;  // 10%
    }
};

class LoyalCustomerDiscount : public DiscountStrategy {
public:
    double calculateDiscount(double total) const override {
        return total * 0.20;  // 20%
    }
};

class CorporateDiscount : public DiscountStrategy {
public:
    double calculateDiscount(double total) const override {
        return 50.0;  // фиксирана отстъпка
    }
};
```

`Order.hpp`
```c++
class Order {
private:
    double total;
    const DiscountStrategy* discount;

public:
    Order(double total, const DiscountStrategy* discount)
        : total(total), discount(discount) {}

    double finalPrice() const {
        return total - discount->calculateDiscount(total);
    }
};
```

`main.cpp`
```c++
int main() {
    NewCustomerDiscount newDiscount;
    Order order1(300, &newDiscount);
    std::cout << "Крайна цена (нов клиент): " << order1.finalPrice() << "\n";

    CorporateDiscount corpDiscount;
    Order order2(300, &corpDiscount);
    std::cout << "Крайна цена (корп. клиент): " << order2.finalPrice() << "\n";

    return 0;
}
```

### Observer
Позволява един обект (наречен `Subject`) да поддържа списък с наблюдатели (`Observers`) и автоматично да ги уведоми, когато състоянието му се промени

Представете си, че имате приложение за акции. Когато цената на дадена акция се промени, искате:
 - да се обнови графиката
 - да се покаже известие
 - да се изпрати email или SMS
 
Единия вариант е класът за акция да извиква необходимите методи на другите класове (`updateGraph()`, `sendEmail()`, `showPopup()`).
Това обаче е лоша практика, защото класът за акция трябва да знае за останалите класове, а и ако се промени името на някоя от фунцкиите
или се добави допълнителна логика, която да става при промяна на цената, ще трябва да променяте класа за акция

Вместо това използваме `Observer` – обектът с цената не знае кой го следи, а просто казва "Има промяна",
 и всички регистрирани наблюдатели се уведомяват автоматично
 
**Пример**
```c++
class Observer {
public:
    virtual void update(double price) = 0;
    virtual ~Observer() = default;
};
```

```c++
class GraphDisplay : public Observer {
public:
    void update(double price) override {
        std::cout << "Graph updated. New price: " << price << "\n";
    }
};

class PriceAlert : public Observer {
public:
    void update(double price) override {
        if (price > 100)
            std::cout << "PriceAlert: Price above 100! Current: " << price << "\n";
    }
};
```

```c++
#include <vector>

class Stock {
private:
    std::vector<Observer*> observers;
    double price = 0.0;

public:
    void addObserver(Observer* o) {
        observers.push_back(o);
    }

    void setPrice(double newPrice) {
        price = newPrice;
        notifyAll();
    }

    void notifyAll() {
        for (Observer* o : observers) {
            o->update(price);
        }
    }
};
```

`main.cpp`
```c++
int main() {
    Stock tesla;

    GraphDisplay graph;
    PriceAlert alert;

    tesla.addObserver(&graph);
    tesla.addObserver(&alert);

    tesla.setPrice(95.0);   // само GraphDisplay реагира
    tesla.setPrice(110.0);  // GraphDisplay + PriceAlert

    return 0;
}
```
