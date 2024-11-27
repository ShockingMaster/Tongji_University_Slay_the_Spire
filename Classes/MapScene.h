#pragma once
#ifndef _MAP_SCENE_H_
#define _MAP_SCENE_H_

#include "MapNode.h"           // ����MapNode�࣬���ڵ�ͼ�ڵ�Ķ���
#include "cocos2d.h"           // ����Cocos2d����ĺ��Ĺ���
#include "NodeConnection.h"    // ����NodeConnection�࣬���ڶ���ڵ������ӹ�ϵ

using namespace std;           // ʹ�ñ�׼�������ռ�
using namespace cocos2d;       // ʹ��Cocos2d�����ռ�


/**
 * MapScene �ࣺ�̳���cocos2d::Scene�����ڹ����ͼ�����ͽڵ㡣
 * ���𴴽���ͼ������ڵ����Ӽ���¼��ɫ���߹�·���ȹ��ܡ�
 */
class MapScene : public cocos2d::Scene {
public:
    
    vector<vector<MapNode*>> MapNodes;    // �洢������ɵĵ�ͼ��㣬һ����άvector��ÿһ������ò������MapNode�ڵ㡣

    vector<NodeConnection*> connections;  // �洢��ͼ�н��֮������ӹ�ϵ����¼�ڵ���·�����ӣ�NodeConnection��������������ͼ����ͨ�ԡ�

    /**
     * createScene: ��̬����������������һ���µ� MapScene ������
     * ���ڴ�����ͼ���������ظó�����ʵ����ͨ�����ڳ������л����ʼ����
     *
     * @return ����һ���µ� MapScene ����ʵ����
     */
    static cocos2d::Scene* createScene();

    /**
     * init: ��ʼ�����������ڳ����ĳ�ʼ�����á�
     * �����ʼ����ͼ�ڵ㡢���ýڵ����ӡ����ó����ĳ�ʼ״̬�ȡ�
     *
     * @return ���س�ʼ���Ƿ�ɹ���
     */
    virtual bool init();

    /**
     * CREATE_FUNC(MapScene): �꣬�Զ�����һ����̬����������
     * �����Զ����� `new MapScene()` �� `init()` �����������ͳ�ʼ��һ���µ� MapScene ����
     * ����Cocos2d�ṩ�ı�ݷ�ʽ�������ֶ���д�ظ��Ĵ��롣
     */
    CREATE_FUNC(MapScene);

};

#endif
