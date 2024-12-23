
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
enum stackType
{
    DURATION,
    EFFECT_LAYERS
};
enum Enemy
{
    NORMAL,
    ELITE,
    BOSS
};

enum CardRarity
{
    COMMON,
    UNCOMMON,
    RARE,
    STATUS,
    ALL
};
enum CardType
{
    ABILITY,
    ATTACK,
    SKILL,
};

#define AUTO_REGISTER_CARD(className)                          \
    const bool className##Registered = []() {                  \
        CardRegistry::registerCard(#className, []() {          \
            return std::make_shared<className>();              \
        });                                                    \
        return true;                                           \
    }();


#define AUTO_REGISTER_BUFF(className)                          \
    const bool className##Registered = []() {                  \
        BuffRegistry::registerBuff(#className, []() {          \
            return std::make_shared<className>();              \
        });                                                    \
        return true;                                           \
    }();

#define AUTO_REGISTER_RELIC(className)                          \
    const bool className##Registered = []() {                  \
        RelicRegistry::registerRelic(#className, []() {          \
           return std::make_shared<className>();               \
        });                                                    \
        return true;                                           \
    }();

#define AUTO_REGISTER_POTION(className)                        \
    const bool className##Registered = []() {                  \
        PotionRegistry::registerPotion(#className, []() {      \
           return std::make_shared<className>();               \
        });                                                    \
        return true;                                           \
    }();

#define AUTO_REGISTER_MONSTER(className)                       \
    const bool className##Registered = []() {                  \
        MonsterRegistry::registerMonster(#className, []() {    \
           return std::make_shared<className>();               \
        });                                                    \
        return true;                                           \
    }();