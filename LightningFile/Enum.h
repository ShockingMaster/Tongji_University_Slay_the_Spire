#pragma once
enum CanBePlayed
{
    PLAYABLE,
    NOT_PLAYABLE
};
enum YesOrNo
{
    YES,
    NO
};
//给一个TriggerType枚举，表示buff的触发时机
enum TriggerType
{
    ON_ATTACK,
    ON_DEFENSE,
    ON_TURN_START,
    ON_TURN_END,
    SHUFFLE,
    TAKE_DAMAGE,
    HEALTH_LOSS
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
