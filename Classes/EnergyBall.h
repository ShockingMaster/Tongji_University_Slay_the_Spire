#ifndef __ENERGY_BALL_H__
#define __ENERGY_BALL_H__

#include "cocos2d.h"

class EnergyBall : public cocos2d::Node {
public:
    // 构造函数与析构函数
    EnergyBall();
    ~EnergyBall();

    // 创建 EnergyBall 实例
    static EnergyBall* create();

    // 设置和更新能量值
    void setEnergy(int current, int total);
    void updateEnergyDisplay();

protected:
    // 初始化方法
    bool init();

private:
    // 成员变量
    int currentEnergy;              // 当前能量
    int totalEnergy;                // 总能量

    cocos2d::Label* energyLabel;    // 显示能量值的标签
};

#endif // __ENERGY_BALL_H__
