#include <locale>
#include <codecvt>
#include "InitialScene.h"
#include "HoverButton.h"
#include "const.h"
#include "AudioPlayer.h"
#include "HelloWorldScene.h"
#include "MenuScene.h"

// 白色颜色定义
constexpr int WHITE_R = 255; // 白色 R 通道
constexpr int WHITE_G = 255; // 白色 G 通道
constexpr int WHITE_B = 255; // 白色 B 通道

// 常量定义
constexpr char START_SCENE_BACKGROUND[] = "scene1.jpg";              // 背景图片路径
constexpr char INPUT_FONT[] = "../Resources/Fonts/FangZhengZhaoGeYuan.ttf"; // 字体文件路径
constexpr char START_MUSIC[] = "../Resources/start.ogg";             // 背景音乐路径
constexpr char CLICK_SOUND[] = "ClickSoundEffect.mp3";               // 点击音效路径

using namespace std;
using namespace cocos2d;

// 全局变量，用于保存玩家昵称
string PlayerName;

// 创建场景方法
Scene* InitialScene::createScene()
{
    // 创建一个场景对象
    auto scene = Scene::create();

    // 创建一个层（当前类的实例）并将其添加到场景
    auto layer = InitialScene::create();
    scene->addChild(layer);

    return scene;
}

// 初始化场景方法
bool InitialScene::init()
{
    // 确保父类初始化成功
    if (!Scene::init()) {
        return false;
    }

    // 播放背景音乐
    audioPlayer(START_MUSIC, true);

    // 获取屏幕尺寸
    const auto screenSize = cocos2d::Director::getInstance()->getVisibleSize();

    // 创建并设置背景图片
    const auto background = Sprite::create(START_SCENE_BACKGROUND);
    background->setPosition(Vec2(screenSize.width / 2 + 50, screenSize.height / 2 - 200));
    this->addChild(background);

    // 创建文本框，用于用户输入昵称
    auto textField = cocos2d::ui::TextField::create(
        u8"点击输入您的游戏昵称", INPUT_FONT, 80); // 默认提示文字
    textField->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2 + 300));
    textField->setMaxLength(20);  // 限制最大长度
    textField->setOpacity(150);   // 设置透明度
    textField->setMaxLengthEnabled(true); // 开启长度限制
    textField->setTextColor(cocos2d::Color4B(WHITE_R, WHITE_G, WHITE_B, 255));
    this->addChild(textField);

    // 创建提示标签
    auto promptLabel = Label::createWithTTF("", INPUT_FONT, 80);
    promptLabel->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2 + 150));
    promptLabel->setOpacity(150);
    promptLabel->setTextColor(cocos2d::Color4B(WHITE_R, WHITE_G, WHITE_B, 255));
    this->addChild(promptLabel);

    // 监听文本框内容变化事件
    PlayerName = textField->getString();
    textField->addEventListener([promptLabel](Ref* sender, cocos2d::ui::TextField::EventType type) {
        if (type == cocos2d::ui::TextField::EventType::INSERT_TEXT || type == cocos2d::ui::TextField::EventType::DELETE_BACKWARD) {
            auto textField = dynamic_cast<cocos2d::ui::TextField*>(sender);
            std::string nickname = textField->getString();
            std::string text = u8"欢迎进入游戏！" + nickname;
            promptLabel->setString(text);
        }
        });

    // 创建开始按钮
    auto startButton = HoverButton::create("button1 (3).png",
        "button1 (2).png",
        "button1 (1).png");
    startButton->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2 - 120));

    // 创建错误提示标签（默认隐藏）
    auto nameLabel = Label::createWithTTF("", INPUT_FONT, 80);
    nameLabel->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2 + 30));
    nameLabel->setVisible(false);
    nameLabel->setOpacity(150);
    nameLabel->setTextColor(cocos2d::Color4B(WHITE_R, WHITE_G, WHITE_B, 255));
    this->addChild(nameLabel);

    // 添加按钮点击事件监听器
    startButton->addTouchEventListener([this, textField, nameLabel](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            audioPlayer(CLICK_SOUND, false); // 播放点击音效
            std::string nickname = textField->getString();

            // 验证昵称是否有效
            if (nickname.empty()) {
                nameLabel->setString(u8"游戏昵称不能为空");
                nameLabel->setVisible(true);
                this->scheduleOnce([nameLabel](float dt) {
                    nameLabel->setVisible(false);
                    }, 1.0, "HideEmptyPromptLabel");
            }
            else if (!isValidString(nickname)) {
                nameLabel->setString(u8"游戏昵称存在非法字符");
                nameLabel->setVisible(true);
                this->scheduleOnce([nameLabel](float dt) {
                    nameLabel->setVisible(false);
                    }, 1.0, "HideInvalidPromptLabel");
            }
            else {
                PlayerName = nickname;
                this->scheduleOnce([](float dt) {
                    // 跳转到下一个场景
                    auto nextScene = MenuScene::createScene();
                    Director::getInstance()->pushScene(TransitionFade::create(1.0f, nextScene));
                    }, 0.2, "LoadNextScene");
            }
        }
        });

    // 添加按钮到场景
    this->addChild(startButton);

    return true;
}

// 检查字符合法性（仅允许字母、数字和部分汉字）
bool InitialScene::isValidCharacter(const char32_t ch)
{
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
        return true; // 英文字母
    }
    if (ch >= '0' && ch <= '9') {
        return true; // 数字
    }
    if ((ch >= 0x4E00 && ch <= 0x9FFF) || (ch >= 0x3400 && ch <= 0x4DBF)) {
        return true; // 常用汉字
    }
    return false; // 其他字符非法
}

// 检查字符串合法性（调用 isValidCharacter 检查每个字符）
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
