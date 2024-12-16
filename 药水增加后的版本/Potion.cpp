#include "IncludeAll.h"
#include "Enum.h"

void Potion::take_effect() {}; 


// ����ҩˮ
class FlamePotion : public Potion {
public:
    FlamePotion()
        : Potion("FlamePotion", "Deals fire damage to a target", 20, 50, 3, true) {}

    void take_effect() override {
        
    }

};
AUTO_REGISTER_POTION(FlamePotion);

// ��Ѫҩˮ���ظ�Ŀ���������ֵ�� 20%
class BloodPotion : public Potion {
public:
    BloodPotion()
        : Potion("BloodPotion", "Restores 20% of the target's max health", 0, 50, 1, true) {}

    // ��Ŀ��Ч��
    void take_effect() override {
        
    }
};
AUTO_REGISTER_POTION(BloodPotion);

// ��ҩˮ�����12���
class BlockPotion : public Potion {
public:
    BlockPotion()
        : Potion("BlockPotion", "Gain 12 Block.", 12, 50, 1, true) {}

    // ��Ŀ��Ч�������Ӹ�
    void take_effect() override {
       
    }
};

AUTO_REGISTER_POTION(BlockPotion);