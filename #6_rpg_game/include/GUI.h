#pragma once
#include "Character.h"
#include "FontC.h"
#include "SrcItems.h"
#include "GUI/common.h"
#include "utils/VarTimer.h"

class GUI
{
/////////////////////////////////////////////////////////////////////////////
//StateCharUI
public:
    class StateCharUI: public Component
    {
    public:
        void Init();
        void Update( const Character& tie );
        void Draw( const FontC& font ) const;
        void Draw_IFL_Character( const FontC& font );
    private:
        float scale = 1.0f;
        Vec2 pos_circle{};
        RectF maxHp_rect{};
        RectF curHp_rect{};
        RectF maxMp_rect{};
        RectF curMp_rect{};
        int rank = 0;
        float rateRank = 66.66f;
        float rate_hp = 0.0f;
        float rate_mp = 0.0f;
        int max_hp = 0;
        int max_mp = 0;
        int cur_hp = 0;
        int cur_mp = 0;

        Vec2 pos_origin_ifl{60.0f, 80.0f};
        std::vector<Vec2> vPosDrawIFLDura;
        float width_each_ifl = 60.0f;
        float pad_ifl_dura = 30.0f;
    };
//PickSkillUI
public:
    class SkillUI: public Component
    {
    public:
        void Init();
        void SetAvatarSkill( const Character& tie );
        void Update( Character& tie );
        void Draw() const;
    private:
        Vec2 pos_origin{};
        int amountEntity = 0;
        int amountCell = 5;
        Vec2 cell_dimens{};
        RectF rect_content_default{};
        float cell_padding = 0.0f;
        int idCell = 3;
        std::vector<int> listPickSkill;
        std::vector<RectF> dura_rects;
        std::vector<bool> dura_isDones;
        std::vector<bool> cell_isPicks;
    };
//Map
// public:
//     class Map: public GUI::Component
//     {
//     public:
//         void Init();
//         void Update();
//         void Draw( const FontC& font) const;
//     private:
//         Vec2 pos_mini_map{};
//         Vec2 dimens_mini_map{};
//         Vec2 center_header{};
//         FontC font;
//         std::string title_header = "";
//     };

//Buttons
public:
    class Buttons: public Component
    {
    public:
        enum class SeqID
        {
            Menu = 0,
            Chat,
            Map
        };
    public:
        void Init();
        void Update();
        void Draw() const;
    private:
        int amount = 0;
        std::vector<Vec2> seq_pos;
        std::vector<Vec2> seq_dimens;
    };
//Tables
public:
    class Tables: public Component
    {
    public:
        friend class GUI;
        enum class SeqID
        {
            Main_Table = 0,
            Front_Tab,
            Behind_Tab,
            Close_Btn,
            Count
        };
        enum class Tab
        {
            Luggage = 0,
            Skill,
            Intrinsic,
            Info,
            Count
        };
    public:
        void Init();
        void Update();
        void Draw( const FontC& font ) const;
        const int& GetCurTab() const;
        const Vec2& GetPosMainTable() const 
            { return seq_pos_default[ids[(int)SeqID::Main_Table]]; }
        const int GetScale() const 
            { return scale; }
    private:
        void InitDefault();
        void SetTab();
        void ExitTable();
        void OnChildContent();
    private:
        int amount_tab = 0;
        Vec2 center_screen = std::move( rayCpp::GetCenterScreen() );
        float distance_match = 0.0f;
        float posY_mainTable = -1.0f;
        int scale = 1;
        int ids[(int)SeqID::Count + 1] = {0};
        std::vector<Vec2> seq_dimens;
        std::vector<Vec2> seq_pos_default;
        
        int curTab = (int)Tab::Luggage;
        int lastTab = -1;
        bool bKBControl = true;
        Layer::ID curLayer = Layer::ID::Luggage;
        std::vector<std::string> tab_names;
    };

///////////////////////////////////////////////////////////////////////
//Main Member of GUI 
public:
    void LoadAsset( const Character& tie );
    void Update( Character& tie );
    void Draw();

    //Get Tabel
    const Tables& GetTable() const { return tables; }
    const Vec2& GetPosMainTable() const { return tables.GetPosMainTable(); }
    
private:
    bool isLoad = false;
    FontC font;
    FontC fontNum;

    StateCharUI stateChar;
    SkillUI pickSkill;
    Buttons buttons;
    Tables tables;

    // Map map;
};