#pragma once
#ifndef _NODE_H_
#define _NODE_H_

#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

// �ڵ����͵ĳ�������
constexpr int Combat = 1;         // ��ͨս���ڵ�
constexpr int Elite = 2;          // ��Ӣս���ڵ�
constexpr int Rest = 3;           // ��Ϣ�ڵ�
constexpr int Chest = 4;          // ����ڵ�
constexpr int Shop = 5;           // �̵�ڵ�
constexpr int UnknownEvent = 6;   // δ֪�¼��ڵ�
constexpr int Boss = 7;           // Boss�ڵ�

/**
 * @class MapNode
 * @brief ��ʾ��ͼ�ϵ�һ���ڵ���󣬰����ڵ����ԡ�ͼƬ��Դ�ͽ�����Ϊ��
 */
class MapNode : public cocos2d::Node {
public:
    // ��Ա����
    int level;                     ///< �ڵ�㼶
    bool isVisited;                ///< ��ǽڵ��Ƿ��ѱ�����
    Vec2 position;                 ///< �ڵ��λ������
    int type;                      ///< �ڵ�����ͣ�1-7��
    Sprite* sprite;                ///< ��ڵ�󶨵ľ������
    vector<MapNode*> connectedNodes; ///< ��ýڵ������Ľڵ㣨�ӵͲ㵽�߲㣩

    // ͼƬ·�����棬����״̬�仯
    string normalImage;            ///< ��ͨ״̬�µ�ͼƬ·��
    string hoverImage;             ///< ��ͣ״̬�µ�ͼƬ·��
    string clickImage;             ///< ���״̬�µ�ͼƬ·��

    /**
     * @brief ���캯������ʼ����Ա������
     */
    MapNode();

    /**
     * @brief �����ڵ����
     * @param pos �ڵ�λ�á�
     * @param type �ڵ����͡�
     * @return ����һ�� `MapNode` ָ�룬�ɹ������򷵻ؽڵ㣬ʧ���򷵻� nullptr��
     */
   static MapNode* create(const Vec2& pos, int type) {

        MapNode* node = new MapNode();
        if (node && node->init(pos, type)) {  // ȷ�������˲���
            return node;
        }
        CC_SAFE_DELETE(node);
        return nullptr;
    }
    /**
     * @brief ��ʼ���ڵ㡣
     * @param pos �ڵ�λ�á�
     * @param type �ڵ����͡�
     * @return ��ʼ���ɹ����� true�����򷵻� false��
     */
    bool init(const Vec2& pos, int type);

    /**
     * @brief ���ݽڵ���������ͼƬ��Դ��
     * @param type �ڵ����͡�
     */
    void setSpriteByType(int type);

    /**
     * @brief ���鱻�����ͣʱ�����ķ�Ӧ��
     */
    void onHover();

    /**
     * @brief ���鱻���ʱ�����ķ�Ӧ��
     */
    void onClick();

    /**
     * @brief �ָ��ڵ�Ϊ��ͨ״̬��
     */
    void onRestore();

    /**
     * @brief ��ȡ�ڵ�����͡�
     * @return ���ؽڵ�����͡�
     */
    int getType() const;

    /**
     * @brief ��ȡ�ڵ�ľ������
     * @return ���ؽڵ�󶨵ľ������
     */
    Sprite* getSprite() const;

    /**
     * @brief ���ýڵ����״̬��
     * @param visited �Ƿ��ѷ��ʡ�
     */
    void setVisited(bool visited);

    /**
     * @brief ��ȡ�ڵ�ķ���״̬��
     * @return �����Ƿ��ѷ��ʡ�
     */
    bool isNodeVisited() const;

    /**
     * @brief ��ȡ�ڵ��λ�á�
     * @return ���ؽڵ�����ꡣ
     */
    virtual cocos2d::Vec2 getPosition();
};

#endif
