#include "RewardLayer.h"
#include "cocos2d.h"
#include "Card.h"
#include "Relic.h"
#include "Potion.h"
#include "HeaderBar.h"
#include "HoverButton.h"
USING_NS_CC;

RewardLayer::RewardLayer()
{
}

RewardLayer::~RewardLayer()
{
}

RewardLayer* RewardLayer::create()
{
    RewardLayer* ret = new RewardLayer();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        return nullptr;
    }
}

bool RewardLayer::init()
{
    // �ж��Ƿ�ɹ���ʼ��
    if (!Layer::init())
    {
        return false;
    }

    // ���ñ���
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ����ͼ��
    auto background = Sprite::create("rewardScreenSheet.png");
    background->setScale(1.2f);
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 300));
    this->addChild(background);
   
    auto background1 = Sprite::create("rewardListItemPanel.png");
    background1->setScale(1.2f);
    background1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 500));
    this->addChild(background1);

    auto background4 = Sprite::create("gold.png");
    background4->setScale(2.0f);
    background4->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 500));
    this->addChild(background4, 2000);

    auto background2 = Sprite::create("rewardListItemPanel.png");
    background2->setScale(1.2f);
    background2->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 300));
    this->addChild(background2);

    auto background5 = Sprite::create("bossCardReward.png");
    background5->setScale(2.0f);
    background5->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 300));
    this->addChild(background5, 2000);

    auto background3 = Sprite::create("rewardListItemPanel.png");
    background3->setScale(1.2f);
    background3->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 100));
    this->addChild(background3);

    auto background6 = Sprite::create("relic_link.png");
    background6->setScale(2.0f);
    background6->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 100));
    this->addChild(background6, 2000);

    auto label = cocos2d::Label::createWithSystemFont(u8" ˫�����ձ���", "Arial", 60);
    label->setPosition(1000,300);  // ���ñ�ǩλ��
    label->setTextColor(cocos2d::Color4B::WHITE);  // ����������ɫΪ��ɫ
    this->addChild(label,1000);
    // �����رհ�ť
    auto closeButton = MenuItemImage::create(
        "takeAll.png",         // ����״̬
        "takeAllUsed.png",     // �����״̬
        CC_CALLBACK_1(RewardLayer::onCloseButtonClicked, this));  // ��ť����ص�

    // ȷ����ť����Ļ�ɼ������ڣ���������ȷ��λ��
    closeButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 -200));

    // �����˵�����Ӱ�ť
    auto menu = Menu::create(closeButton, NULL);
    menu->setPosition(Vec2::ZERO);  // ���ò˵���λ��Ϊ (0, 0)

    // ���˵���ӵ���
    this->addChild(menu); // ע�������1��ȷ����������UIԪ���Ϸ�
    // ��Ӵ����¼�������
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(RewardLayer::onTouchBegan, this);
    listener->setSwallowTouches(true);  // ȷ�������¼����ݸ�������
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}




void RewardLayer::onCloseButtonClicked(Ref* sender)
{
    this->removeFromParentAndCleanup(true);
}
