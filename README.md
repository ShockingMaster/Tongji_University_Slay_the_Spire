
# 项目实现功能与项目技术细节文档
## &#x20;
## 基础实现功能
*   **初始界面和设置界面**
    实现正常的游戏进入和用户自定义管理系统
*   **支持背景音效**
    实现音乐的播放和音量修改，优化游戏体验
*   **地图与场景系统**\
    游戏中的随机生成地图，玩家选择路径前进。
*   **卡牌管理系统**\
    支持卡牌的添加、删除、整理和升级。
*   **战斗系统**\
    处理玩家与敌人之间的战斗，包括攻击、防御、技能、卡牌消耗等。
*   **回合制战斗机制**\
    每个回合处理玩家和敌人的动作，计算卡牌效果。
*   **卡牌生成与抽卡**\
    每个回合抽取随机卡牌，并展示卡牌选择。
*   **体力与能量系统**\
    玩家每回合消耗一定能量，能量用来打出卡牌。
*   **卡牌效果（攻击、防御、技能）**\
    实现卡牌的各种效果，如伤害、防御、抽牌、卡牌升级等。
*   **敌人攻击意图**\
    敌人的行动和策略，每个敌人有不同的攻击方式和目标。
*   **随机事件与商店**\
    玩家在游戏中遇到的随机事件（例如商店、传送门、特殊场景等）。
*   **Boss战机制**\
    设计关底Boss，具有独特的攻击方式和技能。
*   **卡牌选择与构筑**\
    玩家可以在战斗前或某些事件后选择卡牌进行构筑，形成不同的策略。
*   **状态（buff）效果系统**\
    力量、易伤、人工制品等效果，对玩家或敌人产生持续影响。
*   **金钱与资源管理**\
    玩家可以通过战斗或事件获得金钱，用于购买卡牌、装备或治愈等。
*   **强化与强化道具**\
    游戏中的道具系统，包括遗物和药水，可以让玩家增强卡牌、人物能力或战斗策略。
*   **回合结束效果与过渡动画**\
    每回合结束后，显示回合总结和过渡动画，提升游戏的反馈感。
*  **抽牌堆、弃牌堆、手牌、抽牌的设计**\
 在游戏中可以实时查看牌堆的数量和内容变化
 * **支持自定义用户名称，该名称会显示在上方的栏的左侧**
## 加分项
- [x] **智能指针使用**
- 在本项目中，我们大量使用了智能指针，如Card,Buff,Creature等都使用了智能指针。
- **使用智能指针的好处**：
 
  1.**自动管理内存，防止内存泄漏** ：在该项目中涉及到大量的卡牌，敌人，药水，遗物的指针创建，如果使用裸指针而要完全保证内存不泄露是有一定难度的，而使用智能指针中的shared_ptr保持引用计数，在引用计数为0时会自动释放，保证了不会出现内存泄漏。
  2.**避免悬空指针**：智能指针能够保证所指向的对象是有效的，避免了悬空指针的问题。
- **使用智能指针的注意事项**
  当然，智能指针也不是万能的，使用shared_ptr存在的隐患就是循环引用，如果两个共享指针相互引用，会导致引用计数一定不会为0，无法释放内存。而对于这一点，我们使用资源管理外部化，也就是通过CombatSystem和EventSystem来进行资源管理，保证了对象之间不会建立相互持有的共享指针。
- [x] **工厂模式注册**
- 因为该项目希望支持数量庞大的卡牌，遗物，buff等，而大量卡牌的创建显然用switch-case是不现实的，所以我们使用工厂模式进行注册。例如对于卡牌而言，我们定义了一个CardRegistry类，用于管理卡牌的注册和创建。
- 首先使用using声明CardFactory别名，同时利用C++11特性，使用functional库中的function存储工厂方法，灵活性强。
```c++
using CardFactory = std::function<std::shared_ptr<Card>()>;
// 卡牌注册
static void registerCard(const std::string& name, CardFactory factory) {
    getRegistry()[name] = factory;
}
```
- 卡牌的注册使用registerCard方法，传入卡牌名称和工厂方法，将卡牌名称和工厂方法注册到一个map中。
```c++
private:
    static std::map<std::string, CardFactory>& getRegistry() {
        static std::map<std::string, CardFactory> registry;
        return registry;
    }
```
- 卡牌的创建使用createCard方法，传入卡牌名称，根据卡牌名称从map中查找对应的工厂方法，调用工厂方法创建卡牌，并返回卡牌的智能指针。同时，如果出现没有找到的情况，会进行异常抛出。
```c++
// 根据卡牌名称返回相应卡牌的指针
static std::shared_ptr<Card> createCard(const std::string& name)
{
    // 检查卡牌是否升级
    auto newname = name;
    int upgraded = 0;
    if (name.back() == '+')
    {
        newname.pop_back();
        upgraded = 1;
    }
    auto& registry = getRegistry();
    auto it = registry.find(newname);
    if (it != registry.end()) {
        auto card = it->second();
        // 如果卡牌升级，调用upgrade方法
        if (upgraded)
        {
            card->upgrade();
        }
        return card;
    }
    // 卡牌不存在，抛出异常
    throw std::runtime_error("Card not found: " + name);
}
```
- 而为了卡牌创建更加方便，我们定义了宏函数AUTO_REGISTER_CARD，用于自动注册卡牌。
```c++
#define AUTO_REGISTER_CARD(className)                          \
    const bool className##Registered = []() {                  \
        CardRegistry::registerCard(#className, []() {          \
           return std::make_shared<className>();               \
        });                                                    \
        return true;                                           \
    }();
```
- 这样在每一张卡牌完成设计之后，只需要调用AUTO_REGISTER_CARD宏，即可完成卡牌的注册。
```c++
AUTO_REGISTER_CARD(Defense)
```
- [x] **事件驱动机制**
- **CombatSystem设计**
在杀戮尖塔这款游戏中，由于种类繁多的buff和遗物的存在，一张很简单的卡牌的打出都可能触发非常复杂的行为。举例说明，对于打出一张普通的打击来说，如果角色具有余像buff（每打出一张卡牌，角色获得一点格挡）和势不可挡buff（每当角色获得格挡时，对随机敌人造成一定伤害）。而事件驱动机制使得这种复杂的触发行为变得可能。
    CombatSystem的设计思路是，将所有卡牌的触发行为抽象为事件，并将事件分为多种类型，例如对于攻击事件它会通过完成将onAttack这一事件分发给Buff，遗物等完成统一调度，同时也会进行音效、动作的管理。这一种做法将复杂的行为拆解成一个个事件进行有序触发，逻辑清晰，易于维护。
   
```
    void onAttack(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> target,
        int& numeric_value_, std::string cardName = "", bool isForIntentionUpdate = false);    
```
    当一个卡牌具有攻击效果时，它仅仅是告诉CombatSystem发生了攻击事件，而不会管角色身上具体有什么buff，有什么遗物，而是由CombatSystem调用相应的处理函数，以下是onAttack函数的部分实现。
    
```
    void CombatSystem::onAttack(std::shared_ptr<Creature> user, std::shared_ptr<Creature> target,
    int& numeric_value_, std::string cardName, bool isForIntentionUpdate)
    {
        //首先遍历使用者的buff列表，触发所有buff的onAttack效果
        for (auto Buff : user->buffs_)
        {
            if (Buff != nullptr)
                Buff->onAttack(numeric_value_, cardName, user, target);
        }
        ······
    }
    
```
    
- **Buff和遗物设计**
    从上面的CombatSystem中的设计也可以看出，为了让每一种事件CombatSystem能够调用到buff的相应函数，每一个buff也必须能够处理相应的事件，为此，我们采用虚函数的方法进行实现，在Buff基类中定义所有事件处理的虚函数，但没有实际效果。
    
```

    virtual void onAttack(int& numeric_value_, std::string cardName = "", std::shared_ptr<Creature> user = nullptr, std::shared_ptr<Creature> target = nullptr) {};     
```
 
    
    而在相应的buff中，我们可以重写这些虚函数，实现具体的效果。例如对于精准buff而言，它会将卡牌名为“小刀”的攻击牌的攻击数值进行提高

```
void onAttack(int& numeric_value_, std::string cardName,std::shared_ptr<Creature> user, std::shared_ptr<Creature> target) {
        if (cardName == "Blade" || cardName == "Blade+"){
            numeric_value_ += effect_layer_;
        }
    }
```
    
    这样的设计在兼容Combatsystem的同时，也保证了Buff的可扩展性，支持Buff在多种情况下的效果能够触发相应的效果。遗物的实现逻辑是相似的，以PenNib为例，它是一个具有计数功能的遗物，它的设计目标是为玩家提供一个奖励机制，使得每打出一定数量的攻击卡牌，就能自动升级一张卡牌。
    PenNib 遗物的核心是 onCardPlayed 函数，它会在每次玩家打出卡牌时被调用。在这个函数中，我们检查当前打出的卡牌是否为攻击卡牌（通过 card->getType()），如果是，则增加 attackCounter_ 的值。如果计数器值达到了10，意味着玩家已经打出了10张攻击卡牌，此时调用 CombatSystem::getInstance()->upgradeCard(card) 来升级当前卡牌，并将计数器重置为0，等待下次触发。
```
class PenNib : public Relic {
private:
    int attackCounter_; // 记录攻击牌的数量
public:
    PenNib(): Relic("PenNib", "Every 10th Attack you play upgrade that card",1), attackCounter_(0) {}
    void onCardPlayed(std::shared_ptr<Card> card) override {
        if (card && card->getType()==ATTACK) { // 假设 Card 类有 isAttackCard 方法
            attackCounter_++;
            if (attackCounter_ == 10) {
                CombatSystem::getInstance()->upgradeCard(card);
                attackCounter_ = 0;
            }
        }
    }
};
AUTO_REGISTER_RELIC(PenNib);
```
- **卡牌设计**
    同样的，对于卡牌而言，其在不同情况下产生的效果是不同的，比如有的卡牌的触发时机是回合结束，有的是打出等，我们可以沿用前面的设计思路，提供虚函数，在相应的时机触发卡牌的相应的功能。
- **EventSystem设计**
    EventSystem和CombatSystem是非常相似的，区别在于EventSystem管理的是战斗外的所有资源和事件触发，而CombatSystem管理的是战斗内的资源和事件触发。
- [x] **随机数管理**
    杀戮尖塔中有非常多的随机事件，如卡牌的掉落，敌人的出现，进入问号时出现的随机事件，这些随机事件都需要随机数生成，我们使用了随机数管理器，同时提供函数进行随机卡牌，随机怪物等的生成，它管理了游戏中所有随机数生成器，并提供统一的接口。
   
```
 
    std::shared_ptr<Card> RandomGenerator::getRandomCard(int type, int rarity)
    {
        // 使用CARD随机生成器
        std::vector<std::string> allCardNames = CardRegistry::getAllCardNames();
        if (allCardNames.empty()) {
            throw std::runtime_error("No cards registered");
        }
        while (1)
        {
            // 使用均匀分布来生成一个随机索引
            std::uniform_int_distribution<> dist(0, allCardNames.size() - 1);
            int randomIndex = dist(rng[CARD]);
            // 获取随机卡牌名称
            std::string randomCardName = allCardNames[randomIndex];
            // 创建对应的卡牌对象
            auto card = CardRegistry::createCard(randomCardName);
            //如果满足要求则进行返回
            if ((type == ALL || type == card->getType()) && (rarity == ALL || rarity == card->getRarity()) && card->getType() != STATUS)
            {
                return card;
            }
        }
    }
```
    
- [x] **精灵生成器**
    在杀戮尖塔中有众多卡牌，其可视化效果需要精灵来实现，对于一个卡牌来说，它的描述，名称，费用等都已经在进行了定义，只需要加上卡面就可以构成一张完整的卡牌。因此我们使用精灵生成器，它管理了卡牌精灵的生成，并提供统一的接口进行精灵的创建以及更新。
- [x] **随机事件抽象化**
    EventScene 类是一个通用的事件场景类，它通过抽象化了事件的背景图像、事件图像、事件文本和按钮列表，为游戏中的不同事件场景提供了一个基础框架。在这个框架中，我们通过传递不同的参数来创建具有不同背景和交互内容的事件场景。
通过继承 EventScene 类或创建新的事件类，开发者可以轻松地扩展事件系统，添加新的事件类型而无需改变现有的 EventScene 类代码。只需要通过简单的继承和参数传递，就能实现特定场景的个性化逻辑。
事件的具体逻辑（如背景图像、事件图像、文本和按钮）被集中到 EventScene 类中，这使得游戏的其他部分不需要关注事件的具体实现细节。当我们添加新的事件类型时，不需要修改原有的系统代码，而只需在事件类中实现具体的行为。
事件类的设计允许开发者为每个事件传递不同的按钮集合，这些按钮可以有不同的文本和触发的回调函数。通过这种方式，按钮的行为完全由事件的实现控制，而不需要对每个事件的交互进行硬编码。这种设计方法为事件的交互性提供了极大的灵活性，可以根据实际需求动态调整每个事件的按钮和其对应的行为。
```
class EventScene : public Scene {
public:
    EventScene(const std::string& backgroundImage, const std::string& eventImage, const std::string& eventText,const std::vector<std::pair<std::string,std::function<void()>>>& buttons) : backgroundImage_(backgroundImage), eventImage_(eventImage),eventText_(eventText), buttons_(buttons) {}
virtual bool init() override;
static EventScene* create(const std::string& backgroundImage, const std::string& eventImage, const std::string& eventText, const std::vector<std::pair<std::string，std::function<void()>>>& buttons);
private:
    std::string backgroundImage_;
    std::string eventImage_;
    std::string eventText_;
    std::vector<std::pair<std::string, std::function<void()>>> buttons_;
}
```
 
   
- [x] **地图随机生成**

  地图随机生成系统通过一系列的预定义房间模板（如战斗房间、商店房间、随机事件房间等）来生成每一局游戏的关卡。每个房间的位置、类型、敌人配置等均为随机生成。
  通过MapNodes按层组织节点，除了必须固定的节点外，都按照一定的规律随机生成，这样每一层的节点都可以独立管理，并且在不同层之间实现连接。这样的设计让每一层的地图都能够独立且灵活地进行调整和扩展，比如更换地图元素或增加新的层次。
- [x] **药水功能**
药水是玩家在游戏过程中获得的可消耗物品，具有即时生效的效果。药水可以用于增强角色的战斗力、恢复生命、增加某些临时属性等。玩家可以通过战斗、商店、事件等途径获得药水。因为药水的一次性特点，直接将其内嵌在头栏中进行使用。
- [x] **C++11特性使用**  
  使用C++11引入的新特性提升代码的可读性、可维护性及性能，包括语法简化和性能优化。

- *auto关键字使用*  
  `auto`关键字用于自动推导变量类型，减少冗长的类型声明，使代码更加简洁。
```
auto player = EventSystem::getInstance()；
auto headbar = HeaderBar::create(player);
```

- *构造函数参数列表初始化*  
  使用构造函数的初始化列表来初始化成员变量，可以提高代码效率，避免不必要的默认构造和赋值操作。
 
```
Relic(string name, string description, int rarity) :
     name_(name),
     description_(description),
     rarity_(rarity) {};
```

- *继承构造函数*  
  利用C++11中的继承构造函数特性，使派生类自动继承基类的构造函数，避免了冗余代码的编写。
```
// 鲜血药水：回复目标最大生命值的 20%
    BloodPotion() : Potion("BloodPotion", "Restores 20% of the target's max health", 0, 50, 1, true) {}
```

- *空指针nullptr*  
  使用`nullptr`代替传统的`NULL`来表示空指针，它是一种类型安全的指针空值表示方式。
```
if (cardPainting == nullptr){

    CCLOG("Unable to Create CardPainting For %s", card->getName());

}
```



- *functional使用*  
  引入`<functional>`库，使用其中的功能，如`std::function`来封装可调用对象，提高代码的灵活性和可扩展性。

- *lambda表达式*  
  使用lambda表达式来简化代码，特别是在需要快速定义匿名函数时，减少了定义函数的冗余代码。   
```
listener->onTouchBegan = [this, card1, card2, card3, randomCard1, randomCard2, randomCard3, headbar, Label1](Touch* touch, Event* event) {
                        ...
}
```


- [x] **游戏还原度高**  
  设计和实现的游戏系统、功能和场景高度还原原始设计或其他参考素材，增强了游戏的沉浸感和真实性。

- [x] **常变量集中定义**  
  将所有常量、配置项集中定义，方便管理和修改，提高代码的可维护性，避免硬编码。

- [x] **意图显示实时更新**  
   游戏系统需要实时反馈玩家的操作意图，确保界面能够动态地反映当前的状态，并提升用户的交互体验。具体实现方法如下：

  - **调CombatSystem的`onAttack`函数**  
    通过调用`CombatSystem`中的`onAttack`函数来处理攻击相关的计算逻辑。

  - **函数执行但不进行攻击**  
    在意图显示模式下，`onAttack`函数会执行攻击的计算过程，但不会触发实际的伤害处理操作。

  - **不调用`takeDamage`函数**  
    如果当前处于意图显示模式，`onAttack`函数不会调用`takeDamage`函数，因此敌人不会实际受到伤害。

  - **计算执行但不改变状态**  
    在意图显示模式下，只会进行计算，并展示结果，但不会对游戏状态产生实际改变，确保玩家可以查看操作的效果而不实际影响游戏进程。


## 分工
| 成员       | 任务总结                                               | 任务描述                                                                                              | 贡献度 |
|------------|------------------------------------------------------|------------------------------------------------------------------------------------------------------|--------|
| **童敬业** | 负责核心系统的设计与实现，涉及战斗和事件系统的架构与功能。   | 负责战斗系统与事件系统的设计与实现，包括战斗场景、手牌层、生物层的设计与实现，生物类、玩家类、注册类、随机生成器与精灵生成器的设计与实现 | 35%    |
| **叶文成** | 负责游戏界面和辅助系统的设计与实现，包含多种功能模块的实现。 | 负责随机地图生成、头栏展示、初始进入界面、菜单界面、卡牌操作系统、音量设置、随机事件、遗物、药水、休息房间、商店房间与退出管理的实现 | 35%    |
| **徐家乐** | 负责战斗和卡牌相关的系统设计与实现，涉及buff、怪物和卡牌机制。   | 负责buff类的注册、添加、定义与实现，怪物类的实现与行为定义，卡牌设计，"燃烧契约"卡牌场景选择与点击实现，战斗系统部分事件与显示更新 | 30%    |
