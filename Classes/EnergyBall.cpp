#include "EnergyBall.h"

USING_NS_CC;

// 构造函数
EnergyBall::EnergyBall() : currentEnergy(0), totalEnergy(100),  energyLabel(nullptr) {}

// 析构函数
EnergyBall::~EnergyBall() {}

// 创建 EnergyBall 实例
EnergyBall* EnergyBall::create() {
    EnergyBall* energyBall = new(std::nothrow) EnergyBall();
    if (energyBall && energyBall->init()) {
        energyBall->autorelease();
        return energyBall;
    }
    CC_SAFE_DELETE(energyBall);
    return nullptr;
}

// 初始化方法
bool EnergyBall::init() {
 
    // 创建主图像精灵
    auto mainSprite = Sprite::create("Energy.png");
    if (!mainSprite) return false;
    mainSprite->setScale(1.7f);
    mainSprite->setPosition(Vec2(300,-150));
    this->addChild(mainSprite);

    // 创建能量值显示标签
    energyLabel = Label::createWithTTF("0 / 100", "fonts/Marker Felt.ttf", 38);
    if (!energyLabel) return false;
    energyLabel->setColor(cocos2d::Color3B::BLACK);
    energyLabel->setPosition(Vec2(75,166));
    this->addChild(energyLabel);

    return true;
}

// 设置和更新能量值
void EnergyBall::setEnergy(int current, int total) {
    currentEnergy = current;
    totalEnergy = total;

    // 确保值不越界
    if (currentEnergy < 0) currentEnergy = 0;
    if (currentEnergy > totalEnergy) currentEnergy = totalEnergy;

    updateEnergyDisplay();
}

// 更新显示的能量值
void EnergyBall::updateEnergyDisplay() {
    // 更新能量标签的文本内容
    std::string energyText = StringUtils::format("%d / %d", currentEnergy, totalEnergy);
    energyLabel->setString(energyText);
}
