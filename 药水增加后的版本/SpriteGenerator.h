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
        //��֤�ӺŲ���Ӱ�쿨��
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

        // ��ӿ���������
        auto cardTab = Sprite::create("cardTab.png");
        cardTab->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height - 25));
        cardTab->setName("cardTab");
        background->addChild(cardTab);

        // ��ӿ�������
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

        //���������־
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

        // ��ӷ���
        std::string costText = std::to_string(card->getEnergyCost());
        auto costLabel = Label::createWithSystemFont(costText, "Arial", 20);
        costLabel->setPosition(Vec2(20, background->getContentSize().height - 25));
        costLabel->setColor(Color3B::YELLOW);
        costLabel->setName("costLabel");
        background->addChild(costLabel);

        // �������
        auto descriptionLabel = Label::createWithSystemFont(card->getDescription(), "Arial", 18);
        descriptionLabel->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height / 2 - 100));
        descriptionLabel->setColor(Color3B::WHITE);
        descriptionLabel->setAlignment(TextHAlignment::CENTER);
        descriptionLabel->setDimensions(180, 100); // �����ı����С���Զ�����
        descriptionLabel->setName("descriptionLabel");
        background->addChild(descriptionLabel);

        return background;
    }
    static void updateCardSprite(shared_ptr<Card>& card, Node* cardSprite)
    {
        // ���¿�������
        Label* nameLabel = dynamic_cast<Label*>(cardSprite->getChildByName("nameLabel")); // ��ȡ���е� nameLabel
        if (nameLabel) {
            nameLabel->setColor(Color3B::YELLOW);
            nameLabel->setString(card->getName());
        }

        // ���·���
        Label* costLabel = dynamic_cast<Label*>(cardSprite->getChildByName("costLabel"));
        if (costLabel) {
            costLabel->setString(std::to_string(card->getEnergyCost()));
        }

        // ��������
        Label* descriptionLabel = dynamic_cast<Label*>(cardSprite->getChildByName("descriptionLabel"));
        if (descriptionLabel) {
            descriptionLabel->setString(card->getDescription());
        }
    }
};

class PotionSpriteGenerator {
public:
    static Sprite* createCardSprite(const std::shared_ptr<Potion>& potion) {
        // ����ҩˮ�Ŀ�Ƭ����
        std::string Name = potion->name_;
        auto Painting = Sprite::create(Name + ".png");

        Painting->setContentSize(Size(170, 130));
        Painting->setName("cardPainting");

        // ���������ı���Label��Ĭ������
        auto descriptionLabel = Label::createWithSystemFont(potion->description_, "Arial", 18);
        descriptionLabel->setPosition(Vec2(Painting->getContentSize().width / 2, -20)); // ����Painting�·�
        descriptionLabel->setVisible(false);
        descriptionLabel->setName("descriptionLabel");

        // ��Label���ΪPainting���ӽڵ�
        Painting->addChild(descriptionLabel);

        // �������¼�������
        auto listener = EventListenerMouse::create();

        // �������¼�
        listener->onMouseMove = [Painting, descriptionLabel](Event* event) {
            auto mouseEvent = static_cast<EventMouse*>(event);
            auto bounds = Painting->getBoundingBox();
            if (bounds.containsPoint(mouseEvent->getLocationInView())) {
                descriptionLabel->setVisible(true);  // ��ʾ����
            }
            else {
                descriptionLabel->setVisible(false); // ��������
            }
            };

        // ��Ӽ��������¼��ַ���
        Painting->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, Painting);

        return Painting;
    }
};
