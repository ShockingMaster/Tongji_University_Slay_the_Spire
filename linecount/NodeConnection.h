#pragma once
#ifndef _NODECONNECTION_H_
#define _NODECONNECTION_H_
#include "MapNode.h"



/**
 * @class NodeConnection
 * @brief ��ʾ��ͼ���������֮������ӡ�
 *
 * �������ڱ�ʾ����֮������ӹ�ϵ��ÿ�����Ӱ��������յ�������ͼ��㡣
 */
class NodeConnection {
public:
    MapNode* start; ///< ָ�����ӵ���ʼ���
    MapNode* end;   ///< ָ�����ӵ���ֹ���

    /**
     * @brief ���캯������ʼ��һ��������ӡ�
     *
     * @param s ��ʼ���
     * @param e ��ֹ���
     */
    inline NodeConnection(MapNode* s, MapNode* e) : start(s), end(e) {}
};

#endif // !_NODECONNECTION_H_
