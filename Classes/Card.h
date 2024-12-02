#pragma once
#include <vector>
#include <string>
using namespace std;

class Creature;

/**
 * @class Card
 * @brief ����һ�ſ��Ƶ���
 *
 * ���ƾ������֡��������������ġ�����۸�ϡ�жȵ����ԣ�
 * ���ҿ��ܻ�����Ե�������Ŀ���Ч����
 */
class Card{
public:
    string name_;             ///< ��������
    string description_;      ///< ��������
    int energyCost_;          ///< ������������
    int moneyCost_;           ///< �����̵깺��۸�
    int rarity_;              ///< ����ϡ�ж�
    bool isEnabled_;          ///< �Ƿ���Դ���˿���
    bool targetNum;           ///< ʩ�ӹ����Ķ�����������������Ŀ�꣩
    int type_;                ///< �������ͣ��繥�������������ȣ�
    int triggerTimes_;        ///< ����Ч����������
    // ���캯��
    Card(string name, string description, int energyCost, int moneyCost, int rarity, bool isEnabled, int type, int triggerTimes);

    // ȡЧ����������Ե���Ŀ��
    void takeEffect(Creature* target);

    // ȡЧ����������Զ��Ŀ��
    void takeEffect(vector<Creature*> targets);

    //�������� ���쿨��
    void update();
};
