#include "cocos2d.h"
#include "EndingScene.h"
#include "AudioPlayer.h"
#include "const.h"
using namespace std;
using namespace cocos2d;

// ������̬�������ֶ�ʵ�� create() ����
EndingScene* EndingScene::create(bool isVictory)
{
    EndingScene* ret = new EndingScene();
    if (ret && ret->init(isVictory)) {
        ret->autorelease();  // ȷ��������ȷ����
        return ret;
    }
    else {
        delete ret;  // �����ʼ��ʧ�ܣ���ɾ������
        return nullptr;
    }
}

// ��ʼ������������ʤ����ʧ����ʾ��ͬ���ı�
bool EndingScene::init(bool isVictory) {
    // ���ű�������
    audioPlayer("Victory.ogg", true);

    // ��������ͼƬ
    auto backgroundSprite = Sprite::create("ending.jpg");  // ȷ������ͼƬ·����ȷ
    backgroundSprite->setPosition(Director::getInstance()->getVisibleSize() / 2);  // ���ñ���λ��Ϊ��Ļ����
    this->addChild(backgroundSprite);  // ��������ӵ�����

    // �����ı���ǩ
    auto label = Label::createWithSystemFont("", "Arial", 48);  // ʹ�ýϴ�����
    label->setPosition(Director::getInstance()->getVisibleSize() / 2);
    label->setColor(Color3B::WHITE);  // ����������ɫΪ��ɫ

    // ����ʤ����ʧ����ʾ��ͬ���ı�
    if (isVictory) {
        label->setString(u8"��ϲ����Ϸʤ��");
    }
    else {
        label->setString(u8"��Ϸʧ��");
    }
    this->addChild(label);  // ���ı���ӵ�����

    // ��ʱ3����л�����
    this->runAction(Sequence::create(
        DelayTime::create(3.0f),        // ��ʱ3��
        CallFunc::create([=]() {
            Director::getInstance()->popToRootScene();  // ���ص�������
            }),
        nullptr));

    return true;
}
