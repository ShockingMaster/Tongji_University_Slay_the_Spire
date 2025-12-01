#include "CombatDeck.h"
#include "CombatUIController.h"
#include "EndingScene.h"
#include "IncludeAll.h"
#include "RewardLayer.h"
CombatSystem* CombatSystem::instance_ = nullptr;

// ����CombatSytem��Ψһʵ��
CombatSystem* CombatSystem::getInstance()
{
    if (instance_ == nullptr) {
        instance_ = new CombatSystem(); // ����Ψһʵ��
    }
    return instance_;
}

/*
 * �������ƣ�init
 * ���ܣ���ս��ϵͳ�Ƚ�����գ��Խ�ɫ���г�ʼ���������ƶѣ����ƶѣ������еĿ���ȫ���������
 * ������ɹ�������������еĿ��ƽ���һ�θ��ƣ�������ƶѣ����ó����ĳ�ʼ������
 */
void CombatSystem::init(int type)
{
    // �Խ�ɫ���г�ʼ��
    Player::getInstance()->init();

    // �����BOSS����ô������ս��
    if (type == BOSS) {
        isLastCombat = 1;
    } else {
        isLastCombat = 0;
    }

    // 创建CombatDeck实例
    deckManager_ = CombatDeck::create();
    if (!deckManager_) {
        CCLOG("[CombatSystem] Error: Failed to create CombatDeck");
        return;
    }

    // 初始化CombatDeck
    deckManager_->init(Player::getInstance());

    /*
    // ���Կ���ע�ἰ����
    std::vector<std::string>  tempDeck = CardRegistry::getAllCardNames();
    if (tempDeck.empty())
    {
            CCLOG("tempDeck is empty!");
    }
    for (int i = 0;i < 4;i++)
    {
            for (auto name : tempDeck)
            {
                    CCLOG("%s has been added!", name.c_str());
                    drawPile.push(CardRegistry::createCard(name));
            }
    }
    */

    // ��չ����б�,������ɹ���
    Monsters_.clear();
    if (type == ELITE) {
        // ��ȡ��������
        auto monster = RandomGenerator::getInstance()->getRandomMonster(ELITE);
        for (int i = 0; i < monster->getMonsterNum(); i++) {
            Monsters_.push_back(MonsterRegistry::createMonster(monster->getName()));
        }
    } else if (type == BOSS) {
        auto monster = RandomGenerator::getInstance()->getRandomMonster(BOSS);
        for (int i = 0; i < monster->getMonsterNum(); i++) {
            Monsters_.push_back(MonsterRegistry::createMonster(monster->getName()));
        }
    } else {
        auto monster = RandomGenerator::getInstance()->getRandomMonster(NORMAL);
        for (int i = 0; i < monster->getMonsterNum(); i++) {
            Monsters_.push_back(MonsterRegistry::createMonster(monster->getName()));
        }
    }
}

/*
 * �������ƣ�onAttack
 * ������������ָ�룬��������ָ�룬����ԭ��ֵ���������ƣ�Ϊ���жϿ����Ƿ�Ϊ�������ͣ�
 * ���ܣ���ɹ����ߵĹ�����ص�buff���㣬����������buff����д������ֵ
 */
void CombatSystem::onAttack(std::shared_ptr<Creature> user, std::shared_ptr<Creature> target,
    int& numeric_value_, std::string cardName, bool isForIntentionUpdate)
{
    // ���ȱ���ʹ���ߵ�buff�б�����������buff��onAttackЧ��
    for (auto Buff : user->buffs_) {
        if (Buff != nullptr)
            Buff->onAttack(numeric_value_, cardName, user, target);
    }

    // �ٱ���Ŀ���buff�б�����������buff��onAttackedЧ��
    for (auto Buff : target->buffs_) {
        if (Buff != nullptr)
            Buff->onAttacked(numeric_value_, user, target);
    }

    // �������ҷ��𹥻����򴥷������onAttackЧ��
    if (user == Player::getInstance()) {
        for (auto Relic : EventSystem::getInstance()->relics_) {
            if (Relic != nullptr) {
                Relic->onAttack(numeric_value_, cardName, user, target);
            }
        }
        audioPlayer("FastAttackSound.ogg", false);
    }

    if (target == Player::getInstance()) {
        for (auto Relic : EventSystem::getInstance()->relics_) {
            if (Relic != nullptr) {
                Relic->onAttacked(numeric_value_, user, target);
            }
        }
    }

    // ��ֹ����������
    numeric_value_ = max(numeric_value_, 0);

    if (!isForIntentionUpdate) {
        takeDamage(target, numeric_value_);
        if (uiController_) {
            uiController_->playAttackAnimation(user, target, numeric_value_);
        }
    }

    // �˴����ܽ��и��£������ѭ������
    /*
    auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
    scene->creatureLayer->updateDisplay();
    */
}

void CombatSystem::combatStart()
{
    auto player = Player::getInstance();
    for (auto Buff : player->buffs_) {
        if (Buff != nullptr)
            Buff->onCombatStart(player);
    }
    for (auto Relic : EventSystem::getInstance()->relics_) {
        if (Relic != nullptr)
            Relic->onCombatStart();
    }
}

void CombatSystem::combatEnd()
{
    // �ȴ���һ�λغϽ���Ч��
    auto player = Player::getInstance();

    CombatSystem::getInstance()->endTurn(player);

    if (uiController_) {
        uiController_->clearAllHandCards();
    }

    if (player->getHealth() <= 0) {
        if (uiController_) {
            uiController_->showDefeatScreen();
        }
    } else {
        for (auto Buff : player->buffs_) {
            if (Buff != nullptr)
                Buff->onCombatEnd(player);
        }
        for (auto Relic : EventSystem::getInstance()->relics_) {
            if (Relic != nullptr)
                Relic->onCombatEnd();
        }
    }
    // ���������ս��
    if (isLastCombat && player->getHealth() > 0) {
        if (uiController_) {
            uiController_->showVictoryScreen();
        }
    } else if (player->getHealth() > 0) {
        if (uiController_) {
            uiController_->showVictoryScreen();
        }
        return;
    } else {
        // Old code disabled
        if (false) {
            auto blackLayer = LayerColor::create(Color4B(0, 0, 0, 200));
            Director::getInstance()->getRunningScene()->addChild(blackLayer, 100000);

            // ���� RewardLayer
            auto rewardLayer = RewardLayer::create(true, true, false, false, true);
            blackLayer->addChild(rewardLayer); // �� RewardLayer ���ӵ���ɫ��������
            auto startButton = HoverButton::create(
                "button1 (1).png", // Ĭ��ͼƬ
                "button1 (2).png", // ��ť��ͣʱ��ͼƬ
                "button1 (3).png" // ��ť���ʱ��ͼƬ
            );

            // ���ð�ťλ��
            startButton->setPosition(Vec2(1800, 500));
            blackLayer->addChild(startButton);

            // ���Ӱ�ť����¼�������
            startButton->addClickEventListener([=](Ref* sender) {
                // ִ�� popScene ������������һ������
                Director::getInstance()->popScene();
            });
        }
    }
}

/*
 * �������ƣ�takeDamage
 * ��������������ָ�룬����������ֵ�������ߵ�ָ��
 * ���ܣ�������������buff����д��������ֵ
 */
void CombatSystem::takeDamage(std::shared_ptr<Creature> target, int numeric_value_, std::shared_ptr<Creature> attack)
{
    if (target == nullptr) {
        CCLOG("TakeDamage����Ŀ��Ϊ�գ�");
        return;
    }
    // �������������
    // 1.Ŀ��ĸ񵲴��ڵ����˺���������ʱ���ܻ����з�����,��ʱ�¼�Ϊ���׼���
    if (numeric_value_ <= target->getBlockValue()) {
        for (auto Buff : target->buffs_) {
            if (Buff != nullptr)
                Buff->onLoseBlock(numeric_value_);
        }
        if (target == Player::getInstance()) {
            for (auto Relic : EventSystem::getInstance()->relics_) {
                if (Relic != nullptr)
                    Relic->onLoseBlock(numeric_value_);
            }
        }
        audioPlayer("AttackOnBlockSound.ogg", false);
        target->loseBlock(numeric_value_);
    }

    // 2.Ŀ��ĸ񵲲�Ϊ0����С���˺�ֵ����ʱ�������ף�����˺�����ʱ�¼�Ϊ���׼��٣�����ֵ����
    else if (target->getBlockValue() > 0) {
        for (auto Buff : target->buffs_) {
            if (Buff != nullptr)
                Buff->onLoseBlock(numeric_value_);
        }
        if (target == Player::getInstance()) {
            for (auto Relic : EventSystem::getInstance()->relics_) {
                if (Relic != nullptr)
                    Relic->onLoseBlock(numeric_value_);
            }
        }
        int healthLoss = numeric_value_ - target->getBlockValue();
        target->loseBlock(target->getBlockValue());
        for (auto Buff : target->buffs_) {
            if (Buff != nullptr)
                Buff->onLoseHealth(healthLoss);
        }
        if (target == Player::getInstance()) {
            for (auto Relic : EventSystem::getInstance()->relics_) {
                if (Relic != nullptr)
                    Relic->onLoseHealth(healthLoss);
            }
        }
        audioPlayer("DefenseBreakSound.ogg", false);
        target->loseHealth(healthLoss);
    }

    // 3.Ŀ��ĸ�Ϊ0����ʱ�¼�Ϊ����ֵ����
    else {
        int healthLoss = numeric_value_ - target->getBlockValue();
        for (auto Buff : target->buffs_) {
            if (Buff != nullptr)
                Buff->onLoseHealth(healthLoss);
        }
        if (target == Player::getInstance()) {
            for (auto Relic : EventSystem::getInstance()->relics_) {
                if (Relic != nullptr)
                    Relic->onLoseHealth(healthLoss);
            }
        }
        audioPlayer("FastAttackSound.ogg", false);
        target->loseHealth(healthLoss);
    }
    if (target->getHealth() < 0) {
        CombatSystem::getInstance()->onDeath(target);
    }

    if (uiController_) {
        uiController_->updateCreatureDisplay(target);
    }
}

/*
 * �������ƣ�Addblock
 * �������ӻ��ܶ��󣬻�����ֵ����������
 * ���ܣ�����������buff����д����ֵ
 */

void CombatSystem::Addblock(std::shared_ptr<Creature> target, int numeric_value_)
{
    // ����ʹ���ߵ�buff�б�����������buff��AddblockЧ��
    int tempBlock = numeric_value_;
    for (auto Buff : target->buffs_) {
        if (Buff != nullptr)
            Buff->onGetBlock(tempBlock);
    }
    if (target == Player::getInstance()) {
        for (auto Relic : EventSystem::getInstance()->relics_) {
            if (Relic != nullptr)
                Relic->onGetBlock(tempBlock);
        }
    }
    // ��ֹ����������
    numeric_value_ = max(numeric_value_, 0);
    target->addBlock(numeric_value_); // ���ӻ���

    if (numeric_value_ > 0) {
        audioPlayer("GetBlockSound.ogg", false);
    }
    // UI更新 - 通过UIController
    notifyUICreaturesUpdated();
}

/*
 * �������ƣ�exhuastCard
 * ���������ĵڼ��ſ��ƣ���������
 * ���ܣ������ض�����
 */

void CombatSystem::exhaustCard(int num, std::string cardName)
{
    if (num < 0 || num >= hand.size()) {
        CCLOG("Error: Invalid card index %d", num);
        return; // ��� num ��Ч��ֱ�ӷ���
    }

    // ����ʹ���ߵ�buff�б�����������buff��deleteCardЧ��
    for (auto Buff : Player::getInstance()->buffs_) {
        if (Buff != nullptr) {
            Buff->onExhaustCard();
        }
    }
    for (auto Relic : EventSystem::getInstance()->relics_) {
        if (Relic != nullptr) {
            Relic->onExhaustCard();
        }
    }

    // �������Ʊ����ĵ�Ч��
    auto card = hand[num];
    card->takeEffectOnExhaust();

    // UI更新 - 通过UIController移除卡牌
    notifyUICardRemoved(card);

    // ������Ӧλ�ÿ���
    hand.erase(hand.begin() + num);
    CCLOG("Card '%s' at index %d has been exhuasted", cardName.c_str(), num);

    // 通知UI更新玩家状态
    notifyUIPlayerUpdated(Player::getInstance());
}

void CombatSystem::exhaustCard()
{
    // Use getSelectedCards() instead of direct HandPileLayer access
    auto selectedCards = getSelectedCards();
    for (auto& card : selectedCards) {
        for (auto Buff : Player::getInstance()->buffs_) {
            if (Buff != nullptr) {
                Buff->onExhaustCard();
            }
        }
        for (auto Relic : EventSystem::getInstance()->relics_) {
            if (Relic != nullptr) {
                Relic->onExhaustCard();
            }
        }

        card->takeEffectOnExhaust();

        notifyUIRemoveCard(card);
    }
}

void CombatSystem::exhaustCard(std::shared_ptr<Card> card)
{
    for (auto Buff : Player::getInstance()->buffs_) {
        if (Buff != nullptr) {
            Buff->onExhaustCard();
        }
    }
    for (auto Relic : EventSystem::getInstance()->relics_) {
        if (Relic != nullptr) {
            Relic->onExhaustCard();
        }
    }
    card->takeEffectOnExhaust();

    notifyUIRemoveCard(card);

    // 更新对应位置
    hand.erase(std::remove(hand.begin(), hand.end(), card), hand.end());
}

void CombatSystem::endTurnCardPlayed()
{
    std::vector<std::shared_ptr<Card>> Hand = hand;
    for (auto& card : Hand) {
        card->takeeffectonturnend(card);
    }
}

/*
 * �������ƣ�upgradeCard
 * ��������Ҫ�����Ŀ��Ƶ�ָ��
 * ���ܣ�����Ӧ���ƽ���������ͬʱ��������������У�������ʾ
 */
void CombatSystem::upgradeCard(std::shared_ptr<Card> card)
{
    // �������û�н��й���������ô�Կ��ƽ�������
    if (!card->isUpgraded()) {
        card->upgrade();

        // �鿴�����Ƿ��������У��������������Ҫ������ʾ
        if (uiController_) {
            uiController_->updateCardDisplay(card);
        }
    }

    if (uiController_) {
        uiController_->updatePlayerDisplay(Player::getInstance());
    }
}

/*
 * �������ƣ�startTurn
 * ���������ڽ��лغϿ�ʼ�������ָ��
 * ���ܣ���ɸ�����ĻغϿ�ʼ��buff�Ľ��㣬�����лغϿ�ʼ����
 */
void CombatSystem::startTurn(std::shared_ptr<Creature> creature)
{
    // ʧȥ���и�
    creature->loseBlock(creature->getBlockValue());
    // 使用UI通知替代直接UI调用
    if (uiController_) {
        // 通知UI更新生物显示（转换Monsters_为Monster向量）
        std::vector<std::shared_ptr<Monster>> monsters;
        for (const auto& creature : Monsters_) {
            auto monster = std::dynamic_pointer_cast<Monster>(creature);
            if (monster) {
                monsters.push_back(monster);
            }
        }
        notifyUIMonstersUpdated(monsters);
    }

    // ����
    for (auto Buff : creature->buffs_) {
        if (Buff != nullptr) {
            Buff->onTurnStart();
        }
    }

    // �ж��Ƿ�Ϊ��ң��������ң���ô��Ҫ���в���
    // 1.ʧȥ���и�
    // 2.����������޵�����
    // 3.��5����
    if (creature == Player::getInstance()) {
        for (auto Relic : EventSystem::getInstance()->relics_) {
            if (Relic != nullptr) {
                Relic->onTurnStart();
            }
        }
        const int energy = Player::getInstance()->getMaxEnergy(); // ��ȡ��ҵ������������
        addEnergy(Player::getInstance(), energy);
        drawCard(5);
        // 通知UI更新
        notifyUIPlayerUpdated(Player::getInstance());
    }
}

// ����buff
void CombatSystem::updateBuff(std::shared_ptr<Creature> creature)
{
    for (auto it = creature->buffs_.begin(); it != creature->buffs_.end();) {
        auto Buff = *it;
        if (Buff->stack_type_ == DURATION) {
            Buff->duration_--;
            if (Buff->duration_ == 0) {
                it = creature->buffs_.erase(it); // �Ƴ�Ԫ�ز����µ�����
                continue; // �����������ĵ�������
            }
        }
        ++it; // ����δ�Ƴ�ʱ����������
    }
    for (auto Buff : creature->buffs_) {
        if (Buff->stack_type_ != EFFECT_LAYERS) {
            CCLOG("update::have buff: %s. num is %d", typeid(*Buff).name(), Buff->duration_);
        } else {
            CCLOG("update::have buff: %s. num is %d", typeid(*Buff).name(), Buff->effect_layers);
        }
    }
    // ��ʾ����
    if (uiController_) {
        uiController_->updateCreatureDisplay(creature);
    }
}

/*
 * �������ƣ�endTurn
 * ���������ڽ��лغϽ����������ָ��
 * ���ܣ���ɸ�����ĻغϽ�����buff�Ľ���
 */
void CombatSystem::endTurn(std::shared_ptr<Creature> creature)
{
    for (auto Buff : creature->buffs_) {
        if (Buff != nullptr) {
            Buff->onTurnEnd();
        }
    }
    updateBuff(creature);
    if (creature == Player::getInstance()) {
        for (auto Relic : EventSystem::getInstance()->relics_) {
            if (Relic != nullptr) {
                Relic->onTurnEnd();
            }
        }
        const int energy = Player::getInstance()->getCurrentEnergy(); // 清空能量
        addEnergy(Player::getInstance(), -energy);
        for (size_t i = 0; i < hand.size();) {
            std::shared_ptr<Card> card = hand[i];
            CombatSystem::getInstance()->discardPile.push(card); // 将手牌加入弃牌堆
            notifyUIRemoveCardSprite(card);
            // 不增加遍历删除容器
            hand.erase(hand.begin() + i); // 删除当前元素，i 保持不变，因为元素被删除后，后面的元素会前移
        }
        // 清空前端牌堆变化堆叠,弃牌堆进行更新
        notifyUIDiscardPileUpdated(static_cast<int>(discardPile.size()));
    }

    notifyUICreaturesUpdated();
}

/*
 * �������ƣ�cardPlayed
 * ����������Ŀ��Ƶ�ָ��
 * ���ܣ���ɴ���������buff�Ľ���
 */
void CombatSystem::cardPlayed(std::shared_ptr<Card> card)
{

    tem_card = card;
    for (auto Buff : Player::getInstance()->buffs_) {
        if (Buff != nullptr) {
            Buff->onCardPlayed(card);
        }
    }
    for (auto Relic : EventSystem::getInstance()->relics_) {
        if (Relic != nullptr) {
            Relic->onCardPlayed(card);
        }
    }

    card->takeEffect();

    int tempEnergyCost = card->getEnergyCost();
    Player::getInstance()->energyChange(-tempEnergyCost);
    // 通过UIController更新能量显示
    if (uiController_) {
        auto player = Player::getInstance();
        uiController_->updateEnergyDisplay(player->getCurrentEnergy(), player->getMaxEnergy());
    }

    notifyUICreaturesUpdated();
}

/*
 * �������ƣ�addToDiscardPile
 * ���ܣ���һ�������Ŀ����������ƶ�
 */
void CombatSystem::addToDiscardPile(std::shared_ptr<Card> card, int num)
{
    // 重构：使用CombatDeck替代直接操作数据结构
    if (deckManager_) {
        deckManager_->addToDiscardPile(card, num);
    } else {
        CCLOG("[CombatSystem] Error: deckManager_ is null");
    }
    // UI更新已移至CombatDeck的事件回调中处理
}

void CombatSystem::onDeath(std::shared_ptr<Creature> creature)
{
    if (creature == Player::getInstance()) {
        for (auto Relic : EventSystem::getInstance()->relics_) {
            Relic->onDeath();
        }
        if (Player::getInstance()->getHealth() < 0) {
            this->combatEnd();
        }
    } else {
        int is_all_monster_dead = 1;
        for (int i = 0; i < Monsters_.size(); i++) {
            if (Monsters_[i]->getHealth() > 0)
                is_all_monster_dead = 0;
        }
        if (is_all_monster_dead) {
            CombatSystem::combatEnd();
        }
    }
}

void CombatSystem::tem_cardPlayed(std::shared_ptr<Card> card)
{
    for (auto Buff : Player::getInstance()->buffs_) {
        if (Buff != nullptr) {
            Buff->onCardPlayed(card);
        }
    }
    for (auto Relic : EventSystem::getInstance()->relics_) {
        if (Relic != nullptr) {
            Relic->onCardPlayed(card);
        }
    }
    card->takeEffect();
}

/*
 * �������ƣ�getMonsterPointer
 * ������������ָ��
 * ���������ظ�ָ����CombatSystem�е�����ָ��
 */
std::shared_ptr<Creature> CombatSystem::getMonsterPointer(Creature* creature)
{
    if (creature == nullptr) {
        return nullptr;
    }
    for (int i = 0; i < Monsters_.size(); i++) {
        if (Monsters_[i].get() == creature) {
            return Monsters_[i];
        }
    }
    return nullptr;
}

void CombatSystem::use_tem_card()
{
    tem_card->tag = 1;
    tem_cardPlayed(tem_card);
}

void CombatSystem::cardPlayed(std::shared_ptr<Card> card, std::shared_ptr<Creature> creature)
{
    for (auto Buff : Player::getInstance()->buffs_) {
        if (Buff != nullptr) {
            Buff->onCardPlayed(card);
        }
    }
    for (auto Relic : EventSystem::getInstance()->relics_) {
        if (Relic != nullptr) {
            Relic->onCardPlayed(card);
        }
    }
    card->takeEffect(creature);

    int tempEnergyCost = card->getEnergyCost();
    Player::getInstance()->energyChange(-tempEnergyCost);
    // 通过UIController更新能量显示
    if (uiController_) {
        auto player = Player::getInstance();
        uiController_->updateEnergyDisplay(player->getCurrentEnergy(), player->getMaxEnergy());
    }

    notifyUICreaturesUpdated();
}

/*
 *
 *
 *
 */
void CombatSystem::addEnergy(std::shared_ptr<Creature> user, int numeric_value_)
{
    int tempEnergy = numeric_value_;
    for (auto Buff : user->buffs_) {
        if (Buff != nullptr) {
            Buff->onGetEnergy(tempEnergy);
        }
    }
    if (user == Player::getInstance()) {
        for (auto Relic : EventSystem::getInstance()->relics_) {
            if (Relic != nullptr) {
                Relic->onGetEnergy(tempEnergy);
            }
        }
        Player::getInstance()->energyChange(tempEnergy);
    }
    // 通过UIController更新能量显示
    if (uiController_) {
        auto player = Player::getInstance();
        uiController_->updateEnergyDisplay(player->getCurrentEnergy(), player->getMaxEnergy());
    }

    notifyUICreaturesUpdated();
}

/*
 * �������ƣ�addBuff
 * ���������ӵ�Buff���࣬
 */
void CombatSystem::addBuff(std::shared_ptr<Buff> buff, int numeric_value, std::shared_ptr<Creature> target)
{
    // ����Ŀ���buff�б�����������buff��addBuffЧ��
    for (auto it = target->buffs_.begin(); it != target->buffs_.end();) {
        auto Buff = *it;
        Buff->addBuff(buff, numeric_value);
        if (Buff->effect_layers == 0 && Buff->duration_ == 0) {
            it = target->buffs_.erase(it); // �Ƴ�Ԫ�ز����µ�����
            continue; // �����������ĵ�������
        }
        ++it; // ����δ�Ƴ�ʱ����������
    }

    if (numeric_value > 0) {
        int tag = 0;
        // buff�ܷ񱻵���
        if (buff->is_stackable_ == true) {
            for (auto Buff : target->buffs_) {
                // �Ѿ�����ͬ��buff
                if (Buff->name_ == buff->name_) {
                    // 更新持续时间
                    if (Buff->stack_type_ == DURATION) {
                        Buff->duration_ += numeric_value;
                    }
                    // 效果层数叠加
                    else {
                        Buff->effect_layers += numeric_value;
                    }
                    // 更新UI
                    notifyUICreaturesUpdated();
                    tag = 1;
                    break;
                }
            }
            if (tag == 0) {
                // 设置持续时间
                if (buff->stack_type_ == DURATION) {
                    buff->duration_ = numeric_value;
                }
                // 效果层数叠加
                else {
                    buff->effect_layers = numeric_value;
                }
                target->buffs_.push_back(buff);
                // 更新UI
                notifyUICreaturesUpdated();
            }
        }
    }
    // 调试输出
    for (auto Buff : target->buffs_) {
        CCLOG("have buff: %s. num is %d", typeid(*Buff).name(), Buff->effect_layers);
    }
    notifyUICreaturesUpdated();
}

void CombatSystem::addHealth(std::shared_ptr<Creature> target, int numeric_value)
{
    int tempHealthRestore = numeric_value;
    for (auto Buff : target->buffs_) {
        if (Buff != nullptr) {
            Buff->onAddHealth(tempHealthRestore);
        }
    }
    if (target == Player::getInstance()) {
        for (auto Relic : EventSystem::getInstance()->relics_) {
            if (Relic != nullptr) {
                Relic->onAddHealth(tempHealthRestore);
            }
        }
    }
    // 防止负数恢复
    tempHealthRestore = max(0, tempHealthRestore);
    target->addHealth(tempHealthRestore);

    notifyUICreaturesUpdated();
}

/*
 * Function Name: drawCard
 * Description: Draw cards from the deck
 * Functionality: Draw a specified number of cards, trigger related buff/relic effects
 */
void CombatSystem::drawCard(int num)
{
    // Refactored: Use CombatDeck's drawCards method
    if (deckManager_) {
        int drawn = deckManager_->drawCards(num);
        CCLOG("Drew %d cards using CombatDeck", drawn);

        // Restore logic: Trigger Relics and Buffs for each card drawn
        for (int i = 0; i < drawn; i++) {
            // Trigger Relics
            for (auto Relic : EventSystem::getInstance()->relics_) {
                if (Relic != nullptr) {
                    Relic->onDrawCard(1);
                }
            }

            // Trigger Player Buffs
            for (auto Buff : Player::getInstance()->buffs_) {
                if (Buff != nullptr) {
                    Buff->onDrawCard(1);
                }
            }
        }
    } else {
        CCLOG("[CombatSystem] Error: deckManager_ is null");
    }
    // UI updates are handled via CombatDeck events or UIController
}

void CombatSystem::shuffleDeck()
{
    // Refactored: Use CombatDeck's shuffle method
    if (deckManager_) {
        deckManager_->shuffle();
        CCLOG("Shuffled deck using CombatDeck");
    } else {
        CCLOG("[CombatSystem] Error: deckManager_ is null");
    }
}

int CombatSystem::getDrawPileNumber()
{
    // Refactored: Use CombatDeck
    return deckManager_ ? deckManager_->getDrawPileSize() : 0;
}

int CombatSystem::getHandNumber()
{
    // Refactored: Use CombatDeck
    return deckManager_ ? deckManager_->getHandSize() : 0;
}

int CombatSystem::getDiscardPileNumber()
{
    // Refactored: Use CombatDeck
    return deckManager_ ? deckManager_->getDiscardPileSize() : 0;
}

// ==================== UI通知方法（Facade模式重构） ====================

void CombatSystem::setUIController(std::shared_ptr<CombatUIController> uiController)
{
    uiController_ = uiController;
    CCLOG("[CombatSystem] UI Controller set");
}

void CombatSystem::notifyUIPlayerUpdated(std::shared_ptr<Player> player)
{
    if (uiController_) {
        uiController_->updatePlayerDisplay(player);
    }
}

void CombatSystem::notifyUIMonstersUpdated(const std::vector<std::shared_ptr<Monster>>& monsters)
{
    if (uiController_) {
        uiController_->updateMonstersDisplay(monsters);
    }
}

void CombatSystem::notifyUICardDrawn(std::shared_ptr<Card> card)
{
    if (uiController_) {
        uiController_->addCardToHand(card);
    }
}

void CombatSystem::notifyUICardRemoved(std::shared_ptr<Card> card)
{
    if (uiController_) {
        uiController_->removeCardFromHand(card);
    }
}

void CombatSystem::notifyUIDrawPileUpdated(int count)
{
    if (uiController_) {
        uiController_->updateDrawPileDisplay(count);
    }
}

void CombatSystem::notifyUIDiscardPileUpdated(int count)
{
    if (uiController_) {
        uiController_->updateDiscardPileDisplay(count);
    }
}

void CombatSystem::notifyUIEnergyUpdated(int current, int max)
{
    if (uiController_) {
        uiController_->updateEnergyDisplay(current, max);
    }
}

// NEW: Additional UI notification methods for complete UI separation

void CombatSystem::notifyUICreaturesUpdated()
{
    if (uiController_) {
        uiController_->updateAllCreaturesDisplay();
    }
}

void CombatSystem::notifyUIDiscardAllHandCards()
{
    if (uiController_) {
        uiController_->discardAllHandCardsDisplay();
    }
}

void CombatSystem::notifyUIRemoveCardSprite(std::shared_ptr<Card> card)
{
    if (uiController_) {
        uiController_->removeCardSpriteByTag(card.get());
    }
}

std::vector<std::shared_ptr<Card>> CombatSystem::getSelectedCards()
{
    if (uiController_) {
        return uiController_->getSelectedCards();
    }
    // Fallback to direct access for backward compatibility
    return HandPileLayer::getInstance()->select_card_list;
}
