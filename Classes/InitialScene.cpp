#include <locale>
#include <codecvt>
#include "InitialScene.h"
#include "HoverButton.h"
#include "const.h"
#include "AudioPlayer.h"
#include "HelloWorldScene.h"
#include "MenuScene.h"

// ��ɫ��ɫ����
constexpr int WHITE_R = 255; // ��ɫ R ͨ��
constexpr int WHITE_G = 255; // ��ɫ G ͨ��
constexpr int WHITE_B = 255; // ��ɫ B ͨ��

// ��������
constexpr char START_SCENE_BACKGROUND[] = "scene1.jpg";              // ����ͼƬ·��
constexpr char INPUT_FONT[] = "../Resources/Fonts/FangZhengZhaoGeYuan.ttf"; // �����ļ�·��
constexpr char START_MUSIC[] = "../Resources/start.ogg";             // ��������·��
constexpr char CLICK_SOUND[] = "ClickSoundEffect.mp3";               // �����Ч·��

using namespace std;
using namespace cocos2d;

// ȫ�ֱ��������ڱ�������ǳ�
string PlayerName;

// ������������
Scene* InitialScene::createScene()
{
    // ����һ����������
    auto scene = Scene::create();

    // ����һ���㣨��ǰ���ʵ������������ӵ�����
    auto layer = InitialScene::create();
    scene->addChild(layer);

    return scene;
}

// ��ʼ����������
bool InitialScene::init()
{
    // ȷ�������ʼ���ɹ�
    if (!Scene::init()) {
        return false;
    }

    // ���ű�������
    audioPlayer(START_MUSIC, true);

    // ��ȡ��Ļ�ߴ�
    const auto screenSize = cocos2d::Director::getInstance()->getVisibleSize();

    // ���������ñ���ͼƬ
    const auto background = Sprite::create(START_SCENE_BACKGROUND);
    background->setPosition(Vec2(screenSize.width / 2 + 50, screenSize.height / 2 - 200));
    this->addChild(background);

    // �����ı��������û������ǳ�
    auto textField = cocos2d::ui::TextField::create(
        u8"�������������Ϸ�ǳ�", INPUT_FONT, 80); // Ĭ����ʾ����
    textField->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2 + 300));
    textField->setMaxLength(20);  // ������󳤶�
    textField->setOpacity(150);   // ����͸����
    textField->setMaxLengthEnabled(true); // ������������
    textField->setTextColor(cocos2d::Color4B(WHITE_R, WHITE_G, WHITE_B, 255));
    this->addChild(textField);

    // ������ʾ��ǩ
    auto promptLabel = Label::createWithTTF("", INPUT_FONT, 80);
    promptLabel->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2 + 150));
    promptLabel->setOpacity(150);
    promptLabel->setTextColor(cocos2d::Color4B(WHITE_R, WHITE_G, WHITE_B, 255));
    this->addChild(promptLabel);

    // �����ı������ݱ仯�¼�
    PlayerName = textField->getString();
    textField->addEventListener([promptLabel](Ref* sender, cocos2d::ui::TextField::EventType type) {
        if (type == cocos2d::ui::TextField::EventType::INSERT_TEXT || type == cocos2d::ui::TextField::EventType::DELETE_BACKWARD) {
            auto textField = dynamic_cast<cocos2d::ui::TextField*>(sender);
            std::string nickname = textField->getString();
            std::string text = u8"��ӭ������Ϸ��" + nickname;
            promptLabel->setString(text);
        }
        });

    // ������ʼ��ť
    auto startButton = HoverButton::create("button1 (3).png",
        "button1 (2).png",
        "button1 (1).png");
    startButton->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2 - 120));

    // ����������ʾ��ǩ��Ĭ�����أ�
    auto nameLabel = Label::createWithTTF("", INPUT_FONT, 80);
    nameLabel->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2 + 30));
    nameLabel->setVisible(false);
    nameLabel->setOpacity(150);
    nameLabel->setTextColor(cocos2d::Color4B(WHITE_R, WHITE_G, WHITE_B, 255));
    this->addChild(nameLabel);

    // ��Ӱ�ť����¼�������
    startButton->addTouchEventListener([this, textField, nameLabel](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            audioPlayer(CLICK_SOUND, false); // ���ŵ����Ч
            std::string nickname = textField->getString();

            // ��֤�ǳ��Ƿ���Ч
            if (nickname.empty()) {
                nameLabel->setString(u8"��Ϸ�ǳƲ���Ϊ��");
                nameLabel->setVisible(true);
                this->scheduleOnce([nameLabel](float dt) {
                    nameLabel->setVisible(false);
                    }, 1.0, "HideEmptyPromptLabel");
            }
            else if (!isValidString(nickname)) {
                nameLabel->setString(u8"��Ϸ�ǳƴ��ڷǷ��ַ�");
                nameLabel->setVisible(true);
                this->scheduleOnce([nameLabel](float dt) {
                    nameLabel->setVisible(false);
                    }, 1.0, "HideInvalidPromptLabel");
            }
            else {
                PlayerName = nickname;
                this->scheduleOnce([](float dt) {
                    // ��ת����һ������
                    auto nextScene = MenuScene::createScene();
                    Director::getInstance()->pushScene(TransitionFade::create(1.0f, nextScene));
                    }, 0.2, "LoadNextScene");
            }
        }
        });

    // ��Ӱ�ť������
    this->addChild(startButton);

    return true;
}

// ����ַ��Ϸ��ԣ���������ĸ�����ֺͲ��ֺ��֣�
bool InitialScene::isValidCharacter(const char32_t ch)
{
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
        return true; // Ӣ����ĸ
    }
    if (ch >= '0' && ch <= '9') {
        return true; // ����
    }
    if ((ch >= 0x4E00 && ch <= 0x9FFF) || (ch >= 0x3400 && ch <= 0x4DBF)) {
        return true; // ���ú���
    }
    return false; // �����ַ��Ƿ�
}

// ����ַ����Ϸ��ԣ����� isValidCharacter ���ÿ���ַ���
bool InitialScene::isValidString(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string utf32str = converter.from_bytes(str);
    for (auto ch : utf32str) {
        if (!isValidCharacter(ch)) {
            return false;
        }
    }
    return true;
}
