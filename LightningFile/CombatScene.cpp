#include "IncludeAll.h"

bool CombatScene::init() 
{
    // ���������ñ���ͼ��
    auto background = cocos2d::Sprite::create("combatScene.png");
    const cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();
    if (background) 
    {
        background->setContentSize(Size(1.1 * screenSize.width, 1.5 * screenSize.height)); // ���ñ�����С
        background->setPosition(cocos2d::Vec2(screenSize.width / 2 + 0.05 * screenSize.width, 
            screenSize.height / 2 - 0.25 * screenSize.height));
        this->addChild(background);
    }
    else{
        CCLOG("Can't find: combatScene.png!");
    }
    //����������־
    auto energyLayer1 = Sprite::create("energyLayer1.png");
    if (energyLayer1) {
        energyLayer1->setContentSize(Size(200, 200));
        energyLayer1->setPosition(cocos2d::Vec2(0.0976525 * screenSize.width, 0.390625 * screenSize.height));
        this->addChild(energyLayer1);
    }
    else {
        CCLOG("Can't find: energyLayer1.png!");
    }
    auto energyLayer2 = Sprite::create("energyGreenVFX.png");
    if (energyLayer2) {
        energyLayer2->setContentSize(Size(120, 120));
        energyLayer2->setPosition(cocos2d::Vec2(0.0976525 * screenSize.width, 0.390625 * screenSize.height));
        this->addChild(energyLayer2);
    }
    else {
        CCLOG("Can't find: energyGreenVFX.png!");
    }

    // ������ʾ����������
    energyLabel = cocos2d::Label::createWithSystemFont(std::to_string(0) + "/" + std::to_string(0), "Arial", 24);
    energyLabel->setPosition(cocos2d::Vec2(0.0976525 * screenSize.width, 0.390625 * screenSize.height));  // ����������ͼ�������
    energyLabel->setColor(cocos2d::Color3B::BLACK);  // ����������ɫ
    this->addChild(energyLabel);

    // ���������и���
    updateEnergyDisplay();

    //ս����ʼʱ�����趨��Ϊ�ҷ��غ�
    isMyTurn = 0;

    //�����غϽ�����ť,������غϽ�����ť֮�󣬶������еĿ���
    auto endTurnButton = HoverButton::create("endTurnButton.png", "endTurnButtonGlow.png", "endTurnButton.png");
    endTurnButton->setTitleText(u8"�غϽ���");
    endTurnButton->setScale(1.5f);
    endTurnButton->setTitleFontSize(20);
    endTurnButton->setPosition(Vec2(0.906525 * screenSize.width, 0.25625 * screenSize.height));

    //����ʹ�ã��غϿ�ʼ��ť��������غϿ�ʼ��ť֮�󣬿��Գ�5����
    auto startTurnButton = HoverButton::create("endTurnButton.png", "endTurnButtonGlow.png", "endTurnButton.png");
    startTurnButton->setTitleText(u8"�غϿ�ʼ");
    startTurnButton->setScale(1.5f);
    startTurnButton->setTitleFontSize(20);
    startTurnButton->setPosition(Vec2(0.0976525 * screenSize.width, 0.25625 * screenSize.height));
    
    // �����غϰ�ť
    endTurnButton->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            if(isMyTurn)
            {
                CCLOG("End Turn clicked!");  // ��ӡ��־
                CombatSystem::getInstance()->endTurn(Player::getInstance());//ִ����һغϽ���Ч��
            }
            isMyTurn = 0;
        }
        });

    // ��ʼ�غϰ�ť
    startTurnButton->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
            if (!isMyTurn)
            {
                CCLOG("Start Turn clicked!");  // ��ӡ��־
                CombatSystem::getInstance()->startTurn(Player::getInstance());// ִ����һغϿ�ʼЧ��
            }
            isMyTurn = 1;
        }
        });
    this->addChild(endTurnButton);
    this->addChild(startTurnButton);


    HandPileLayer::getInstance()->init();
    //����ʹ�ã�����һ���ܴ���Ƶ����򣬵����Ʊ��϶����������ʱ�����
    playArea = Rect(screenSize.width / 2 - 0.15 * screenSize.width, screenSize.height / 2, 
        0.3 * screenSize.width, 0.3 * screenSize.height); // ���ô������
    auto playAreaNode = DrawNode::create();
    playAreaNode->drawRect(playArea.origin, playArea.origin + playArea.size, Color4F::GRAY);
    this->addChild(playAreaNode);

    this->addChild(HandPileLayer::getInstance());

    return true;
}

/*
* �������ƣ�updateEnergyDisplay
* ���ܣ�����PlayerΨһʵ������������͵�ǰ�������и���
*/
void CombatScene::updateEnergyDisplay()
{
    const int currentEnergy = Player::getInstance()->getCurrentEnergy();
    const int maxEnergy = Player::getInstance()->getMaxEnergy();
    energyLabel->setString(std::to_string(currentEnergy) + "/" + std::to_string(maxEnergy));  // ���±�ǩ�ı�Ϊ��ǰ����ֵ
}

