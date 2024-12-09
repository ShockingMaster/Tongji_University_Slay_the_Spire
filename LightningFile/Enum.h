#pragma once
enum CanBePlayed
{
    NOT_PLAYABLE,
    PLAYABLE
};
enum YesOrNo
{
    NO,
    YES
};

enum CardRarity
{
    COMMON,
    UNCOMMON,
    RARE
};
enum CardType
{
    ABILITY,
    ATTACK,
    SKILL
};

#define AUTO_REGISTER_CARD(className)                          \
    const bool className##Registered = []() {                  \
        CardRegistry::registerCard(#className, []() {          \
            return std::make_shared<className>();              \
        });                                                    \
        return true;                                           \
    }();
