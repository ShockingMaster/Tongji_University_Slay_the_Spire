#include "SelectionScene.h"
#include "MenuScene.h"
#include "HoverButton.h"
#include "MapScene.h"
#include "enum.h"
#include "AudioPlayer.h"
#include <locale>
#include <codecvt>


using namespace std;
using namespace cocos2d;

Scene* SelectionScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SelectionScene::create();
    scene->addChild(layer);
    return scene;
}
bool SelectionScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    // ������ɫ����
    auto background = LayerColor::create(Color4B(0, 0, 0, 255)); // ��ɫ����
    this->addChild(background);

    // ��������ͼƬ�� Sprite
    vector<Sprite*> images;
    images.push_back(Sprite::create("circle1.png"));
    images.push_back(Sprite::create("circle2.png"));
    images.push_back(Sprite::create("circle3.png"));
    images.push_back(Sprite::create("circle4.png"));
    images.push_back(Sprite::create("circle5.png"));

    // ������ʾ�ı���Label
    auto label = Label::createWithSystemFont(u8"�����ͼ������...", "Arial", 60);
    label->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2 - 300));
    label->setTextColor(Color4B::WHITE); // ����������ɫΪ��ɫ
    this->addChild(label);

    // ��ÿ��ͼƬ����Ϊ����Ļ����λ��
    for (auto& image : images) {
        image->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2-100));
        image->setVisible(false);  // ��ʼʱ����ʾ
        this->addChild(image);
    }

    // ��ʾ��һ��ͼƬ
    images[0]->setVisible(true);

    // ����һ����ʱ����ÿ��һ��ʱ���л�ͼƬ
    int currentImageIndex = 0;
    schedule([=](float deltaTime) mutable {
        // ���ص�ǰͼƬ
        images[currentImageIndex]->setVisible(false);

        // ��ʾ��һ��ͼƬ
        currentImageIndex = (currentImageIndex + 1) % images.size();
        images[currentImageIndex]->setVisible(true);
        }, 0.5f, "image_switch_key");

    // ʹ�õ������ӳ������ֹͣͼƬ�л�
    scheduleOnce([=](float deltaTime) {
        unschedule("image_switch_key");  // ֹͣ��ʱ��
        }, 2.0f, "stop_image_switch");

    // �ӳ�2���ʼ����
    auto fadeOut = FadeOut::create(2.0f);
    auto delay = DelayTime::create(2.0f);
    auto sequence = Sequence::create(delay, fadeOut, RemoveSelf::create(), nullptr); // �ӳ�+����+�Ƴ�
    label->runAction(sequence);  // ΪlabelӦ�ö���
    background->runAction(Sequence::create(delay, fadeOut->clone(), nullptr));

    // ȷ���ڵ������������л�����
    scheduleOnce([=](float dt) {
        auto nextScene = MapScene::createScene();
        Director::getInstance()->pushScene(TransitionFade::create(1.0f, nextScene));  // ʹ��1��ĵ������Ч��
        }, 2.0f, "LoadNextScene");

    return true;
}
