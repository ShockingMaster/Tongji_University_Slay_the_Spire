#pragma once
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