#include <locale>
#include <set>    // 用于节点的去重操作
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

// 全局变量，用于保存玩家名称
extern string PlayerName;

// 当前关卡索引，初始为 1
static int currentLevel = 1;

/**
 * 创建 MapScene 场景
 * @return 场景实例
 */
Scene* MapScene::createScene() {
    auto scene = Scene::create();
    auto layer = MapScene::create();
    scene->addChild(layer);
    return scene;
}

/**
 * 初始化 MapScene
 * @return 初始化是否成功
 */
bool MapScene::init() {
    // 检查父类初始化是否成功
    if (!Scene::init()) {
        return false;
    }

    // 播放背景音乐
    audioPlayer("start.ogg", true);
    const auto screenSize = Director::getInstance()->getVisibleSize();
    audioPlayer("../Resources/start.ogg", true);

    // 创建地图容器，用于存放所有地图
    auto mapContainer = Node::create();
    this->addChild(mapContainer);

    // 加载四张地图
    auto map1 = Sprite::create("map4.png");
    auto map2 = Sprite::create("map3.png");
    auto map3 = Sprite::create("map2.png");
    auto map4 = Sprite::create("map1.png");

    // 设置地图的锚点和初始位置，地图依次堆叠
    map1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    map2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    map3->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    map4->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    map1->setPosition(Vec2(0, 0));
    map2->setPosition(Vec2(0, map1->getContentSize().height));
    map3->setPosition(Vec2(0, map1->getContentSize().height + map2->getContentSize().height));
    map4->setPosition(Vec2(0, map1->getContentSize().height + map2->getContentSize().height + map3->getContentSize().height));

    // 添加地图到容器
    mapContainer->addChild(map1);
    mapContainer->addChild(map2);
    mapContainer->addChild(map3);
    mapContainer->addChild(map4);

    // 设置容器的 contentSize 确保可以滚动
    float totalHeight = map1->getContentSize().height + map2->getContentSize().height + map3->getContentSize().height + map4->getContentSize().height;
    mapContainer->setContentSize(Size(screenSize.width, totalHeight));
    mapContainer->setPosition(Vec2(0, 220));  // 初始化容器位置

    // 创建鼠标滚动监听器
    auto listener = EventListenerMouse::create();
    listener->onMouseScroll = [mapContainer](Event* event) {
        auto scrollEvent = static_cast<EventMouse*>(event); // 获取鼠标滚动事件
        Vec2 currentPos = mapContainer->getPosition();

        // 根据滚动方向调整地图容器位置
        if (scrollEvent->getScrollY() > 0) {
            currentPos.y += 20; // 向上滚动
        }
        else {
            currentPos.y -= 20; // 向下滚动
        }

        mapContainer->setPosition(currentPos); // 更新容器位置
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // 创建返回按钮
    auto backButton = HoverButton::create("button4(1).png", "button4(2).png", "button4(1).png");
    backButton->setPosition(Vec2(200, 300));  // 设置按钮位置
    backButton->setTitleText(u8"返回");
    backButton->setTitleFontSize(50);
    backButton->addClickEventListener([=](Ref* sender) {
        // 点击返回按钮时，切换到菜单场景
        Director::getInstance()->replaceScene(MenuScene::createScene());
        });

    this->addChild(backButton);
    //接下来绘制地图，基本就是不同层数依次绘制，懒得优化了
    srand(static_cast<unsigned>(time(nullptr)));
    for (int i = 1; i < 9; i++) {  // 8层节点
        vector<MapNode*> levelNodes;  // 当前层的节点
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
        MapNodes.push_back(levelNodes);  // 保存当前层
    }

    //接下来添加固定节点
    
    //第九层 应该被固定为休息区域
    MapNode* Node9 = MapNode::create(map3->getPosition() + Vec2(1000, 900), 3);
    Node9->level = 9;
    mapContainer->addChild(Node9);
    MapNodes.push_back({ Node9 });
    //第十层 被固定为商店区域
    MapNode* Node10 = MapNode::create(map4->getPosition() + Vec2(1000, 100), 5);
    Node10->level = 10;
    mapContainer->addChild(Node10);
    MapNodes.push_back({ Node10 });
    //第十一层 被固定为Boss
    MapNode* Node11 = MapNode::create(map4->getPosition() + Vec2(1000, 600), 7);
    Node11->level = 11;
    mapContainer->addChild(Node11);
    MapNodes.push_back({ Node11 });


    // 创建一个 DrawNode 对象
    auto drawNode = DrawNode::create();
    mapContainer->addChild(drawNode);


    // uniqueConnections 是一个集合，用于防止重复连接，存储唯一的节点对
    std::set<std::pair<MapNode*, MapNode*>> uniqueConnections;

    // 遍历每一层节点，确保所有节点都至少有一个有效连接
    for (int level = 0; level < MapNodes.size() - 1; level++) { // 遍历从第0层到倒数第二层
        for (MapNode* current : MapNodes[level]) { // 遍历当前层的每个节点
            bool connectedToNext = false; // 标记当前节点是否已经连接到下一层节点

            // 检查当前节点是否已连接到下一层的任何节点
            for (MapNode* next : MapNodes[level + 1]) {
                if (uniqueConnections.count({ current, next }) > 0) { // 如果连接已存在
                    connectedToNext = true;
                    break;
                }
            }

            // 如果当前节点没有连接到下一层，则随机创建一个连接
            if (!connectedToNext && !MapNodes[level + 1].empty()) { // 确保下一层非空
                MapNode* next = MapNodes[level + 1][rand() % MapNodes[level + 1].size()]; // 随机选择下一层的一个节点
                if (uniqueConnections.insert({ current, next }).second) { // 插入成功说明是新连接
                    current->connectedNodes.push_back(next); // 添加到当前节点的连接列表
                    connections.push_back(new NodeConnection(current, next)); // 存储连接到 connections 容器
                }
            }
        }

        // 确保下一层的每个节点都有至少一个节点连接到它
        for (MapNode* next : MapNodes[level + 1]) { // 遍历下一层的每个节点
            bool hasIncomingConnection = false; // 标记该节点是否有上层连接到它

            // 检查是否有节点连接到当前节点
            for (MapNode* current : MapNodes[level]) {
                if (uniqueConnections.count({ current, next }) > 0) { // 如果连接已存在
                    hasIncomingConnection = true;
                    break;
                }
            }

            // 如果当前节点没有任何连接，则强制创建一个从上层到它的连接
            if (!hasIncomingConnection && !MapNodes[level].empty()) { // 确保上一层非空
                MapNode* current = MapNodes[level][rand() % MapNodes[level].size()]; // 随机选择上一层的一个节点
                if (uniqueConnections.insert({ current, next }).second) { // 插入成功说明是新连接
                    current->connectedNodes.push_back(next); // 添加到当前节点的连接列表
                    connections.push_back(new NodeConnection(current, next)); // 存储连接到 connections 容器
                }
            }
        }
    }

    // 绘制路径并实现渐变色效果
    for (auto con : connections) {
        // 配置渐变色效果的相关参数
        Color4F startColor(1.0f, 1.0f, 1.0f, 0.15f); // 起始颜色，白色，透明度较高
        Color4F endColor(1.0f, 1.0f, 1.0f, 0.05f);   // 终止颜色，白色，透明度较低
        float lineWidth = 20.0f; // 定义路径的线条宽度

        // 使用绘制节点（drawNode）绘制三段渐变色路径
        drawNode->drawSegment(con->start->getPosition(), con->end->getPosition(), lineWidth, startColor); // 主体路径
        drawNode->drawSegment(con->start->getPosition(), con->end->getPosition(), lineWidth, endColor);   // 渐变效果路径
        drawNode->drawSegment(con->start->getPosition(), con->end->getPosition(), lineWidth * 1.5f, Color4F(1.0f, 1.0f, 1.0f, 0.05f)); // 外部光晕
    }

    return true;
}
