#pragma once
#ifndef _NODECONNECTION_H_
#define _NODECONNECTION_H_
#include "MapNode.h"



/**
 * @class NodeConnection
 * @brief 表示地图中两个结点之间的连接。
 *
 * 该类用于表示两点之间的连接关系，每个连接包含起点和终点两个地图结点。
 */
class NodeConnection {
public:
    MapNode* start; ///< 指向连接的起始结点
    MapNode* end;   ///< 指向连接的终止结点

    /**
     * @brief 构造函数，初始化一个结点连接。
     *
     * @param s 起始结点
     * @param e 终止结点
     */
    inline NodeConnection(MapNode* s, MapNode* e) : start(s), end(e) {}
};

#endif // !_NODECONNECTION_H_
