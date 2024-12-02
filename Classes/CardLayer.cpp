#include "CardLayer.h" 
#include "ui/CocosGUI.h"  
#include "Card.h"

CardLayer::CardLayer() : _cards({}), _scrollView(nullptr), _background(nullptr) {
}

CardLayer::~CardLayer() {
}

CardLayer* CardLayer::create(std::vector<Card*> cards,int op) {
    CardLayer* ret = new CardLayer();
    if (ret && ret->init(cards,op)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CardLayer::init(std::vector<Card*> cards, int op) {
    if (!Layer::init()) {
        return false;
    }
    operation = op;
    // ���ر���
    _background = cocos2d::Sprite::create("cardlayer.png");
    if (_background) {
        _background->setScale(1.0f);
        _background->setColor(cocos2d::Color3B(240, 240, 240));
        _background->setPosition(cocos2d::Vec2(1000, 720));
        _background->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
        this->addChild(_background, 100);
    }

    // ��ʼ������
    _cards = cards;

    // ���� ScrollView
    _scrollView = cocos2d::ui::ScrollView::create();
    _scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);  // ����Ϊ�������
    _scrollView->setContentSize(cocos2d::Size(2000, 1000));  // ���ù��������С
    _scrollView->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
    _scrollView->setPosition(cocos2d::Vec2(320, 240));  // ������ͼ��λ��
    this->addChild(_scrollView, 101);  // ���ƹ�������Ĳ㼶���ڱ���

    displayCards();  // ��ʾ����

    // �����رհ�ť
    auto closeButton = cocos2d::MenuItemImage::create(
        "cancelButton.png", "cancelButtonOutline.png",
        CC_CALLBACK_1(CardLayer::closeLayerCallback, this)
    );
    closeButton->setPosition(cocos2d::Vec2(1940, 190));
    closeButton->setScale(1.3f);
    auto menu = cocos2d::Menu::create(closeButton, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 102);  // ��ť�Ĳ㼶���ڿ��ƺͱ���
    // ��������Label
    auto returnLabel = cocos2d::Label::createWithSystemFont(u8"����", "Arial", 60);
    returnLabel->setTextColor(cocos2d::Color4B::WHITE);
    returnLabel->setPosition(cocos2d::Vec2(1750, 260));  // ����Label��λ�ã�ʹ���ڰ�ť�Ϸ�
    this->addChild(returnLabel, 103);  // Label�Ĳ㼶���ڰ�ť

    if (operation == 1) {
        auto inform = cocos2d::Label::createWithSystemFont(u8"��ǰ\n����", "Marker Felt.ttf", 80);
        inform->setTextColor(cocos2d::Color4B::WHITE);
        inform->setPosition(cocos2d::Vec2(1750, 700));  // ����Label��λ�ã�ʹ���ڰ�ť�Ϸ�
        this->addChild(inform, 103);  // Label�Ĳ㼶���ڰ�ť
    }
    if (operation == 2) {
        auto inform = cocos2d::Label::createWithSystemFont(u8"��ѡ��\nһ����\n��ɾ��", "Marker Felt.ttf", 80);
        inform->setTextColor(cocos2d::Color4B::WHITE);
        inform->setPosition(cocos2d::Vec2(1700, 700));  // ����Label��λ�ã�ʹ���ڰ�ť�Ϸ�
        this->addChild(inform, 103);  // Label�Ĳ㼶���ڰ�ť
    }
    if (operation == 3) {
        auto inform = cocos2d::Label::createWithSystemFont(u8"��ѡ��\nһ����\n������", "Marker Felt.ttf", 80);
        inform->setTextColor(cocos2d::Color4B::WHITE);
        inform->setPosition(cocos2d::Vec2(1700, 700));  // ����Label��λ�ã�ʹ���ڰ�ť�Ϸ�
        this->addChild(inform, 103);  // Label�Ĳ㼶���ڰ�ť
    }
    return true;
}
void CardLayer::displayCards() {
    float startX = 400;
    float startY = 1000;
    size_t cardCount = _cards.size();

    // ������������
    cocos2d::Node* cardContainer = cocos2d::Node::create();
    _scrollView->addChild(cardContainer, 200);  // ������������ӵ�������ͼ��

    // �������Ʋ���ӵ�������
    for (size_t i = 0; i < cardCount; ++i) {
        auto card = _cards[i];
        auto cardSprite = cocos2d::Sprite::create("cardbackground.jpg");

        if (i % 4 == 0 && i != 0) {
            startY -= 400;
        }

        cardSprite->setPosition(startX + 350 * (i % 4), startY);
        cardSprite->setScale(0.4f);
        cardContainer->addChild(cardSprite, 200);  // ��ʼ����

        // ��ÿ�����Ƶľ�����ӵ������У����洢���Ƶ�ָ���Ա�֮��ʹ��
        _cardSprites.push_back(cardSprite);

        // ��������¼�������
        auto cardListener = cocos2d::EventListenerMouse::create();

        // ����ƶ��¼�
        cardListener->onMouseMove = [cardSprite, cardContainer](cocos2d::Event* event) {
            cocos2d::EventMouse* mouseEvent = dynamic_cast<cocos2d::EventMouse*>(event);
            if (mouseEvent) {
                cocos2d::Vec2 mousePosition = mouseEvent->getLocation();

                // ���������Կ�������������
                cocos2d::Vec2 containerPos = cardContainer->getPosition();
                mousePosition -= containerPos;

                // �����ⷶΧ��ͨ������bounding box�������ƴ�������
                cocos2d::Rect extendedBoundingBox = cardSprite->getBoundingBox();
                extendedBoundingBox.origin.x -= 20; // ������չ20����
                extendedBoundingBox.origin.y -= 20; // ������չ20����
                extendedBoundingBox.size.width += 40; // ��չ���40����
                extendedBoundingBox.size.height += 40; // ��չ�߶�40����

                // �ж�����Ƿ��ڿ��Ƶ���չ��Χ��
                if (extendedBoundingBox.containsPoint(mousePosition)) {
                    // �Ŵ���
                    cardSprite->setScale(0.5f);
                    cardSprite->setLocalZOrder(300);  // ��� zOrder��ȷ������ʾ���ϲ�
                }
                else {
                    // �ָ�ԭ�ߴ�
                    cardSprite->setScale(0.4f);  // �ָ���ԭ�ߴ�
                    cardSprite->setLocalZOrder(200);  // �ָ���ʼ zOrder
                }
            }
            };

        // Ϊÿ�������������¼�������
        _eventDispatcher->addEventListenerWithSceneGraphPriority(cardListener, cardSprite);
    }

    // ���ù�����ͼ�����������С
    float contentHeight = 1200;  // ������ͼ���ݵĸ߶�
    float contentWidth = 400 * 4;  // ���ݵĿ��

    // ����������¼�������
    auto listener = cocos2d::EventListenerMouse::create();
    listener->onMouseScroll = CC_CALLBACK_1(CardLayer::onMouseScroll, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // ���ù�����ͼ�ĳߴ��λ��
    _scrollView->setContentSize(cocos2d::Size(1600, 1200));
    _scrollView->setPosition(cocos2d::Vec2(800, 600));

    // ���ù�����ͼ���������λ��
    _scrollView->setInnerContainerPosition(cocos2d::Vec2(0, 0));

    // ȷ��������ͼ����������û���ڵ�
    cardContainer->setLocalZOrder(100); // �Ͳ㼶��ʾ������������ֹ��������ͼ�ڵ�
}

void CardLayer::onMouseScroll(cocos2d::EventMouse* event) {
    // ��ȡ�����ֵ�ƫ����
    float scrollDelta = event->getScrollY();

    // ��ȡ��ǰScrollView�Ĺ���λ��
    cocos2d::Vec2 currentPos = _scrollView->getInnerContainerPosition();

    // ��ȡScrollView����ʾ�������������ĳߴ�
    cocos2d::Size innerSize = _scrollView->getInnerContainerSize();
    cocos2d::Size contentSize = _scrollView->getContentSize();

    // ���������������
    float maxScrollY = 200;
    float minScrollY = -700; // ��С����λ�ã��������򳬳���ʾ����ʱΪ��ֵ

    // �����µĹ���λ��
    float newYPos = currentPos.y + scrollDelta * 2;
     
    // �����µĹ���λ�������½���֮��
    newYPos = std::min(std::max(newYPos, minScrollY), maxScrollY);

    // �����µĹ���λ��
    _scrollView->setInnerContainerPosition(cocos2d::Vec2(0, newYPos));

    // ���¸��¿��Ƶ�BoundingBox��λ�ã�
    updateCardBoundingBoxes();
}



// ���¿���BoundingBox��λ��
void CardLayer::updateCardBoundingBoxes() {
    cocos2d::Vec2 containerPos = _scrollView->getInnerContainerPosition();

    // �������п��ƾ��鲢�������ǵ�boundingBox
    for (auto& cardSprite : _cardSprites) {
        // ��ȡ���Ƶĵ�ǰλ�ò�����BoundingBox
        cocos2d::Vec2 cardPos = cardSprite->getPosition();
        cardPos += containerPos;  // ���¿��Ƶ�λ�ã�ʹ�䷴ӳ������ͼ��ƫ����

        // ���¿��Ƶ�BoundingBox
        cardSprite->setPosition(cardPos);
    }
}


void CardLayer::closeLayerCallback(cocos2d::Ref* sender) {
    this->removeFromParentAndCleanup(true);
}
