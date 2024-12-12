#include <HeaderBar.h>
#include "IncludeAll.h"
#include "cocos2d.h"
#include "MapScene.h"
#include "CardLayer.h"
#include "AudioPlayer.h"

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
bool HeaderBar::init(EventSystem* eventSystem) {
    if (!Node::init()) {
        return false;
    }

    // ������ҳ�ʼ��Ϣ
    setPlayerInfo(eventSystem);

    // ��ӵײ�����
    auto backgroundBar = Sprite::create("bar.png");
    if (!backgroundBar) {
        CCLOG("Failed to load bar.png");
        return false;
    }
    backgroundBar->setScale(1.1f);
    backgroundBar->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2+55, 125)); // �ײ�����λ��
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
    potionIcons->setPosition(Vec2(230, 50));
    backgroundBar->addChild(potionIcons);

    // ��ʼ����ǩ����ӵ�����
    nameLabel = Label::createWithSystemFont(name , "Marker Felt.ttf", 40);  // ʹ����������
    nameLabel->setPosition(Vec2(150, 100)); // ������ʾ����
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
            auto cardLayer= CardLayer::create(cards, 1);
            Director::getInstance()->getRunningScene()->addChild(cardLayer); // ʹ�ø��ߵĲ㼶��ȷ���� HeaderBar ֮��
        }
    );

    // ���ð�ť��λ��
    auto menu = Menu::create(button, nullptr);
    menu->setPosition(Vec2(1600, 160));  // ���ð�ťλ��
    this->addChild(menu);


    potionIcons = Node::create();
    int index = 0;
    for (auto potion : potions) {
        // ����һ����ť�ľ��飬ʹ��MenuItemImage����Sprite
        cocos2d::MenuItemImage* potionMenuItem = cocos2d::MenuItemImage::create(
            "potion_t_glass.png",    // ��ͨ״̬ͼƬ
            "potion_t_glass.png",
            [=](Ref* sender) {       // ����¼��ص�
                // ���ҩˮʱ����ѯ��ʹ��ҩˮ��Layer
                auto usePotionLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // ��͸������
                auto visibleSize = Director::getInstance()->getVisibleSize();
                usePotionLayer->setContentSize(visibleSize);
                usePotionLayer->setPosition(Vec2(0, 0));
                Director::getInstance()->getRunningScene()->addChild(usePotionLayer, 100);

                // ���� "Yes" ��ť
                auto yesButton = MenuItemImage::create(
                    "potionyes.png",  // ��ͨ״̬ͼƬ
                    "potionyes.png",  // ����״̬ͼƬ
                    [=](Ref* sender) {
                        // ִ��ʹ��ҩˮ����
                        CCLOG("Potion used!");
                        auto it = std::find(potions.begin(), potions.end(), potion);
                        potions.erase(it); // ɾ����Ӧ��ҩˮ
                        EventSystem::getInstance()->potions_ = potions;
                        audioPlayer("SOTE_SFX_Potion_1_v2.ogg", false);
                        this->updateHeader(EventSystem::getInstance());
                        usePotionLayer->removeFromParent();  // �Ƴ�ѯ�ʲ�
                    }
                );
                yesButton->setPosition(visibleSize.width / 2 - 100, visibleSize.height / 2 + 200);
                yesButton->setScale(0.5f);

                // ���� "No" ��ť
                auto noButton = MenuItemImage::create(
                    "potionno.png",
                    "potionno.png",  // ����״̬ͼƬ
                    [=](Ref* sender) {
                        // ȡ��ʹ��ҩˮ
                        CCLOG("Potion use canceled!");
                        usePotionLayer->removeFromParent();  // �Ƴ�ѯ�ʲ�
                    }
                );
                noButton->setPosition(visibleSize.width / 2 + 100, visibleSize.height / 2 + 200);
                noButton->setScale(0.5f);

                // �����˵�����Ӱ�ť
                auto menu = Menu::create(yesButton, noButton, nullptr);
                menu->setPosition(Vec2::ZERO);  // ���ò˵�λ��
                usePotionLayer->addChild(menu);  // ���˵���ӵ�ѯ�ʲ�


                auto askLabel = Label::createWithSystemFont(u8"�Ƿ����ҩˮ", "Marker Felt.ttf", 50); // ʹ����������
                askLabel->setPosition(Vec2(1000, 800)); // ���� healthIcon
                askLabel->setColor(Color3B::WHITE);  // ����Ϊ��ɫ
                usePotionLayer->addChild(askLabel);
            });

        // ����ҩˮ�˵�������ź�λ��
        potionMenuItem->setScale(1.8f);
        potionMenuItem->setPosition(cocos2d::Vec2(950 + 80 * index, 100));

        // ����һ���˵�������MenuItem��ӵ��˵���
        auto menu = cocos2d::Menu::create(potionMenuItem, nullptr);
        menu->setPosition(cocos2d::Vec2::ZERO);  // Menu�����λ�ò�Ӱ��Item��λ��
        potionIcons->addChild(menu);  // ���˵���ӵ�����

        // ����ҩˮ�����ҩˮ�˵����ť��
        potionMenuItem->setUserData((void*)potion.get());

        index++;
    }

    backgroundBar->addChild(potionIcons, 200000);


    relicIcons = Node::create();
    relicIcons->setPosition(Vec2(200, 50));
    backgroundBar->addChild(relicIcons);
    int i = 0;
    for (auto relic :relics) {
        // ����һ����ť�ľ��飬ʹ��MenuItemImage����Sprite
        cocos2d::MenuItemImage* relicMenuItem = cocos2d::MenuItemImage::create(
            "bell.png",  "bell.png");

        // ����ҩˮ�˵�������ź�λ��
        relicMenuItem->setScale(1.0f);
        relicMenuItem->setPosition(cocos2d::Vec2(50 + 80 * i, 30));

        // ����һ���˵�������MenuItem��ӵ��˵���
        auto menu = cocos2d::Menu::create(relicMenuItem, nullptr);
        menu->setPosition(cocos2d::Vec2::ZERO);  // Menu�����λ�ò�Ӱ��Item��λ��
        potionIcons->addChild(menu);  // ���˵���ӵ�����

        // ����ҩˮ�����ҩˮ�˵����ť��
        relicMenuItem->setUserData((void*)relic.get());

        i++;
    }

 


    return true;
}



// ����ͷ����Ϣ
void HeaderBar::updateHeader(EventSystem* player) {
    // �������״̬
    setPlayerInfo(player);
    // ���±�ǩ����
    healthLabel->setString("Health: " + to_string(health) + "/" + to_string(fullHealth));
    coinsLabel->setString("Coins: " + to_string(coins));
    levelLabel->setString("Level: " + to_string(currentLevel-1));
    cards = player->cards_;
    potions = player->potions_;
    potionIcons->removeAllChildren();
    potionIcons = Node::create();
    int index = 0;
    for (auto potion : potions) {
        // ����һ����ť�ľ��飬ʹ��MenuItemImage����Sprite
        cocos2d::MenuItemImage* potionMenuItem = cocos2d::MenuItemImage::create(
            "potion_t_glass.png",    // ��ͨ״̬ͼƬ
            "potion_t_glass.png",
            [=](Ref* sender) {       // ����¼��ص�
                // ���ҩˮʱ����ѯ��ʹ��ҩˮ��Layer
                auto usePotionLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // ��͸������
                auto visibleSize = Director::getInstance()->getVisibleSize();
                usePotionLayer->setContentSize(visibleSize);
                usePotionLayer->setPosition(Vec2(0, 0));
                Director::getInstance()->getRunningScene()->addChild(usePotionLayer, 100);

                // ���� "Yes" ��ť
                auto yesButton = MenuItemImage::create(
                    "potionyes.png",  // ��ͨ״̬ͼƬ
                    "potionyes.png",  // ����״̬ͼƬ
                    [=](Ref* sender) {
                        // ִ��ʹ��ҩˮ����
                        CCLOG("Potion used!");
                        audioPlayer("SOTE_SFX_Potion_1_v2.ogg", false);
                        auto it = std::find(potions.begin(), potions.end(), potion);
                        potions.erase(it); // ɾ����Ӧ��ҩˮ
                        EventSystem::getInstance()->potions_ = potions;
                        this->updateHeader(EventSystem::getInstance());
                        usePotionLayer->removeFromParent();  // �Ƴ�ѯ�ʲ�
                    }
                );
                yesButton->setPosition(visibleSize.width / 2 - 100, visibleSize.height / 2 + 200);
                yesButton->setScale(0.5f);

                // ���� "No" ��ť
                auto noButton = MenuItemImage::create(
                    "potionno.png",
                    "potionno.png",  // ����״̬ͼƬ
                    [=](Ref* sender) {
                        // ȡ��ʹ��ҩˮ
                        CCLOG("Potion use canceled!");
                        usePotionLayer->removeFromParent();  // �Ƴ�ѯ�ʲ�
                    }
                );
                noButton->setPosition(visibleSize.width / 2 + 100, visibleSize.height / 2 + 200);
                noButton->setScale(0.5f);

                // �����˵�����Ӱ�ť
                auto menu = Menu::create(yesButton, noButton, nullptr);
                menu->setPosition(Vec2::ZERO);  // ���ò˵�λ��
                usePotionLayer->addChild(menu);  // ���˵���ӵ�ѯ�ʲ�


                auto askLabel = Label::createWithSystemFont(u8"�Ƿ����ҩˮ", "Marker Felt.ttf", 50); // ʹ����������
                askLabel->setPosition(Vec2(1000, 800)); // ���� healthIcon
                askLabel->setColor(Color3B::WHITE);  // ����Ϊ��ɫ
                usePotionLayer->addChild(askLabel);
            });

        // ����ҩˮ�˵�������ź�λ��
        potionMenuItem->setScale(1.8f);
        potionMenuItem->setPosition(cocos2d::Vec2(950 + 80 * index, 110));

        // ����һ���˵�������MenuItem��ӵ��˵���
        auto menu = cocos2d::Menu::create(potionMenuItem, nullptr);
        menu->setPosition(cocos2d::Vec2::ZERO);  // Menu�����λ�ò�Ӱ��Item��λ��
        potionIcons->addChild(menu);  // ���˵���ӵ�����

        // ����ҩˮ�����ҩˮ�˵����ť��
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
        // ����һ����ť�ľ��飬ʹ��MenuItemImage����Sprite
        cocos2d::MenuItemImage* relicMenuItem = cocos2d::MenuItemImage::create(
            "bell.png", "bell.png");

        // ����ҩˮ�˵�������ź�λ��
        relicMenuItem->setScale(1.0f);
        relicMenuItem->setPosition(cocos2d::Vec2(30 + 80 * i, 30));

        // ����һ���˵�������MenuItem��ӵ��˵���
        auto menu = cocos2d::Menu::create(relicMenuItem, nullptr);
        menu->setPosition(cocos2d::Vec2::ZERO);  // Menu�����λ�ò�Ӱ��Item��λ��
        potionIcons->addChild(menu);  // ���˵���ӵ�����
        relicMenuItem->setUserData((void*)relic.get());
        i++;
    }


}

// ��̬��������
HeaderBar* HeaderBar::create(EventSystem* eventSystem) {
    HeaderBar* headerBar = new (std::nothrow) HeaderBar();
    if (headerBar && headerBar->init(eventSystem)) {
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
void HeaderBar::setPotions(const vector<std::shared_ptr<Potion>>& potions) {
    this->potions = potions;
    updateHeader(nullptr); // ����ҩˮͼ��
}

// ���ùؿ�
void HeaderBar::setLevel(int level) {
    this->level = level;
    levelLabel->setString("Level: " + to_string(level));
}

// ���ص�ǰ����ֵ
int HeaderBar::getCurrentHealth()
{
    return health;
}

// �����������ֵ
int HeaderBar::getFullHealth()
{
    return fullHealth;
}

// ���ص�ǰ���
int HeaderBar::getCoins()
{
    return coins;
}




