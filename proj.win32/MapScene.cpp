#include <locale>
#include <set>    // ���ڽڵ��ȥ�ز���
#include <codecvt>
#include "MapScene.h"
#include "MenuScene.h"
#include "SelectionScene.h"
#include "HoverButton.h"
#include "proj.win32/const.h"
#include "proj.win32/AudioPlayer.h"
#include "NodeConnection.h"

using namespace std;
using namespace cocos2d;

// ȫ�ֱ��������ڱ����������
extern string PlayerName;

// ��ǰ�ؿ���������ʼΪ 1
static int currentLevel = 1;

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

/**
 * ��ʼ�� MapScene
 * @return ��ʼ���Ƿ�ɹ�
 */
bool MapScene::init() {
    // ��鸸���ʼ���Ƿ�ɹ�
    if (!Scene::init()) {
        return false;
    }

    // ���ű�������
    audioPlayer("start.ogg", true);
    const auto screenSize = Director::getInstance()->getVisibleSize();
    audioPlayer("../Resources/start.ogg", true);

    // ������ͼ���������ڴ�����е�ͼ
    auto mapContainer = Node::create();
    this->addChild(mapContainer);

    // �������ŵ�ͼ
    auto map1 = Sprite::create("map4.png");
    auto map2 = Sprite::create("map3.png");
    auto map3 = Sprite::create("map2.png");
    auto map4 = Sprite::create("map1.png");

    // ���õ�ͼ��ê��ͳ�ʼλ�ã���ͼ���ζѵ�
    map1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    map2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    map3->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    map4->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    map1->setPosition(Vec2(0, 0));
    map2->setPosition(Vec2(0, map1->getContentSize().height));
    map3->setPosition(Vec2(0, map1->getContentSize().height + map2->getContentSize().height));
    map4->setPosition(Vec2(0, map1->getContentSize().height + map2->getContentSize().height + map3->getContentSize().height));

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
    backButton->setPosition(Vec2(200, 300));  // ���ð�ťλ��
    backButton->setTitleText(u8"����");
    backButton->setTitleFontSize(50);
    backButton->addClickEventListener([=](Ref* sender) {
        // ������ذ�ťʱ���л����˵�����
        Director::getInstance()->replaceScene(MenuScene::createScene());
        });

    this->addChild(backButton);
    //���������Ƶ�ͼ���������ǲ�ͬ�������λ��ƣ������Ż���
    srand(static_cast<unsigned>(time(nullptr)));
    for (int i = 1; i < 9; i++) {  // 8��ڵ�
        vector<MapNode*> levelNodes;  // ��ǰ��Ľڵ�
        if (i == 1) {
            Vec2 one = Vec2(500, 410);
            Vec2 two = Vec2(900, 300);
            Vec2 three = Vec2(1300, 460);
            Vec2 four = Vec2(1500, 320);
            int ran1 = rand() % 6 + 1;
            int ran2 = rand() % 6 + 1;
            int ran3 = rand() % 6 + 1;
            int ran4 = rand() % 6 + 1;
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
            Vec2 one = Vec2(600, 600);
            Vec2 two = Vec2(840, 560);
            Vec2 three = Vec2(1200, 680);
            Vec2 four = Vec2(1510, 590);
            int ran1 = rand() % 6 + 1;
            int ran2 = rand() % 6 + 1;
            int ran3 = rand() % 6 + 1;
            int ran4 = rand() % 6 + 1;
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
            Vec2 two = Vec2(910, 990);
            Vec2 three = Vec2(1100, 880);
            Vec2 four = Vec2(1450, 840);
            int ran1 = rand() % 6 + 1;
            int ran2 = rand() % 6 + 1;
            int ran3 = rand() % 6 + 1;
            int ran4 = rand() % 6 + 1;
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
            Vec2 one = Vec2(500, 230);
            Vec2 two = Vec2(900, 200);
            Vec2 three = Vec2(1300, 140);
            Vec2 four = Vec2(1500, 280);
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
        if (i == 5) {
            Vec2 one = Vec2(540, 440);
            Vec2 two = Vec2(800, 510);
            Vec2 three = Vec2(1000, 550);
            Vec2 four = Vec2(1450, 400);
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
        if (i == 6) {
            Vec2 one = Vec2(500, 840);
            Vec2 two = Vec2(800, 720);
            Vec2 three = Vec2(1230, 800);
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
            Vec2 one = Vec2(600, 320);
            Vec2 two = Vec2(750, 210);
            Vec2 three = Vec2(1000, 290);
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
            Vec2 three = Vec2(1100, 630);
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
    MapNode* Node9 = MapNode::create(map3->getPosition() + Vec2(1000, 900), 3);
    Node9->level = 9;
    mapContainer->addChild(Node9);
    MapNodes.push_back({ Node9 });
    //��ʮ�� ���̶�Ϊ�̵�����
    MapNode* Node10 = MapNode::create(map4->getPosition() + Vec2(1000, 100), 5);
    Node10->level = 10;
    mapContainer->addChild(Node10);
    MapNodes.push_back({ Node10 });
    //��ʮһ�� ���̶�ΪBoss
    MapNode* Node11 = MapNode::create(map4->getPosition() + Vec2(1000, 600), 7);
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
        for (MapNode* current : MapNodes[level]) { // ������ǰ���ÿ���ڵ�
            bool connectedToNext = false; // ��ǵ�ǰ�ڵ��Ƿ��Ѿ����ӵ���һ��ڵ�

            // ��鵱ǰ�ڵ��Ƿ������ӵ���һ����κνڵ�
            for (MapNode* next : MapNodes[level + 1]) {
                if (uniqueConnections.count({ current, next }) > 0) { // ��������Ѵ���
                    connectedToNext = true;
                    break;
                }
            }

            // �����ǰ�ڵ�û�����ӵ���һ�㣬���������һ������
            if (!connectedToNext && !MapNodes[level + 1].empty()) { // ȷ����һ��ǿ�
                MapNode* next = MapNodes[level + 1][rand() % MapNodes[level + 1].size()]; // ���ѡ����һ���һ���ڵ�
                if (uniqueConnections.insert({ current, next }).second) { // ����ɹ�˵����������
                    current->connectedNodes.push_back(next); // ��ӵ���ǰ�ڵ�������б�
                    connections.push_back(new NodeConnection(current, next)); // �洢���ӵ� connections ����
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
                MapNode* current = MapNodes[level][rand() % MapNodes[level].size()]; // ���ѡ����һ���һ���ڵ�
                if (uniqueConnections.insert({ current, next }).second) { // ����ɹ�˵����������
                    current->connectedNodes.push_back(next); // ��ӵ���ǰ�ڵ�������б�
                    connections.push_back(new NodeConnection(current, next)); // �洢���ӵ� connections ����
                }
            }
        }
    }

    // ����·����ʵ�ֽ���ɫЧ��
    for (auto con : connections) {
        // ���ý���ɫЧ������ز���
        Color4F startColor(1.0f, 1.0f, 1.0f, 0.15f); // ��ʼ��ɫ����ɫ��͸���Ƚϸ�
        Color4F endColor(1.0f, 1.0f, 1.0f, 0.05f);   // ��ֹ��ɫ����ɫ��͸���Ƚϵ�
        float lineWidth = 20.0f; // ����·�����������

        // ʹ�û��ƽڵ㣨drawNode���������ν���ɫ·��
        drawNode->drawSegment(con->start->getPosition(), con->end->getPosition(), lineWidth, startColor); // ����·��
        drawNode->drawSegment(con->start->getPosition(), con->end->getPosition(), lineWidth, endColor);   // ����Ч��·��
        drawNode->drawSegment(con->start->getPosition(), con->end->getPosition(), lineWidth * 1.5f, Color4F(1.0f, 1.0f, 1.0f, 0.05f)); // �ⲿ����
    }

    return true;
}
