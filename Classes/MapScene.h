#pragma once
#ifndef _MAP_SCENE_H_
#define _MAP_SCENE_H_

#include "MapNode.h"           // 引入MapNode类，用于地图节点的定义
#include "cocos2d.h"           // 引入Cocos2d引擎的核心功能
#include "NodeConnection.h"    // 引入NodeConnection类，用于定义节点间的连接关系

using namespace std;           // 使用标准库命名空间
using namespace cocos2d;       // 使用Cocos2d命名空间


/**
 * MapScene 类：继承自cocos2d::Scene，用于管理地图场景和节点。
 * 负责创建地图、处理节点连接及记录角色的走过路径等功能。
 */
class MapScene : public cocos2d::Scene {
public:
    
    vector<vector<MapNode*>> MapNodes;    // 存储随机生成的地图结点，一个二维vector，每一层包含该层的所有MapNode节点。

    vector<NodeConnection*> connections;  // 存储地图中结点之间的连接关系，记录节点间的路径连接（NodeConnection），帮助构建地图的连通性。

    /**
     * createScene: 静态函数，创建并返回一个新的 MapScene 场景。
     * 用于创建地图场景并返回该场景的实例，通常用于场景的切换或初始化。
     *
     * @return 返回一个新的 MapScene 场景实例。
     */
    static cocos2d::Scene* createScene();

    /**
     * init: 初始化函数，用于场景的初始化设置。
     * 负责初始化地图节点、设置节点连接、配置场景的初始状态等。
     *
     * @return 返回初始化是否成功。
     */
    virtual bool init();

    /**
     * CREATE_FUNC(MapScene): 宏，自动生成一个静态创建函数。
     * 它会自动调用 `new MapScene()` 和 `init()` 函数来创建和初始化一个新的 MapScene 对象。
     * 这是Cocos2d提供的便捷方式，避免手动编写重复的代码。
     */
    CREATE_FUNC(MapScene);

};

#endif
