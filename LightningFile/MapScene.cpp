#include <locale>
#include <set>    // ���ڽڵ��ȥ�ز���
#include <codecvt>
#include "MapScene.h"
#include "MenuScene.h"
#include "SelectionScene.h"
#include "HoverButton.h"
#include "const.h"
#include "AudioPlayer.h"
#include "NodeConnection.h"
#include "Player.h"
#include "HeaderBar.h"

using namespace std;
using namespace cocos2d;



// ��ǰ�߹��Ľ��·�������ڼ�¼��һ��ɫ�ڵ�ͼ���߹��Ľڵ㣬ͨ��������·�����ݻ���ʾ��
vector<MapNode*> visitPath;

//��ǰ�߹��ؿ�
int currentLevel;

/**
 * ���� MapScene ����
 * @return ����ʵ��
 */
Scene* MapScene::createScene() {
    auto scene = Scene::create();
    auto layer = MapScene::create();
    scene->addChild(layer);
    return scene;
}



void MapScene::onEnter() {
    Scene::onEnter();
    audioPlayer("menu.ogg", true);
    static int i = 0;
    CCLOG("onEnter called!");  // �������
    if (i) {
        auto headbar = dynamic_cast<HeaderBar*>(this->getChildByName("HeaderBar"));
        if (headbar) {
            auto player = EventSystem::getInstance();
            headbar->updateHeader(player);  // ʹ�� player ���������ݸ��� headbar
            headbar->level = currentLevel - 1;
        }
    }
    i++;
}
/**
 * ��ʼ�� MapScene
 * @return ��ʼ���Ƿ�ɹ�
 */
bool MapScene::init() {
    // ��鸸���ʼ���Ƿ�ɹ�
    if (!Scene::init()) {
        return false;
    }
    auto player = EventSystem::getInstance();
    auto headbar = HeaderBar::create(player);
    headbar->setName("HeaderBar");  // ��������
    headbar->setPosition(Vec2(50, 750));
    this->addChild(headbar);
    headbar->setLocalZOrder(2000000);  // �� headbar �� Z ˳������Ϊ 100��ȷ����λ�����ϲ�
    currentLevel = 1;
    visitPath.clear();
    // ���ű�������
    audioPlayer("menu.ogg", true);
    const auto screenSize = Director::getInstance()->getVisibleSize();
    audioPlayer("start.ogg", true);

    // ������ͼ���������ڴ�����е�ͼ
    auto mapContainer = Node::create();
    this->addChild(mapContainer);

    // �������ŵ�ͼ
    auto map1 = Sprite::create("map1.png");
    auto map2 = Sprite::create("map2.png");
    auto map3 = Sprite::create("map3.png");
    auto map4 = Sprite::create("map4.png");

    auto map5 = Sprite::create("map5.png");
    map5->setPosition(Vec2(1800,600));
    this->addChild(map5);

    auto continueLabel3 = Label::createWithSystemFont(u8"ս������\nս����Ӣ\n����¼�\n�������\n��Ϣ����\n�����̵�\n  BOSS", "Fonts/Kreon-Bold.ttf", 45);
    continueLabel3->setPosition(Vec2(1800, 550));
    continueLabel3->setColor(Color3B::BLACK);
    this->addChild(continueLabel3);

    // ���õ�ͼ��ê��ͳ�ʼλ�ã���ͼ���ζѵ�
    map1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    map2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    map3->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    map4->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    map1->setPosition(Vec2(0, -200));
    map2->setPosition(Vec2(0, map1->getContentSize().height-200));
    map3->setPosition(Vec2(0, map1->getContentSize().height + map2->getContentSize().height-200));
    map4->setPosition(Vec2(0, -200+map1->getContentSize().height + map2->getContentSize().height + map3->getContentSize().height));

    // ��ӵ�ͼ������
    mapContainer->addChild(map1);
    mapContainer->addChild(map2);
    mapContainer->addChild(map3);
    mapContainer->addChild(map4);

    // ���������� contentSize ȷ�����Թ���
    float totalHeight = map1->getContentSize().height + map2->getContentSize().height + map3->getContentSize().height + map4->getContentSize().height;
    mapContainer->setContentSize(Size(screenSize.width, totalHeight));
    mapContainer->setPosition(Vec2(0, 220));  // ��ʼ������λ��

    // ����������������
    auto listener = EventListenerMouse::create();
    listener->onMouseScroll = [mapContainer](Event* event) {
        auto scrollEvent = static_cast<EventMouse*>(event); // ��ȡ�������¼�
        Vec2 currentPos = mapContainer->getPosition();

        // ���ݹ������������ͼ����λ��
        if (scrollEvent->getScrollY() > 0) {
            currentPos.y += 20; // ���Ϲ���
        }
        else {
            currentPos.y -= 20; // ���¹���
        }

        mapContainer->setPosition(currentPos); // ��������λ��
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // �������ذ�ť
    auto backButton = HoverButton::create("button4(1).png", "button4(2).png", "button4(1).png");
    backButton->setPosition(Vec2(200, 100));  // ���ð�ťλ��
    backButton->setTitleText(u8"����");
    backButton->setTitleFontSize(50);
    backButton->addClickEventListener([=](Ref* sender) {
        // ������ذ�ťʱ���л����˵�����
        Director::getInstance()->pushScene(MenuScene::createScene());
        });

    this->addChild(backButton);
    //���������Ƶ�ͼ���������ǲ�ͬ�������λ���,Ȼ������һЩ���������Ե�����
    srand(static_cast<unsigned>(time(nullptr)));
    for (int i = 1; i < 9; i++) {  // 8��ڵ�
        int ran;
        while (1) {
            ran = rand();
            if (ran < 40 && ran>-40) {
                break;
            }
        }
        vector<MapNode*> levelNodes;  // ��ǰ��Ľڵ�
        if (i == 1) {

            Vec2 one = Vec2(500 + ran, 410);
            Vec2 two = Vec2(900, 300 - ran);
            Vec2 three = Vec2(1300 + ran, 460);
            Vec2 four = Vec2(1500, 320 - ran);
            //ǰ��㲻���־�Ӣ���� ��һ�㲻������Ϣ
            int ranNum = rand() % 6 + 1;
            int ran1 = (ranNum == Elite || ranNum == Rest) ? Combat : ranNum;
            ranNum = rand() % 6 + 1;
            int ran2 = (ranNum == Elite || ranNum == Rest ) ? Combat : ranNum;
            ranNum = rand() % 6 + 1;
            int ran3 = (ranNum == Elite || ranNum == Rest ) ? Combat : ranNum;
            ranNum = rand() % 6 + 1;
            int ran4 = (ranNum == Elite || ranNum == Rest) ? Combat : ranNum;
            MapNode* Node1 = MapNode::create(one, ran1);
            Node1->level = 1;
            levelNodes.push_back(Node1);
            mapContainer->addChild(Node1);
            MapNode* Node2 = MapNode::create(two, ran2);
            Node2->level = 1;
            levelNodes.push_back(Node2);
            mapContainer->addChild(Node2);
            MapNode* Node3 = MapNode::create(three, ran3);
            Node3->level = 1;
            levelNodes.push_back(Node3);
            mapContainer->addChild(Node3);
            MapNode* Node4 = MapNode::create(four, ran4);
            Node4->level = 1;
            levelNodes.push_back(Node4);
            mapContainer->addChild(Node4);
        }
        if (i == 2) {
            Vec2 one = Vec2(600 + ran, 600);
            Vec2 two = Vec2(840, 560 - ran);
            Vec2 three = Vec2(1200 + ran, 680);
            Vec2 four = Vec2(1510, 590 + ran);
            int ranNum = rand() % 6 + 1;
            int ran1 = (ranNum == Elite) ? Combat : ranNum;
            ranNum = rand() % 6 + 1;
            int ran2 = (ranNum == Elite) ? Combat : ranNum;
            ranNum = rand() % 6 + 1;
            int ran3 = (ranNum == Elite) ? Combat : ranNum;
            ranNum = rand() % 6 + 1;
            int ran4 = (ranNum == Elite) ? Combat : ranNum;
            MapNode* Node1 = MapNode::create(one, ran1);
            Node1->level = 2;
            levelNodes.push_back(Node1);
            mapContainer->addChild(Node1);
            MapNode* Node2 = MapNode::create(two, ran2);
            Node2->level = 2;
            levelNodes.push_back(Node2);
            mapContainer->addChild(Node2);
            MapNode* Node3 = MapNode::create(three, ran3);
            Node3->level = 2;
            levelNodes.push_back(Node3);
            mapContainer->addChild(Node3);
            MapNode* Node4 = MapNode::create(four, ran4);
            Node4->level = 2;
            levelNodes.push_back(Node4);
            mapContainer->addChild(Node4);
        }
        if (i == 3) {
            Vec2 one = Vec2(560, 910);
            Vec2 two = Vec2(910, 800 + ran);
            Vec2 three = Vec2(1100 + ran, 880);
            Vec2 four = Vec2(1450 + ran, 840);
            int ranNum = rand() % 6 + 1;
            int ran1 = (ranNum == Elite) ? Combat : ranNum;
            ranNum = rand() % 6 + 1;
            int ran2 = (ranNum == Elite) ? Combat : ranNum;
            ranNum = rand() % 6 + 1;
            int ran3 = (ranNum == Elite) ? Combat : ranNum;
            ranNum = rand() % 6 + 1;
            int ran4 = (ranNum == Elite) ? Combat : ranNum;
            MapNode* Node1 = MapNode::create(one, ran1);
            Node1->level = 3;
            levelNodes.push_back(Node1);
            mapContainer->addChild(Node1);
            MapNode* Node2 = MapNode::create(two, ran2);
            Node2->level = 3;
            levelNodes.push_back(Node2);
            mapContainer->addChild(Node2);
            MapNode* Node3 = MapNode::create(three, ran3);
            Node3->level = 3;
            levelNodes.push_back(Node3);
            mapContainer->addChild(Node3);
            MapNode* Node4 = MapNode::create(four, ran4);
            Node4->level = 3;
            levelNodes.push_back(Node4);
            mapContainer->addChild(Node4);
        }
        if (i == 4) {
            Vec2 one = Vec2(500 + ran, 230);
            Vec2 two = Vec2(900, 200 + ran);
            Vec2 three = Vec2(1300, 140);
            Vec2 four = Vec2(1500, ran + 280);
            int ranNum = rand() % 6 + 1;
            int ran1 = (ranNum == Elite) ? Combat : ranNum;
            ranNum = rand() % 6 + 1;
            int ran2 = (ranNum == Elite) ? Combat : ranNum;
            ranNum = rand() % 6 + 1;
            int ran3 = (ranNum == Elite) ? Combat : ranNum;
            ranNum = rand() % 6 + 1;
            int ran4 = (ranNum == Elite) ? Combat : ranNum;
            MapNode* Node1 = MapNode::create(map2->getPosition() + one, ran1);
            Node1->level = i;
            levelNodes.push_back(Node1);
            mapContainer->addChild(Node1);
            MapNode* Node2 = MapNode::create(map2->getPosition() + two, ran2);
            Node2->level = i;
            levelNodes.push_back(Node2);
            mapContainer->addChild(Node2);
            MapNode* Node3 = MapNode::create(map2->getPosition() + three, ran3);
            Node3->level = i;
            levelNodes.push_back(Node3);
            mapContainer->addChild(Node3);
            MapNode* Node4 = MapNode::create(map2->getPosition() + four, ran4);
            Node4->level = i;
            levelNodes.push_back(Node4);
            mapContainer->addChild(Node4);
        }
        if (i == 5) {
            Vec2 one = Vec2(540 + ran, 440);
            Vec2 two = Vec2(800 + ran, 510);
            Vec2 three = Vec2(1000, 550 + ran);
            Vec2 four = Vec2(1450, 400 + ran);
            int ranNum = rand() % 6 + 1;
            int ran1 = (ranNum == Elite) ? Combat : ranNum;
            ranNum = rand() % 6 + 1;
            int ran2 = (ranNum == Elite) ? Combat : ranNum;
            ranNum = rand() % 6 + 1;
            int ran3 = (ranNum == Elite) ? Combat : ranNum;
            ranNum = rand() % 6 + 1;
            int ran4 = (ranNum == Elite) ? Combat : ranNum;
            MapNode* Node1 = MapNode::create(map2->getPosition() + one, ran1);
            Node1->level = i;
            levelNodes.push_back(Node1);
            mapContainer->addChild(Node1);
            MapNode* Node2 = MapNode::create(map2->getPosition() + two, ran2);
            Node2->level = i;
            levelNodes.push_back(Node2);
            mapContainer->addChild(Node2);
            MapNode* Node3 = MapNode::create(map2->getPosition() + three, ran3);
            Node3->level = i;
            levelNodes.push_back(Node3);
            mapContainer->addChild(Node3);
            MapNode* Node4 = MapNode::create(map2->getPosition() + four, ran4);
            Node4->level = i;
            levelNodes.push_back(Node4);
            mapContainer->addChild(Node4);
        }
        if (i == 6) {
            Vec2 one = Vec2(500 + ran, 840);
            Vec2 two = Vec2(800, 720);
            Vec2 three = Vec2(1230 + ran, 800);
            Vec2 four = Vec2(1390, 820);
            int ran1 = rand() % 6 + 1;
            int ran2 = rand() % 6 + 1;
            int ran3 = rand() % 6 + 1;
            int ran4 = rand() % 6 + 1;
            MapNode* Node1 = MapNode::create(map2->getPosition() + one, ran1);
            Node1->level = i;
            levelNodes.push_back(Node1);
            mapContainer->addChild(Node1);
            MapNode* Node2 = MapNode::create(map2->getPosition() + two, ran2);
            Node2->level = i;
            levelNodes.push_back(Node2);
            mapContainer->addChild(Node2);
            MapNode* Node3 = MapNode::create(map2->getPosition() + three, ran3);
            Node3->level = i;
            levelNodes.push_back(Node3);
            mapContainer->addChild(Node3);
            MapNode* Node4 = MapNode::create(map2->getPosition() + four, ran4);
            Node4->level = i;
            levelNodes.push_back(Node4);
            mapContainer->addChild(Node4);
        }
        if (i == 7) {
            Vec2 one = Vec2(600 + ran, 320);
            Vec2 two = Vec2(750, 210 + ran);
            Vec2 three = Vec2(1000 + ran, 290);
            Vec2 four = Vec2(1300, 350);
            int ran1 = rand() % 6 + 1;
            int ran2 = rand() % 6 + 1;
            int ran3 = rand() % 6 + 1;
            int ran4 = rand() % 6 + 1;
            MapNode* Node1 = MapNode::create(map3->getPosition() + one, ran1);
            Node1->level = i;
            levelNodes.push_back(Node1);
            mapContainer->addChild(Node1);
            MapNode* Node2 = MapNode::create(map3->getPosition() + two, ran2);
            Node2->level = i;
            levelNodes.push_back(Node2);
            mapContainer->addChild(Node2);
            MapNode* Node3 = MapNode::create(map3->getPosition() + three, ran3);
            Node3->level = i;
            levelNodes.push_back(Node3);
            mapContainer->addChild(Node3);
            MapNode* Node4 = MapNode::create(map3->getPosition() + four, ran4);
            Node4->level = i;
            levelNodes.push_back(Node4);
            mapContainer->addChild(Node4);
        }
        if (i == 8) {
            Vec2 one = Vec2(570, 610);
            Vec2 two = Vec2(810, 520);
            Vec2 three = Vec2(1100 + ran, 630);
            Vec2 four = Vec2(1280, 500);
            int ran1 = rand() % 6 + 1;
            int ran2 = rand() % 6 + 1;
            int ran3 = rand() % 6 + 1;
            int ran4 = rand() % 6 + 1;
            MapNode* Node1 = MapNode::create(map3->getPosition() + one, ran1);
            Node1->level = i;
            levelNodes.push_back(Node1);
            mapContainer->addChild(Node1);
            MapNode* Node2 = MapNode::create(map3->getPosition() + two, ran2);
            Node2->level = i;
            levelNodes.push_back(Node2);
            mapContainer->addChild(Node2);
            MapNode* Node3 = MapNode::create(map3->getPosition() + three, ran3);
            Node3->level = i;
            levelNodes.push_back(Node3);
            mapContainer->addChild(Node3);
            MapNode* Node4 = MapNode::create(map3->getPosition() + four, ran4);
            Node4->level = i;
            levelNodes.push_back(Node4);
            mapContainer->addChild(Node4);
        }
        MapNodes.push_back(levelNodes);  // ���浱ǰ��
    }

    //��������ӹ̶��ڵ�

    //�ھŲ� Ӧ�ñ��̶�Ϊ��Ϣ����
    MapNode* Node9 = MapNode::create(map3->getPosition() + Vec2(1000, 900), Rest);
    Node9->level = 9;
    mapContainer->addChild(Node9);
    MapNodes.push_back({ Node9 });
    //��ʮ�� ���̶�Ϊ�̵�����
    MapNode* Node10 = MapNode::create(map4->getPosition() + Vec2(1000, 100), Shop);
    Node10->level = 10;
    mapContainer->addChild(Node10);
    MapNodes.push_back({ Node10 });
    //��ʮһ�� ���̶�ΪBoss
    MapNode* Node11 = MapNode::create(map4->getPosition() + Vec2(1000, 600), Boss);
    Node11->level = 11;
    mapContainer->addChild(Node11);
    MapNodes.push_back({ Node11 });


    // ����һ�� DrawNode ����
    auto drawNode = DrawNode::create();
    mapContainer->addChild(drawNode);


    // uniqueConnections ��һ�����ϣ����ڷ�ֹ�ظ����ӣ��洢Ψһ�Ľڵ��
    std::set<std::pair<MapNode*, MapNode*>> uniqueConnections;

    // ����ÿһ��ڵ㣬ȷ�����нڵ㶼������һ����Ч����
    for (int level = 0; level < MapNodes.size() - 1; level++) { // �����ӵ�0�㵽�����ڶ���
        // ���սڵ��ˮƽλ�ý��������Լ��ٽ���
        std::sort(MapNodes[level].begin(), MapNodes[level].end(), [](MapNode* a, MapNode* b) {
            return a->position.x < b->position.x; // ����ڵ���position.x�ֶα�ʾˮƽλ��
            });

        for (MapNode* current : MapNodes[level]) { // ������ǰ���ÿ���ڵ�
            bool connectedToNext = false; // ��ǵ�ǰ�ڵ��Ƿ��Ѿ����ӵ���һ��ڵ�

            // ��鵱ǰ�ڵ��Ƿ������ӵ���һ����κνڵ�
            for (MapNode* next : MapNodes[level + 1]) {
                if (uniqueConnections.count({ current, next }) > 0) { // ��������Ѵ���
                    connectedToNext = true;
                    break;
                }
            }

            // �����ǰ�ڵ�û�����ӵ���һ�㣬��Ѱ��һ�����ʵ����ӵ�
            if (!connectedToNext && !MapNodes[level + 1].empty()) { // ȷ����һ��ǿ�
                // ����һ����ѡ����ӽ���ǰ�ڵ�ˮƽλ�õĽڵ�
                MapNode* next = nullptr;
                int minDistance = std::numeric_limits<int>::max();

                for (MapNode* candidate : MapNodes[level + 1]) {
                    int distance = std::abs(candidate->position.x - current->position.x);
                    if (distance < minDistance) {
                        minDistance = distance;
                        next = candidate;
                    }
                }

                // ���ѡ�е���һ��ڵ��뵱ǰ�ڵ�֮��û���ظ����ӣ���������
                if (next && uniqueConnections.insert({ current, next }).second) {
                    current->connectedNodes.push_back(next);
                    connections.push_back(new NodeConnection(current, next));
                }
            }
        }

        // ȷ����һ���ÿ���ڵ㶼������һ���ڵ����ӵ���
        for (MapNode* next : MapNodes[level + 1]) { // ������һ���ÿ���ڵ�
            bool hasIncomingConnection = false; // ��Ǹýڵ��Ƿ����ϲ����ӵ���

            // ����Ƿ��нڵ����ӵ���ǰ�ڵ�
            for (MapNode* current : MapNodes[level]) {
                if (uniqueConnections.count({ current, next }) > 0) { // ��������Ѵ���
                    hasIncomingConnection = true;
                    break;
                }
            }

            // �����ǰ�ڵ�û���κ����ӣ���ǿ�ƴ���һ�����ϲ㵽��������
            if (!hasIncomingConnection && !MapNodes[level].empty()) { // ȷ����һ��ǿ�
                // ����һ����ѡ����ӽ���ǰ�ڵ�ˮƽλ�õĽڵ�
                MapNode* current = nullptr;
                int minDistance = std::numeric_limits<int>::max();

                for (MapNode* candidate : MapNodes[level]) {
                    int distance = std::abs(candidate->position.x - next->position.x);
                    if (distance < minDistance) {
                        minDistance = distance;
                        current = candidate;
                    }
                }

                // ���ѡ�е���һ��ڵ��뵱ǰ�ڵ�֮��û���ظ����ӣ���������
                if (current && uniqueConnections.insert({ current, next }).second) {
                    current->connectedNodes.push_back(next);
                    connections.push_back(new NodeConnection(current, next));
                }
            }
        }
    }


    // ����·����ʵ�ֽ���ɫЧ��
    for (auto con : connections) {
        // ���ý���ɫЧ������ز���
        Color4F startColor(1.0f, 1.0f, 1.0f, 0.25f); // ��ʼ��ɫ����ɫ��͸���Ƚϸ�
        Color4F endColor(1.0f, 1.0f, 1.0f, 0.05f);   // ��ֹ��ɫ����ɫ��͸���Ƚϵ�
        float lineWidth = 20.0f; // ����·�����������

        // ʹ�û��ƽڵ㣨drawNode���������ν���ɫ·��
        drawNode->drawSegment(con->start->getPosition(), con->end->getPosition(), lineWidth, startColor); // ����·��
        drawNode->drawSegment(con->start->getPosition(), con->end->getPosition(), lineWidth, endColor);   // ����Ч��·��
        drawNode->drawSegment(con->start->getPosition(), con->end->getPosition(), lineWidth * 1.5f, Color4F(1.0f, 1.0f, 1.0f, 0.05f)); // �ⲿ����
    }

    return true;
}
