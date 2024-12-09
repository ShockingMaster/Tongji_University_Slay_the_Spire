#pragma once
#include <queue>
#include <vector>
#include <memory>
#include <iostream>
#include <map>
#include <functional>
class Creature;
class Card;
class Relic;
class CardRegistry {
public:
    using CardFactory = std::function<std::shared_ptr<Card>()>;
    static void registerCard(const std::string& name, CardFactory factory) {
        getRegistry()[name] = factory;
    }

    static std::shared_ptr<Card> createCard(const std::string& name) {
        auto& registry = getRegistry();
        auto it = registry.find(name);
        if (it != registry.end()) {
            return it->second();
        }
        throw std::runtime_error("Card not found: " + name);
    }

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

#define AUTO_REGISTER_CARD(className)                          \
    const bool className##Registered = []() {                  \
        CardRegistry::registerCard(#className, []() {          \
           return std::make_shared<className>();               \
        });                                                    \
        return true;                                           \
    }();





class RelicRegistry {
public:
    using RelicFactory = std::function<std::shared_ptr<Relic>()>;

    // 注册遗物
    static void registerRelic(const std::string& name, RelicFactory factory) {
        getRegistry()[name] = factory;
    }

    // 创建遗物
    static std::shared_ptr<Relic> createRelic(const std::string& name) {
        auto& registry = getRegistry();
        auto it = registry.find(name);
        if (it != registry.end()) {
            return it->second();
        }
        throw std::runtime_error("Relic not found: " + name);
    }

    // 获取所有注册的遗物名称
    static std::vector<std::string> getAllRelicNames() {
        std::vector<std::string> names;
        for (const auto& pair : getRegistry()) {
            names.push_back(pair.first);
        }
        return names;
    }

private:
    // 获取静态注册表
    static std::map<std::string, RelicFactory>& getRegistry() {
        static std::map<std::string, RelicFactory> registry;
        return registry;
    }
};







#define AUTO_REGISTER_RELIC(className)                           \
    const bool className##Registered = []() {                     \
        RelicRegistry::registerRelic(#className, []() {           \
           return std::make_shared<className>();                  \
        });                                                       \
        return true;                                              \
    }();
    