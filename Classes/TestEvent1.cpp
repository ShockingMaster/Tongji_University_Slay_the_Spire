#include "IncludeAll.h"
#include "TestEvent1.h"
void Event1::onGiveGold()
{
     // 按钮事件逻辑
     shared_ptr<Player> player = Player::getInstance();
     if (player->coins_ <= 85) {
         auto continueLabel = Label::createWithSystemFont(u8"金币不足", "Fonts/Kreon-Bold.ttf", 45);
         continueLabel->setPosition(Vec2(1200, 200));
         continueLabel->setColor(Color3B::WHITE);
         this->addChild(continueLabel);
         CCLOG("Get the fuck off");
     }
     else {
         // 获得遗物逻辑待更新
         auto delay = DelayTime::create(2.0f);
         auto pop = CallFunc::create([]() {
             Director::getInstance()->popScene();
             });
         auto sequence = Sequence::create(delay, pop, nullptr);
         this->runAction(sequence);
         CCLOG("Get the fuck off");
     }
}
void Event1::onLeave()
{
    // 按钮事件逻辑
    auto delay = DelayTime::create(2.0f);
    auto pop = CallFunc::create([]() {
        Director::getInstance()->popScene();
        });
    auto sequence = Sequence::create(delay, pop, nullptr);
    this->runAction(sequence);
    CCLOG("Get the fuck off");
}