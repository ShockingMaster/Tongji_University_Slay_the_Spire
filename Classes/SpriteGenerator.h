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
        // �������Ʊ���
        auto background = Sprite::create("card_background.png"); // �滻Ϊʵ�ʱ���ͼƬ
        
        if (!background) {
            CCLOG("Failed to load card background.");
            return nullptr;
        }

        // ���ñ�����С���ɵ�����
        background->setContentSize(Size(200, 300)); // ���ÿ��ƴ�С

        //��ӿ���
        auto cardPainting = Sprite::create("CardPainting/" + card->getName() + ".png");
        if (cardPainting == nullptr)
        {
            CCLOG("Unable to Create CardPainting For %s", card->getName());
        }
        cardPainting->setContentSize(Size(170, 130));
        cardPainting->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height - 110));
        background->addChild(cardPainting);

        // ��ӿ���������
        auto cardTab = Sprite::create("cardTab.png");
        cardTab->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height - 25));
        background->addChild(cardTab);

        // ��ӿ�������
        auto nameLabel = Label::createWithSystemFont(card->getName(), "Arial", 24);
        nameLabel->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height - 30));
        nameLabel->setColor(Color3B::WHITE);
        background->addChild(nameLabel);

        //���������־
        auto energyLayer1 = Sprite::create("energyLayer1.png");
        energyLayer1->setContentSize(Size(80, 80));
        energyLayer1->setPosition(Vec2(20, background->getContentSize().height - 25));
        background->addChild(energyLayer1);
        auto energyLayer2 = Sprite::create("energyLayer2.png");
        energyLayer2->setContentSize(Size(30, 30));
        energyLayer2->setPosition(Vec2(20, background->getContentSize().height - 25));
        background->addChild(energyLayer2);

        // ��ӷ���
        std::string costText = std::to_string(card->getEnergyCost());
        auto costLabel = Label::createWithSystemFont(costText, "Arial", 20);
        costLabel->setPosition(Vec2(20, background->getContentSize().height - 25));
        costLabel->setColor(Color3B::YELLOW);
        background->addChild(costLabel);

        // �������
        auto descriptionLabel = Label::createWithSystemFont(card->getDescription(), "Arial", 18);
        descriptionLabel->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height / 2 - 100));
        descriptionLabel->setColor(Color3B::WHITE);
        descriptionLabel->setAlignment(TextHAlignment::CENTER);
        descriptionLabel->setDimensions(180, 100); // �����ı����С���Զ�����
        background->addChild(descriptionLabel);


        return background;
    }
};