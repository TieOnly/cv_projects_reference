#pragma once
#include "SrcEntities.h"
#include "SrcItems.h"
#include "FontC.h"

class InfoChar
{
///////////////////////////////////////////////////////////////////////////
public:
    enum class SkillID
    {
        Default = 0,
        Water1,
        Wind1,
        Fire1,
    };
    class Skill
    {
    public:
        Skill(
            const int id,
            const std::string name,
            const int rank,
            const float damageBuff,
            const float duraPeriod
        );
        const int& GetId() const;
        const std::string& GetName() const;
        const int& GetRank() const;
        const float GetDamageBuff() const;
        const float& GetTimerPeriod() const;
        const float& GetDuraPeriod() const;
        const int& GetIdPickSkill() const;
        void Fire();
        void Update( const float dTime );
        bool isDonePeriod = true;
    private:
        int id = -1;
        std::string name = "";
        int rank = 0;
        float damageBuff = 0.0f;
        float duraPeriod = 0.0f;
        float timerPeriod = 0.0f;
    };
///////////////////////////////////////////////////////////////////////////
public:
    class Luggage
    {
    public:
        struct ItemDesc
        {
            int id;
            int amount;
            int order;
            bool isLocked;
        };
        
        bool LoadData( const std::string& fileName );
        void InitItems();
        void PushItem( const int& id_item );
        void RemoveItem( const int& id_item );

        const std::vector<ItemDesc>& GetItemsDesc() const { return items; }

        const Item& ItemByIndex( const int index ) const;
        Item& ItemByIndex( const int index );
        const int& GetAmountCell() const { return amountCell; }
        const int& GetAmountItem() const { return amountItem; }
        const std::vector<int>& GetItemsID() const { return items_id; }
        const std::vector<int>& GetItemsAmount() const { return items_amount; }
        const std::vector<int>& GetItemsOrder() const { return items_order; }
    private:
        FileIO data;

        int amountCell = 0;
        int amountItem = 0;
        std::vector<ItemDesc> items;
        std::vector<int> items_id;
        std::vector<int> items_amount;
        std::vector<int> items_order;
        std::vector<bool> items_isLocked;
    };
///////////////////////////////////////////////////////////////////////////
protected:
    std::mt19937 rng = std::mt19937(std::random_device()());

    SrcEntities SSkills;
    std::vector<Animation> Askills;
    SkillID iCurSkill = SkillID::Default;
    int iAskill = 0;
    int sizeAskill = 0;
    std::vector<int> idPickSkills;
    std::vector<Skill> skills;
    int amountSkill = 0;

protected:
    int rank = 1;
    float rateRank = 0.0f;
    float speed = 180.0f;
    float vision = 100.0f;
    float range = 30.0f;
    float dame_to_hp = 0.10f;
     
    //=========Attack================//
    int atk_min_base = 30;
    int atk_max_base = 40;
    int atk_add = 0;
    int atk_add_pc = 0;
    int atk_add_vt = 0;
    int atk_add_pc_vt = 0;

    int attack_min = atk_min_base;
    int attack_max = atk_max_base;
    
    //=========HP, MP================//
    int hp_base = 300;
    int hp_add = 0;
    int hp_add_pc = 0;
    int hp_add_vt = 0;
    int hp_add_pc_vt = 0;

    int mp_base = 100;
    int mp_add = 0;
    int mp_add_pc = 0;
    int mp_add_vt = 0;
    int mp_add_pc_vt = 0;

    int max_hp = hp_base;
    int cur_hp = 200;
    int max_mp = mp_base;
    int cur_mp = 80;
    
    //=========Armor================//
    int armor_base = 3;
    int armor_add = 0;
    int armor_add_pc = 0;
    int armor_add_vt = 0;
    int armor_add_pc_vt = 0;

    int armor_max = armor_base;

public:
    //Get
    const int CalculateDamage();

    const int& GetRank() const;
    const float& GetRateRank() const;

    const int& GetCurHP() const;
    const int& GetCurMP() const;
    const int& GetMaxHP() const;
    const int& GetMaxMP() const;

    const std::vector<int>& GetIdPickSkills() const;
    const Skill& GetSkillById( const int id ) const;
    
    Luggage luggage;

    //Set
    void PickSkillById( const int id );
};