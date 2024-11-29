#include<HeaderBar.h>
#include"IncludeAll.h"
#include "cocos2d.h"
#include "MapScene.h"
using namespace std;
using namespace cocos2d;

int currentlevel = 1;//测试用
// 构造函数
HearderBar::HearderBar() 
    : name(""), character(""), health(0), fullHealth(0), coins(0), level(0),
    nameLabel(nullptr), healthLabel(nullptr), coinsLabel(nullptr), levelLabel(nullptr),
    potionIcons(nullptr), healthIcon(nullptr), coinsIcon(nullptr), potionIcon(nullptr) {}

// 析构函数
HearderBar::~HearderBar() {}

// 初始化头栏
bool HearderBar::init(Player* player) {
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
    backgroundBar->setAnchorPoint(Vec2(0.5, 0));  // 锚点设置为底部中心
    backgroundBar->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 50)); // 底部中心位置
    this->addChild(backgroundBar);

    // 初始化生命值图标并添加到背景
    healthIcon = Sprite::create("health_icon.png");
    healthIcon->setPosition(Vec2(175, 100)); // 在 bar.png 的顶部显示
    backgroundBar->addChild(healthIcon);

    // 初始化金币图标并添加到背景
    coinsIcon = Sprite::create("coins_icon.png");
    coinsIcon->setPosition(Vec2(450, 100)); // 与 healthIcon 垂直对齐
    backgroundBar->addChild(coinsIcon);

    // 初始化药水容器并添加到背景
    potionIcons = Node::create();
    potionIcons->setPosition(Vec2(200, 50));
    backgroundBar->addChild(potionIcons);

    // 初始化标签并添加到背景
    string playerinfo = name + character;
    nameLabel = Label::createWithSystemFont(playerinfo, "Arial", 30);
    nameLabel->setPosition(Vec2(70, 100)); // 顶部显示名称
    backgroundBar->addChild(nameLabel);

    healthLabel = Label::createWithSystemFont("Health: " + to_string(health) + "/" + to_string(fullHealth), "Arial", 30);
    healthLabel->setPosition(Vec2(300, 100)); // 紧邻 healthIcon
    backgroundBar->addChild(healthLabel);

    coinsLabel = Label::createWithSystemFont("Coins: " + to_string(coins), "Arial", 30);
    coinsLabel->setPosition(Vec2(550, 100)); // 紧邻 coinsIcon
    backgroundBar->addChild(coinsLabel);

    levelLabel = Label::createWithSystemFont("Level: " + to_string(level), "Arial", 30);
    levelLabel->setPosition(Vec2(1000, 100)); // 显示当前关卡信息
    backgroundBar->addChild(levelLabel);

    displayHeader();

    return true;
}


// 更新头栏信息
void HearderBar::updateHeader(Player* player) {
    // 更新玩家状态
    setPlayerInfo(player);

    // 更新标签内容
    healthLabel->setString("Health: " + to_string(health) + "/" + to_string(fullHealth));
    coinsLabel->setString("Coins: " + to_string(coins));
    levelLabel->setString("Level: " + to_string(level));

    // 更新药水图标，图没找齐，先注释掉
    /*potionIcons->removeAllChildren(); // 清空当前药水图标
    for (size_t i = 0; i < potions.size(); ++i) {
        auto potionSprite = Sprite::create("potion_icon.png");
        potionSprite->setPosition(Vec2(50 + i * 30, 0));
        potionIcons->addChild(potionSprite);
    }*/
}

// 显示头栏内容
void HearderBar::displayHeader() {
    // 显示名称
    nameLabel->setString(name);
}

// 静态创建函数
HearderBar* HearderBar::create(Player* player) {
    HearderBar* headerBar = new (std::nothrow) HearderBar();
    if (headerBar && headerBar->init(player)) {
        headerBar->autorelease();
        return headerBar;
    }
    CC_SAFE_DELETE(headerBar);
    return nullptr;
}

// 设置玩家信息
void HearderBar::setPlayerInfo(const string& name, const string& character, int fullHealth, int coins) {
    this->name = name;
    this->character = character;
    this->fullHealth = fullHealth;
    this->coins = coins;
}

void HearderBar::setPlayerInfo(Player* player) {
    this->name = player->name_;
    this->character = player->character_;
    this->health = player->health_;
    this->fullHealth = player->fullhealth_;
    this->coins = player->coins_;
    this->potions = player->potions_;
    this->level = currentlevel;
}

// 设置生命值
void HearderBar::setHealth(int health) {
    this->health = health;
    healthLabel->setString("Health: " + to_string(health) + "/" + to_string(fullHealth));
}

// 设置金币
void HearderBar::setCoins(int coins) {
    this->coins = coins;
    coinsLabel->setString("Coins: " + to_string(coins));
}

// 设置药水
void HearderBar::setPotions(const vector<Potion*>& potions) {
    this->potions = potions;
    updateHeader(nullptr); // 更新药水图标
}

// 设置关卡
void HearderBar::setLevel(int level) {
    this->level = level;
    levelLabel->setString("Level: " + to_string(level));
}

