#include "IncludeAll.h"
#include "Enum.h"

int Relic::getRarity() const
{
    return rarity_;
}


class BurningBlood : public Relic {
public:
    BurningBlood()
        : Relic("Burning Blood",
            "At the end of combat, heal 6 HP",
            0) {}

    void onTurnEnd() override {
       
       
    }
};
AUTO_REGISTER_RELIC(BurningBlood);


class CeramicFish : public Relic {
public:
    CeramicFish()
        : Relic("Ceramic Fish",
            "Whenever you add a card to your deck, gain 9 gold",
            1) {}

    void onAddCard(std::shared_ptr<Card> card) override {
        if (card) {
            int goldGain = 9;
            
        }
    }
};
AUTO_REGISTER_RELIC(CeramicFish);

class PenNib : public Relic {
private:
    int attackCounter_; // ��¼�����Ƶ�����
public:
    PenNib()
        : Relic("Pen Nib",
            "Every 10th Attack you play deals double damage",
            1), attackCounter_(0) {}

    void onCardPlayed(std::shared_ptr<Card> card) override {
        if (card && card->getType()==ATTACK) { // ���� Card ���� isAttackCard ����
            attackCounter_++;
            if (attackCounter_ == 10) {
               
                attackCounter_ = 0;
            }
        }
    }
};
AUTO_REGISTER_RELIC(PenNib);