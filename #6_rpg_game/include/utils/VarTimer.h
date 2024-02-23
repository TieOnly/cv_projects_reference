#pragma once
#include "SrcItems.h"
#include "Entity.h"
#include "FontC.h"

class VarTimer
{
public:
    enum class Var
    {
        //ID item will be ticked timer
        Heart_Buffer = 1,
        Power_Buffer,
        Armor_Buffer,

        Count
    };
    static void Init() { Get()._Init(); }
    static void Timer_Update( const float& dTime ) 
        { Get()._Timer_Update(dTime); }
    static void Tick( const Var& varTimer, const int dura )
        { Get()._Tick(varTimer, dura); }
    static const float GetTimer( const Var& varTimer )
    {
        if( Get().vTimers.count(varTimer) )
        {
            return Get().vTimers[varTimer].first;
        }
        return -1.0f;
    }
    static const bool IsVarTimeUp( const Var& varTimer )
        { return Get()._IsVarTimeUp(varTimer); }
    static void ForceVarTimeUp( const Var& varTime )
    {
        if( Get().vTimers.count(varTime) )
        {
            Get().vTimers[varTime].second = true;
            Get().vTimers[varTime].first = 0.0f;
        }
    }
    static std::unordered_map<Var, Entity>& GetActiveIFL()
        { return Get().vActivedIFL; }
    static const bool BoolMustUpdate_PosDrawIFL()
    {
        if( Get().bMustUpdate_PosIFL )
        {
            Get().bMustUpdate_PosIFL = false;
            return true;
        }
        return false;
    }
private:
    static VarTimer& Get()
    {
        static VarTimer timer;
        return timer;
    }
    void _Init();
    void LoadData( const std::string );
    bool _IsVarTimeUp( const Var& varTimer );
    void _Timer_Update( const float& dTime );
    void _Tick( const Var& varTimer, const int dura );
    //Roster VarTimers, <ID item, < dura, isTicked > >
    std::unordered_map<Var, std::pair<float, bool>> vTimers;
    //List Influence Draw
    std::unordered_map<Var, Entity> vActivedIFL;
    bool bMustUpdate_PosIFL = true;
};