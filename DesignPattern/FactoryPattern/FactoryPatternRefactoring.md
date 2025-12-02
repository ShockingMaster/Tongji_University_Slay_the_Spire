[TOC]

# Factory Pattern Refactoring

## 1. Brief Introduction to Factory Pattern

### 1.1 What is Factory Pattern

The **Factory Pattern** is a creational design pattern that provides an interface for creating objects in a superclass, but allows subclasses to alter the type of objects that will be created. In our context, we specifically use a **Registration-based Factory** variant (also known as Self-Registering Factory) to decouple the factory from the concrete product classes.

**Key Components:**
- **Factory Interface**: Declares the creation method (CardRegistry)
- **Concrete Products**: Different implementations of the product interface (Attack, Defense, etc.)
- **Registration Mechanism**: A way for products to register themselves with the factory automatically

---

## 2. Previous Implementation and Problems

### 2.1 Original Implementation Approach

Before refactoring, the card creation system typically relied on a monolithic conditional structure (switch or if-else chains) to instantiate the correct card class based on a string identifier or enum.

**Example: Traditional Factory (Anti-Pattern)**

```cpp
std::shared_ptr<Card> CardFactory::createCard(const std::string& name) {
    if (name == "Strike") {
        return std::make_shared<Strike>();
    } else if (name == "Defend") {
        return std::make_shared<Defend>();
    } else if (name == "Bash") {
        return std::make_shared<Bash>();
    }
    // ... hundreds of other cards
    else {
        return nullptr;
    }
}
```

### 2.2 Problems with Original Approach

**1. Violation of Open/Closed Principle (OCP)**
- To add a new card, you must modify the `createCard` method.
- The factory class grows indefinitely as new content is added.
- High risk of merge conflicts when multiple developers add cards simultaneously.

**2. High Coupling**
- The Factory must include the headers of ALL concrete card classes.
- Any change in a concrete card class header might trigger a recompilation of the Factory and all its clients.

**3. Maintenance Nightmare**
- A single file becomes a dependency bottleneck.
- Logic is prone to errors (typos in string matching inside the huge if-else block).

---

## 3. Overall Refactoring Design

### 3.1 Architecture Overview

The refactored system uses a **Self-Registering Factory** mechanism. Instead of the factory knowing every card, each card registers itself with the factory at program startup.

**Core Components:**

```
CardRegistry (Factory)
    ├── map<string, CardFactory> registry (Stores creators)
    ├── registerCard() (Adds creator to map)
    └── createCard() (Lookups creator and executes)

ConcreteCard (Product)
    ├── Constructor
    └── Static Registration Block (AUTO_REGISTER_CARD macro)
```

### 3.2 Design Decisions

**1. Macro-Based Registration (`AUTO_REGISTER_CARD`)**
- We use a preprocessor macro to automate the registration process.
- This hides the complexity of static variable initialization and lambda functions from the card developer.
- Usage is as simple as `AUTO_REGISTER_CARD(CardName)` in the .cpp file.

**2. Static Initialization Trick**
- We leverage C++'s static variable initialization order.
- Anonymous lambdas are executed at program startup to register factories before `main()` runs (or effectively before first use).

**3. `std::function` as Creator**
- The registry stores `std::function<std::shared_ptr<Card>()>`, allowing for flexible creation logic (e.g., using `std::make_shared`).

### 3.3 UML Class Diagram

#### Before Refactoring (Simple Factory)

![Before Refactoring - Simple Factory](./assets/figure_1_uml_before_refactoring.png)

*(Concept: Client -> Factory -> [Depend on All Concrete Classes])*

- **Coupling**: Factory depends on CardA, CardB, CardC...
- **Flow**: `createCard("A")` -> `if "A" return new CardA()`

#### After Refactoring (Self-Registering Factory)

![After Refactoring - Self-Registering Factory](./assets/figure_2_uml_after_refactoring.png)

*(Concept: Concrete Classes -> [Register to] -> Factory)*

- **Decoupling**: Factory has NO compile-time dependency on Concrete Cards.
- **Flow**: 
    1. Program Start: `CardA` registers itself to `CardRegistry`.
    2. Runtime: `CardRegistry::createCard("A")` -> looks up map -> invokes registered lambda.

---

## 4. Detailed Implementation

### 4.1 Core File Changes

**Involved Files:**
```
Classes/
├── Register.h       (The Registry/Factory)
├── Enum.h           (The Registration Macro)
└── Card.cpp         (Example Usage)
```

### 4.2 The Registry (`Register.h`)

The `CardRegistry` class manages a map of string names to creation functions.

```cpp
class CardRegistry {
public:
    using CardFactory = std::function<std::shared_ptr<Card>()>;

    // Register a card factory function
    static void registerCard(const std::string& name, CardFactory factory) {
        getRegistry()[name] = factory;
    }

    // Create a card by name
    static std::shared_ptr<Card> createCard(const std::string& name) {
        // ... (Handling upgrades logic omitted for brevity) ...
        auto& registry = getRegistry();
        auto it = registry.find(newname);
        if (it != registry.end()) {
            return it->second(); // Invoke the factory lambda
        }
        throw std::runtime_error("Card not found: " + name);
    }

private:
    // Meyer's Singleton for the map to ensure correct initialization order
    static std::map<std::string, CardFactory>& getRegistry() {
        static std::map<std::string, CardFactory> registry;
        return registry;
    }
};
```

### 4.3 The Registration Macro (`Enum.h`)

A macro simplifies the registration process for developers.

```cpp
#define AUTO_REGISTER_CARD(className)                          \
    const bool className##Registered = []() {                  \
        CardRegistry::registerCard(#className, []() {          \
            return std::make_shared<className>();              \
        });                                                    \
        return true;                                           \
    }();
```

**How it works:**
1. Defines a static `const bool` variable.
2. Initializes it with an immediately invoked lambda.
3. Inside the lambda, calls `CardRegistry::registerCard`.
4. Uses `#className` stringification to pass the class name as a string.

### 4.4 Example Usage (`Card.cpp`)

Adding a new card now requires **zero changes** to the Factory or Registry. You simply add the macro in your implementation file.

```cpp
// In Attack.cpp or Card.cpp
AUTO_REGISTER_CARD(Attack)
AUTO_REGISTER_CARD(Defense)
AUTO_REGISTER_CARD(Bash)
```
