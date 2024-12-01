#include "MapNode.h"
#include "RestScene.h"
#include "event1.h"
#include "audioPlayer.h"
// 构造函数，初始化 MapNode 对象的默认属性
MapNode::MapNode()
    : isVisited(false),         // 默认未访问
    type(UnknownEvent),       // 节点类型默认为未知事件
    sprite(nullptr),          // 精灵对象指针初始化为空
    normalImage(""),          // 默认普通状态图片为空
    hoverImage(""),           // 默认悬停状态图片为空
    clickImage("")            // 默认点击状态图片为空
{}

// 初始化节点
bool MapNode::init(const Vec2& pos, int type) {
    // 设置节点的位置和类型
    this->position = pos;
    this->type = type;

    // 根据节点类型设置对应的精灵
    this->setSpriteByType(type);

    // 初始化节点为未访问状态
    this->isVisited = false;

    // 创建鼠标事件监听器
    auto listener = EventListenerMouse::create();

    // 鼠标移动事件处理
    listener->onMouseMove = [this](Event* event) {
        // 将鼠标位置转换为节点坐标系
        auto mouseEvent = static_cast<EventMouse*>(event);
        Vec2 mousePos = this->convertToNodeSpace(mouseEvent->getLocationInView());

        // 判断鼠标是否悬停在节点上
        bool isHovered = false;
        for (auto& child : this->getChildren()) {
            if (child->getBoundingBox().containsPoint(mousePos)) {
                isHovered = true;
                break;
            }
        }

        // 根据鼠标状态改变精灵图像
        if (isHovered) {
            onHover();  // 鼠标悬停，显示悬停图像
        }
        else {
            onRestore();  // 鼠标离开，恢复正常图像
        }
        };

    // 鼠标按下事件处理
    listener->onMouseDown = [this](Event* event) {
        // 将鼠标位置转换为节点坐标系
        auto mouseEvent = static_cast<EventMouse*>(event);
        Vec2 mousePos = this->convertToNodeSpace(mouseEvent->getLocationInView());

        // 判断鼠标是否点击了节点
        for (auto& child : this->getChildren()) {
            if (child->getBoundingBox().containsPoint(mousePos)) {
                onClick();  // 调用点击事件处理函数
            }
        }
        };

    // 鼠标释放事件处理（可根据需求扩展）
    listener->onMouseUp = [this](Event* event) {
        // 在鼠标释放时，可添加其他逻辑
        };

    // 将鼠标事件监听器注册到事件分发器
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true; // 返回初始化成功
}

// 根据节点类型设置对应的精灵图像
void MapNode::setSpriteByType(int type) {
    // 根据节点类型设置精灵的普通、悬停和点击状态图片
    switch (type) {
        case Combat:
            normalImage = "combat_normal.png";
            hoverImage = "combat_hover.png";
            clickImage = "combat_normal.png";
            break;
        case Elite:
            normalImage = "elite_normal.png";
            hoverImage = "elite_hover.png";
            clickImage = "elite_normal.png";
            break;
        case Rest:
            normalImage = "rest_normal.png";
            hoverImage = "rest_hover.png";
            clickImage = "rest_normal.png";
            break;
        case Shop:
            normalImage = "shop_normal.png";
            hoverImage = "shop_hover.png";
            clickImage = "shop_normal.png";
            break;
        case Boss:
            normalImage = "boss_normal.png";
            hoverImage = "boss_hover.png";
            clickImage = "boss_normal.png";
            break;
        case Chest:
            normalImage = "chest_normal.png";
            hoverImage = "chest_hover.png";
            clickImage = "chest_normal.png";
            break;
        default:
            normalImage = "unknown_normal.png";
            hoverImage = "unknown_hover.png";
            clickImage = "unknown_normal.png";
            break;
    }

    // 如果已有精灵，先移除旧精灵
    if (sprite) {
        sprite->removeFromParent();
        sprite = nullptr;
    }

    // 创建新的精灵并设置为当前节点的子节点
    sprite = Sprite::create(normalImage);
    if (!sprite) {
        CCLOG("Failed to create sprite for MapNode!"); // 创建精灵失败时记录日志
        return;
    }

    sprite->setScale(1.5f); // 设置精灵的缩放比例
    if (isVisited) {
        
        sprite->setColor(Color3B(165, 42, 42));
    }
    sprite->setPosition(position); // 设置精灵的位置
    this->addChild(sprite); // 添加精灵到节点
}

// 鼠标悬停时的事件处理
void MapNode::onHover() {
    if (hoverImage != "" && sprite) {
        sprite->setTexture(hoverImage); // 切换到悬停图像
    }
}

// 鼠标离开时的事件处理
void MapNode::onRestore() {
    if (normalImage != "" && sprite) {
        sprite->setTexture(normalImage); // 恢复到普通图像
    }
}

extern vector<MapNode*> visitPath;
extern int currentLevel;
// 鼠标点击时的事件处理
void MapNode::onClick() {

    if (currentLevel == 1&&this->level==1) {
        this->setVisited(true); // 标记当前节点已访问
        visitPath.push_back(this);
        sprite->setColor(Color3B(165, 42, 42));
        if (type == Rest) {
            this->scheduleOnce([](float dt) {
                auto nextScene = RestScene::createScene(); // 创建目标场景
                Director::getInstance()->pushScene(TransitionFade::create(0.5f, nextScene));
                }, 1.0f, "LoadNextScene");
        }
        if (type == UnknownEvent) {
            this->scheduleOnce([](float dt) {
                auto nextScene = event1::createScene(); // 创建目标场景
                Director::getInstance()->pushScene(TransitionFade::create(0.5f, nextScene));
                }, 1.0f, "LoadNextScene");
        }
        currentLevel++;
        return;
    }
  
    // 判断当前节点的 level 是否等于 currentLevel，如果不等，返回
    if (this->level != currentLevel) {
        return;
    }
    

    // 如果有路径（pathVector）并且路径中最后一个节点的 isConnected 包含当前节点
    if (!visitPath.empty()) {
        
        // 获取路径中最后一个节点
        MapNode* lastNode = visitPath.back();

        // 检查该节点的 isConnected 中是否包含当前节点
        bool isConnected = false;
        for (auto& connectedNode : lastNode->connectedNodes){
            if (connectedNode == this) {
                isConnected = true;
                break;
            }
        }

        if (isConnected) {
            this->setVisited(true); // 标记当前节点已访问
            visitPath.push_back(this);
            sprite->setColor(Color3B(165,42,42));  
            CCLOG("Node clicked! Type: %d", type); // 输出节点点击的日志信息
            audioPlayer("clickSoundEffect.mp3", false);

            if (type == Rest) {
                this->scheduleOnce([](float dt) {
                    auto nextScene = RestScene::createScene(); // 创建目标场景
                    Director::getInstance()->pushScene(TransitionFade::create(0.5f, nextScene));
                    }, 1.0f, "LoadNextScene");
            }
            if (type == UnknownEvent) {
                this->scheduleOnce([](float dt) {
                    auto nextScene = event1::createScene(); // 创建目标场景
                    Director::getInstance()->pushScene(TransitionFade::create(0.5f, nextScene));
                    }, 1.0f, "LoadNextScene");
            }
            currentLevel++;
        }
    }
    
    
}

// 获取节点位置
Vec2 MapNode::getPosition() {
    return position; // 返回节点的位置信息
}

// 获取节点类型
int MapNode::getType() const {
    return type; // 返回节点的类型
}

// 获取节点的精灵对象
Sprite* MapNode::getSprite() const {
    return sprite; // 返回节点的精灵对象指针
}

// 设置节点的访问状态
void MapNode::setVisited(bool visited) {
    isVisited = visited; // 设置访问状态
}

// 判断节点是否已被访问
bool MapNode::isNodeVisited() const {
    return isVisited; // 返回访问状态
}
