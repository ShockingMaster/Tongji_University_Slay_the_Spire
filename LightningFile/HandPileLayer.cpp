#include "IncludeAll.h"

HandPileLayer* HandPileLayer::instance_ = nullptr;

HandPileLayer* HandPileLayer::getInstance()
{
    if (instance_ == nullptr)
    {
        instance_ = new HandPileLayer(); // ����Ψһʵ��  
    }
    return instance_;
}
bool HandPileLayer::init()
{
    card_num_select = 0;
    const cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();
    // �������ƶ�ͼ��
    auto drawPileIcon = cocos2d::Sprite::create("drawPileIcon.png");
    if (drawPileIcon) {
        drawPileIcon->setPosition(cocos2d::Vec2(0.1 * screenSize.width, 0.12 * screenSize.height));
        this->addChild(drawPileIcon);
    }
    else {
        CCLOG("Can't find: drawPileIcon.png");
    }

    // �������ƶ��������
    auto drawPileNumCount = cocos2d::Sprite::create("countCircle.png");
    if (drawPileNumCount) {
        drawPileNumCount->setPosition(cocos2d::Vec2(0.078125 * screenSize.width, 0.078125 * screenSize.height));
        this->addChild(drawPileNumCount);
    }
    HandPileLayer::getInstance()->drawPileNumLabel = cocos2d::Label::createWithSystemFont(std::to_string(0), "Arial", 20);
    HandPileLayer::getInstance()->drawPileNumLabel->setPosition(cocos2d::Vec2(0.078125 * screenSize.width, 0.078125 * screenSize.height));  // ����������ͼ�������
    HandPileLayer::getInstance()->drawPileNumLabel->setColor(cocos2d::Color3B::WHITE);  // ����������ɫ
    this->addChild(HandPileLayer::getInstance()->drawPileNumLabel);
    updateDrawPileDisplay();

    // �������ƶ�ͼ��
    auto discardPileIcon = cocos2d::Sprite::create("discardPileIcon.png");
    if (discardPileIcon) {
        discardPileIcon->setPosition(cocos2d::Vec2(0.88 * screenSize.width, 0.12 * screenSize.height));
        this->addChild(discardPileIcon);
    }
    else {
        CCLOG("Can't find: discardPileIcon.png");
    }


    // �������ƶѿ���������־
    auto discardPileNumCount = cocos2d::Sprite::create("countCircle.png");
    if (discardPileNumCount) {
        discardPileNumCount->setPosition(cocos2d::Vec2(0.9 * screenSize.width, 0.078125 * screenSize.height));
        this->addChild(discardPileNumCount);
    }
    HandPileLayer::getInstance()->discardPileNumLabel = cocos2d::Label::createWithSystemFont(std::to_string(0), "Arial", 20);
    HandPileLayer::getInstance()->discardPileNumLabel->setPosition(cocos2d::Vec2(0.9 * screenSize.width, 0.078125 * screenSize.height));  // ����������ͼ�������
    HandPileLayer::getInstance()->discardPileNumLabel->setColor(cocos2d::Color3B::WHITE);  // ����������ɫ
    this->addChild(HandPileLayer::getInstance()->discardPileNumLabel);


    setSceneType(HandPileLayer::SceneType::SCENE_TYPE_1); //��ʼ����Ϊս������
    return true;
}

/*
* ��������:enableCardDrag
* ���������ƾ��鼰��Ӧ�Ŀ�������ָ��
* ���ܣ��趨���ƿ��Ա��϶�
*/
void HandPileLayer::enableCardDrag(Sprite* cardSprite, std::shared_ptr<Card> card)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    const cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();
    auto playArea = Rect(screenSize.width / 2 - 0.15 * screenSize.width, screenSize.height / 2,
        0.3 * screenSize.width, 0.3 * screenSize.height);
    // �ڳ���1��ִ�еĲ���
       // �����϶�


        // ������������п��ƣ���ô����true
    listener->onTouchBegan = [=](Touch* touch, Event* event) -> bool {
        auto location = touch->getLocation();
        if (cardSprite->getBoundingBox().containsPoint(location)) {
            return true;
        }
        return false;
        };

    // ��������Ŀ��Ƹ�������ƶ�
    listener->onTouchMoved = [=](Touch* touch, Event* event) {
        auto location = touch->getLocation();
        cardSprite->setPosition(location);
        };




    // ����ʹ�ã������趨���ƴ�������������һ�����ͷſ��ƣ���ô�������

    listener->onTouchEnded = [=](Touch* touch, Event* event) {
        auto location = touch->getLocation();
        auto& newhand = CombatSystem::getInstance()->hand;                                      //��ȡ��������
        auto& monsters = CombatSystem::getInstance()->Monsters_;
        if (playArea.containsPoint(location) && card->getCanBePlayed() && !card->needTarget()   //���ڲ���Ҫѡ�е��˵Ŀ���
            && card->getEnergyCost() <= Player::getInstance()->getCurrentEnergy())
        {
            //auto& tempcard = card;
            newhand.erase(std::remove(newhand.begin(), newhand.end(), card), newhand.end());    //���������Ƴ����ſ���
            CombatSystem::getInstance()->cardPlayed(card);                                      //����ս��ϵͳ���������
            cardSprite->removeFromParent();                                                     //�������Ƴ�����Ļ

            if (!card->isExhaust()) {                                                           //������Ʋ�����������
                CombatSystem::getInstance()->discardPile.push(card);                            //�����ƶ��м������ſ���
                HandPileLayer::getInstance()->updateDiscardPileDisplay();                       //�����ƶѽ��и���
            }
        }
        //��Ҫ���£���������������������������������������������������������������������������������������������������������
        //
        //
        
        else if (card->getCanBePlayed() && card->needTarget()   //������Ҫѡ�е��˵Ŀ���
            && card->getEnergyCost() <= Player::getInstance()->getCurrentEnergy())//����Ҫ���¶�����Ҫѡ�е��˵Ŀ��Ƶ��߼�
        {
            for (int i = 0;i < monsters.size();i++)
            {
                // ��������Ѿ�û������ֵ����ô����ѡ��
                if (monsters[i]->getHealth() <= 0)
                    continue;
                shared_ptr<Monster> target = static_pointer_cast<Monster>(monsters[i]);
                if (target->area.containsPoint(location))
                {
                    //auto& tempcard = card;
                    newhand.erase(std::remove(newhand.begin(), newhand.end(), card), newhand.end());    //���������Ƴ����ſ���
                    CombatSystem::getInstance()->cardPlayed(card, target);                              //����ս��ϵͳ���������
                    cardSprite->removeFromParent();                                                     //�������Ƴ�����Ļ

                    if (!card->isExhaust()) {                                                           //������Ʋ�����������
                        CombatSystem::getInstance()->discardPile.push(card);                            //�����ƶ��м������ſ���
                        HandPileLayer::getInstance()->updateDiscardPileDisplay();                       //�����ƶѽ��и���
                    }
                }
            }
        }
        
        adjustHandPile();                                                                   //ÿ�ν��е������������λ��
        };
    // ����϶�����
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, cardSprite);

    // ʹ�ÿ��Ƶ�ַ��ΪΨһ tag
    cardSprite->setTag(reinterpret_cast<intptr_t>(card.get()));
     
}


/*
* ��������:enableCardHighlight
* ���������ƾ��鼰��Ӧ�Ŀ�������ָ��
* ���ܣ��趨���ƿ��Ա�ѡ���޷������
*/
void HandPileLayer::enableCardHighlight(Sprite* cardSprite, std::shared_ptr<Card> card) {
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    //���ÿ��Ƹ������Ŵ󣩹���
        // ��һ���Զ���������������Ƿ��Ѿ�����
    bool* isCardMoved = new bool(false);  // Ϊÿ�ſ������һ��������״̬����



    // ������������п��ƣ���ô����true
    listener->onTouchBegan = [=](Touch* touch, Event* event) -> bool {
        auto location = touch->getLocation();
        if (cardSprite->getBoundingBox().containsPoint(location)) {
            if (!(*isCardMoved)) {
                // ��������һ��
                cardSprite->runAction(MoveBy::create(0.2f, Vec2(0, 50)));  // 0.2���������ƶ�50����λ
                *isCardMoved = true;
                select_card_list.push_back(card);
                card_num_select++;
                if (judge_select_num()) {
                    canSwitchScene = true;
                    SelectScene::getInstance()->update_button();

                    auto combatsystem = CombatSystem::getInstance();

                    std::vector<std::shared_ptr<Card>> list = combatsystem->hand;

                    for (auto& card : combatsystem->hand) {
                        auto it = std::find(select_card_list.begin(), select_card_list.end(), card);
                        if (it == select_card_list.end()) {
                            switchToblank(card);
                        }
                    }

                }
                else {
                    canSwitchScene = false;
                    SelectScene::getInstance()->update_button();
                }
            }
            else {
                // �ָ���ԭ����λ��
                cardSprite->runAction(MoveBy::create(0.2f, Vec2(0, -50)));  // 0.2���ڻָ���ԭ����λ��
                *isCardMoved = false;
                select_card_list.erase(std::remove(select_card_list.begin(), select_card_list.end(), card), select_card_list.end());


                card_num_select--;
                if (judge_select_num()) {
                    canSwitchScene = true;
                    SelectScene::getInstance()->update_button();


                }
                else {
                    canSwitchScene = false;
                    SelectScene::getInstance()->update_button();
                    auto combatsystem = CombatSystem::getInstance();

                    std::vector<std::shared_ptr<Card>> list = combatsystem->hand;

                    for (auto& card : combatsystem->hand) {
                        auto it = std::find(select_card_list.begin(), select_card_list.end(), card);
                        if (it == select_card_list.end()) {
                            switchToCardHighlight(card);  
                        }
                    }
                }
            }
            return true;
        }
        return false;
        };
    // ��Ӵ�������
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, cardSprite);

    // ʹ�ÿ��Ƶ�ַ��ΪΨһ tag
    cardSprite->setTag(reinterpret_cast<intptr_t>(card.get()));
}


void HandPileLayer::exhaustCard() {
    auto combatsystem = CombatSystem::getInstance();
    for (auto& card : select_card_list) {
        combatsystem->hand.erase(std::remove(combatsystem->hand.begin(), combatsystem->hand.end(), card), combatsystem->hand.end());
    }
}

void HandPileLayer::drawCard(std::shared_ptr<Card> card)
{
    // ��������
    auto cardSprite = CardSpriteGenerator::createCardSprite(card);
    cardSprite->setPosition(Vec2(400, 300));                                // ��ʼλ�ã����ֵû���κ�Ӱ��,��������и���
    this->addChild(cardSprite);
    
    //����SceneType���ò�ͬ����
    switch (_currentSceneType) {
    case SceneType::SCENE_TYPE_1:
        // �����϶�
        enableCardDrag(cardSprite, card);
        break;
    case SceneType::SCENE_TYPE_2:
        enableCardHighlight(cardSprite, card);
        break;
    }
    // �Գ��ƶѽ��и���
    updateDrawPileDisplay();
    //adjustHandPile();
}


//�л���CardHighlight
void HandPileLayer::switchToCardHighlight(std::shared_ptr<Card> card) {
    auto cardSpriteNode = this->getChildByTag(reinterpret_cast<intptr_t>(card.get()));
    Sprite* cardSprite = static_cast<Sprite*>(cardSpriteNode);
    // �Ƴ���ǰ���϶�������
    _eventDispatcher->removeEventListenersForTarget(cardSprite);

    // ���ø�������
    enableCardHighlight(cardSprite, card);
}


//�л���enableCardDrag
void HandPileLayer::switchToenableCardDrag(std::shared_ptr<Card> card) {
    auto cardSpriteNode = this->getChildByTag(reinterpret_cast<intptr_t>(card.get()));
    Sprite* cardSprite = static_cast<Sprite*>(cardSpriteNode);
    // �Ƴ���ǰ���϶�������
    _eventDispatcher->removeEventListenersForTarget(cardSprite);

    // ���ø�������
    enableCardDrag(cardSprite, card);
}

void HandPileLayer::switchToblank(std::shared_ptr<Card> card) {
    auto cardSpriteNode = this->getChildByTag(reinterpret_cast<intptr_t>(card.get()));
    Sprite* cardSprite = static_cast<Sprite*>(cardSpriteNode);

    _eventDispatcher->removeEventListenersForTarget(cardSprite);
}


void HandPileLayer::clearAll()
{
    auto& newhand = CombatSystem::getInstance()->hand;
    for (size_t i = 0; i < newhand.size(); ++i)
    {
         auto cardSprite = HandPileLayer::getInstance()->getChildByTag(reinterpret_cast<intptr_t>(newhand[i].get()));
         HandPileLayer::getInstance()->removeChild(cardSprite);
    }
    while (!newhand.empty())
    {
        newhand.pop_back();
    }
    while (!CombatSystem::getInstance()->drawPile.empty())
    {
        CombatSystem::getInstance()->drawPile.pop();
    }
    while (!CombatSystem::getInstance()->discardPile.empty())
    {
        CombatSystem::getInstance()->discardPile.pop();
    }
}


/*
* �������ƣ�removeCard
* ���ܣ�����ĳһ�ſ��Ƶ�ָ�룬���ÿ��ƶ�Ӧ�ľ����Ƴ�
*/
void HandPileLayer::removeCard(std::shared_ptr<Card> card)
{
    auto& newhand = CombatSystem::getInstance()->hand;
    for (size_t i = 0; i < newhand.size(); ++i)
    {
        if (card == newhand[i])
        {
            auto cardSprite = HandPileLayer::getInstance()->getChildByTag(reinterpret_cast<intptr_t>(newhand[i].get()));
            HandPileLayer::getInstance()->removeChild(cardSprite);
        }
    }
}
/*
* �������ƣ�adjustHandPile()
*
*
*/
void HandPileLayer::adjustHandPile()
{
    // ��������λ��
    const cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();
    Vec2 handCenter = Vec2(screenSize.width / 2, 0.13 * screenSize.height);
    const float cardSpacing = 0.1 * screenSize.height;                              // ���Ƽ��
    auto& newhand = CombatSystem::getInstance()->hand;
    const float totalWidth = (newhand.size() - 1) * cardSpacing;
    CCLOG("now my hand has %d cards", newhand.size());
    for (size_t i = 0; i < newhand.size(); ++i)
    {
        // ÿ�����ƶ�Ӧ�ľ����� hand[i] �ĵ�ַ��ΪΨһ tag,ȷ��Ψһ����
        auto cardSprite = this->getChildByTag(reinterpret_cast<intptr_t>(newhand[i].get()));
        if (cardSprite) {

            // ����������������λ�ã�ȷ��������ʾ
            const float xOffset = -totalWidth / 2 + i * cardSpacing;
            Vec2 newPosition = handCenter + Vec2(xOffset, 0);

            // ʹ�ö����ƶ�����λ�ã��Եø�ƽ��
            auto moveAction = MoveTo::create(0.2f, newPosition);

            // ȷ��ֻ��һ���ƶ�������ִ��
            cardSprite->stopAllActions();
            cardSprite->runAction(moveAction);
    
            CCLOG("Card %zu position: (%f, %f)", i, newPosition.x, newPosition.y);
        }
    }
}
/*
* �������ƣ�updateDrawPileDisplay
* ���ܣ�����ս��ϵͳ�ĳ��ƶ��������и���
*/
void HandPileLayer::updateDrawPileDisplay()
{
    const int drawPileNum = CombatSystem::getInstance()->getDrawPileNumber();
    HandPileLayer::getInstance()->drawPileNumLabel->setString(std::to_string(drawPileNum));
}

/*
* ��������:updateDiscardPileDisplay
* ���ܣ�����ս��ϵͳ�����ƶ��������и���
*/
void HandPileLayer::updateDiscardPileDisplay()
{
    const int discardPileNum = CombatSystem::getInstance()->getDiscardPileNumber();
    HandPileLayer::getInstance()->discardPileNumLabel->setString(std::to_string(discardPileNum));
}
