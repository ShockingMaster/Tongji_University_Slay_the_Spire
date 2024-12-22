# Tongji_University_Slay_the_Spire
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
#define AUTO_REGISTER_CARD(className)                          \
    const bool className##Registered = []() {                  \
        CardRegistry::registerCard(#className, []() {          \
           return std::make_shared<className>();               \
        });                                                    \
        return true;                                           \
    }();
```
- 这样在每一张卡牌完成设计之后，只需要调用AUTO_REGISTER_CARD宏，即可完成卡牌的注册。
```c++
AUTO_REGISTER_CARD(Defense)
```
- [x] **事件驱动机制**
- **CombatSystem设计**
在杀戮尖塔这款游戏中，由于种类繁多的buff和遗物的存在，一张很简单的卡牌的打出都可能触发非常复杂的行为。举例说明，对于打出一张普通的打击来说，如果角色具有余像buff（每打出一张卡牌，角色获得一点格挡）和势不可挡buff（每当角色获得格挡时，对随机敌人造成一定伤害）。而事件驱动机制使得这种复杂的触发行为变得可能。

    CombatSystem的设计思路是，将所有卡牌的触发行为抽象为事件，并将事件分为多种类型，例如对于攻击事件
    ```c++
    void onAttack(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> target,
		int& numeric_value_, std::string cardName = "", bool isForIntentionUpdate = false);    
    ```
    当一个卡牌具有攻击效果时，它仅仅是告诉CombatSystem发生了攻击事件，而不会管角色身上具体有什么buff，有什么遗物，而是由CombatSystem调用相应的处理函数，以下是onAttack函数的部分实现。
    ```c++
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
    它会通过完成将onAttack这一事件分发给Buff，遗物等完成统一调度，同时也会进行音效、动作的管理。这一种做法将复杂的行为拆解成一个个事件进行有序触发，逻辑清晰，易于维护。
- **Buff、遗物设计**
    从上面的CombatSystem中的设计也可以看出，为了让每一种事件CombatSystem能够调用到buff的相应函数，每一个buff也必须能够处理相应的事件，为此，我们采用虚函数的方法进行实现，在Buff基类中定义所有事件处理的虚函数，但没有实际效果。
    ```c++
    virtual void onAttack(int& numeric_value_, std::string cardName = "", std::shared_ptr<Creature> user = nullptr, std::shared_ptr<Creature> target = nullptr) {};      
    ```
    而在相应的buff中，我们可以重写这些虚函数，实现具体的效果。例如对于精准buff而言，它会将卡牌名为“小刀”的攻击牌的攻击数值进行提高
    ```c++
    void onAttack(int& numeric_value_, std::string cardName,
        std::shared_ptr<Creature> user, std::shared_ptr<Creature> target)
    {
        if (cardName == "Blade" || cardName == "Blade+")
        {
            numeric_value_ += effect_layer_;
        }
    } 
    ```
    这样的设计在兼容Combatsystem的同时，也保证了Buff的可扩展性，支持Buff在多种情况下的效果能够触发相应的效果。
- **卡牌设计**
    同样的，对于卡牌而言，其在不同情况下产生的效果是不同的，比如有的卡牌的触发时机是回合结束，有的是打出等，我们可以沿用前面的设计思路，提供虚函数，在相应的时机触发卡牌的相应的功能。
- **EventSystem设计**
    EventSystem和CombatSystem是非常相似的，区别在于EventSystem管理的是战斗外的所有资源和事件触发，而CombatSystem管理的是战斗内的资源和事件触发。
- [x] **随机数管理**
    杀戮尖塔中有非常多的随机事件，如卡牌的掉落，敌人的出现，进入问号时出现的随机事件，这些随机事件都需要随机数生成，我们使用了随机数管理器，同时提供函数进行随机卡牌，随机怪物等的生成，它管理了游戏中所有随机数生成器，并提供统一的接口。
    ```c++
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
- [x] **C++11特性使用**

- *auto关键字使用*
- *构造函数参数列表初始化*
- *继承构造函数*
- *空指针nullptr*
- *functional使用*
- *lambda表达式*

- [x] **游戏还原度高**
- [x] **常变量集中定义**
- [x] **意图显示实时更新**


