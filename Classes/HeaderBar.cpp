#include <HeaderBar.h>
#include "IncludeAll.h"
#include "cocos2d.h"
#include "MapScene.h"
#include "CardLayer.h"
#include "AudioPlayer.h"

using namespace std;
using namespace cocos2d;

extern int  currentLevel;

// 构造函数
HeaderBar::HeaderBar() 
    : name(""), character(""), health(0), fullHealth(0), coins(0), level(0),
    nameLabel(nullptr), healthLabel(nullptr), coinsLabel(nullptr), levelLabel(nullptr),
    potionIcons(nullptr), healthIcon(nullptr), coinsIcon(nullptr), potionIcon(nullptr) {}

// 析构函数
HeaderBar::~HeaderBar() {}

// 初始化头栏
bool HeaderBar::init(EventSystem* eventSystem) {
    if (!Node::init()) {
        return false;
    }

    // 设置玩家初始信息
    setPlayerInfo(eventSystem);

    // 添加底部背景
    auto backgroundBar = Sprite::create("bar.png");
    if (!backgroundBar) {
        CCLOG("Failed to load bar.png");
        return false;
    }
    backgroundBar->setScale(1.1f);
    backgroundBar->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2+55, 125)); // 底部中心位置
    this->addChild(backgroundBar);

    // 初始化生命值图标并添加到背景
    healthIcon = Sprite::create("health_icon.png");
    healthIcon->setPosition(Vec2(280, 100)); // 在 bar.png 的顶部显示
    backgroundBar->addChild(healthIcon);

    // 初始化金币图标并添加到背景
    coinsIcon = Sprite::create("coins_icon.png");
    coinsIcon->setPosition(Vec2(650, 100)); // 与 healthIcon 垂直对齐
    backgroundBar->addChild(coinsIcon);

    // 初始化药水容器并添加到背景
    potionIcons = Node::create();
    potionIcons->setPosition(Vec2(230, 50));
    backgroundBar->addChild(potionIcons);

    // 初始化标签并添加到背景
    nameLabel = Label::createWithSystemFont(name , "Marker Felt.ttf", 40);  // 使用艺术字体
    nameLabel->setPosition(Vec2(150, 100)); // 顶部显示名称
    backgroundBar->addChild(nameLabel);

    // 修改生命标签：红色
    healthLabel = Label::createWithSystemFont("Health: " + to_string(health) + "/" + to_string(fullHealth), "Marker Felt.ttf", 40); // 使用艺术字体
    healthLabel->setPosition(Vec2(460, 100)); // 紧邻 healthIcon
    healthLabel->setColor(Color3B::RED);  // 设置为红色
    backgroundBar->addChild(healthLabel);

    // 修改金币标签：金黄色
    coinsLabel = Label::createWithSystemFont("Coins: " + to_string(coins), "Marker Felt.ttf", 40); // 使用艺术字体
    coinsLabel->setPosition(Vec2(790, 100)); // 紧邻 coinsIcon
    coinsLabel->setColor(Color3B(255, 223, 0));  // 设置为金黄色
    backgroundBar->addChild(coinsLabel);

    levelLabel = Label::createWithSystemFont("Level: " + to_string( currentLevel-1), "Marker Felt.ttf", 40); // 使用艺术字体
    levelLabel->setPosition(Vec2(1300, 100)); // 显示当前关卡信息
    backgroundBar->addChild(levelLabel);

    auto button = MenuItemImage::create(
        "deck.png",    // 普通状态图片
        "deck.png",    // 按下状态图片
        [this](Ref* sender) {
            auto cardLayer= CardLayer::create(cards, 1);
            Director::getInstance()->getRunningScene()->addChild(cardLayer); // 使用更高的层级，确保在 HeaderBar 之上
        }
    );

    // 设置按钮的位置
    auto menu = Menu::create(button, nullptr);
    menu->setPosition(Vec2(1600, 160));  // 设置按钮位置
    this->addChild(menu);


    potionIcons = Node::create();
    int index = 0;
    for (auto potion : potions) {
        // 创建一个按钮的精灵，使用MenuItemImage代替Sprite
        cocos2d::MenuItemImage* potionMenuItem = cocos2d::MenuItemImage::create(
            "potion_t_glass.png",    // 普通状态图片
            "potion_t_glass.png",
            [=](Ref* sender) {       // 点击事件回调
                // 点击药水时弹出询问使用药水的Layer
                auto usePotionLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // 半透明背景
                auto visibleSize = Director::getInstance()->getVisibleSize();
                usePotionLayer->setContentSize(visibleSize);
                usePotionLayer->setPosition(Vec2(0, 0));
                Director::getInstance()->getRunningScene()->addChild(usePotionLayer, 100);

                // 创建 "Yes" 按钮
                auto yesButton = MenuItemImage::create(
                    "potionyes.png",  // 普通状态图片
                    "potionyes.png",  // 按下状态图片
                    [=](Ref* sender) {
                        // 执行使用药水操作
                        CCLOG("Potion used!");
                        auto it = std::find(potions.begin(), potions.end(), potion);
                        potions.erase(it); // 删除对应的药水
                        EventSystem::getInstance()->potions_ = potions;
                        audioPlayer("SOTE_SFX_Potion_1_v2.ogg", false);
                        this->updateHeader(EventSystem::getInstance());
                        usePotionLayer->removeFromParent();  // 移除询问层
                    }
                );
                yesButton->setPosition(visibleSize.width / 2 - 100, visibleSize.height / 2 + 200);
                yesButton->setScale(0.5f);

                // 创建 "No" 按钮
                auto noButton = MenuItemImage::create(
                    "potionno.png",
                    "potionno.png",  // 按下状态图片
                    [=](Ref* sender) {
                        // 取消使用药水
                        CCLOG("Potion use canceled!");
                        usePotionLayer->removeFromParent();  // 移除询问层
                    }
                );
                noButton->setPosition(visibleSize.width / 2 + 100, visibleSize.height / 2 + 200);
                noButton->setScale(0.5f);

                // 创建菜单并添加按钮
                auto menu = Menu::create(yesButton, noButton, nullptr);
                menu->setPosition(Vec2::ZERO);  // 设置菜单位置
                usePotionLayer->addChild(menu);  // 将菜单添加到询问层


                auto askLabel = Label::createWithSystemFont(u8"是否喝下药水", "Marker Felt.ttf", 50); // 使用艺术字体
                askLabel->setPosition(Vec2(1000, 800)); // 紧邻 healthIcon
                askLabel->setColor(Color3B::WHITE);  // 设置为红色
                usePotionLayer->addChild(askLabel);
            });

        // 设置药水菜单项的缩放和位置
        potionMenuItem->setScale(1.8f);
        potionMenuItem->setPosition(cocos2d::Vec2(950 + 80 * index, 100));

        // 创建一个菜单，并将MenuItem添加到菜单中
        auto menu = cocos2d::Menu::create(potionMenuItem, nullptr);
        menu->setPosition(cocos2d::Vec2::ZERO);  // Menu本身的位置不影响Item的位置
        potionIcons->addChild(menu);  // 将菜单添加到场景

        // 关联药水对象和药水菜单项（按钮）
        potionMenuItem->setUserData((void*)potion.get());

        index++;
    }

    backgroundBar->addChild(potionIcons, 200000);


    relicIcons = Node::create();
    relicIcons->setPosition(Vec2(200, 50));
    backgroundBar->addChild(relicIcons);
    int i = 0;
    for (auto relic :relics) {
        // 创建一个按钮的精灵，使用MenuItemImage代替Sprite
        cocos2d::MenuItemImage* relicMenuItem = cocos2d::MenuItemImage::create(
            "bell.png",  "bell.png");

        // 设置药水菜单项的缩放和位置
        relicMenuItem->setScale(1.0f);
        relicMenuItem->setPosition(cocos2d::Vec2(50 + 80 * i, 30));

        // 创建一个菜单，并将MenuItem添加到菜单中
        auto menu = cocos2d::Menu::create(relicMenuItem, nullptr);
        menu->setPosition(cocos2d::Vec2::ZERO);  // Menu本身的位置不影响Item的位置
        potionIcons->addChild(menu);  // 将菜单添加到场景

        // 关联药水对象和药水菜单项（按钮）
        relicMenuItem->setUserData((void*)relic.get());

        i++;
    }

 


    return true;
}



// 更新头栏信息
void HeaderBar::updateHeader(EventSystem* player) {
    // 更新玩家状态
    setPlayerInfo(player);
    // 更新标签内容
    healthLabel->setString("Health: " + to_string(health) + "/" + to_string(fullHealth));
    coinsLabel->setString("Coins: " + to_string(coins));
    levelLabel->setString("Level: " + to_string(currentLevel-1));
    cards = player->cards_;
    potions = player->potions_;
    potionIcons->removeAllChildren();
    potionIcons = Node::create();
    int index = 0;
    for (auto potion : potions) {
        // 创建一个按钮的精灵，使用MenuItemImage代替Sprite
        cocos2d::MenuItemImage* potionMenuItem = cocos2d::MenuItemImage::create(
            "potion_t_glass.png",    // 普通状态图片
            "potion_t_glass.png",
            [=](Ref* sender) {       // 点击事件回调
                // 点击药水时弹出询问使用药水的Layer
                auto usePotionLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // 半透明背景
                auto visibleSize = Director::getInstance()->getVisibleSize();
                usePotionLayer->setContentSize(visibleSize);
                usePotionLayer->setPosition(Vec2(0, 0));
                Director::getInstance()->getRunningScene()->addChild(usePotionLayer, 100);

                // 创建 "Yes" 按钮
                auto yesButton = MenuItemImage::create(
                    "potionyes.png",  // 普通状态图片
                    "potionyes.png",  // 按下状态图片
                    [=](Ref* sender) {
                        // 执行使用药水操作
                        CCLOG("Potion used!");
                        audioPlayer("SOTE_SFX_Potion_1_v2.ogg", false);
                        auto it = std::find(potions.begin(), potions.end(), potion);
                        potions.erase(it); // 删除对应的药水
                        EventSystem::getInstance()->potions_ = potions;
                        this->updateHeader(EventSystem::getInstance());
                        usePotionLayer->removeFromParent();  // 移除询问层
                    }
                );
                yesButton->setPosition(visibleSize.width / 2 - 100, visibleSize.height / 2 + 200);
                yesButton->setScale(0.5f);

                // 创建 "No" 按钮
                auto noButton = MenuItemImage::create(
                    "potionno.png",
                    "potionno.png",  // 按下状态图片
                    [=](Ref* sender) {
                        // 取消使用药水
                        CCLOG("Potion use canceled!");
                        usePotionLayer->removeFromParent();  // 移除询问层
                    }
                );
                noButton->setPosition(visibleSize.width / 2 + 100, visibleSize.height / 2 + 200);
                noButton->setScale(0.5f);

                // 创建菜单并添加按钮
                auto menu = Menu::create(yesButton, noButton, nullptr);
                menu->setPosition(Vec2::ZERO);  // 设置菜单位置
                usePotionLayer->addChild(menu);  // 将菜单添加到询问层


                auto askLabel = Label::createWithSystemFont(u8"是否喝下药水", "Marker Felt.ttf", 50); // 使用艺术字体
                askLabel->setPosition(Vec2(1000, 800)); // 紧邻 healthIcon
                askLabel->setColor(Color3B::WHITE);  // 设置为红色
                usePotionLayer->addChild(askLabel);
            });

        // 设置药水菜单项的缩放和位置
        potionMenuItem->setScale(1.8f);
        potionMenuItem->setPosition(cocos2d::Vec2(950 + 80 * index, 110));

        // 创建一个菜单，并将MenuItem添加到菜单中
        auto menu = cocos2d::Menu::create(potionMenuItem, nullptr);
        menu->setPosition(cocos2d::Vec2::ZERO);  // Menu本身的位置不影响Item的位置
        potionIcons->addChild(menu);  // 将菜单添加到场景

        // 关联药水对象和药水菜单项（按钮）
        potionMenuItem->setUserData((void*)potion.get());

        index++;
    }
    potionIcons->setPosition(Vec2(65, 50));
    this->addChild(potionIcons);
    relicIcons = Node::create();
    relicIcons->setPosition(Vec2(230, 50));
    this->addChild(relicIcons);
    int i = 0;
    for (auto relic : relics) {
        // 创建一个按钮的精灵，使用MenuItemImage代替Sprite
        cocos2d::MenuItemImage* relicMenuItem = cocos2d::MenuItemImage::create(
            "bell.png", "bell.png");

        // 设置药水菜单项的缩放和位置
        relicMenuItem->setScale(1.0f);
        relicMenuItem->setPosition(cocos2d::Vec2(30 + 80 * i, 30));

        // 创建一个菜单，并将MenuItem添加到菜单中
        auto menu = cocos2d::Menu::create(relicMenuItem, nullptr);
        menu->setPosition(cocos2d::Vec2::ZERO);  // Menu本身的位置不影响Item的位置
        potionIcons->addChild(menu);  // 将菜单添加到场景
        relicMenuItem->setUserData((void*)relic.get());
        i++;
    }


}

// 静态创建函数
HeaderBar* HeaderBar::create(EventSystem* eventSystem) {
    HeaderBar* headerBar = new (std::nothrow) HeaderBar();
    if (headerBar && headerBar->init(eventSystem)) {
        headerBar->autorelease();
        return headerBar;
    }
    CC_SAFE_DELETE(headerBar);
    return nullptr;
}

// 设置玩家信息
void HeaderBar::setPlayerInfo(const string& name, const string& character, int fullHealth, int coins) {
    this->name = name;
    this->character = character;
    this->fullHealth = fullHealth;
    this->coins = coins;

}

void HeaderBar::setPlayerInfo(EventSystem* player) {
    this->name = player->name_;
    this->health = player->health_;
    this->fullHealth = player->fullHealth_;
    this->coins = player->coins_;
    this->potions = player->potions_;
    this->relics = player->relics_;
    this->level =  currentLevel-1;
    this->cards = player->cards_;
}

// 设置生命值
void HeaderBar::setHealth(int health) {
    this->health = health;
    healthLabel->setString("Health: " + to_string(health) + "/" + to_string(fullHealth));
}

// 设置金币
void HeaderBar::setCoins(int coins) {
    this->coins = coins;
    coinsLabel->setString("Coins: " + to_string(coins));
}

// 设置药水
void HeaderBar::setPotions(const vector<std::shared_ptr<Potion>>& potions) {
    this->potions = potions;
    updateHeader(nullptr); // 更新药水图标
}

// 设置关卡
void HeaderBar::setLevel(int level) {
    this->level = level;
    levelLabel->setString("Level: " + to_string(level));
}

// 返回当前生命值
int HeaderBar::getCurrentHealth()
{
    return health;
}

// 返回最大生命值
int HeaderBar::getFullHealth()
{
    return fullHealth;
}

// 返回当前金币
int HeaderBar::getCoins()
{
    return coins;
}




