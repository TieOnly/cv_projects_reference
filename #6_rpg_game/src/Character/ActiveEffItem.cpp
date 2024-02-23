#include "raylibCpp.h"
#include "Character.h"

void Character::ProcessIFL_BeTimeUp()
{
    if( VarTimer::IsVarTimeUp( VarTimer::Var::Heart_Buffer ) )
    {
        IFL_HP_Add_PC_VT( 0 );
    }
    if( VarTimer::IsVarTimeUp( VarTimer::Var::Power_Buffer ) )
    {
        IFL_Atk_Add_PC_VT( 0 );
    }
    if( VarTimer::IsVarTimeUp( VarTimer::Var::Armor_Buffer ) )
    {
        IFL_Armor_Add_PC_VT( 0 );
    }
}
///////////////////////////////////////////////////////////////////////////////
//Tick Variable Timer
void Character::IFL_Tick_VarTimer( const int id, const int dura )
{
    VarTimer::Var varTimer = VarTimer::Var::Count;
    switch (id)
    {
    case 1: varTimer = VarTimer::Var::Heart_Buffer; break;
    case 2: varTimer = VarTimer::Var::Power_Buffer; break;
    case 3: varTimer = VarTimer::Var::Armor_Buffer; break;
    default: break;
    }
    if( varTimer != VarTimer::Var::Count )
    {
        if( VarTimer::GetTimer( varTimer ) > 0.0f )
        {
            VarTimer::ForceVarTimeUp( varTimer );
            ProcessIFL_BeTimeUp();
        }
        VarTimer::Tick( varTimer, dura );
    }
}
///////////////////////////////////////////////////////////////////////////////
//Attack
void Character::IFL_Calc_ATK()
{
    attack_min = atk_min_base + atk_add + atk_add_vt;
    attack_min += (atk_add_pc * attack_min) / 100;
    attack_min += (atk_add_pc_vt * attack_min) / 100;

    attack_max = atk_max_base + atk_add + atk_add_vt;
    attack_max += (atk_add_pc * attack_max) / 100;
    attack_max += (atk_add_pc_vt * attack_max) / 100;
}
void Character::IFL_Atk_Add( const int amount )
{
    atk_add = amount;
    IFL_Calc_ATK();
}
void Character::IFL_Atk_Add_PC( const int amount )
{
    atk_add_pc = amount;
    IFL_Calc_ATK();
}
void Character::IFL_Atk_Add_VT( const int amount )
{
    atk_add_vt = amount;
    IFL_Calc_ATK();
}
void Character::IFL_Atk_Add_PC_VT( const int amount )
{
    atk_add_pc_vt = amount;
    IFL_Calc_ATK();
}
///////////////////////////////////////////////////////////////////////////////
//HP
void Character::IFL_Calc_HP_Max()
{
    max_hp = hp_base + hp_add + hp_add_vt;
    max_hp += (hp_add_pc * max_hp) / 100;
    max_hp += (hp_add_pc_vt * max_hp) / 100;
    if( cur_hp > max_hp ) cur_hp = max_hp;
}
void Character::IFL_Add_HP_Cur( const int amount )
{
    cur_hp += amount;
    if( cur_hp <= 0 ) cur_hp = 0;
    else if( cur_hp > max_hp ) cur_hp = max_hp;
    else fontNum.AddEffText( 
        "+" + std::to_string(amount), 
        pos + Vec2{-6.0f, 0.0f}, GREEN, Vec2{10.0f, 16.0f}
    );
}
void Character::IFL_HP_Add( const int amount )
{
    hp_add = amount;
    IFL_Calc_HP_Max();
}
void Character::IFL_HP_Add_PC( const int amount )
{
    hp_add_pc = amount;
    IFL_Calc_HP_Max();
}
void Character::IFL_HP_Add_VT( const int amount )
{
    hp_add_vt = amount;
    IFL_Calc_HP_Max();
}
void Character::IFL_HP_Add_PC_VT( const int amount )
{
    hp_add_pc_vt = amount;
    IFL_Calc_HP_Max();
}
void Character::IFL_HP_PerSeconds( const int amount )
{

}
///////////////////////////////////////////////////////////////////////////////
//MP
void Character::IFL_Calc_MP_Max()
{
    max_mp = mp_base + mp_add + mp_add_vt;
    max_mp += (mp_add_pc * max_mp) / 100;
    max_mp += (mp_add_pc_vt * max_mp) / 100;
    if( cur_mp > max_mp ) cur_mp = max_mp;
}
void Character::IFL_Add_MP_Cur( const int amount )
{
    cur_mp += amount;
    if( cur_mp <= 0 ) cur_mp = 0;
    else if( cur_mp > max_mp ) cur_mp = max_mp;
    else fontNum.AddEffText( 
        "+" + std::to_string(amount), 
        pos + Vec2{-6.0f, 0.0f}, SKYBLUE, Vec2{10.0f, 16.0f}
    );
}
void Character::IFL_MP_Add( const int amount )
{
    mp_add = amount;
    IFL_Calc_MP_Max();
}
void Character::IFL_MP_Add_PC( const int amount )
{
    mp_add_pc = amount;
    IFL_Calc_MP_Max();
}
void Character::IFL_MP_Add_VT( const int amount )
{
    mp_add_vt = amount;
    IFL_Calc_MP_Max();
}
void Character::IFL_MP_Add_PC_VT( const int amount )
{
    mp_add_pc_vt = amount;
    IFL_Calc_MP_Max();
}
void Character::IFL_MP_PerSeconds( const int amount )
{

}
///////////////////////////////////////////////////////////////////////////////

void Character::IFL_Speed( const int amount )
{
    speed += amount;
}
void Character::IFL_Skill_CountDown( const int amount )
{}
void Character::IFL_Crit( const int amount )
{}
///////////////////////////////////////////////////////////////////////////////
//Armor
void Character::IFL_Calc_Armor()
{
    armor_max = armor_base + armor_add + armor_add_vt;
    armor_max += (armor_add_pc * armor_max) / 100;
    armor_max += (armor_add_pc_vt * armor_max) / 100;
}
void Character::IFL_Armor_Add( const int amount )
{
    armor_add = amount;
    IFL_Calc_Armor();
}
void Character::IFL_Armor_Add_PC( const int amount )
{
    armor_add_pc = amount;
    IFL_Calc_Armor();
}
void Character::IFL_Armor_Add_VT( const int amount )
{
    armor_add_vt = amount;
    IFL_Calc_Armor();
}
void Character::IFL_Armor_Add_PC_VT( const int amount )
{
    armor_add_pc_vt = amount;
    IFL_Calc_Armor();
}
///////////////////////////////////////////////////////////////////////////////

void Character::IFL_Dizzy( const int amount )
{}


