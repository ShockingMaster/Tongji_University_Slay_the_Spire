#pragma once
#ifndef _NODE_H_
#define _NODE_H_

#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

// 节点类型的常量定义
constexpr int Combat = 1;         // 普通战斗节点
constexpr int Elite = 2;          // 精英战斗节点
constexpr int Rest = 3;           // 休息节点
constexpr int Chest = 4;          // 宝箱节点
constexpr int Shop = 5;           // 商店节点
constexpr int UnknownEvent = 6;   // 未知事件节点
constexpr int Boss = 7;           // Boss节点

/**
 * @class MapNode
 * @brief 表示地图上的一个节点对象，包括节点属性、图片资源和交互行为。
 */
class MapNode : public cocos2d::Node {
public:
    // 成员变量
    int level;                     ///< 节点层级
    bool isVisited;                ///< 标记节点是否已被访问
    Vec2 position;                 ///< 节点的位置坐标
    int type;                      ///< 节点的类型（1-7）
    Sprite* sprite;                ///< 与节点绑定的精灵对象
    vector<MapNode*> connectedNodes; ///< 与该节点相连的节点（从低层到高层）

    // 图片路径缓存，用于状态变化
    string normalImage;            ///< 普通状态下的图片路径
    string hoverImage;             ///< 悬停状态下的图片路径
    string clickImage;             ///< 点击状态下的图片路径

    /**
     * @brief 构造函数，初始化成员变量。
     */
    MapNode();

    /**
     * @brief 创建节点对象。
     * @param pos 节点位置。
     * @param type 节点类型。
     * @return 返回一个 `MapNode` 指针，成功创建则返回节点，失败则返回 nullptr。
     */
   static MapNode* create(const Vec2& pos, int type) {

        MapNode* node = new MapNode();
        if (node && node->init(pos, type)) {  // 确保传递了参数
            return node;
        }
        CC_SAFE_DELETE(node);
        return nullptr;
    }
    /**
     * @brief 初始化节点。
     * @param pos 节点位置。
     * @param type 节点类型。
     * @return 初始化成功返回 true，否则返回 false。
     */
    bool init(const Vec2& pos, int type);

    /**
     * @brief 根据节点类型设置图片资源。
     * @param type 节点类型。
     */
    void setSpriteByType(int type);

    /**
     * @brief 精灵被鼠标悬停时触发的反应。
     */
    void onHover();

    /**
     * @brief 精灵被点击时触发的反应。
     */
    void onClick();

    /**
     * @brief 恢复节点为普通状态。
     */
    void onRestore();

    /**
     * @brief 获取节点的类型。
     * @return 返回节点的类型。
     */
    int getType() const;

    /**
     * @brief 获取节点的精灵对象。
     * @return 返回节点绑定的精灵对象。
     */
    Sprite* getSprite() const;

    /**
     * @brief 设置节点访问状态。
     * @param visited 是否已访问。
     */
    void setVisited(bool visited);

    /**
     * @brief 获取节点的访问状态。
     * @return 返回是否已访问。
     */
    bool isNodeVisited() const;

    /**
     * @brief 获取节点的位置。
     * @return 返回节点的坐标。
     */
    virtual cocos2d::Vec2 getPosition();
};

#endif
