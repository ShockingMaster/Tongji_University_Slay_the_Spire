#include<HeaderBar.h>
#include"IncludeAll.h"
#include "cocos2d.h"
#include "MapScene.h"
using namespace std;
using namespace cocos2d;

int currentlevel = 1;//������
// ���캯��
HearderBar::HearderBar() 
    : name(""), character(""), health(0), fullHealth(0), coins(0), level(0),
    nameLabel(nullptr), healthLabel(nullptr), coinsLabel(nullptr), levelLabel(nullptr),
    potionIcons(nullptr), healthIcon(nullptr), coinsIcon(nullptr), potionIcon(nullptr) {}

// ��������
HearderBar::~HearderBar() {}

// ��ʼ��ͷ��
bool HearderBar::init(Player* player) {
    if (!Node::init()) {
        return false;
    }

    // ������ҳ�ʼ��Ϣ
    setPlayerInfo(player);

    // ��ӵײ�����
    auto backgroundBar = Sprite::create("bar.png");
    if (!backgroundBar) {
        CCLOG("Failed to load bar.png");
        return false;
    }
    backgroundBar->setAnchorPoint(Vec2(0.5, 0));  // ê������Ϊ�ײ�����
    backgroundBar->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 50)); // �ײ�����λ��
    this->addChild(backgroundBar);

    // ��ʼ������ֵͼ�겢��ӵ�����
    healthIcon = Sprite::create("health_icon.png");
    healthIcon->setPosition(Vec2(175, 100)); // �� bar.png �Ķ�����ʾ
    backgroundBar->addChild(healthIcon);

    // ��ʼ�����ͼ�겢��ӵ�����
    coinsIcon = Sprite::create("coins_icon.png");
    coinsIcon->setPosition(Vec2(450, 100)); // �� healthIcon ��ֱ����
    backgroundBar->addChild(coinsIcon);

    // ��ʼ��ҩˮ��������ӵ�����
    potionIcons = Node::create();
    potionIcons->setPosition(Vec2(200, 50));
    backgroundBar->addChild(potionIcons);

    // ��ʼ����ǩ����ӵ�����
    string playerinfo = name + character;
    nameLabel = Label::createWithSystemFont(playerinfo, "Arial", 30);
    nameLabel->setPosition(Vec2(70, 100)); // ������ʾ����
    backgroundBar->addChild(nameLabel);

    healthLabel = Label::createWithSystemFont("Health: " + to_string(health) + "/" + to_string(fullHealth), "Arial", 30);
    healthLabel->setPosition(Vec2(300, 100)); // ���� healthIcon
    backgroundBar->addChild(healthLabel);

    coinsLabel = Label::createWithSystemFont("Coins: " + to_string(coins), "Arial", 30);
    coinsLabel->setPosition(Vec2(550, 100)); // ���� coinsIcon
    backgroundBar->addChild(coinsLabel);

    levelLabel = Label::createWithSystemFont("Level: " + to_string(level), "Arial", 30);
    levelLabel->setPosition(Vec2(1000, 100)); // ��ʾ��ǰ�ؿ���Ϣ
    backgroundBar->addChild(levelLabel);

    displayHeader();

    return true;
}


// ����ͷ����Ϣ
void HearderBar::updateHeader(Player* player) {
    // �������״̬
    setPlayerInfo(player);

    // ���±�ǩ����
    healthLabel->setString("Health: " + to_string(health) + "/" + to_string(fullHealth));
    coinsLabel->setString("Coins: " + to_string(coins));
    levelLabel->setString("Level: " + to_string(level));

    // ����ҩˮͼ�꣬ͼû���룬��ע�͵�
    /*potionIcons->removeAllChildren(); // ��յ�ǰҩˮͼ��
    for (size_t i = 0; i < potions.size(); ++i) {
        auto potionSprite = Sprite::create("potion_icon.png");
        potionSprite->setPosition(Vec2(50 + i * 30, 0));
        potionIcons->addChild(potionSprite);
    }*/
}

// ��ʾͷ������
void HearderBar::displayHeader() {
    // ��ʾ����
    nameLabel->setString(name);
}

// ��̬��������
HearderBar* HearderBar::create(Player* player) {
    HearderBar* headerBar = new (std::nothrow) HearderBar();
    if (headerBar && headerBar->init(player)) {
        headerBar->autorelease();
        return headerBar;
    }
    CC_SAFE_DELETE(headerBar);
    return nullptr;
}

// ���������Ϣ
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

// ��������ֵ
void HearderBar::setHealth(int health) {
    this->health = health;
    healthLabel->setString("Health: " + to_string(health) + "/" + to_string(fullHealth));
}

// ���ý��
void HearderBar::setCoins(int coins) {
    this->coins = coins;
    coinsLabel->setString("Coins: " + to_string(coins));
}

// ����ҩˮ
void HearderBar::setPotions(const vector<Potion*>& potions) {
    this->potions = potions;
    updateHeader(nullptr); // ����ҩˮͼ��
}

// ���ùؿ�
void HearderBar::setLevel(int level) {
    this->level = level;
    levelLabel->setString("Level: " + to_string(level));
}

