#pragma once
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Buff;

class Creature {
public:
    string name_;               // ��������
    string description_;        // ��������
    int health_;                // ��ǰ����ֵ
    int fullhealth_;            // �������ֵ
    int block_;                 // ����ֵ�����ٲ����˺�
    int can_end_turn_;          // �Ƿ���Խ����غϣ���������ĳЩ״̬Ч���жϣ�
    vector<Buff*> buffs_;       // �洢���ﵱǰ���е�����/����Ч��

    void updateHealth();        // �������������ֵ
    void updateBuff();          // �������������/����Ч��
    void updateblock();         // ��������Ļ���ֵ
    void addBuff(Buff* buff);   // Ϊ�������һ���µ� Buff Ч��
    void takeDamage(int numeric_value);  // �ܵ��˺�ʱ��Ч���ж�
    void healthLoss(int numeric_value);  // ���㲢Ӧ���˺����������ʧ
    void modifyValue(Creature* target, int TriggerType, int& numeric_value); // ���������ֵ�����޸ĵ��ж�
    void buffSettle(Creature* target, int TriggerType); // ��������� Buff ���㣬���漰��ֵ�޸�
    void show_info();           // ��ʾ����Ļ�����Ϣ
    void take_damage(int damage);  // �ܵ��˺�ʱ�Ĵ�����
    virtual void start_turn() = 0; // ��ʼ�غϵĴ��������麯����
    virtual void end_turn() = 0;   // �����غϵĴ��������麯����
};
