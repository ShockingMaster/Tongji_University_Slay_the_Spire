#include "ui/CocosGUI.h"
#include "const.h"
#include "AudioPlayer.h"
#include "StartScene.h"
#include "InitialScene.h"
#include "HelloWorldScene.h"

using namespace std;
using namespace cocos2d;

// ȫ�ֱ�������
int backgroundMusicId = -1;         ///< �������� ID
float backgroundVolumn = 0.5;      ///< ������������
int soundEffectId =-1;             ///< ��Ч ID
float soundEffectVolumn = 0.5;     ///< ��Ч����

/**
 * @brief ���� StartScene ������
 *
 * @return cocos2d::Scene* ���� StartScene �ĳ���ʵ����
 */
Scene* StartScene::createScene() {
    auto scene = Scene::create();          // ����һ���հ׳���
    auto layer = StartScene::create();     // ���� StartScene ��
    scene->addChild(layer);                // ������ӵ�����
    return scene;
}

/**
 * @brief ��ʼ�� StartScene ������
 *
 * ���ر���ͼƬ�����ű������֡����ü��ؽ������Լ���ת����һ������
 *
 * @return bool ��ʼ���Ƿ�ɹ���
 */
bool StartScene::init() {
    // ȷ������ Scene ��ʼ���ɹ�
    if (!Scene::init()) {
        return false;
    }

    // ���ű�������
    audioPlayer("../Resources/start.ogg", true);

    // ��ȡ��Ļ�ߴ�
    const auto screenSize = cocos2d::Director::getInstance()->getVisibleSize();

    // ���ر���ͼƬ
    const auto background0 = Sprite::create("title4.png");
    background0->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2));
    this->addChild(background0);

    const auto background1 = Sprite::create("title5.png");
    background1->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2));
    this->addChild(background1);

    const auto background2 = Sprite::create("scene0.jfif");

    // �������ű���������������С
    float scaleX = screenSize.width / background2->getContentSize().width;
    float scaleY = screenSize.height / background2->getContentSize().height;
    float scale = std::min(scaleX, scaleY); // ȷ������������Ļ
    background2->setScale(scale);
    background2->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2 + 200));
    this->addChild(background2);

    // �������ؽ�����
    auto progressBar = cocos2d::ui::LoadingBar::create("StartupLoadingBar.png");
    progressBar->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2 - 200));
    progressBar->setPercent(0); // ��ʼΪ 0%
    this->addChild(progressBar);

    // �������ؽ����ı�
    auto progressLabel = Label::createWithTTF("0%", "../Resources/Fonts/arial.ttf", 50);
    progressLabel->setPosition(Vec2(progressBar->getPosition().x, progressBar->getPosition().y));
    progressLabel->setVisible(true);
    this->addChild(progressLabel);

    // ������������¼��
    constexpr float interval = 2.0 / 100.0f; // ÿ���� 1% ����ʱ��

    // ���½�����
    for (int i = 0; i <= 100; i++) {
        this->scheduleOnce([progressBar, progressLabel, i](float dt) {
            progressLabel->setVisible(i >= 10); // 10% ����ʾ��������
            progressBar->setPercent(i);         // ���ý������ٷֱ�
            progressLabel->setString(std::to_string(i) + "%");

            // ��̬�����������ֵ�λ��
            float percentage = progressBar->getPercent() / 100.0f;
            float xPosition = progressBar->getPosition().x - progressBar->getContentSize().width / 2
                + progressBar->getContentSize().width * percentage - 30;
            progressLabel->setPosition(Vec2(xPosition, progressBar->getPosition().y));

        
            }, interval * i, "UpdateStartupLoadingBar" + std::to_string(i));
    }

    // ��ת����һ����
    this->scheduleOnce([](float dt) {
        auto nextScene = InitialScene::createScene(); // ����Ŀ�곡��
        Director::getInstance()->pushScene(TransitionFade::create(1.0f, nextScene)); // ʹ�� 1 �뵭��Ч��
        }, interval * 200, "LoadNextScene");

    return true;
}
