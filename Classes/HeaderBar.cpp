#include<HeaderBar.h>
#include"IncludeAll.h"
#include "cocos2d.h"
#include "MapScene.h"
#include "CardLayer.h"
using namespace std;
using namespace cocos2d;

extern int  currentLevel;
// ���캯��
HeaderBar::HeaderBar() 
    : name(""), character(""), health(0), fullHealth(0), coins(0), level(0),
    nameLabel(nullptr), healthLabel(nullptr), coinsLabel(nullptr), levelLabel(nullptr),
    potionIcons(nullptr), healthIcon(nullptr), coinsIcon(nullptr), potionIcon(nullptr) {}

// ��������
HeaderBar::~HeaderBar() {}

// ��ʼ��ͷ��
bool HeaderBar::init(Player* player) {
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
    backgroundBar->setScaleX(1.05);
    backgroundBar->setAnchorPoint(Vec2(0.5, 0));  // ê������Ϊ�ײ�����
    backgroundBar->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 50)); // �ײ�����λ��
    this->addChild(backgroundBar);

    // ��ʼ������ֵͼ�겢��ӵ�����
    healthIcon = Sprite::create("health_icon.png");
    healthIcon->setPosition(Vec2(280, 100)); // �� bar.png �Ķ�����ʾ
    backgroundBar->addChild(healthIcon);

    // ��ʼ�����ͼ�겢��ӵ�����
    coinsIcon = Sprite::create("coins_icon.png");
    coinsIcon->setPosition(Vec2(650, 100)); // �� healthIcon ��ֱ����
    backgroundBar->addChild(coinsIcon);

    // ��ʼ��ҩˮ��������ӵ�����
    potionIcons = Node::create();
    potionIcons->setPosition(Vec2(200, 50));
    backgroundBar->addChild(potionIcons);

    // ��ʼ����ǩ����ӵ�����
    string playerInfo = name + " (" + character + ")";  // �ϲ�name��character
    nameLabel = Label::createWithSystemFont(playerInfo, "Marker Felt.ttf", 40);  // ʹ����������
    nameLabel->setPosition(Vec2(100, 100)); // ������ʾ����
    backgroundBar->addChild(nameLabel);

    // �޸�������ǩ����ɫ
    healthLabel = Label::createWithSystemFont("Health: " + to_string(health) + "/" + to_string(fullHealth), "Marker Felt.ttf", 40); // ʹ����������
    healthLabel->setPosition(Vec2(460, 100)); // ���� healthIcon
    healthLabel->setColor(Color3B::RED);  // ����Ϊ��ɫ
    backgroundBar->addChild(healthLabel);

    // �޸Ľ�ұ�ǩ�����ɫ
    coinsLabel = Label::createWithSystemFont("Coins: " + to_string(coins), "Marker Felt.ttf", 40); // ʹ����������
    coinsLabel->setPosition(Vec2(790, 100)); // ���� coinsIcon
    coinsLabel->setColor(Color3B(255, 223, 0));  // ����Ϊ���ɫ
    backgroundBar->addChild(coinsLabel);

    levelLabel = Label::createWithSystemFont("Level: " + to_string( currentLevel-1), "Marker Felt.ttf", 40); // ʹ����������
    levelLabel->setPosition(Vec2(1300, 100)); // ��ʾ��ǰ�ؿ���Ϣ
    backgroundBar->addChild(levelLabel);

    auto button = MenuItemImage::create(
        "deck.png",    // ��ͨ״̬ͼƬ
        "deck.png",    // ����״̬ͼƬ
        [this](Ref* sender) {
            // ��������ʾ CardLayer
            Player* player = Player::getInstance();
            auto cardLayer = CardLayer::create(player->cards_, 1);
            Director::getInstance()->getRunningScene()->addChild(cardLayer); // ʹ�ø��ߵĲ㼶��ȷ���� HeaderBar ֮��
        }
    );

    // ���ð�ť��λ��
    auto menu = Menu::create(button, nullptr);
    menu->setPosition(Vec2(1600, 150));  // ���ð�ťλ��
    this->addChild(menu);

    return true;
}



// ����ͷ����Ϣ
void HeaderBar::updateHeader(Player* player) {
    // �������״̬
    setPlayerInfo(player);

    // ���±�ǩ����
    healthLabel->setString("Health: " + to_string(health) + "/" + to_string(fullHealth));
    coinsLabel->setString("Coins: " + to_string(coins));
    levelLabel->setString("Level: " + to_string( currentLevel-1));

    // ����ҩˮͼ�꣬ͼû���룬��ע�͵�
    /*potionIcons->removeAllChildren(); // ��յ�ǰҩˮͼ��
    for (size_t i = 0; i < potions.size(); ++i) {
        auto potionSprite = Sprite::create("potion_icon.png");
        potionSprite->setPosition(Vec2(50 + i * 30, 0));
        potionIcons->addChild(potionSprite);
    }*/
}

// ��̬��������





HeaderBar* HeaderBar::create(Player* player) {
    HeaderBar* headerBar = new (std::nothrow) HeaderBar();
    if (headerBar && headerBar->init(player)) {
        headerBar->autorelease();
        return headerBar;
    }
    CC_SAFE_DELETE(headerBar);
    return nullptr;
}

// ���������Ϣ
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

// ��������ֵ
void HeaderBar::setHealth(int health) {
    this->health = health;
    healthLabel->setString("Health: " + to_string(health) + "/" + to_string(fullHealth));
}

// ���ý��
void HeaderBar::setCoins(int coins) {
    this->coins = coins;
    coinsLabel->setString("Coins: " + to_string(coins));
}

// ����ҩˮ
void HeaderBar::setPotions(const vector<Potion*>& potions) {
    this->potions = potions;
    updateHeader(nullptr); // ����ҩˮͼ��
}

// ���ùؿ�
void HeaderBar::setLevel(int level) {
    this->level = level;
    levelLabel->setString("Level: " + to_string(level));
}

