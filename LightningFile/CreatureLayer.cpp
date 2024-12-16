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
    // 首先通过PLayer进行创建人物
    auto playerModel = cocos2d::Sprite::create("Defect.png");
    // 如果没有找到的话直接失效
    if (!playerModel) {
        CCLOG("Strongest Character missing!");
        return false;
    }
    playerModel->setPosition(0.1953125 * kWindowSizeX, 0.452631 * kWindowSizeY);
    playerModel->setTag(reinterpret_cast<intptr_t>(Player::getInstance().get()));//设定唯一Tag
    this->addChild(playerModel);

    // 创建血条底部
    auto playerHealthBackground = cocos2d::Sprite::create("testground.png");
    if (!playerHealthBackground) {
        CCLOG("Healthbackground missing!");
        return false;
    }
    playerHealthBackground->setPosition(cocos2d::Vec2(0.1953125 * kWindowSizeX, 0.3263157 * kWindowSizeY));  // 设置血条的位置
    this->addChild(playerHealthBackground);

    // 创建血条进度条
    auto playerHealthBar = cocos2d::Sprite::create("testbar.png");
    if (!playerHealthBar) {
        CCLOG("HealthBar missing!");
        return false;
    }
    playerHealth = cocos2d::ProgressTimer::create(playerHealthBar);
    playerHealth->setType(cocos2d::ProgressTimer::Type::BAR);
    playerHealth->setMidpoint(cocos2d::Vec2(0, 0));  // 设置进度条起始位置
    playerHealth->setBarChangeRate(cocos2d::Vec2(1, 0));  // 设置进度条的方向
    playerHealth->setPosition(cocos2d::Vec2(0.1953125 * kWindowSizeX, 0.3263157 * kWindowSizeY));  // 设置进度条位置
    playerHealth->setName("PlayerHealth");
    this->addChild(playerHealth);

    // 创建血条标签
    playerHealthLabel = cocos2d::Label::createWithSystemFont(std::to_string(0), "Arial", 20);
    playerHealthLabel->setPosition(cocos2d::Vec2(0.1953125 * screenSize.width, 
        0.3263157 * screenSize.height));  
    playerHealthLabel->setColor(cocos2d::Color3B::BLACK);  // 设置文字颜色
    this->addChild(playerHealthLabel);

    // 创建护盾图标
    playerDefend = cocos2d::Sprite::create("block.png");
    playerDefend->setPosition(cocos2d::Vec2(0.1193125 * screenSize.width,
        0.3263157 * screenSize.height));
    this->addChild(playerDefend);

    // 创建护盾标签
    playerBlockLabel = cocos2d::Label::createWithSystemFont(std::to_string(0), "Arial", 20);
    playerBlockLabel->setPosition(cocos2d::Vec2(0.1193125 * screenSize.width,
        0.3263157 * screenSize.height));
    playerBlockLabel->setColor(cocos2d::Color3B::BLACK);  // 设置文字颜色
    this->addChild(playerBlockLabel);


    for (int i = 0;i < CombatSystem::getInstance()->Monsters_.size();i++)
    {
        auto& monster = static_pointer_cast<Monster>(CombatSystem::getInstance()->Monsters_[i]);
        cocos2d::Vec2 monsterPosition = monster->getRect().origin;

        monsterPosition.x += screenSize.width * 100 / 2048;
        monsterPosition.y -= screenSize.height * 50 / 950;

        // 创建血条底部
        auto monsterHealthBackground = cocos2d::Sprite::create("testground.png");
        if (!monsterHealthBackground) {
            CCLOG("Healthbackground missing!");
            return false;
        }
        monsterHealthBackground->setPosition(monsterPosition);  // 设置进度条位置
        this->addChild(monsterHealthBackground);

        // 创建血条进度条
        auto monsterHealthBar = cocos2d::Sprite::create("testbar.png");
        if (!monsterHealthBar) {
            CCLOG("HealthBar missing!");
            return false;
        }
        monsterHealth.push_back(cocos2d::ProgressTimer::create(monsterHealthBar));
        monsterHealth[i]->setType(cocos2d::ProgressTimer::Type::BAR);
        monsterHealth[i]->setMidpoint(cocos2d::Vec2(0, 0));  // 设置进度条起始位置
        monsterHealth[i]->setBarChangeRate(cocos2d::Vec2(1, 0));  // 设置进度条的方向
        monsterHealth[i]->setPosition(monsterPosition);  // 设置进度条位置
        monsterHealth[i]->setTag(reinterpret_cast<intptr_t>(monster.get()));
        this->addChild(monsterHealth[i]);

        // 创建血条标签
        monsterHealthLabel.push_back(cocos2d::Label::createWithSystemFont(std::to_string(0), "Arial", 20));
        monsterHealthLabel[i]->setPosition(monsterPosition);
        monsterHealthLabel[i]->setColor(cocos2d::Color3B::BLACK);  // 设置文字颜色
        this->addChild(monsterHealthLabel[i]);

        // 创建护盾图标
        monsterPosition.x -= 0.076 * screenSize.width;
        monsterBlock.push_back(cocos2d::Sprite::create("block.png"));
        monsterBlock[i]->setPosition(monsterPosition);
        this->addChild(monsterBlock[i]);

        // 创建护盾标签
        monsterBlockLabel.push_back(cocos2d::Label::createWithSystemFont(std::to_string(0), "Arial", 20));
        monsterBlockLabel[i]->setPosition(monsterPosition);
        monsterBlockLabel[i]->setColor(cocos2d::Color3B::BLACK);  // 设置文字颜色
        this->addChild(monsterBlockLabel[i]);
    }
    return true;
}

void CreatureLayer::updateDisplay()
{
    // 首先通过 player 获取玩家信息
    const int playerHealth = Player::getInstance()->getHealth();
    const int playerFullHealth = Player::getInstance()->getMaxHealth();
    const int block = Player::getInstance()->getBlockValue();

    CreatureLayer* layer;
    HeaderBar* headBar;
    // 获取场景和头栏信息
    
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



        // 对于头栏进行更新
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
        timer->setPercentage(healthPercentage * 100);  // 更新玩家血量显示

        // 更新玩家血量显示
        auto PlayerHealthLabel = layer->playerHealthLabel;
        PlayerHealthLabel->setString(std::to_string(playerHealth) + "/" + std::to_string(playerFullHealth));

        // 更新玩家护盾显示
        auto PlayerBlockLabel = layer->playerBlockLabel;
        auto PlayerBlockSprite = layer->playerDefend;
        PlayerBlockLabel->setString(std::to_string(block));
        if (block == 0) {
            PlayerBlockLabel->setVisible(false);  // 隐藏标签
            PlayerBlockSprite->setVisible(false);
        }
        else {
            PlayerBlockLabel->setVisible(true);   // 显示标签
            PlayerBlockSprite->setVisible(true);
            PlayerBlockLabel->setString(std::to_string(block));  // 更新标签内容
        }


        for (int i = 0; i < CombatSystem::getInstance()->Monsters_.size(); i++)
        {
            // 首先通过 Monster 获取玩家信息
            auto& monster = static_pointer_cast<Monster>(CombatSystem::getInstance()->Monsters_[i]);
            const int monsterHealth = monster->getHealth();
            const int monsterFullHealth = monster->getMaxHealth();
            const int monsterblock = monster->getBlockValue();
            const float monsterhealthPercentage = static_cast<float>(monsterHealth) / static_cast<float>(monsterFullHealth);

            cocos2d::ProgressTimer* monstertimer = reinterpret_cast<cocos2d::ProgressTimer*>(layer->getChildByTag(reinterpret_cast<intptr_t>(monster.get())));
            monstertimer->setPercentage(monsterhealthPercentage * 100);  // 更新玩家血量显示

            // 更新玩家血量显示
            auto monsterHealthLabel = layer->monsterHealthLabel[i];
            monsterHealthLabel->setString(std::to_string(monsterHealth) + "/" + std::to_string(monsterFullHealth));

            // 更新玩家护盾显示
            auto monsterBlockLabel = layer->monsterBlockLabel[i];
            auto monsterBlockSprite = layer->monsterBlock[i];
            monsterBlockLabel->setString(std::to_string(monsterblock));
            if (monsterblock == 0) {
                monsterBlockLabel->setVisible(false);  // 隐藏标签
                monsterBlockSprite->setVisible(false);
            }
            else {
                monsterBlockLabel->setVisible(true);   // 显示标签
                monsterBlockSprite->setVisible(true);
                monsterBlockLabel->setString(std::to_string(monsterblock));  // 更新标签内容
            }
        }
    }
    else {
        CCLOG("Failed to cast to CombatScene.");
    }

    
}

