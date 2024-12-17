#include "IncludeAll.h"
CreatureLayer* CreatureLayer::create(std::vector<std::shared_ptr<Creature>>& monsters)
{
    CreatureLayer* ret = new CreatureLayer();
    if (ret && ret->init(monsters)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CreatureLayer::init(std::vector<std::shared_ptr<Creature>>& monsters)
{
    if (!Layer::init()) {
        return false;
    }
    const cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();
    // ����ͨ��PLayer���д�������
    auto playerModel = cocos2d::Sprite::create("Defect.png");
    // ���û���ҵ��Ļ�ֱ��ʧЧ
    if (!playerModel) {
        CCLOG("Strongest Character missing!");
        return false;
    }
    playerModel->setPosition(0.1953125 * kWindowSizeX, 0.452631 * kWindowSizeY);
    playerModel->setTag(reinterpret_cast<intptr_t>(Player::getInstance().get()));//�趨ΨһTag
    this->addChild(playerModel);

    // ����Ѫ���ײ�
    auto playerHealthBackground = cocos2d::Sprite::create("testground.png");
    if (!playerHealthBackground) {
        CCLOG("Healthbackground missing!");
        return false;
    }
    playerHealthBackground->setPosition(cocos2d::Vec2(0.1953125 * kWindowSizeX, 0.3263157 * kWindowSizeY));  // ����Ѫ����λ��
    this->addChild(playerHealthBackground);

    // ����Ѫ��������
    auto playerHealthBar = cocos2d::Sprite::create("testbar.png");
    if (!playerHealthBar) {
        CCLOG("HealthBar missing!");
        return false;
    }
    playerHealth = cocos2d::ProgressTimer::create(playerHealthBar);
    playerHealth->setType(cocos2d::ProgressTimer::Type::BAR);
    playerHealth->setMidpoint(cocos2d::Vec2(0, 0));  // ���ý�������ʼλ��
    playerHealth->setBarChangeRate(cocos2d::Vec2(1, 0));  // ���ý������ķ���
    playerHealth->setPosition(cocos2d::Vec2(0.1953125 * kWindowSizeX, 0.3263157 * kWindowSizeY));  // ���ý�����λ��
    playerHealth->setName("PlayerHealth");
    this->addChild(playerHealth);

    // ����Ѫ����ǩ
    playerHealthLabel = cocos2d::Label::createWithSystemFont(std::to_string(0), "Arial", 20);
    playerHealthLabel->setPosition(cocos2d::Vec2(0.1953125 * screenSize.width, 
        0.3263157 * screenSize.height));  
    playerHealthLabel->setColor(cocos2d::Color3B::BLACK);  // ����������ɫ
    this->addChild(playerHealthLabel);

    // ��������ͼ��
    playerDefend = cocos2d::Sprite::create("block.png");
    playerDefend->setPosition(cocos2d::Vec2(0.1193125 * screenSize.width,
        0.3263157 * screenSize.height));
    this->addChild(playerDefend);

    // �������ܱ�ǩ
    playerBlockLabel = cocos2d::Label::createWithSystemFont(std::to_string(0), "Arial", 20);
    playerBlockLabel->setPosition(cocos2d::Vec2(0.1193125 * screenSize.width,
        0.3263157 * screenSize.height));
    playerBlockLabel->setColor(cocos2d::Color3B::BLACK);  // ����������ɫ
    this->addChild(playerBlockLabel);


    for (int i = 0;i < CombatSystem::getInstance()->Monsters_.size();i++)
    {
        auto& monster = static_pointer_cast<Monster>(CombatSystem::getInstance()->Monsters_[i]);
        cocos2d::Vec2 monsterPosition = monster->getRect().origin;

        monsterPosition.x += screenSize.width * 100 / 2048;
        monsterPosition.y -= screenSize.height * 50 / 950;

        // ����Ѫ���ײ�
        auto monsterHealthBackground = cocos2d::Sprite::create("testground.png");
        if (!monsterHealthBackground) {
            CCLOG("Healthbackground missing!");
            return false;
        }
        monsterHealthBackground->setPosition(monsterPosition);  // ���ý�����λ��
        this->addChild(monsterHealthBackground);

        // ����Ѫ��������
        auto monsterHealthBar = cocos2d::Sprite::create("testbar.png");
        if (!monsterHealthBar) {
            CCLOG("HealthBar missing!");
            return false;
        }
        monsterHealth.push_back(cocos2d::ProgressTimer::create(monsterHealthBar));
        monsterHealth[i]->setType(cocos2d::ProgressTimer::Type::BAR);
        monsterHealth[i]->setMidpoint(cocos2d::Vec2(0, 0));  // ���ý�������ʼλ��
        monsterHealth[i]->setBarChangeRate(cocos2d::Vec2(1, 0));  // ���ý������ķ���
        monsterHealth[i]->setPosition(monsterPosition);  // ���ý�����λ��
        monsterHealth[i]->setTag(reinterpret_cast<intptr_t>(monster.get()));
        this->addChild(monsterHealth[i]);

        // ����Ѫ����ǩ
        monsterHealthLabel.push_back(cocos2d::Label::createWithSystemFont(std::to_string(0), "Arial", 20));
        monsterHealthLabel[i]->setPosition(monsterPosition);
        monsterHealthLabel[i]->setColor(cocos2d::Color3B::BLACK);  // ����������ɫ
        this->addChild(monsterHealthLabel[i]);

        // ��������ͼ��
        monsterPosition.x -= 0.076 * screenSize.width;
        monsterBlock.push_back(cocos2d::Sprite::create("block.png"));
        monsterBlock[i]->setPosition(monsterPosition);
        this->addChild(monsterBlock[i]);

        // �������ܱ�ǩ
        monsterBlockLabel.push_back(cocos2d::Label::createWithSystemFont(std::to_string(0), "Arial", 20));
        monsterBlockLabel[i]->setPosition(monsterPosition);
        monsterBlockLabel[i]->setColor(cocos2d::Color3B::BLACK);  // ����������ɫ
        this->addChild(monsterBlockLabel[i]);
    }
    return true;
}

void CreatureLayer::updateDisplay()
{
    // ����ͨ�� player ��ȡ�����Ϣ
    const int playerHealth = Player::getInstance()->getHealth();
    const int playerFullHealth = Player::getInstance()->getMaxHealth();
    const int block = Player::getInstance()->getBlockValue();

    CreatureLayer* layer;
    HeaderBar* headBar;
    // ��ȡ������ͷ����Ϣ
    
    CombatScene* scene = dynamic_cast<CombatScene*>(Director::getInstance()->getRunningScene());

    auto currentScene = Director::getInstance()->getRunningScene();
    if (currentScene) {
        CCLOG("Current running scene: %s", typeid(*currentScene).name());
    }
    else {
        CCLOG("No running scene.");
    }

    if (scene) {
        CCLOG("Successfully cast to CombatScene.");
        layer = scene->creatureLayer;
        headBar = scene->headbar;



        // ����ͷ�����и���
        if (playerHealth != EventSystem::getInstance()->getCurrentHealth())
        {
            EventSystem::getInstance()->setHealth(playerHealth);
        }
        if (playerFullHealth != EventSystem::getInstance()->getFullHealth())
        {
            EventSystem::getInstance()->setFullHealth(playerFullHealth);
        }
        headBar->updateHeader(EventSystem::getInstance());
        const float healthPercentage = static_cast<float>(playerHealth) / static_cast<float>(playerFullHealth);

        cocos2d::ProgressTimer* timer = reinterpret_cast<cocos2d::ProgressTimer*>(layer->getChildByName("PlayerHealth"));
        timer->setPercentage(healthPercentage * 100);  // �������Ѫ����ʾ

        // �������Ѫ����ʾ
        auto PlayerHealthLabel = layer->playerHealthLabel;
        PlayerHealthLabel->setString(std::to_string(playerHealth) + "/" + std::to_string(playerFullHealth));

        // ������һ�����ʾ
        auto PlayerBlockLabel = layer->playerBlockLabel;
        auto PlayerBlockSprite = layer->playerDefend;
        PlayerBlockLabel->setString(std::to_string(block));
        if (block == 0) {
            PlayerBlockLabel->setVisible(false);  // ���ر�ǩ
            PlayerBlockSprite->setVisible(false);
        }
        else {
            PlayerBlockLabel->setVisible(true);   // ��ʾ��ǩ
            PlayerBlockSprite->setVisible(true);
            PlayerBlockLabel->setString(std::to_string(block));  // ���±�ǩ����
        }


        for (int i = 0; i < CombatSystem::getInstance()->Monsters_.size(); i++)
        {
            // ����ͨ�� Monster ��ȡ�����Ϣ
            auto& monster = static_pointer_cast<Monster>(CombatSystem::getInstance()->Monsters_[i]);
            const int monsterHealth = monster->getHealth();
            const int monsterFullHealth = monster->getMaxHealth();
            const int monsterblock = monster->getBlockValue();
            const float monsterhealthPercentage = static_cast<float>(monsterHealth) / static_cast<float>(monsterFullHealth);

            cocos2d::ProgressTimer* monstertimer = reinterpret_cast<cocos2d::ProgressTimer*>(layer->getChildByTag(reinterpret_cast<intptr_t>(monster.get())));
            monstertimer->setPercentage(monsterhealthPercentage * 100);  // �������Ѫ����ʾ

            // �������Ѫ����ʾ
            auto monsterHealthLabel = layer->monsterHealthLabel[i];
            monsterHealthLabel->setString(std::to_string(monsterHealth) + "/" + std::to_string(monsterFullHealth));

            // ������һ�����ʾ
            auto monsterBlockLabel = layer->monsterBlockLabel[i];
            auto monsterBlockSprite = layer->monsterBlock[i];
            monsterBlockLabel->setString(std::to_string(monsterblock));
            if (monsterblock == 0) {
                monsterBlockLabel->setVisible(false);  // ���ر�ǩ
                monsterBlockSprite->setVisible(false);
            }
            else {
                monsterBlockLabel->setVisible(true);   // ��ʾ��ǩ
                monsterBlockSprite->setVisible(true);
                monsterBlockLabel->setString(std::to_string(monsterblock));  // ���±�ǩ����
            }
        }
    }
    else {
        CCLOG("Failed to cast to CombatScene.");
    }

    
}
