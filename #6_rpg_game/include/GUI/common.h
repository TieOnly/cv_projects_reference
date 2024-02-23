#pragma once
#include "SrcEntities.h"
#include "SrcItems.h"
#include "FontC.h"
#include "FrameTime.h"
#include "GUI/layer.h"

//SrcEntities Common
class GuiCommon
{
public:
    enum class SeqID
    {
        Area_Notify_Vert,
        Area_Notify_Hori,
        Button_0,
        Button_1,
        Btn_Frame_Slt
    };
    static const GuiCommon& Get() 
        { return Retrieve(); }
    
    static void LoadSrcEntity( const std::string& fileName )
    {
        Retrieve().Sentities.LoadSrcEntities(fileName);
    }
    static void LoadVariable( const std::string& fileName ) 
        { Retrieve()._LoadVariable(fileName); }

    static const Entity& GetEntity( const SeqID& id )
        { return Retrieve().Sentities.EntityByIndex((int)id); }

    static const Entity& GetEntity( const int id )
        { return Retrieve().Sentities.EntityByIndex(id); }
    
    std::string text_confirm;
private:
    static GuiCommon& Retrieve()
    {
        static GuiCommon gui_common;
        return gui_common;
    }
    void _LoadVariable( const std::string& fileName );
    SrcEntities Sentities;
    FileIO data_var;
    std::set<std::string> filesLoaded;
    
};
//Note Action
class NoteAction
{
public:
    static void Init( const FontC& font ) { Get()._Init( font ); }
    static void Push( const std::string& text ) 
        { Get()._Push( text ); }
    static void Update( const float& dTime ) { Get()._Update( dTime ); }
    static void Draw() { Get()._Draw(); }
private:
    static NoteAction& Get()
    {
        static NoteAction note;
        return note;
    }
    void _Init( const FontC& font );
    void _Push( const std::string& text );
    void _Update( const float& dTime );
    void _Draw();
    FontC font;
    Vec2 pos_center;
    float dura = 1.0f;
    float distanceY = 50.0f;
    int amount = 0;
    std::vector<std::pair<float, std::string>> show;
};

//Component
class Component
{
public:
    void LoadSrc( const std::string& fileName );
protected:
    SrcEntities Sentities;
    std::vector<Entity> entities;
};

//Button
class Button : public Entity
{
public:
    void Update();
    void SetTitle( const std::string& title ) 
        { this->title = title; }
    const bool IsClicked() const;
    const bool IsReleased() const;
    const bool IsPressed() const;
    void Draw( const FontC& font ) const;
    const std::string& GetTitle() const { return title; }
protected:
    std::string title;
};

//ListStyle
class ListStyle: public Component
{
public:
    void LoadData( const std::string& fileName );
    void ScaleVariable();
    void Update();
    void Draw( const FontC& font ) const;

    bool isMoving = false;
protected:
    void ProcessInput();
    void ProcessEventRelease();
    void LerpVerticalList( const float distance, const float speed = 0.08f );
protected:
    int amount_elem_int = 0;
    int amount_elem_hori = 0;
    int scale = 1;
    float elem_size = 0.0f;
    float elem_margin = 0.0f;
    std::vector<int> elems_id;
    float top_elems = 0.0f;
    float bottom_elems = 0.0f;
    float offsetY = 0.0f;
    float delta_move= 0.0f;
    float inertia = 0.0f;
    RectF boundary_frame{};
    Vec2 boundary_dimens{};
    Vec2 padcorner_vs_maintable;
    std::vector<Vec2> pos_default;
    Entity elem_select{};
    int id_elemActive = -1;
    Vec2 mousePos{};
    bool isNewPick = false;
    bool bAllowKeyControl = true;
    bool isKeyControl = false;
};

//Area Lerp
class AreaLerp : public Entity
{
public:
    AreaLerp() = default;
    AreaLerp( const Entity& entity );
    AreaLerp( const Vec2& pos, const Vec2& dimens );
    void SetNewDest( const Vec2& pos, const Vec2& dimens );
    void Init();
    void Update();
protected:
    void ProcessInput();
    void LerpVertical( const float distance, const float speed = 0.08f );
protected:
    float top = 0.0f;
    float bottom = 0.0f;
    RectF bound_frame{};
    float offsetY = 0.0f;
    float delta_move= 0.0f;
    float inertia = 0.0f;
    float height_bound = 0.0f;

    Vec2 mousePos{};
};
//Area Description
class AreaDesc : public AreaLerp
{
public:
    enum class Type { Desc_Item, Desc_Text };

    struct ItemDesc
    {
        std::string name;
        std::string info;
        std::string quality;
        std::string cost_sell;
    };

    AreaDesc() = default;
    void Init( const Type& type );
    void SetFont( const FontC& font ) { this->font = font; }
    void SetItemDesc( const Item& item );
    void SetTextDesc( const std::string& text );
    void Update();
    void Draw() const;
    
    //From origin pos of this entity
    float padding_hori = 10.0f;
    float pad_header = 10.0f;
    float pad_info = 40.0f;
protected:
    Item item{};
    RectF area_desc_separate{};

    FontC font;
    Type type = Type::Desc_Text;
    ItemDesc item_desc;
    std::string text_desc;

};

//Area Action to Item
class AreaAction
{
public:
    void Load( const std::string& data );
    void SetAreaAction( const Vec2& pos_origin, const Entity& btn_surf, const int scale = 1 );
    void Init( const std::vector<std::string>& vActions, Layer::Var toggle_var = Layer::Var::Default );
    void Update();
    void Draw( const FontC& font ) const;
    
    const bool IsBtnClicked( const int index ) const;
    const bool IsBtnPressed( const int index );
    void SetAllowKeyControl( const bool allow ) { bAllowKeyControl = allow; }

    int id_modal_own = -1;
    
    float pad_hori = 10.0f;
    float pad_vert = 0.0f;
protected:
    bool bUseDataByLoad = false;
    Vec2 pos_center{};
    int scale = 1;
    Entity btn_surf{};
    int amount_action = 0;
    int curBtn_select = 0;
    Entity curBtn_select_surf{};
    std::vector<std::string> vBtnName;
    std::vector<Button> buttons;
    bool isNewPress = false;
    bool bAllowKeyControl = false;
    bool* pToggle_KBC = nullptr;
};

//Area Notify
class AreaConfirm : public Entity
{
public:
    AreaConfirm() = default;
    AreaConfirm( const Entity& surf );
    void SetSurf( const Animation& surf );
    void SetWrapDest( const Vec2& pos, const int scale );
    void SetFont( const FontC& font ) { this->font = font; }
    void Init( const Entity& surf, const Vec2& pos, const int scale );
    void ContentNotify( const std::string& text );
    void Update();
    void Draw() const;
    void GiveIDModalForOwner( AreaAction& owner_action );
    const int& GetIDModal() const { return id_modal; }

    const bool IsYesPressed();
    const bool IsNoPressed();
    const bool IsBtnPressed( const int id );
    //Padding
    Vec2 pad_desc{20.0f, 10.0f};
protected:
    int id_modal = -1;
    AreaDesc area_desc;
    AreaAction area_action;

    FontC font;
    bool display = false;
};
