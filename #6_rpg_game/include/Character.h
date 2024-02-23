#pragma once
#include "TilesMap.h"
#include "SrcEntities.h"
#include "Sprite.h"
#include "InfoChar.h"
#include "utils/VarTimer.h"
#include "GUI/common.h"

class Character: public InfoChar
{
public:
    enum class Moves
    {
        Walking,
        Standing,
        Upping,
        Downing,
        Round,
        Count
    };
    enum class Actions
    {
        Hurt = 0,
        Dead,
        Attack,
        None
    };
public: 
    Character( const Vec2& pos );
    void ProcessInput( const bool allow_control );
    void SetDirection();
    void ProcessAction();
    void ProcessActiveSprite( Sprite& sprite, const RectF& camRect );
    void IgnoreActive();
    void ProcessCollision( const TilesMap& tileMap, const float dTime );
    
    void Update( float dTime );
    void Draw() const;

    const Vec2& GetVel() const;
    const float GetSpeed() const;
    const Vec2& GetPos() const;
    const float GetRange() const;
    const float GetVision() const;
    const RectF GetRectVision() const;
    const RectF GetPosBox() const;
    const RectF GetPosBoxNext( const Vec2& dir, const float dTime ) const;
    const bool IsLeft() const;
    const Moves& GetCurMove() const;
    const Actions& GetCurActions() const;
    void PickSkillById( const int id );
    bool IsLanding() const;
private:
    const int GetIMove() const;
    const int GetIAction() const;
    const int GetISkill() const;
///////////////////////////////////////////////////////////////////////
private:
    SrcEntities SMoves;
    SrcEntities SActions;
    Vec2 pos;
    Vec2 draw_offset = { -32.0f, -26.0f };
    Vec2 direct = { 0.0f, 0.0f };
    Vec2 vel = { 0.0f, 0.0f };
    Vec2 dimension = { 64.0f, 64.0f };
    
    static constexpr float speedJump = 560.0f;
    static constexpr float gravity = 18.0f;
    bool isLanding = false;
    bool isActiving = false;

    static constexpr Vec2 posBoxOffset = { 0.0f, 10.0f };
    static constexpr float hwPosBox = 20.0f;
    static constexpr float hhPosBox = 20.0f;

    bool isLeft = false;
    std::vector<Animation> Amoves;
    std::vector<Animation> Aactions;
    Moves iCurMove = Moves::Standing;
    int iMove = 0;
    int sizeAmoves = 0;
    Actions iCurAction = Actions::None;
    int iAction = 0;
    int sizeAactions = 0;

    Sound stepSound;
    int nFramePerSound = 4;
///////////////////////////////////////////////////////////////////////////////
public:
    enum class StateEff
    {
        Fall_Down_Water = 0,
        Under_Water,
        Count
    };
public:
    void LoadEffects( const std::string& fileName, const FontC& font );
    void UpdateEff( const float dTime );
    void UpdateEffText( const float dTime );
    void DrawEff() const;
    void DrawEffText() const;
private:
    void SetEffByIndex( Entity& e, const int index );
private:
    SrcEntities EffActive;
    FontC font;
    FontC fontNum;

    std::vector<Entity> effChars;
    int iEffChar = 0;
    bool tickEffActives[(int)StateEff::Count] = {false};  
///////////////////////////////////////////////////////////////////////////////
public:
    bool UseItem( const int& id_item );
    void GetOf_IFL_Item( const int& id_item );
    void InjureCurHp( const int amount );
    void ProcessIFL_BeTimeUp();
private:
    void IFL_Tick_VarTimer( const int id, const int dura );

    //=========Attack================//
    void IFL_Calc_ATK();

    void IFL_Atk_Add( const int amount );
    void IFL_Atk_Add_PC( const int amount );
    void IFL_Atk_Add_VT( const int amount );
    void IFL_Atk_Add_PC_VT( const int amount );

    //=========HP, MP================//
    void IFL_Calc_HP_Max();
    void IFL_Calc_MP_Max();

    void IFL_Add_HP_Cur( const int amount );
    void IFL_HP_Add( const int amount );
    void IFL_HP_Add_PC( const int amount );
    void IFL_HP_Add_VT( const int amount );
    void IFL_HP_Add_PC_VT( const int amount );
    void IFL_HP_PerSeconds( const int amount );

    void IFL_Add_MP_Cur( const int amount );
    void IFL_MP_Add( const int amount );
    void IFL_MP_Add_PC( const int amount );
    void IFL_MP_Add_VT( const int amount );
    void IFL_MP_Add_PC_VT( const int amount );
    void IFL_MP_PerSeconds( const int amount );

    //=========Armor================//
    void IFL_Calc_Armor();

    void IFL_Armor_Add( const int amount );
    void IFL_Armor_Add_PC( const int amount );
    void IFL_Armor_Add_VT( const int amount );
    void IFL_Armor_Add_PC_VT( const int amount );

    void IFL_Speed( const int amount );
    void IFL_Skill_CountDown( const int amount );
    void IFL_Crit( const int amount );


    void IFL_Dizzy( const int amount );
};