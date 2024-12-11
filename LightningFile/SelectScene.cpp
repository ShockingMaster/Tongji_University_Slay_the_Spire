#include "IncludeAll.h"

bool SelectScene::init()
{
    // ���������ñ���ͼ��
    auto background = cocos2d::Sprite::create("combatScene.png");
    const cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();
    if (background)
    {
        background->setContentSize(Size(1.1 * screenSize.width, 1.5 * screenSize.height)); // ���ñ�����С
        background->setPosition(cocos2d::Vec2(screenSize.width / 2 + 0.05 * screenSize.width,
            screenSize.height / 2 - 0.25 * screenSize.height));
        background->setColor(Color3B(150, 150, 150)); // ������ɫ�䰵
        this->addChild(background);
    }
    else {
        CCLOG("Can't find: combatScene.png!");
    }

    // ����������־
    auto energyLayer1 = Sprite::create("energyLayer1.png");
    if (energyLayer1) {
        energyLayer1->setContentSize(Size(200, 200));
        energyLayer1->setPosition(cocos2d::Vec2(0.0976525 * screenSize.width, 0.390625 * screenSize.height));
        energyLayer1->setColor(Color3B(100, 100, 100)); // ������ɫ�䰵
        this->addChild(energyLayer1);
    }
    else {
        CCLOG("Can't find: energyLayer1.png!");
    }

    auto energyLayer2 = Sprite::create("energyGreenVFX.png");
    if (energyLayer2) {
        energyLayer2->setContentSize(Size(120, 120));
        energyLayer2->setPosition(cocos2d::Vec2(0.0976525 * screenSize.width, 0.390625 * screenSize.height));
        energyLayer2->setOpacity(128); // ����͸���ȱ䰵
        this->addChild(energyLayer2);
    }
    else {
        CCLOG("Can't find: energyGreenVFX.png!");
    }

    // ������ʾ����������
    energyLabel = cocos2d::Label::createWithSystemFont(std::to_string(0) + "/" + std::to_string(0), "Arial", 24);
    energyLabel->setPosition(cocos2d::Vec2(0.0976525 * screenSize.width, 0.390625 * screenSize.height));
    energyLabel->setColor(cocos2d::Color3B(100, 100, 100)); // ����������ɫ�䰵
    this->addChild(energyLabel);

    // ���������и���
    updateEnergyDisplay();

    // �����л�������ť
    auto switchSceneButton = HoverButton::create("endTurnButton.png", "endTurnButtonGlow.png", "endTurnButton.png");
    switchSceneButton->setTitleText(u8"�л�����");
    switchSceneButton->setScale(1.5f);
    switchSceneButton->setTitleFontSize(20);
    switchSceneButton->setPosition(Vec2(0.906525 * screenSize.width, 0.15625 * screenSize.height)); // ���ð�ťλ��

    // �л�������ť�¼�
    switchSceneButton->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            CCLOG("Switch Scene clicked!");  // ��ӡ��־

            // �ڴ˴�ִ���л������Ĳ���

            auto newScene = CombatScene::create();  // �����³���
            cocos2d::Director::getInstance()->pushScene(newScene);
        }
        });

    // ��Ӱ�ť����ǰ����
    this->addChild(switchSceneButton);




    // ���� HandPileLayer ʵ������ʼ��
    HandPileLayer::getInstance()->init();

    // ����ʹ�ã�����һ���䰵�Ĵ������
    playArea = Rect(screenSize.width / 2 - 0.15 * screenSize.width, screenSize.height / 2,
        0.3 * screenSize.width, 0.3 * screenSize.height);
    auto playAreaNode = DrawNode::create();
    playAreaNode->drawRect(playArea.origin, playArea.origin + playArea.size, Color4F(0.5f, 0.5f, 0.5f, 0.8f)); // ��ɫ����
    this->addChild(playAreaNode);

    this->addChild(HandPileLayer::getInstance());

    return true;
}


/*
* �������ƣ�updateEnergyDisplay
* ���ܣ�����PlayerΨһʵ������������͵�ǰ�������и���
*/
void SelectScene::updateEnergyDisplay()
{
    const int currentEnergy = Player::getInstance()->getCurrentEnergy();
    const int maxEnergy = Player::getInstance()->getMaxEnergy();
    energyLabel->setString(std::to_string(currentEnergy) + "/" + std::to_string(maxEnergy));  // ���±�ǩ�ı�Ϊ��ǰ����ֵ
}