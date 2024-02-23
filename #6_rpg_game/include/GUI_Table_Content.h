#pragma once
#include "Character.h"
#include "FontC.h"
#include "GUI/common.h"


class GUI_Table_Content
{
///////////////////////////////////////////////////////////////////////////////
//Luggage
public:
    class Luggage: public ListStyle
    {
    public:
        enum class SeqID
        {
            Cell_LK = 0,
            Cell_ULK,
            Cell_SLT
        };
        class ItemDesc : public Item
        {
        public:
            ItemDesc() = default;
            ItemDesc( const Item& item ) : Item(item) {}
            Character::Luggage::ItemDesc text_desc;
            Vec2 pos_draw_amount{};
            Color color_amount = WHITE;
        };
    public:
        void LoadData( const std::string& fileName );
        void InitPosOrigin( const Vec2& pos_main_table );
        void InitData( 
            const int amountCell, const int amountItem,
            const std::vector<Character::Luggage::ItemDesc>& vItemsDesc 
        );
        void InitTabControl( const std::vector<Layer::Var>& var_tabs );
        void Update( 
            const std::vector<Character::Luggage::ItemDesc>& vItemsDesc 
        );
        void RemoveItem( const int id_item_active );
        void AddItem( const int id_item_active );
        void Draw( const FontC& font ) const;

        const Entity& GetAreaDesc() const { return area_desc; }
        const Vec2 GetPosAreaAction() const;
        const Vec2 GetPosAreaConfirm() const;

        const int& GetScale() const { return ListStyle::scale; }
        const std::string& GetDataBuff() const;

        const bool IsNewPickCell() const { return isNewPick; }
        const Item ItemPick();

        void TakeBackPermissionKeyControl();
        
        int idItemPick = -1;
    private:
        std::vector<ItemDesc> items_desc;
        Entity area_desc{};
        Vec2 pos_main_table{};
        Vec2 pos_area_action{};
        int amount_tab_control = 0;
        std::unordered_map<Layer::Var, bool> vBeAllowKeyControl;
    };
public:
    class Lug_Action : public AreaAction
    {
    public:
        enum class SeqID
        {
            Use, Remove
        };
        void Update( Character& tie, GUI_Table_Content::Luggage& gui_lug, const int id_item_active );
    private:
        void Use( Character& tie, GUI_Table_Content::Luggage& gui_lug, const int id_item_active );
        void RemoveItem( Character& tie, GUI_Table_Content::Luggage& gui_lug, const int id_item_active );
    };
public:
    class Lug_Confirm : public AreaConfirm
    {
    public:
        void Update();
    };
/////////////////////////////////////////////////////////////////////////////
//Main member of GUI_Table_Content
public:
    void LoadAsset( const Character& tie, const FontC& font, const Vec2& pos_main_table );
    void Update( Character& tie );
    void Draw() const;
private:
    void SetLayerById( const int& id );
private:
    bool isLoad = false;
    FontC font;

    Luggage luggage;
    AreaDesc lug_area_desc;
    Lug_Action lug_action_item;
    Lug_Confirm lug_area_notify;
};
