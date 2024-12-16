#pragma once
#include "Potion.h"
#include "cocos2d.h"
#include "Player.h"
#include "Relic.h"
#include "Card.h"
#include "CardLayer.h"
#include <memory> // For smart pointers
using namespace std;
using namespace cocos2d;

class EventSystem : public Node {
private:

    static EventSystem* instance;

public:
    int level;  // ��ǰ�ؿ�
    string name_;                                // �������
    string character_;                           // ��ҽ�ɫ
    int health_;                                 // ��ǰ����ֵ
    int fullHealth_;                             // �������ֵ
    int maxPotions_;                             // ҩˮ�������
    int coins_;                                  // �������
    vector<std::shared_ptr<Potion>> potions_;    // ҩˮ
    vector<std::shared_ptr<Relic>> relics_;      // ����
    vector<std::shared_ptr<Card>> cards_;        // ��ǰ����
    // ���캯������������
    EventSystem();
    ~EventSystem();

    // ��ʼ��ͷ��
    bool init();

    // ��ȡΨһʵ��
    static EventSystem* getInstance();

    // ���úͻ�ȡ����
    void setPlayerInfo(const string& name, const string& character, int fullHealth, int coins);

    // ���������Ϣ�ĺ���
    void setHealth(int health);
    void setCoins(int coins);
    void setPotions(const vector<std::shared_ptr<Potion>>& potions);
    void setLevel(int level);
    void setFullHealth(int health_);

    // ��ȡͷ����Ϣ
    int getCurrentHealth();
    int getFullHealth();
    int getCoins();

    // �¼�ϵͳ
    int changeCoins(int coinChange);                   //�޸Ľ��
    void changeHealth(int healthChange);               //�޸�����ֵ����Ҫ�ж��Ƿ�����
    void changeMaxHealth(int maxHealthChange);         //�޸��������ֵ
    int upgradeCard(std::shared_ptr<Card> card);       //�Կ��ƽ�����������
    int upgradeCard(Card* card);
    int deleteCard(std::shared_ptr<Card> card);        //�Կ��ƽ�������ɾ��
    int deleteCard(Card* card);
    void addCard(std::shared_ptr<Card> card);          //��ӿ���
    int addPotion(std::shared_ptr<Potion> potion);     //���ҩˮ
    void addRelic(std::shared_ptr<Relic> relic);       //�������
};
