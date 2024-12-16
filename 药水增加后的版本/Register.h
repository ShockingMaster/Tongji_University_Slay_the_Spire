#pragma once
#include <queue>
#include <vector>
#include <memory>
#include <iostream>
#include <map>
#include <functional>
class Creature;
class Card;
class Potion;
class Relic;
class Buff;
class Monster;

// 卡牌注册
class CardRegistry {
public:
    using CardFactory = std::function<std::shared_ptr<Card>()>;
    // 卡牌注册
    static void registerCard(const std::string& name, CardFactory factory) {
        getRegistry()[name] = factory;
    }

    // 根据卡牌名称返回相应卡牌的指针
    static std::shared_ptr<Card> createCard(const std::string& name) {
        auto& registry = getRegistry();
        auto it = registry.find(name);
        if (it != registry.end()) {
            return it->second();
        }
        throw std::runtime_error("Card not found: " + name);
    }

    // 返回所有卡牌的名称
    static std::vector<std::string> getAllCardNames() {
        std::vector<std::string> names;
        for (const auto& pair : getRegistry()) {
            names.push_back(pair.first);
        }
        return names;
    }

private:
    static std::map<std::string, CardFactory>& getRegistry() {
        static std::map<std::string, CardFactory> registry;
        return registry;
    }
};

// 卡牌自动注册
#define AUTO_REGISTER_CARD(className)                          \
    const bool className##Registered = []() {                  \
        CardRegistry::registerCard(#className, []() {          \
           return std::make_shared<className>();               \
        });                                                    \
        return true;                                           \
    }();

// 遗物注册，方式同上
class RelicRegistry {
public:
    using RelicFactory = std::function<std::shared_ptr<Relic>()>;
    static void registerRelic(const std::string& name, RelicFactory factory) {
        getRegistry()[name] = factory;
    }

    static std::shared_ptr<Relic> createRelic(const std::string& name) {
        auto& registry = getRegistry();
        auto it = registry.find(name);
        if (it != registry.end()) {
            return it->second();
        }
        throw std::runtime_error("Relic not found: " + name);
    }

    static std::vector<std::string> getAllRelicNames() {
        std::vector<std::string> names;
        for (const auto& pair : getRegistry()) {
            names.push_back(pair.first);
        }
        return names;
    }

private:
    static std::map<std::string, RelicFactory>& getRegistry() {
        static std::map<std::string, RelicFactory> registry;
        return registry;
    }
};

#define AUTO_REGISTER_RELIC(className)                          \
    const bool className##Registered = []() {                  \
        RelicRegistry::registerRelic(#className, []() {          \
           return std::make_shared<className>();               \
        });                                                    \
        return true;                                           \
    }();

// buff注册，方法同上
class BuffRegistry {
public:
    using BuffFactory = std::function<std::shared_ptr<Buff>()>;
    static void registerBuff(const std::string& name, BuffFactory factory) {
        getRegistry()[name] = factory;
    }

    static std::shared_ptr<Buff> createBuff(const std::string& name) {
        auto& registry = getRegistry();
        auto it = registry.find(name);
        if (it != registry.end()) {
            return it->second();
        }
        throw std::runtime_error("Buff not found: " + name);
    }

    static std::vector<std::string> getAllBuffNames() {
        std::vector<std::string> names;
        for (const auto& pair : getRegistry()) {
            names.push_back(pair.first);
        }
        return names;
    }

private:
    static std::map<std::string, BuffFactory>& getRegistry() {
        static std::map<std::string, BuffFactory> registry;
        return registry;
    }
};

#define AUTO_REGISTER_BUFF(className)                          \
    const bool className##Registered = []() {                  \
        RelicRegistry::registerBuff(#className, []() {          \
           return std::make_shared<className>();               \
        });                                                    \
        return true;                                           \
    }();

// 药水注册，效果同上
class PotionRegistry {
public:
    using PotionFactory = std::function<std::shared_ptr<Potion>()>;
    static void registerPotion(const std::string& name, PotionFactory factory) {
        getRegistry()[name] = factory;
    }

    static std::shared_ptr<Potion> createPotion(const std::string& name) {
        auto& registry = getRegistry();
        auto it = registry.find(name);
        if (it != registry.end()) {
            return it->second();
        }
        throw std::runtime_error("Potion not found: " + name);
    }

    static std::vector<std::string> getAllPotionNames() {
        std::vector<std::string> names;
        for (const auto& pair : getRegistry()) {
            names.push_back(pair.first);
        }
        return names;
    }

private:
    static std::map<std::string, PotionFactory>& getRegistry() {
        static std::map<std::string, PotionFactory> registry;
        return registry;
    }
};

#define AUTO_REGISTER_POTION(className)                        \
    const bool className##Registered = []() {                  \
        PotionRegistry::registerPotion(#className, []() {      \
           return std::make_shared<className>();               \
        });                                                    \
        return true;                                           \
    }();


// 怪物注册，效果同上
class MonsterRegistry {
public:
    using MonsterFactory = std::function<std::shared_ptr<Monster>()>;
    static void registerMonster(const std::string& name, MonsterFactory factory) {
        getRegistry()[name] = factory;
    }

    static std::shared_ptr<Monster> createMonster(const std::string& name) {
        auto& registry = getRegistry();
        auto it = registry.find(name);
        if (it != registry.end()) {
            return it->second();
        }
        throw std::runtime_error("Monster not found: " + name);
    }

    static std::vector<std::string> getAllMonsterNames() {
        std::vector<std::string> names;
        for (const auto& pair : getRegistry()) {
            names.push_back(pair.first);
        }
        return names;
    }

private:
    static std::map<std::string, MonsterFactory>& getRegistry() {
        static std::map<std::string, MonsterFactory> registry;
        return registry;
    }
};

#define AUTO_REGISTER_MONSTER(className)                       \
    const bool className##Registered = []() {                  \
        MonsterRegistry::registerMonster(#className, []() {    \
           return std::make_shared<className>();               \
        });                                                    \
        return true;                                           \
    }();