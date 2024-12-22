#include "ShopScene.h"
#include "AudioPlayer.h"
#include "NodeConnection.h"
#include "Player.h"
#include "CardLayer.h"
#include "HeaderBar.h"

// ��������
cocos2d::Scene* ShopScene::createScene() {
    auto scene = Scene::create();          // ����һ���հ׳���
    auto layer = ShopScene::create();     
    scene->addChild(layer);                // ������ӵ�����
    return scene;
}


bool ShopScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // ���ű�����Ч
    audioPlayer("event.ogg", true);
    auto player = EventSystem::getInstance();
    auto headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 750));          // ����λ�ã�����Ļ�ϲ���
    this->addChild(headbar);
    headbar->setLocalZOrder(2000100);
    // ����ͼ��
    const auto background = Sprite::create("walkscene.jpg");
    background->setPosition(Vec2(1100, 100));
    this->addChild(background);

    // ��ӭ��ǩ
    auto continueLabel = Label::createWithSystemFont(u8"����ӭ�����̵꣡��\nһ������ߵ�����\n�����������ǰ", "Fonts/Kreon-Bold.ttf", 65);
    continueLabel->setPosition(Vec2(600, 600));
    continueLabel->setColor(Color3B::WHITE);
    this->addChild(continueLabel);

    // ���˱���ͼ��
    const auto background0 = Sprite::create("merchant-removebg-preview.png");
    background0->setScale(1.0f);
    background0->setOpacity(200);
    background0->setPosition(Vec2(1500, 400));
    this->addChild(background0);

    // ʹ�� DelayTime �������ӳ�
    auto delay = DelayTime::create(0.5f); 

    // ����һ���ص����������ӳ�ʱ���ִ��
    auto showLayerCallback = CallFunc::create([this]() {
        static int deleteprice = 75;
        // �����²㣬�����ñ���ͼ
        auto newLayer = Sprite::create("fin.png");
        newLayer->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2 -10));
        newLayer->setScale(0.9f);
        this->addChild(newLayer,1); // 100�ǲ㼶��ȷ��������Ԫ��֮��
        auto player = EventSystem::getInstance();
        auto headbar = HeaderBar::create(player);
        headbar->setPosition(Vec2(0, 750));          // ����λ�ã�����Ļ�ϲ���
        this->addChild(headbar);
        headbar->setLocalZOrder(2000100);
        auto randomCard1 = RandomGenerator::getInstance()->getRandomCard(ATTACK);
        auto card1 = CardSpriteGenerator::createCardSprite(randomCard1);
        card1->setScale(1.2f);
        card1->setPosition(Vec2(500, 600));
        this->addChild(card1, 2000);
        auto label1= Label::createWithSystemFont(to_string(randomCard1->getMoneyCost()), "Fonts/Kreon-Bold.ttf", 45);
        label1->setPosition(Vec2(500, 400));
        this->addChild(label1, 2000);
        auto randomCard2 = RandomGenerator::getInstance()->getRandomCard(ATTACK);
        auto card2 = CardSpriteGenerator::createCardSprite(randomCard2);
        card2->setScale(1.2f);
        card2->setPosition(Vec2(800, 600));
        this->addChild(card2, 2000);
        auto label2 = Label::createWithSystemFont(to_string(randomCard2->getMoneyCost()), "Fonts/Kreon-Bold.ttf", 45);
        label2->setPosition(Vec2(800, 400));
        this->addChild(label2, 2000);

        auto randomCard3 = RandomGenerator::getInstance()->getRandomCard(SKILL);
        auto card3 = CardSpriteGenerator::createCardSprite(randomCard3);
        card3->setScale(1.2f);
        card3->setPosition(Vec2(1100, 600));
        this->addChild(card3, 2000);
        auto label3 = Label::createWithSystemFont(to_string(randomCard3->getMoneyCost()), "Fonts/Kreon-Bold.ttf", 45);
        label3->setPosition(Vec2(1100, 400));
        this->addChild(label3, 2000);

        auto randomCard4 = RandomGenerator::getInstance()->getRandomCard(SKILL);
        auto card4 = CardSpriteGenerator::createCardSprite(randomCard4);
        card4->setScale(1.2f);
        card4->setPosition(Vec2(1400, 600));
        this->addChild(card4, 2000);
        auto label4 = Label::createWithSystemFont(to_string(randomCard4->getMoneyCost()), "Fonts/Kreon-Bold.ttf", 45);
        label4->setPosition(Vec2(1400, 400));
        this->addChild(label4, 2000);

        auto randomCard5 = RandomGenerator::getInstance()->getRandomCard(SKILL);
        auto card5 = CardSpriteGenerator::createCardSprite(randomCard5);
        card5->setScale(1.2f);
        card5->setPosition(Vec2(500, 200));
        this->addChild(card5, 2000);
        auto label5 = Label::createWithSystemFont(to_string(randomCard5->getMoneyCost()), "Fonts/Kreon-Bold.ttf", 45);
        label5->setPosition(Vec2(500, 20));
        this->addChild(label5, 2000);

        auto randomCard6 = RandomGenerator::getInstance()->getRandomCard(SKILL);
        auto card6 = CardSpriteGenerator::createCardSprite(randomCard6);
        card6->setScale(1.2f);
        card6->setPosition(Vec2(800, 200));
        this->addChild(card6, 2000);
        auto label6 = Label::createWithSystemFont(to_string(randomCard6->getMoneyCost()), "Fonts/Kreon-Bold.ttf", 45);
        label6->setPosition(Vec2(800, 20));
        this->addChild(label6, 2000);

        auto deleteCardService = Sprite::create("zhs.png");
        deleteCardService->setPosition(Vec2(1400, 200));
        deleteCardService->setScale(0.8f);
        this->addChild(deleteCardService, 2000);
        auto label7 = Label::createWithSystemFont(to_string(deleteprice), "Fonts/Kreon-Bold.ttf", 45);
        label7->setPosition(Vec2(1400, 20));
        this->addChild(label7, 2000);

        // ���������¼�������
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);  // ��ֹ�����¼�����

        // ͳһ�������¼�
        listener->onTouchBegan = [=](Touch* touch, Event* event) {
            Vec2 touchLocation = touch->getLocation();

            // �жϵ���Ƿ�����card1��
            if (card1->getBoundingBox().containsPoint(touchLocation)&& randomCard1->getMoneyCost()< EventSystem::getInstance()->getCoins()) {
                CCLOG("Card 1 clicked!");
                // ִ��card1������߼�
                audioPlayer("gold.ogg", false);
                EventSystem::getInstance()->changeCoins(-randomCard1->getMoneyCost());
                EventSystem::getInstance()->addCard(randomCard1);
                headbar->updateHeader(EventSystem::getInstance());
                return true;
            }

            // �жϵ���Ƿ�����card2��
            if (card2->getBoundingBox().containsPoint(touchLocation)&&randomCard2->getMoneyCost() < EventSystem::getInstance()->getCoins()) {
                CCLOG("Card 2 clicked!");
                // ִ��card2������߼�
                audioPlayer("gold.ogg", false);
                EventSystem::getInstance()->changeCoins(-randomCard2->getMoneyCost());
                EventSystem::getInstance()->addCard(randomCard2);
                headbar->updateHeader(EventSystem::getInstance());
                return true;
            }

            // �жϵ���Ƿ�����card3��
            if (card3->getBoundingBox().containsPoint(touchLocation)&&randomCard3->getMoneyCost() < EventSystem::getInstance()->getCoins()) {
                CCLOG("Card 3 clicked!");
                audioPlayer("gold.ogg", false);
                EventSystem::getInstance()->changeCoins(-randomCard3->getMoneyCost());
                EventSystem::getInstance()->addCard(randomCard3);
                headbar->updateHeader(EventSystem::getInstance());
                
                return true;
            }

            // �жϵ���Ƿ�����card4��
            if (card4->getBoundingBox().containsPoint(touchLocation)&&randomCard4->getMoneyCost() < EventSystem::getInstance()->getCoins()) {
                CCLOG("Card 4 clicked!");
                audioPlayer("gold.ogg", false);
                EventSystem::getInstance()->changeCoins(-randomCard4->getMoneyCost());
                EventSystem::getInstance()->addCard(randomCard4);
                headbar->updateHeader(EventSystem::getInstance());
                
               
                return true;
            }

            // �жϵ���Ƿ�����card5��
            if (card5->getBoundingBox().containsPoint(touchLocation)&& randomCard5->getMoneyCost() < EventSystem::getInstance()->getCoins()) {
                CCLOG("Card 5 clicked!");
                audioPlayer("gold.ogg", false);
                EventSystem::getInstance()->changeCoins(-randomCard5->getMoneyCost());
                EventSystem::getInstance()->addCard(randomCard5);
                headbar->updateHeader(EventSystem::getInstance());
                return true;
            }

            if (card6->getBoundingBox().containsPoint(touchLocation)&&randomCard6->getMoneyCost() < EventSystem::getInstance()->getCoins()) {
                CCLOG("Card 6 clicked!");
                audioPlayer("gold.ogg", false);
                EventSystem::getInstance()->changeCoins(-randomCard6->getMoneyCost());
                EventSystem::getInstance()->addCard(randomCard6);
                headbar->updateHeader(EventSystem::getInstance());
                return true;
            }

            // �жϵ���Ƿ�����deleteCardService��
            if (deleteCardService->getBoundingBox().containsPoint(touchLocation)&& deleteprice < EventSystem::getInstance()->getCoins()) {
                CCLOG("Delete Card Service clicked!");
                // ִ��ɾ����Ƭ����ť������߼�
                auto player = EventSystem::getInstance();
                EventSystem::getInstance()->changeCoins(-deleteprice);
                deleteprice += 25;

                // �ӳ�0.5��
                auto delay = DelayTime::create(0.35f);
                auto callFunc = CallFunc::create([=]() {
                    // �ӳ�ִ�еĲ���
                    auto cardLayer = CardLayer::create(player->cards_, 2);
                    Director::getInstance()->getRunningScene()->addChild(cardLayer);
                    });

                // ���ӳٺͻص�����һ��˳��ִ��
                auto sequence = Sequence::create(delay, callFunc, nullptr);
                Director::getInstance()->getRunningScene()->runAction(sequence);

                return true;
            }

            return false;
            };

        // ��Ӽ��������¼��ַ���
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


        // �������ذ�ť
        auto backButton = MenuItemImage::create(
            "cancelButton.png",    // ��ͨ״̬ͼƬ
            "cancelButtonOutline.png",   // ����״̬ͼƬ
            [this](Ref* sender) { // �����ťʱִ��
                // ���ص���һ������
                Director::getInstance()->popScene();  // ������ǰ����������ǰһ������
            }
        );

        // ���ð�ťλ��
        backButton->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 60, 100)); 

        // �����˵�����Ӱ�ť
        auto menu = Menu::create(backButton, nullptr);
        menu->setPosition(Vec2::ZERO);  // �˵���ê������Ϊ (0, 0)
        newLayer->addChild(menu);  // ����ť��ӵ� newLayer
       

        });


    // ����һ��˳����
    auto sequence = Sequence::create(delay, showLayerCallback, nullptr);

    // ִ�ж���
    this->runAction(sequence);

    return true;
}
