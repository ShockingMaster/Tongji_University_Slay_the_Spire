#ifndef __ENERGY_BALL_H__
#define __ENERGY_BALL_H__

#include "cocos2d.h"

class EnergyBall : public cocos2d::Node {
public:
    // ���캯������������
    EnergyBall();
    ~EnergyBall();

    // ���� EnergyBall ʵ��
    static EnergyBall* create();

    // ���ú͸�������ֵ
    void setEnergy(int current, int total);
    void updateEnergyDisplay();

protected:
    // ��ʼ������
    bool init();

private:
    // ��Ա����
    int currentEnergy;              // ��ǰ����
    int totalEnergy;                // ������

    cocos2d::Label* energyLabel;    // ��ʾ����ֵ�ı�ǩ
};

#endif // __ENERGY_BALL_H__
