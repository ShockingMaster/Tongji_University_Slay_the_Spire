#pragma once
enum YesOrNo
{
    YES,
    NO
};
//��һ��TriggerTypeö�٣���ʾbuff�Ĵ���ʱ��
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