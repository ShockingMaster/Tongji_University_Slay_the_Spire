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
        auto cardPainting = Sprite::create("CardPainting/" + card->getName() + ".png");
        if (cardPainting == nullptr)
        {
            CCLOG("Unable to Create CardPainting For %s", card->getName());
        }
        cardPainting->setContentSize(Size(170, 130));
        cardPainting->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height - 110));
        background->addChild(cardPainting);

        // 添加卡牌名称栏
        auto cardTab = Sprite::create("cardTab.png");
        cardTab->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height - 25));
        background->addChild(cardTab);

        // 添加卡牌名称
        auto nameLabel = Label::createWithSystemFont(card->getName(), "Arial", 24);
        nameLabel->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height - 30));
        nameLabel->setColor(Color3B::WHITE);
        background->addChild(nameLabel);

        //添加能量标志
        auto energyLayer1 = Sprite::create("energyLayer1.png");
        energyLayer1->setContentSize(Size(80, 80));
        energyLayer1->setPosition(Vec2(20, background->getContentSize().height - 25));
        background->addChild(energyLayer1);
        auto energyLayer2 = Sprite::create("energyLayer2.png");
        energyLayer2->setContentSize(Size(30, 30));
        energyLayer2->setPosition(Vec2(20, background->getContentSize().height - 25));
        background->addChild(energyLayer2);

        // 添加费用
        std::string costText = std::to_string(card->getEnergyCost());
        auto costLabel = Label::createWithSystemFont(costText, "Arial", 20);
        costLabel->setPosition(Vec2(20, background->getContentSize().height - 25));
        costLabel->setColor(Color3B::YELLOW);
        background->addChild(costLabel);

        // 添加描述
        auto descriptionLabel = Label::createWithSystemFont(card->getDescription(), "Arial", 18);
        descriptionLabel->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height / 2 - 100));
        descriptionLabel->setColor(Color3B::WHITE);
        descriptionLabel->setAlignment(TextHAlignment::CENTER);
        descriptionLabel->setDimensions(180, 100); // 设置文本框大小，自动换行
        background->addChild(descriptionLabel);


        return background;
    }
};