#include "SettingsScene.h"
#include "HoverButton.h"
#include "MenuScene.h"
#include "AudioEngine.h"
#include "const.h"
#include "AudioPlayer.h"

// �����ռ�
using namespace cocos2d;

int backgroundMusicId = 0;         ///< �������� ID
float backgroundVolumn = 0.3f;      ///< ������������
int soundEffectId = 0;             ///< ��Ч ID
float soundEffectVolumn = 0.3f;     ///< ��Ч����


// ��������
Scene* SettingsScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SettingsScene::create();
    scene->addChild(layer);
    return scene;
}


// ��ʼ������
bool SettingsScene::init()
{
    // ��������
    if (!Scene::init()) {
        return false;
    }

    // ��������
    audioPlayer("start.ogg", true);

    // ���ر���
    const auto screenSize = cocos2d::Director::getInstance()->getVisibleSize();
    const auto background = Sprite::create("scene2.jfif");
    background->setPosition(Vec2(screenSize.width / 2+50, screenSize.height / 2 + 10));
    this->addChild(background);

    // ����˵������
    auto bgMusicLabel = Label::createWithSystemFont(u8"������������", "Arial", 50);
    bgMusicLabel->setPosition(Vec2(450, 540));  // ����λ��
    this->addChild(bgMusicLabel);

    auto soundEffectLabel = Label::createWithSystemFont(u8"��Ч����", "Arial", 50);
    soundEffectLabel->setPosition(Vec2(450, 250));  // ����λ��
    this->addChild(soundEffectLabel);

    // ������0���͡�100����˵������
    auto zeroLabel1 = Label::createWithSystemFont("0", "Arial", 50);
    zeroLabel1->setPosition(Vec2(200, 500));  // ����λ��
    this->addChild(zeroLabel1);

    auto hundredLabel1 = Label::createWithSystemFont("100", "Arial", 50);
    hundredLabel1->setPosition(Vec2(750, 500));  // ����λ��
    this->addChild(hundredLabel1);

    auto zeroLabel2 = Label::createWithSystemFont("0", "Arial", 50);
    zeroLabel2->setPosition(Vec2(200, 200));  // ����λ��
    this->addChild(zeroLabel2);

    auto hundredLabel2 = Label::createWithSystemFont("100", "Arial", 50);
    hundredLabel2->setPosition(Vec2(750, 200));  // ����λ��
    this->addChild(hundredLabel2);

    // ����������
    auto backgroundMusicVolumnSlider = cocos2d::ui::Slider::create();
    backgroundMusicVolumnSlider->loadBarTexture("SliderBackground.png");
    backgroundMusicVolumnSlider->loadSlidBallTextures("SliderNodeNormal.png", "SliderNodePress.png", "SliderNodeDisable.png");
    backgroundMusicVolumnSlider->loadProgressBarTexture("SliderBar.png");
    backgroundMusicVolumnSlider->setPosition(cocos2d::Vec2(450, 500));
    backgroundMusicVolumnSlider->setPercent(backgroundVolumn * 100);

    auto effectVolumnslider = cocos2d::ui::Slider::create();
    effectVolumnslider->loadBarTexture("SliderBackground.png");
    effectVolumnslider->loadSlidBallTextures("SliderNodeNormal.png", "SliderNodePress.png", "SliderNodeDisable.png");
    effectVolumnslider->loadProgressBarTexture("SliderBar.png");
    effectVolumnslider->setPosition(cocos2d::Vec2(450, 200));
    effectVolumnslider->setPercent(soundEffectVolumn * 100);

    // Ϊ����������¼�������
    backgroundMusicVolumnSlider->addEventListener([=](Ref* sender, cocos2d::ui::Slider::EventType type) {
        if (type == cocos2d::ui::Slider::EventType::ON_SLIDEBALL_DOWN) {
            // ���ص����Ч
            audioPlayer("ClickSoundEffect.mp3", false);
        }
        if (type == cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
            const cocos2d::ui::Slider* slider = dynamic_cast<cocos2d::ui::Slider*>(sender);
            const float percent = slider->getPercent();
            backgroundVolumn = percent / 100.0f;
            // ���ñ�����������
            cocos2d::experimental::AudioEngine::setVolume(backgroundMusicId, backgroundVolumn);
            audioPlayer("start.ogg", true);
        }
        });

    effectVolumnslider->addEventListener([=](Ref* sender, cocos2d::ui::Slider::EventType type) {
        if (type == cocos2d::ui::Slider::EventType::ON_SLIDEBALL_DOWN) {
            // ���ص����Ч
            audioPlayer("ClickSoundEffect.mp3", false);
        }
        if (type == cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
            const cocos2d::ui::Slider* slider = dynamic_cast<cocos2d::ui::Slider*>(sender);
            const float percent = slider->getPercent();
            soundEffectVolumn = percent / 100.0f;
            // ������Ч����
            cocos2d::experimental::AudioEngine::setVolume(soundEffectId, soundEffectVolumn);
        }
        });

    // �����������������
    this->addChild(backgroundMusicVolumnSlider);
    this->addChild(effectVolumnslider);

    // �������ذ�ť��HoverButton��
    auto backButton = HoverButton::create("button4(1).png", "button4(2).png", "button4(1).png");
    backButton->setPosition(Vec2(1250, 100));  // ���ð�ťλ��
    backButton->setTitleText(u8"����");
    backButton->setTitleFontSize(50);
    backButton->addClickEventListener([=](Ref* sender) {
        // ��ť���ʱ���� MenuScene
        Director::getInstance()->replaceScene(MenuScene::createScene());
        });

    this->addChild(backButton);

    return true;
}
