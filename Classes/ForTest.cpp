#include "MapScene.h"
#include "SimpleAudioEngine.h"
#include <vector>
USING_NS_CC;
class Monster : public cocos2d::Sprite {
public:
    Monster() {
        // 初始化怪物  
    }

    void move() {
        // 定义移动行为  
    }
};
Scene* GroundMap::createScene()
{
    return GroundMap::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GroundMapScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GroundMap::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
        "CloseSelected.png",
        "CloseSelected.png",
        CC_CALLBACK_1(GroundMap::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    //这里我们进行测试,这里可以知道menu是会相互覆盖的
    auto newItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(GroundMap::changeScene, this));
    Vector<Sprite*> monsters;
    auto newone = Monster::create();
    monsters.pushBack(newone);
    auto newtwo = Monster::create();
    monsters.pushBack(newtwo);
    Sprite* a;
    a.
    for (auto monster : monsters)
    {
        if (monster == newtwo)
        {
            

            if (newItem == nullptr ||
                newItem->getContentSize().width <= 0 ||
                newItem->getContentSize().height <= 0)
            {
                problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
            }
            else
            {
                float x = origin.x + visibleSize.width - newItem->getContentSize().width / 2;
                float y = origin.y + newItem->getContentSize().height / 2;
                //newItem->setPosition(Vec2(x, y + 100));
            }
        }
    }
    
    // create menu, it's an autorelease object
    auto newmenu = Menu::create(newItem, closeItem, NULL);
    newmenu->setPosition(Vec2::ZERO);
    this->addChild(newmenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    // add "GroundMap" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }
    return true;
}
void GroundMap::changeScene(Ref* pSender)
{ 
    static int count = 0;
    count += 10;
    //Director::getInstance()->purgeCachedData();
    auto newScene = GroundMap::createScene(); // 创造一个新场景  
    Director::getInstance()->pushScene(newScene); // 切换到新场景  
    // 像精灵这种内容应该在 newScene 的初始化中进行  
    auto newphoto = Sprite::create("HelloWorld.png"); // 确保这个文件存在  
    if (newphoto == nullptr)
    {
        problemLoading("'GroundMap.png'");
    }
    else
    {
        newphoto->setPosition(Vec2(count, count)); // 将精灵放在屏幕中心  
        newScene->addChild(newphoto, 0); // 将精灵添加至新场景  
    }
}

void GroundMap::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    Director::getInstance()->popScene();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}
