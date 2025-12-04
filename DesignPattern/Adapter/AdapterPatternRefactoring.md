[TOC]

# Adapter Pattern Refactoring

## 1. Brief Introduction to Adapter Pattern

### 1.1 What is Adapter Pattern

The **Adapter Pattern** is a structural design pattern that allows objects with incompatible interfaces to work together. It acts as a bridge between two incompatible interfaces, converting the interface of a class into another interface that clients expect. This pattern enables classes to collaborate that otherwise couldn't because of incompatible interfaces.

**Key Components:**
- **Target Interface**: The interface that the client expects and can work with (IEffect)
- **Adapter**: Converts the interface of the Adaptee to the Target interface (EffectAdapter)
- **Adaptee**: The existing interface that needs adapting (Card and Buff classes with incompatible effect implementations)
- **Client**: Uses the Target interface (Card and Buff classes using the IEffect interface)

---

## 2. Previous Implementation and Problems

### 2.1 Original Implementation Approach

Before refactoring, the card and buff system implemented effects directly within each class using specific methods, leading to inconsistent interfaces:

**Example: Traditional Effect Implementation**

```cpp
// Card implementation with direct effect
class Attack : public Card {
public:
    Attack() {
        // Initialize card properties
    }
    
    bool takeEffect(Creature* source, Creature* target, Card* card) override {
        // Direct implementation of attack effect
        int damage = 6;
        CombatSystem::onAttack(source, target, damage, card);
        return true;
    }
    
    void upgrade() override {
        // Upgrade implementation
    }
};

// Buff implementation with different interface
class StrengthBuff : public Buff {
public:
    StrengthBuff(int turns = -1) {
        // Initialize buff properties
    }
    
    void onAttack(Creature* source, Creature* target, int& damage, Card* card) override {
        // Different interface for attack-related effects
        damage += 3;
    }
};
```

### 2.2 Problems with Original Approach

**1. Inconsistent Interfaces**
- Card effects were implemented in `takeEffect()` methods
- Buff effects were spread across multiple event methods (`onAttack()`, `onAttacked()`, etc.)
- No unified way to represent or manage effects

**2. Limited Reusability**
- Effects were tightly coupled to specific card/buff classes
- No way to reuse the same effect across different cards or buffs

**3. Difficulty in Effect Combination**
- Adding multiple effects to a single card required modifying its `takeEffect()` method
- No flexible way to compose complex effects from simpler ones

**4. Poor Extensibility**
- Adding new effect types required modifying existing classes
- Each new card/buff had to reimplement similar effect logic

---

## 3. Overall Refactoring Design

### 3.1 Architecture Overview

The refactored system uses an **Adapter Pattern** to create a unified interface for all effects, whether they originate from cards or buffs. This allows consistent management and application of effects across different game components.

**Core Components:**

```
IEffect (Target Interface)
    ├── execute(source, target, card, value) (Unified effect method)
    
EffectAdapter (Adapter Base)
    ├── execute() (Default implementation)
    
├── AttackEffectAdapter
├── BlockEffectAdapter  
└── SecondWindEffectAdapter
    └── execute() (Adaptee-specific implementation)

Card/Buff (Client)
    ├── effects_ (vector of IEffect pointers)
    ├── addEffect() (Adds effects to the list)
    └── executeAllEffects() (Applies all effects)
```

### 3.2 Design Decisions

**1. Unified Effect Interface (`IEffect`)**
- Provides a single method `execute()` that all effects must implement
- Takes consistent parameters (source, target, card, value reference) to handle various effect scenarios
- Allows effects to be treated uniformly regardless of their origin

**2. Adapter Base Class (`EffectAdapter`)**
- Implements the `IEffect` interface with default empty implementations
- Reduces boilerplate code for effect adapters that only need to implement specific logic
- Simplifies the creation of new effect types

**3. Composition Over Inheritance**
- Cards and buffs now contain a list of effects rather than inheriting effect behavior
- Allows dynamic addition and removal of effects at runtime
- Enables easy combination of multiple effects on a single card/buff

### 3.3 UML Class Diagram

#### Before Refactoring (Inconsistent Interfaces)

```
┌─────────┐       ┌─────────┐       ┌─────────┐
│  Card   │       │  Buff   │       │ Combat  │
├─────────┤       ├─────────┤       ├─────────┤
│takeEffect()│     │onAttack()│     │onAttack()│
│         │     │onAttacked()│     │Addblock()│
└─────────┘     │... (17 methods)│   └─────────┘
               └─────────┘
```

#### After Refactoring (Unified Interface)

```
┌─────────┐       ┌─────────┐       ┌─────────┐
│  Card   │       │  Buff   │       │ IEffect │
├─────────┤       ├─────────┤       ├─────────┤
│effects_ │       │effects_ │       │execute()│
│addEffect()│     │addEffect()│     └─────────┘
│executeAllEffects()│   │executeAllEffects()│   ▲
└─────────┘     └─────────┘       │
                                    │
                          ┌─────────┴─────────┐
                          │EffectAdapter      │
                          ├───────────────────┤
                          │execute() (default)│
                          └───────────────────┘
                                    ▲
                                    │
                          ┌─────────┴─────────┐
                          │AttackEffectAdapter│
                          │BlockEffectAdapter │
                          │SecondWindEffectAdapter│
                          └───────────────────┘
```

---

## 4. Detailed Implementation

### 4.1 Core File Changes

**Involved Files:**
```
Classes/
├── IEffect.h                 (Unified effect interface)
├── EffectAdapter.h           (Adapter base class)
├── AttackEffectAdapter.h     (Concrete attack adapter)
├── BlockEffectAdapter.h      (Concrete block adapter)
├── SecondWindEffectAdapter.h (Concrete second wind adapter)
├── Card.h                    (Updated card class)
├── Card.cpp                  (Updated card implementations)
└── IncludeAll.h              (Added new headers)
```

### 4.2 The Target Interface (`IEffect.h`)

The `IEffect` interface defines the unified method for all effects:

```cpp
class IEffect {
public:
    virtual ~IEffect() = default;
    virtual bool execute(Creature* source, Creature* target, Card* card, int& value) = 0;
};
```

### 4.3 The Adapter Base Class (`EffectAdapter.h`)

The `EffectAdapter` provides a default implementation of the `IEffect` interface:

```cpp
#include "IEffect.h"

class EffectAdapter : public IEffect {
public:
    virtual ~EffectAdapter() = default;
    virtual bool execute(Creature* source, Creature* target, Card* card, int& value) override {
        return true; // Default empty implementation
    }
};
```

### 4.4 Concrete Adapters

**Attack Effect Adapter (`AttackEffectAdapter.h`)**
```cpp
#include "EffectAdapter.h"
#include "CombatSystem.h"

class AttackEffectAdapter : public EffectAdapter {
public:
    explicit AttackEffectAdapter(int damage) : damage_(damage) {}
    
    bool execute(Creature* source, Creature* target, Card* card, int& value) override {
        CombatSystem::onAttack(source, target, damage_, card);
        return true;
    }
    
private:
    int damage_;
};
```

**Block Effect Adapter (`BlockEffectAdapter.h`)**
```cpp
#include "EffectAdapter.h"
#include "CombatSystem.h"

class BlockEffectAdapter : public EffectAdapter {
public:
    explicit BlockEffectAdapter(int block) : block_(block) {}
    
    bool execute(Creature* source, Creature* target, Card* card, int& value) override {
        CombatSystem::Addblock(source, block_);
        return true;
    }
    
private:
    int block_;
};
```

### 4.5 Updated Card Class

The Card class now uses the adapter pattern:

```cpp
// In Card.h
class Card : public std::enable_shared_from_this<Card> {
    // ... existing members ...
    
private:
    std::vector<std::shared_ptr<IEffect>> effects_;
    
public:
    void addEffect(std::shared_ptr<IEffect> effect) {
        effects_.push_back(effect);
    }
    
    bool executeAllEffects(Creature* source, Creature* target, int& value) {
        bool result = true;
        for (auto& effect : effects_) {
            result &= effect->execute(source, target, this, value);
        }
        return result;
    }
};

// In Card.cpp - Using adapters in card implementations
Attack::Attack() {
    // ... card initialization ...
    addEffect(std::make_shared<AttackEffectAdapter>(6));
}

void Attack::upgrade() {
    // ... upgrade logic ...
    effects_.clear();
    addEffect(std::make_shared<AttackEffectAdapter>(9));
}

bool Attack::takeEffect(Creature* source, Creature* target, Card* card) {
    int value = 0;
    return executeAllEffects(source, target, value);
}
```

---

## 5. Example Usage

### 5.1 Creating Cards with Effects

```cpp
// Simple attack card
class Attack : public Card {
public:
    Attack() {
        // Card properties
        name_ = "Attack";
        cost_ = 1;
        type_ = CardType::ATTACK;
        
        // Add attack effect using adapter
        addEffect(std::make_shared<AttackEffectAdapter>(6));
    }
    
    void upgrade() override {
        // Upgrade the card
        upgraded_ = true;
        cost_ = 0;
        
        // Replace old effect with upgraded version
        effects_.clear();
        addEffect(std::make_shared<AttackEffectAdapter>(9));
    }
    
    bool takeEffect(Creature* source, Creature* target, Card* card) override {
        int value = 0;
        return executeAllEffects(source, target, value);
    }
};
```

### 5.2 Creating Cards with Multiple Effects

```cpp
// Card with combined effects
class Bash : public Card {
public:
    Bash() {
        // Card properties
        name_ = "Bash";
        cost_ = 2;
        type_ = CardType::ATTACK;
        
        // Add multiple effects
        addEffect(std::make_shared<AttackEffectAdapter>(8));
        addEffect(std::make_shared<WeaknessEffectAdapter>()); // Hypothetical weakness adapter
    }
    
    // ... upgrade and takeEffect methods ...
};
```

### 5.3 Creating New Effect Types

```cpp
// New poison effect adapter
class PoisonEffectAdapter : public EffectAdapter {
public:
    explicit PoisonEffectAdapter(int stacks) : stacks_(stacks) {}
    
    bool execute(Creature* source, Creature* target, Card* card, int& value) override {
        // Apply poison stacks
        target->addBuff(std::make_shared<Poison>(stacks_));
        return true;
    }
    
private:
    int stacks_;
};

// Using the new effect in a card
class NoxiousFumes : public Card {
public:
    NoxiousFumes() {
        // Card properties
        name_ = "Noxious Fumes";
        cost_ = 1;
        type_ = CardType::SKILL;
        
        // Add poison effect
        addEffect(std::make_shared<PoisonEffectAdapter>(3));
    }
    
    // ... upgrade and takeEffect methods ...
};
```

---

## 5. Benefits of the Refactored System

### 5.1 Improved Consistency
- All effects use the same `execute()` method signature
- Consistent way to manage and apply effects across cards and buffs

### 5.2 Enhanced Reusability
- Effects can be reused across different cards and buffs
- No need to rewrite similar effect logic for each card

### 5.3 Flexible Effect Composition
- Multiple effects can be easily combined on a single card
- Effects can be added, removed, or replaced dynamically

### 5.4 Better Extensibility
- Adding new effect types only requires creating new adapter classes
- Existing cards and buffs don't need modification when adding new effect types

### 5.5 Simplified Maintenance
- Effect logic is centralized in adapter classes
- Easier to debug and modify effects

---

## 6. Conclusion

The Adapter Pattern refactoring has successfully unified the effect system, providing a consistent interface for all effects in the game. This approach has addressed the limitations of the original implementation, including inconsistent interfaces, limited reusability, and poor extensibility.

By introducing a unified `IEffect` interface and concrete adapters, we've created a flexible system that allows easy addition of new effect types and seamless combination of multiple effects. The refactored system follows the Open/Closed Principle, enabling new effects to be added without modifying existing code.

This refactoring lays the foundation for a more robust and scalable effect system, making it easier to develop and maintain the game's card and buff mechanics in the future.