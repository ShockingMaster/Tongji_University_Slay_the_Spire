#include "IncludeAll.h"
#include "string"


cocos2d::Scene* CombatScene::createScene()
{
    auto scene = Scene::create();          // ����һ���հ׳���
    auto layer = CombatScene::create();     
    scene->addChild(layer);                // ������ӵ�����
    return scene;
}

void CombatScene::onEnter() {
    Scene::onEnter();

    // ��ʼ���
    this->scheduleOnce([this](float dt) {
        checkScene(); // ���ü�麯��

        isMyTurn = 1;
        CombatSystem::getInstance()->combatStart();
        CombatSystem::getInstance()->startTurn(Player::getInstance());
        creatureLayer->updateDisplay();
        }, 0.5f, "CheckSceneAfterDelay");

}

void CombatScene::checkScene() {
    auto currentScene = Director::getInstance()->getRunningScene();

    if (currentScene) {
        CCLOG("Current running scene: %s", typeid(*currentScene).name());
    }
    else {
        CCLOG("No running scene.");
    }

    // ���Խ���ǰ����ת��Ϊ CombatScene
    CombatScene* scene = dynamic_cast<CombatScene*>(currentScene);
    if (scene) {
        CCLOG("Successfully cast to CombatScene.");
        // ���� updateDisplay
        if (creatureLayer) {
            creatureLayer->updateDisplay();
        }
    }
    else {
        CCLOG("Failed to cast to CombatScene. Retrying...");
        // ���ʧ���ˣ���һ��ʱ���ٳ���
        this->scheduleOnce([this](float dt) {
            checkScene(); // �ݹ�����Լ��������
            }, 0.05f, "RetryCheckScene");
    }
}

bool CombatScene::init() 
{
    // �Ƚ���ս��ϵͳ��ʼ��
    CombatSystem::getInstance()->init();
    const cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();
    auto combat = CombatSystem::getInstance();
    for (int i = 0;i < combat->Monsters_.size();i++)
    {
        auto monster = static_pointer_cast<Monster>(combat->Monsters_[i]);

        // �趨��������
        static_pointer_cast<Monster>(combat->Monsters_[i])->
            setRect(cocos2d::Vec2(0.83 * screenSize.width + combat->Monsters_.size() * 0.12207 * screenSize.width / 2 - (i + 1) * 0.12207 * screenSize.width, 0.526315 * screenSize.height), 0.09765 * screenSize.width, 0.315789 * screenSize.height);
        cocos2d::Rect monsterRect = monster->getRect();

        // ������ʹ�ã������ɴ���������� 
        auto drawNode = cocos2d::DrawNode::create();
        // �ú�ɫ���ƾ���
        drawNode->drawRect(monsterRect.origin, cocos2d::Vec2(monsterRect.origin.x + monsterRect.size.width, monsterRect.origin.y + monsterRect.size.height), cocos2d::Color4F(1, 0, 0, 1));
        this->addChild(drawNode,100);  // �� DrawNode ��ӵ�������
    }

    auto player = EventSystem::getInstance();
    headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 750));          // ����λ�ã�����Ļ�ϲ���
    this->addChild(headbar,1);

    // ���������ñ���ͼ��
    auto background = cocos2d::Sprite::create("combatScene.png");
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
                isMyTurn = 0;
                CCLOG("End Turn clicked!");  // ��ӡ��־
                CombatSystem::getInstance()->endTurnCardPlayed();
                CombatSystem::getInstance()->endTurn(Player::getInstance());//ִ����һغϽ���Ч��
                
                for (int i = 0; i < CombatSystem::getInstance()->Monsters_.size(); i++)
                {
                    auto monster = static_pointer_cast<Monster>(CombatSystem::getInstance()->Monsters_[i]);
                    if (monster->getHealth() > 0)
                    {
                        CombatSystem::getInstance()->startTurn(monster);
                        monster->takeEffect();
                        CombatSystem::getInstance()->endTurn(monster);
                    }
                }
            }
            CombatSystem::getInstance()->startTurn(Player::getInstance());
            isMyTurn = 1;
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


    // ����ĳ����� Layer �д���ս��������ť
    auto endBattleButton = cocos2d::ui::Button::create("endTurnButton.png", "endTurnButton.png");  // ���ð�ť�ĳ�̬�Ͱ���̬ͼƬ
    endBattleButton->setPosition(cocos2d::Vec2(400, 800));  // ���ð�ťλ��
    endBattleButton->setTitleText("End Battle");  // ���ð�ť�ı�
    // ��Ӱ�ť����ǰ��
    this->addChild(endBattleButton);
    // ���ô����¼�������
    endBattleButton->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            CCLOG("End Battle button clicked!");  // ��ӡ��־
            CombatSystem::getInstance()->endTurn(Player::getInstance());
            // ����ս����������ǰ������������һ������
            Director::getInstance()->popScene();
        }
        });



    HandPileLayer::getInstance()->init();
    //����ʹ�ã�����һ���ܴ���Ƶ����򣬵����Ʊ��϶����������ʱ�����
    playArea = Rect(screenSize.width / 2 - 0.15 * screenSize.width, screenSize.height / 2,
        0.3 * screenSize.width, 0.3 * screenSize.height);
    auto playAreaNode = DrawNode::create();
    playAreaNode->drawRect(playArea.origin, playArea.origin + playArea.size, Color4F::GRAY);
    this->addChild(playAreaNode);

    this->addChild(HandPileLayer::getInstance());

    // ����
    creatureLayer = CreatureLayer::create(CombatSystem::getInstance()->Monsters_);
    this->addChild(creatureLayer);
    
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

