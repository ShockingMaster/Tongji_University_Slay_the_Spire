#pragma once
#include "EffectAdapter.h"

/**
 * @brief Strategy for BurningContract card: two-phase discard then draw.
 */
class BurningContractEffectAdapter : public EffectAdapter {
public:
    BurningContractEffectAdapter(int baseDraw, int upgradeBonus)
        : baseDraw_(baseDraw), upgradeBonus_(upgradeBonus) {}

    void execute(std::shared_ptr<Creature> source = nullptr,
                std::shared_ptr<Creature> target = nullptr,
                std::shared_ptr<Card> card = nullptr,
                int& numeric_value = 0) override {
        if (!card) {
            return;
        }

        int draw_num = baseDraw_;
        if (card->isUpgraded()) {
            draw_num += upgradeBonus_;
        }

        if (card->tag == 0) {
            HandPileLayer::getInstance()->removeFromParent();
            Scene* selectScene = SelectScene::create();
            auto selectScenePtr = dynamic_cast<SelectScene*>(selectScene);
            auto scene = dynamic_cast<CombatScene*>(Director::getInstance()->getRunningScene());
            selectScenePtr->setCombatScene(scene);
            HandPileLayer::getInstance()->card_num_select_target = 1;
            cocos2d::Director::getInstance()->pushScene(selectScene);
        } else if (card->tag == 1) {
            CombatSystem::getInstance()->drawCard(draw_num);
            HandPileLayer::getInstance()->adjustHandPile();
            card->tag = 0;
        }
    }

private:
    int baseDraw_;
    int upgradeBonus_;
};
