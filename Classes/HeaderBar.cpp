#include<HeaderBar.h>
#include"IncludeAll.h"
#include "cocos2d.h"
#include "MapScene.h"
#include "CardLayer.h"
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
bool HeaderBar::init(Player* player) {
    if (!Node::init()) {
        return false;
    }

    // 设置玩家初始信息
    setPlayerInfo(player);

    // 添加底部背景
    auto backgroundBar = Sprite::create("bar.png");
    if (!backgroundBar) {
        CCLOG("Failed to load bar.png");
        return false;
    }
    backgroundBar->setScaleX(1.05);
    backgroundBar->setAnchorPoint(Vec2(0.5, 0));  // 锚点设置为底部中心
    backgroundBar->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 50)); // 底部中心位置
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
    potionIcons->setPosition(Vec2(200, 50));
    backgroundBar->addChild(potionIcons);

    // 初始化标签并添加到背景
    string playerInfo = name + " (" + character + ")";  // 合并name和character
    nameLabel = Label::createWithSystemFont(playerInfo, "Marker Felt.ttf", 40);  // 使用艺术字体
    nameLabel->setPosition(Vec2(100, 100)); // 顶部显示名称
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
            // 创建并显示 CardLayer
            Player* player = Player::getInstance();
            auto cardLayer = CardLayer::create(player->cards_, 1);
            Director::getInstance()->getRunningScene()->addChild(cardLayer); // 使用更高的层级，确保在 HeaderBar 之上
        }
    );

    // 设置按钮的位置
    auto menu = Menu::create(button, nullptr);
    menu->setPosition(Vec2(1600, 150));  // 设置按钮位置
    this->addChild(menu);

    return true;
}



// 更新头栏信息
void HeaderBar::updateHeader(Player* player) {
    // 更新玩家状态
    setPlayerInfo(player);

    // 更新标签内容
    healthLabel->setString("Health: " + to_string(health) + "/" + to_string(fullHealth));
    coinsLabel->setString("Coins: " + to_string(coins));
    levelLabel->setString("Level: " + to_string( currentLevel-1));

    // 更新药水图标，图没找齐，先注释掉
    /*potionIcons->removeAllChildren(); // 清空当前药水图标
    for (size_t i = 0; i < potions.size(); ++i) {
        auto potionSprite = Sprite::create("potion_icon.png");
        potionSprite->setPosition(Vec2(50 + i * 30, 0));
        potionIcons->addChild(potionSprite);
    }*/
}

// 静态创建函数





HeaderBar* HeaderBar::create(Player* player) {
    HeaderBar* headerBar = new (std::nothrow) HeaderBar();
    if (headerBar && headerBar->init(player)) {
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

void HeaderBar::setPlayerInfo(Player* player) {
    this->name = player->name_;
    this->character = player->character_;
    this->health = player->health_;
    this->fullHealth = player->fullhealth_;
    this->coins = player->coins_;
    this->potions = player->potions_;
    this->level =  currentLevel-1;
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
void HeaderBar::setPotions(const vector<Potion*>& potions) {
    this->potions = potions;
    updateHeader(nullptr); // 更新药水图标
}

// 设置关卡
void HeaderBar::setLevel(int level) {
    this->level = level;
    levelLabel->setString("Level: " + to_string(level));
}

