#include "IncludeAll.h"
#include "Enum.h"

void Potion::take_effect() {}; 


// 火焰药水
class FlamePotion : public Potion {
public:
    FlamePotion()
        : Potion("FlamePotion", "Deals fire damage to a target", 20, 50, 3, true) {}

    void take_effect() override {
        
    }

};
AUTO_REGISTER_POTION(FlamePotion);

// 鲜血药水：回复目标最大生命值的 20%
class BloodPotion : public Potion {
public:
    BloodPotion()
        : Potion("BloodPotion", "Restores 20% of the target's max health", 0, 50, 1, true) {}

    // 单目标效果
    void take_effect() override {
        
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
       
    }
};

AUTO_REGISTER_POTION(BlockPotion);