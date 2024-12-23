#include "IncludeAll.h"
#include "Enum.h"

void Potion::take_effect() {}; 




// 鲜血药水：回复目标最大生命值的 20%
class BloodPotion : public Potion {
public:
    BloodPotion()
        : Potion("BloodPotion", "Restores 20% of the target's max health", 0, 50, 1, true) {}

    // 单目标效果
    void take_effect() override {
        CCLOG("blood potion used!");
        CombatSystem::getInstance()->addHealth(Player::getInstance(), 0.2f * Player::getInstance()->getMaxHealth());
        
    }
};
AUTO_REGISTER_POTION(BloodPotion);

// 格挡药水：获得12点格挡
class BlockPotion : public Potion {
public:
    BlockPotion()
        : Potion("BlockPotion", "Gain 12 Block.", 12, 50, 1, true) {}

    // 单目标效果：增加格挡
    void take_effect() override {
        CCLOG("block potion used!");
        CombatSystem::getInstance()->Addblock(Player::getInstance(), 12);
    }
};

AUTO_REGISTER_POTION(BlockPotion);