#include "ShopScene.h"
#include "AudioPlayer.h"
#include "NodeConnection.h"
#include "Player.h"
#include "CardLayer.h"
#include "HeaderBar.h"

// 创建场景
cocos2d::Scene* ShopScene::createScene() {
    auto scene = Scene::create();          // 创建一个空白场景
    auto layer = ShopScene::create();     
    scene->addChild(layer);                // 将层添加到场景
    return scene;
}


bool ShopScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // 播放背景音效
    audioPlayer("event.ogg", true);
    auto player = EventSystem::getInstance();
    auto headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 750));          // 设置位置（在屏幕上部）
    this->addChild(headbar);
    headbar->setLocalZOrder(2000100);
    // 背景图像
    const auto background = Sprite::create("walkscene.jpg");
    background->setPosition(Vec2(1100, 100));
    this->addChild(background);

    // 欢迎标签
    auto continueLabel = Label::createWithSystemFont(u8"“欢迎来到商店！”\n一个戴面具的商人\n出现在你的面前", "Fonts/Kreon-Bold.ttf", 65);
    continueLabel->setPosition(Vec2(600, 600));
    continueLabel->setColor(Color3B::WHITE);
    this->addChild(continueLabel);

    // 商人背景图像
    const auto background0 = Sprite::create("merchant-removebg-preview.png");
    background0->setScale(1.0f);
    background0->setOpacity(200);
    background0->setPosition(Vec2(1500, 400));
    this->addChild(background0);

    // 使用 DelayTime 来创建延迟
    auto delay = DelayTime::create(0.5f); 

    // 创建一个回调函数，在延迟时间后执行
    auto showLayerCallback = CallFunc::create([this]() {
        static int deleteprice = 75;
        // 创建新层，并设置背景图
        auto newLayer = Sprite::create("fin.png");
        newLayer->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2 -10));
        newLayer->setScale(0.9f);
        this->addChild(newLayer,1); // 100是层级，确保在其他元素之上
        auto player = EventSystem::getInstance();
        auto headbar = HeaderBar::create(player);
        headbar->setPosition(Vec2(0, 750));          // 设置位置（在屏幕上部）
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

        // 创建触摸事件监听器
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);  // 防止触摸事件传播

        // 统一处理触摸事件
        listener->onTouchBegan = [=](Touch* touch, Event* event) {
            Vec2 touchLocation = touch->getLocation();

            // 判断点击是否发生在card1上
            if (card1->getBoundingBox().containsPoint(touchLocation)&& randomCard1->getMoneyCost()< EventSystem::getInstance()->getCoins()) {
                CCLOG("Card 1 clicked!");
                // 执行card1的相关逻辑
                audioPlayer("gold.ogg", false);
                EventSystem::getInstance()->changeCoins(-randomCard1->getMoneyCost());
                EventSystem::getInstance()->addCard(randomCard1);
                headbar->updateHeader(EventSystem::getInstance());
                return true;
            }

            // 判断点击是否发生在card2上
            if (card2->getBoundingBox().containsPoint(touchLocation)&&randomCard2->getMoneyCost() < EventSystem::getInstance()->getCoins()) {
                CCLOG("Card 2 clicked!");
                // 执行card2的相关逻辑
                audioPlayer("gold.ogg", false);
                EventSystem::getInstance()->changeCoins(-randomCard2->getMoneyCost());
                EventSystem::getInstance()->addCard(randomCard2);
                headbar->updateHeader(EventSystem::getInstance());
                return true;
            }

            // 判断点击是否发生在card3上
            if (card3->getBoundingBox().containsPoint(touchLocation)&&randomCard3->getMoneyCost() < EventSystem::getInstance()->getCoins()) {
                CCLOG("Card 3 clicked!");
                audioPlayer("gold.ogg", false);
                EventSystem::getInstance()->changeCoins(-randomCard3->getMoneyCost());
                EventSystem::getInstance()->addCard(randomCard3);
                headbar->updateHeader(EventSystem::getInstance());
                
                return true;
            }

            // 判断点击是否发生在card4上
            if (card4->getBoundingBox().containsPoint(touchLocation)&&randomCard4->getMoneyCost() < EventSystem::getInstance()->getCoins()) {
                CCLOG("Card 4 clicked!");
                audioPlayer("gold.ogg", false);
                EventSystem::getInstance()->changeCoins(-randomCard4->getMoneyCost());
                EventSystem::getInstance()->addCard(randomCard4);
                headbar->updateHeader(EventSystem::getInstance());
                
               
                return true;
            }

            // 判断点击是否发生在card5上
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

            // 判断点击是否发生在deleteCardService上
            if (deleteCardService->getBoundingBox().containsPoint(touchLocation)&& deleteprice < EventSystem::getInstance()->getCoins()) {
                CCLOG("Delete Card Service clicked!");
                // 执行删除卡片服务按钮的相关逻辑
                auto player = EventSystem::getInstance();
                EventSystem::getInstance()->changeCoins(-deleteprice);
                deleteprice += 25;

                // 延迟0.5秒
                auto delay = DelayTime::create(0.35f);
                auto callFunc = CallFunc::create([=]() {
                    // 延迟执行的操作
                    auto cardLayer = CardLayer::create(player->cards_, 2);
                    Director::getInstance()->getRunningScene()->addChild(cardLayer);
                    });

                // 将延迟和回调函数一起顺序执行
                auto sequence = Sequence::create(delay, callFunc, nullptr);
                Director::getInstance()->getRunningScene()->runAction(sequence);

                return true;
            }

            return false;
            };

        // 添加监听器到事件分发器
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


        // 创建返回按钮
        auto backButton = MenuItemImage::create(
            "cancelButton.png",    // 普通状态图片
            "cancelButtonOutline.png",   // 按下状态图片
            [this](Ref* sender) { // 点击按钮时执行
                // 返回到上一个场景
                Director::getInstance()->popScene();  // 弹出当前场景并返回前一个场景
            }
        );

        // 设置按钮位置
        backButton->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 60, 100)); 

        // 创建菜单并添加按钮
        auto menu = Menu::create(backButton, nullptr);
        menu->setPosition(Vec2::ZERO);  // 菜单的锚点设置为 (0, 0)
        newLayer->addChild(menu);  // 将按钮添加到 newLayer
       

        });


    // 创建一个顺序动作
    auto sequence = Sequence::create(delay, showLayerCallback, nullptr);

    // 执行动作
    this->runAction(sequence);

    return true;
}
