/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
         LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "event2.h"
#include "StartScene.h"
#include "HelloWorldScene.h"
// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(2048,1536);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("demo", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("demo");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();

    // create a scene. it's an autorelease object
    auto scene = StartScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}/*
#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "cocos2d.h"
#include <queue>
#include <vector>
#include <memory>
#include <iostream>
// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

using namespace cocos2d;
using namespace std;
#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 820);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

class Card {
public:
    virtual ~Card() = default;

    virtual std::string getName() const = 0;
    virtual int getCost() const = 0;
    virtual std::string getType() const = 0;
    virtual std::string getDescription() const = 0;

    // 重载的 use 方法
    virtual void use() {
        std::cout << "Default use() with no parameters.\n";
    }

    virtual void use(int targetId) {
        std::cout << "Default use(int targetId).\n";
    }
};

class CardRegistry {
public:
    using CardFactory = std::function<std::shared_ptr<Card>()>;
    static void registerCard(const std::string& name, CardFactory factory) {
        getRegistry()[name] = factory;
    }

    static std::shared_ptr<Card> createCard(const std::string& name) {
        auto& registry = getRegistry();
        auto it = registry.find(name);
        if (it != registry.end()) {
            return it->second();
        }
        throw std::runtime_error("Card not found: " + name);
    }

    static std::vector<std::string> getAllCardNames() {
        std::vector<std::string> names;
        for (const auto& pair : getRegistry()) {
            names.push_back(pair.first);
        }
        return names;
    }

private:
    static std::map<std::string, CardFactory>& getRegistry() {
        static std::map<std::string, CardFactory> registry;
        return registry;
    }
};

class FireballCard : public Card {
public:
    std::string getName() const override { return "Fireball"; }
    int getCost() const override { return 2; }
    std::string getType() const override { return "Attack"; }
    std::string getDescription() const override { return "Deal 6 damage."; }

    void use() override {
        std::cout << "Fireball: Deal 6 damage to all enemies.\n";
    }

    void use(int targetId) override {
        std::cout << "Fireball: Deal 6 damage to target " << targetId << ".\n";
    }
};

class HealCard : public Card {
public:
    std::string getName() const override { return name; }
    int getCost() const override { return 3; }
    std::string getType() const override { return "Skill"; }
    std::string getDescription() const override { return "Restore 5 HP."; }

    void use() override {
        std::cout << "Heal: Restore 5 HP.\n";
    }

    void use(int targetId) override {
        std::cout << "Heal: Heal target " << targetId << ".\n";
    }
private:
    std::string name = "Heal";
};

#define AUTO_REGISTER_CARD(className)                          \
    const bool className##Registered = []() {                  \
        CardRegistry::registerCard(#className, []() {          \
            return std::make_shared<className>();              \
        });                                                    \
        return true;                                           \
    }();

AUTO_REGISTER_CARD(FireballCard)
AUTO_REGISTER_CARD(HealCard)


class CardSpriteGenerator {
public:
    static Sprite* createCardSprite(const std::shared_ptr<Card>& card) {
        // 创建卡牌背景
        auto background = Sprite::create("card_background.png"); // 替换为实际背景图片
        if (!background) {
            CCLOG("Failed to load card background.");
            return nullptr;
        }

        // 设置背景大小（可调整）
        background->setContentSize(Size(200, 300)); // 设置卡牌大小

        // 添加卡牌名称
        auto nameLabel = Label::createWithSystemFont(card->getName(), "Arial", 24);
        nameLabel->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height - 30));
        nameLabel->setColor(Color3B::BLACK);
        background->addChild(nameLabel);

        // 添加费用
        std::string costText = "Cost: " + std::to_string(card->getCost());
        auto costLabel = Label::createWithSystemFont(costText, "Arial", 20);
        costLabel->setPosition(Vec2(30, background->getContentSize().height - 30));
        costLabel->setColor(Color3B::BLACK);
        background->addChild(costLabel);

        // 添加描述
        auto descriptionLabel = Label::createWithSystemFont(card->getDescription(), "Arial", 18);
        descriptionLabel->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height / 2));
        descriptionLabel->setColor(Color3B::BLACK);
        descriptionLabel->setAlignment(TextHAlignment::CENTER);
        descriptionLabel->setDimensions(180, 100); // 设置文本框大小，自动换行
        background->addChild(descriptionLabel);

        return background;
    }
};


class GameLayer : public cocos2d::Layer {
private:
    std::queue<std::shared_ptr<Card>> drawPile;        // 抽牌堆
    std::vector<std::shared_ptr<Card>> hand;           // 手牌
    std::vector<std::shared_ptr<Card>> discardPile;    // 弃牌堆
    cocos2d::Rect playArea;                            // 打出区域
    cocos2d::Menu* drawCardButton;                    // 抽牌按钮

public:
    virtual bool init() override;

    // 显示打出区域
    void showPlayArea();

    // 初始化抽牌堆
    void setupInitialDrawPile();

    // 抽取卡牌并启用拖动
    void drawCardsWithDrag(int count);

    // 单独抽一张牌
    void drawSingleCard();

    // 启用单张卡牌的拖动功能
    void enableCardDrag(cocos2d::Sprite* cardSprite, std::shared_ptr<Card> card);

    // 调整手牌的位置
    void adjustHandPositions();

    CREATE_FUNC(GameLayer);
};


bool GameLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    // 设置抽牌堆和打出区域
    setupInitialDrawPile();
    showPlayArea();

    // 创建抽牌按钮
    auto drawButtonLabel = Label::createWithSystemFont("Draw Card", "Arial", 24);
    auto drawButtonItem = MenuItemLabel::create(drawButtonLabel, CC_CALLBACK_0(GameLayer::drawSingleCard, this));
    drawCardButton = Menu::create(drawButtonItem, nullptr);
    drawCardButton->setPosition(Vec2(100, 500)); // 按钮的位置
    this->addChild(drawCardButton);

    // 初始化抽三张牌
    drawCardsWithDrag(3);

    return true;
}

void GameLayer::showPlayArea() {
    playArea = Rect(500, 500, 400, 200); // 设置打出区域
    auto playAreaNode = DrawNode::create();
    playAreaNode->drawRect(playArea.origin, playArea.origin + playArea.size, Color4F::GRAY);
    this->addChild(playAreaNode);
}

void GameLayer::setupInitialDrawPile() {
    // 从注册表中添加卡牌到抽牌堆
    auto allCardNames = CardRegistry::getAllCardNames();
    for (int i = 0; i < 9; i++)
    {
        for (const auto& name : allCardNames) {
            drawPile.push(CardRegistry::createCard(name));
        }
    }
}

void GameLayer::drawCardsWithDrag(int count) {
    for (int i = 0; i < count; ++i) {
        if (drawPile.empty()) {
            CCLOG("No more cards in draw pile!");
            return;
        }

        // 抽取卡牌
        auto card = drawPile.front();
        drawPile.pop();
        hand.push_back(card);

        // 创建精灵
        auto cardSprite = CardSpriteGenerator::createCardSprite(card);
        cardSprite->setPosition(Vec2(400, 300)); // 初始位置
        this->addChild(cardSprite);

        // 启用拖动
        enableCardDrag(cardSprite, card);
    }

    // 调整手牌位置
    adjustHandPositions();
}

void GameLayer::drawSingleCard() {
    drawCardsWithDrag(1); // 抽一张牌
}

void GameLayer::enableCardDrag(Sprite* cardSprite, std::shared_ptr<Card> card) {
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [=](Touch* touch, Event* event) -> bool {
        auto location = touch->getLocation();
        if (cardSprite->getBoundingBox().containsPoint(location)) {
            return true;
        }
        return false;
        };

    listener->onTouchMoved = [=](Touch* touch, Event* event) {
        auto location = touch->getLocation();
        cardSprite->setPosition(location);
        };

    listener->onTouchEnded = [=](Touch* touch, Event* event) {
        auto location = touch->getLocation();
        if (playArea.containsPoint(location)) {
            CCLOG("Played card: %s", card->getName().c_str());
            discardPile.push_back(card);
            hand.erase(std::remove(hand.begin(), hand.end(), card), hand.end());
            cardSprite->removeFromParent();

        }
        adjustHandPositions();
        };

    // 添加拖动监听
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, cardSprite);

    // 使用卡牌地址作为唯一 tag
    cardSprite->setTag(reinterpret_cast<intptr_t>(card.get()));
}


void GameLayer::adjustHandPositions() {
    // 手牌中心位置
    Vec2 handCenter = Vec2(400, 150);
    float cardSpacing = 100.0f; // 卡牌间距
    float totalWidth = (hand.size() - 1) * cardSpacing;

    for (size_t i = 0; i < hand.size(); ++i)
    {
        // 假设每个手牌对应的精灵用 hand[i] 的地址作为唯一 tag
        auto cardSprite = this->getChildByTag(reinterpret_cast<intptr_t>(hand[i].get()));
        if (cardSprite) {
            // 根据手牌数量调整位置，确保居中显示
            float xOffset = -totalWidth / 2 + i * cardSpacing;
            Vec2 newPosition = handCenter + Vec2(xOffset, 0);

            // 使用动作移动到新位置，显得更平滑
            cardSprite->runAction(MoveTo::create(0.2f, newPosition));
        }
    }
}







AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("Test_for_cocos2dx", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("Test_for_cocos2dx");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {
        director->setContentScaleFactor(MIN(largeResolutionSize.height / designResolutionSize.height, largeResolutionSize.width / designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {
        director->setContentScaleFactor(MIN(mediumResolutionSize.height / designResolutionSize.height, mediumResolutionSize.width / designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {
        director->setContentScaleFactor(MIN(smallResolutionSize.height / designResolutionSize.height, smallResolutionSize.width / designResolutionSize.width));
    }

    register_all_packages();

    // create a scene. it's an autorelease object

    // run

    auto scene = Scene::create();
    auto layer = GameLayer::create();
    scene->addChild(layer);

    // 运行场景
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
*/