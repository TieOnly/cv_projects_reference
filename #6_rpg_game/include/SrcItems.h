#pragma once
#include "SrcEntities.h"

class Item: public Entity
{
public:
    enum class Influence
    {
        TimeIFL = 0,

        Attack_Add,
        Attack_Add_PC,
        Attack_Add_VT,
        Attack_Add_PC_VT,
        
        Add_HP_Cur,
        HP_Add,
        HP_Add_PC,
        HP_Add_VT,
        HP_Add_PC_VT,

        Add_MP_Cur,
        MP_Add,
        MP_Add_PC,
        MP_Add_VT,
        MP_Add_PC_VT,
        
        Armor_Add,
        Armor_Add_PC,
        Armor_Add_VT,
        Armor_Add_PC_VT,

        Speed,
        Skill_CountDown,
        Crit,
        Dame_To_HP,

        Dizzy,

        Count
    };
    enum class Quality
    {
        Gray, Green, Blue, Purple, Orange, Gold
    };
public:
    Item() = default;
    Item( const Entity& surface );
    void SetItem( 
        const std::string& name,
        const std::string& info,
        const bool isLocked,
        const int cost_sell
    );
    void SetEffects( const std::vector<Vei2>& effs );
    const std::string& GetName() const { return name; }
    const std::string& GetInfo() const { return info; }
    const std::vector<std::pair<Influence, int>>& GetEffects() const;
    bool isLocked = true;
private:
    std::string name = "";
    std::string info = "";
    Quality quality = Quality::Gray;
    std::vector<std::pair<Influence, int>> effects;
    int cost_sell = 0;
};

class SrcItems
{
public:
    static bool LoadSrcItems( const std::string& fileName ) 
        { return Call()._LoadSrcItems( fileName ); }
    static const int GetAmountEntity() 
        { return Call()._GetAmountEntity(); }
    static const int IndexById( const int id )
        { return Call()._IndexById(id); }
    static const Item& ItemByIndex( const int index )
        { return Call()._ItemByIndex(index); }
    static const Item& ItemByID( const int id )
        { return Call()._ItemByID(id); }
private:
    bool _LoadSrcItems( const std::string& fileName );
    void InitSrcItems();

    const int _GetAmountEntity() const;
    const int _IndexById( const int id ) const;
    const Item& _ItemByIndex( const int index ) const;
    const Item& _ItemByID( const int id ) const;
    static SrcItems& Call() 
    {
        static SrcItems srcItem;
        return srcItem;
    }
private:
    SrcEntities surfItems;
    std::vector<Item> items;
    Item item_default{};

    int iItem = 0;
    std::vector<std::string> nameItems;
    std::vector<std::string> infoItems;
    std::vector<std::vector<Vei2>> effItems;
    std::vector<int> costSellItems;
};