#include "MapNode.h"
#include "RestScene.h"
#include "event1.h"
#include "audioPlayer.h"
// ���캯������ʼ�� MapNode �����Ĭ������
MapNode::MapNode()
    : isVisited(false),         // Ĭ��δ����
    type(UnknownEvent),       // �ڵ�����Ĭ��Ϊδ֪�¼�
    sprite(nullptr),          // �������ָ���ʼ��Ϊ��
    normalImage(""),          // Ĭ����ͨ״̬ͼƬΪ��
    hoverImage(""),           // Ĭ����ͣ״̬ͼƬΪ��
    clickImage("")            // Ĭ�ϵ��״̬ͼƬΪ��
{}

// ��ʼ���ڵ�
bool MapNode::init(const Vec2& pos, int type) {
    // ���ýڵ��λ�ú�����
    this->position = pos;
    this->type = type;

    // ���ݽڵ��������ö�Ӧ�ľ���
    this->setSpriteByType(type);

    // ��ʼ���ڵ�Ϊδ����״̬
    this->isVisited = false;

    // ��������¼�������
    auto listener = EventListenerMouse::create();

    // ����ƶ��¼�����
    listener->onMouseMove = [this](Event* event) {
        // �����λ��ת��Ϊ�ڵ�����ϵ
        auto mouseEvent = static_cast<EventMouse*>(event);
        Vec2 mousePos = this->convertToNodeSpace(mouseEvent->getLocationInView());

        // �ж�����Ƿ���ͣ�ڽڵ���
        bool isHovered = false;
        for (auto& child : this->getChildren()) {
            if (child->getBoundingBox().containsPoint(mousePos)) {
                isHovered = true;
                break;
            }
        }

        // �������״̬�ı侫��ͼ��
        if (isHovered) {
            onHover();  // �����ͣ����ʾ��ͣͼ��
        }
        else {
            onRestore();  // ����뿪���ָ�����ͼ��
        }
        };

    // ��갴���¼�����
    listener->onMouseDown = [this](Event* event) {
        // �����λ��ת��Ϊ�ڵ�����ϵ
        auto mouseEvent = static_cast<EventMouse*>(event);
        Vec2 mousePos = this->convertToNodeSpace(mouseEvent->getLocationInView());

        // �ж�����Ƿ����˽ڵ�
        for (auto& child : this->getChildren()) {
            if (child->getBoundingBox().containsPoint(mousePos)) {
                onClick();  // ���õ���¼�������
            }
        }
        };

    // ����ͷ��¼������ɸ���������չ��
    listener->onMouseUp = [this](Event* event) {
        // ������ͷ�ʱ������������߼�
        };

    // ������¼�������ע�ᵽ�¼��ַ���
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true; // ���س�ʼ���ɹ�
}

// ���ݽڵ��������ö�Ӧ�ľ���ͼ��
void MapNode::setSpriteByType(int type) {
    // ���ݽڵ��������þ������ͨ����ͣ�͵��״̬ͼƬ
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

    // ������о��飬���Ƴ��ɾ���
    if (sprite) {
        sprite->removeFromParent();
        sprite = nullptr;
    }

    // �����µľ��鲢����Ϊ��ǰ�ڵ���ӽڵ�
    sprite = Sprite::create(normalImage);
    if (!sprite) {
        CCLOG("Failed to create sprite for MapNode!"); // ��������ʧ��ʱ��¼��־
        return;
    }

    sprite->setScale(1.5f); // ���þ�������ű���
    if (isVisited) {
        
        sprite->setColor(Color3B(165, 42, 42));
    }
    sprite->setPosition(position); // ���þ����λ��
    this->addChild(sprite); // ��Ӿ��鵽�ڵ�
}

// �����ͣʱ���¼�����
void MapNode::onHover() {
    if (hoverImage != "" && sprite) {
        sprite->setTexture(hoverImage); // �л�����ͣͼ��
    }
}

// ����뿪ʱ���¼�����
void MapNode::onRestore() {
    if (normalImage != "" && sprite) {
        sprite->setTexture(normalImage); // �ָ�����ͨͼ��
    }
}

extern vector<MapNode*> visitPath;
extern int currentLevel;
// �����ʱ���¼�����
void MapNode::onClick() {

    if (currentLevel == 1&&this->level==1) {
        this->setVisited(true); // ��ǵ�ǰ�ڵ��ѷ���
        visitPath.push_back(this);
        sprite->setColor(Color3B(165, 42, 42));
        if (type == Rest) {
            this->scheduleOnce([](float dt) {
                auto nextScene = RestScene::createScene(); // ����Ŀ�곡��
                Director::getInstance()->pushScene(TransitionFade::create(0.5f, nextScene));
                }, 1.0f, "LoadNextScene");
        }
        if (type == UnknownEvent) {
            this->scheduleOnce([](float dt) {
                auto nextScene = event1::createScene(); // ����Ŀ�곡��
                Director::getInstance()->pushScene(TransitionFade::create(0.5f, nextScene));
                }, 1.0f, "LoadNextScene");
        }
        currentLevel++;
        return;
    }
  
    // �жϵ�ǰ�ڵ�� level �Ƿ���� currentLevel��������ȣ�����
    if (this->level != currentLevel) {
        return;
    }
    

    // �����·����pathVector������·�������һ���ڵ�� isConnected ������ǰ�ڵ�
    if (!visitPath.empty()) {
        
        // ��ȡ·�������һ���ڵ�
        MapNode* lastNode = visitPath.back();

        // ���ýڵ�� isConnected ���Ƿ������ǰ�ڵ�
        bool isConnected = false;
        for (auto& connectedNode : lastNode->connectedNodes){
            if (connectedNode == this) {
                isConnected = true;
                break;
            }
        }

        if (isConnected) {
            this->setVisited(true); // ��ǵ�ǰ�ڵ��ѷ���
            visitPath.push_back(this);
            sprite->setColor(Color3B(165,42,42));  
            CCLOG("Node clicked! Type: %d", type); // ����ڵ�������־��Ϣ
            audioPlayer("clickSoundEffect.mp3", false);

            if (type == Rest) {
                this->scheduleOnce([](float dt) {
                    auto nextScene = RestScene::createScene(); // ����Ŀ�곡��
                    Director::getInstance()->pushScene(TransitionFade::create(0.5f, nextScene));
                    }, 1.0f, "LoadNextScene");
            }
            if (type == UnknownEvent) {
                this->scheduleOnce([](float dt) {
                    auto nextScene = event1::createScene(); // ����Ŀ�곡��
                    Director::getInstance()->pushScene(TransitionFade::create(0.5f, nextScene));
                    }, 1.0f, "LoadNextScene");
            }
            currentLevel++;
        }
    }
    
    
}

// ��ȡ�ڵ�λ��
Vec2 MapNode::getPosition() {
    return position; // ���ؽڵ��λ����Ϣ
}

// ��ȡ�ڵ�����
int MapNode::getType() const {
    return type; // ���ؽڵ������
}

// ��ȡ�ڵ�ľ������
Sprite* MapNode::getSprite() const {
    return sprite; // ���ؽڵ�ľ������ָ��
}

// ���ýڵ�ķ���״̬
void MapNode::setVisited(bool visited) {
    isVisited = visited; // ���÷���״̬
}

// �жϽڵ��Ƿ��ѱ�����
bool MapNode::isNodeVisited() const {
    return isVisited; // ���ط���״̬
}
