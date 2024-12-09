#include "EnergyBall.h"

USING_NS_CC;

// ���캯��
EnergyBall::EnergyBall() : currentEnergy(0), totalEnergy(100),  energyLabel(nullptr) {}

// ��������
EnergyBall::~EnergyBall() {}

// ���� EnergyBall ʵ��
EnergyBall* EnergyBall::create() {
    EnergyBall* energyBall = new(std::nothrow) EnergyBall();
    if (energyBall && energyBall->init()) {
        energyBall->autorelease();
        return energyBall;
    }
    CC_SAFE_DELETE(energyBall);
    return nullptr;
}

// ��ʼ������
bool EnergyBall::init() {
 
    // ������ͼ����
    auto mainSprite = Sprite::create("Energy.png");
    if (!mainSprite) return false;
    mainSprite->setScale(1.7f);
    mainSprite->setPosition(Vec2(300,-150));
    this->addChild(mainSprite);

    // ��������ֵ��ʾ��ǩ
    energyLabel = Label::createWithTTF("0 / 100", "fonts/Marker Felt.ttf", 38);
    if (!energyLabel) return false;
    energyLabel->setColor(cocos2d::Color3B::BLACK);
    energyLabel->setPosition(Vec2(75,166));
    this->addChild(energyLabel);

    return true;
}

// ���ú͸�������ֵ
void EnergyBall::setEnergy(int current, int total) {
    currentEnergy = current;
    totalEnergy = total;

    // ȷ��ֵ��Խ��
    if (currentEnergy < 0) currentEnergy = 0;
    if (currentEnergy > totalEnergy) currentEnergy = totalEnergy;

    updateEnergyDisplay();
}

// ������ʾ������ֵ
void EnergyBall::updateEnergyDisplay() {
    // ����������ǩ���ı�����
    std::string energyText = StringUtils::format("%d / %d", currentEnergy, totalEnergy);
    energyLabel->setString(energyText);
}
