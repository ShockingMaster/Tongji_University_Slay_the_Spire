#include"IncludeAll.h"
#include "TestScene.h"

bool TestScene::init()
{
    // ����һ����ť�������л���ս������
    auto switchButton = cocos2d::MenuItemLabel::create(
        cocos2d::Label::createWithTTF("Go to Combat Scene", "fonts/arial.ttf", 30),
        CC_CALLBACK_1(TestScene::switchToCombatScene, this)
    );

    switchButton->setPosition(cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width / 2,
        cocos2d::Director::getInstance()->getVisibleSize().height / 2));

    // �����˵�
    auto menu = cocos2d::Menu::create(switchButton, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu);

    return true;
}

void TestScene::switchToCombatScene(cocos2d::Ref* sender)
{

    CombatSystem::getInstance()->init();
    // ���� CombatScene ��ʹ�� pushScene �����л�
    auto combatScene = CombatScene::create();
    cocos2d::Director::getInstance()->pushScene(combatScene);  // �л��� CombatScene
}