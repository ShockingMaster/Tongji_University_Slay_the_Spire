#pragma once
#include "cocos2d.h"
#include <queue>
#include <vector>
#include <memory>
#include <iostream>
#include "Card.h"
using namespace cocos2d;
class CardSpriteGenerator
{
public:
    static Sprite* createCardSprite(const std::shared_ptr<Card>& card)
    {
        // 创建卡牌背景
        auto background = Sprite::create("card_background.png"); // 替换为实际背景图片
        
        if (!background) {
            CCLOG("Failed to load card background.");
            return nullptr;
        }

        // 设置背景大小（可调整）
        background->setContentSize(Size(200, 300)); // 设置卡牌大小

        //添加卡面
        //保证加号不会影响卡面
        std::string cardName = card->getName();
        if (!cardName.empty() && cardName.back() == '+')
        {
            cardName.pop_back();
        }
        auto cardPainting = Sprite::create("CardPainting/" + cardName + ".png");
        if (cardPainting == nullptr)
        {
            CCLOG("Unable to Create CardPainting For %s", card->getName());
        }
        cardPainting->setContentSize(Size(170, 130));
        cardPainting->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height - 110));
        cardPainting->setName("cardPaiting");
        background->addChild(cardPainting);

        // 添加卡牌名称栏
        auto cardTab = Sprite::create("cardTab.png");
        cardTab->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height - 25));
        cardTab->setName("cardTab");
        background->addChild(cardTab);

        // 添加卡牌名称
        auto nameLabel = Label::createWithSystemFont(card->getName(), "Arial", 24);
        nameLabel->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height - 30));
        if (!card->isUpgraded())
        {
            nameLabel->setColor(Color3B::WHITE);
        }
        else
        {
            nameLabel->setColor(Color3B::YELLOW);
        }
        nameLabel->setName("nameLabel");
        background->addChild(nameLabel);

        //添加能量标志
        auto energyLayer1 = Sprite::create("energyLayer1.png");
        energyLayer1->setContentSize(Size(80, 80));
        energyLayer1->setPosition(Vec2(20, background->getContentSize().height - 25));
        energyLayer1->setName("energyLayer1");
        background->addChild(energyLayer1);
        auto energyLayer2 = Sprite::create("energyLayer2.png");
        energyLayer2->setContentSize(Size(30, 30));
        energyLayer2->setPosition(Vec2(20, background->getContentSize().height - 25));
        energyLayer2->setName("energyLayer2");
        background->addChild(energyLayer2);

        // 添加费用
        std::string costText = std::to_string(card->getEnergyCost());
        auto costLabel = Label::createWithSystemFont(costText, "Arial", 20);
        costLabel->setPosition(Vec2(20, background->getContentSize().height - 25));
        costLabel->setColor(Color3B::YELLOW);
        costLabel->setName("costLabel");
        background->addChild(costLabel);

        // 添加描述
        auto descriptionLabel = Label::createWithSystemFont(card->getDescription(), "Arial", 18);
        descriptionLabel->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height / 2 - 100));
        descriptionLabel->setColor(Color3B::WHITE);
        descriptionLabel->setAlignment(TextHAlignment::CENTER);
        descriptionLabel->setDimensions(180, 100); // 设置文本框大小，自动换行
        descriptionLabel->setName("descriptionLabel");
        background->addChild(descriptionLabel);

        return background;
    }
    static void updateCardSprite(shared_ptr<Card>& card, Node* cardSprite)
    {
        // 更新卡牌名称
        Label* nameLabel = dynamic_cast<Label*>(cardSprite->getChildByName("nameLabel")); // 获取现有的 nameLabel
        if (nameLabel) {
            nameLabel->setColor(Color3B::YELLOW);
            nameLabel->setString(card->getName());
        }

        // 更新费用
        Label* costLabel = dynamic_cast<Label*>(cardSprite->getChildByName("costLabel"));
        if (costLabel) {
            costLabel->setString(std::to_string(card->getEnergyCost()));
        }

        // 更新描述
        Label* descriptionLabel = dynamic_cast<Label*>(cardSprite->getChildByName("descriptionLabel"));
        if (descriptionLabel) {
            descriptionLabel->setString(card->getDescription());
        }
    }
};

class PotionSpriteGenerator {
public:
    static Sprite* createCardSprite(const std::shared_ptr<Potion>& potion) {
        // 创建药水的卡片精灵
        std::string Name = potion->name_;
        auto Painting = Sprite::create(Name + ".png");

        Painting->setContentSize(Size(170, 130));
        Painting->setName("cardPainting");

        // 创建描述文本的Label，默认隐藏
        auto descriptionLabel = Label::createWithSystemFont(potion->description_, "Arial", 18);
        descriptionLabel->setPosition(Vec2(Painting->getContentSize().width / 2, -20)); // 放在Painting下方
        descriptionLabel->setVisible(false);
        descriptionLabel->setName("descriptionLabel");

        // 将Label添加为Painting的子节点
        Painting->addChild(descriptionLabel);

        // 添加鼠标事件监听器
        auto listener = EventListenerMouse::create();

        // 鼠标进入事件
        listener->onMouseMove = [Painting, descriptionLabel](Event* event) {
            auto mouseEvent = static_cast<EventMouse*>(event);
            auto bounds = Painting->getBoundingBox();
            if (bounds.containsPoint(mouseEvent->getLocationInView())) {
                descriptionLabel->setVisible(true);  // 显示描述
            }
            else {
                descriptionLabel->setVisible(false); // 隐藏描述
            }
            };

        // 添加监听器到事件分发器
        Painting->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, Painting);

        return Painting;
    }
};
