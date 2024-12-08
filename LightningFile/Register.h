#pragma once
#include <queue>
#include <vector>
#include <memory>
#include <iostream>
#include <map>
#include <functional>
class Creature;
class Card;
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
            return std::make_shared<className>();              \
        });                                                    \
        return true;                                           \
    }();