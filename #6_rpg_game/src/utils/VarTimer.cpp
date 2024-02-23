#include "raylibCpp.h"
#include "utils/VarTimer.h"

void VarTimer::_Init()
{
    for( int i = 0; i < (int)Var::Count; i++ )
    {
        vTimers.insert( {Var::Heart_Buffer, {0.0f, false}} );
        vTimers.insert( {Var::Power_Buffer, {0.0f, false}} );
        vTimers.insert( {Var::Armor_Buffer, {0.0f, false}} );
    }
    vActivedIFL.clear();
}
bool VarTimer::_IsVarTimeUp( const Var& varTimer )
{
    if( vTimers.count(varTimer) )
    {
        if(vTimers[varTimer].second && vTimers[varTimer].first <= 0.0f)
        {
            vTimers[varTimer].second = false;
            vTimers[varTimer].first = 0.0f;
            vActivedIFL.erase( varTimer );
            bMustUpdate_PosIFL = true;
            return true;
        }
    }
    return false;
}
void VarTimer::_Timer_Update( const float& dTime )
{
    for( auto& v : vTimers )
    {
        if( v.second.first >= 0.0f )
        {
            v.second.first -= dTime;
        }
    }
}
void VarTimer::_Tick( const Var& varTimer, const int dura )
{
    if( vTimers.count( varTimer ) && dura >= 0 )
    {
        vTimers[varTimer].first = (float)dura;
        vTimers[varTimer].second = true;
        
        Entity entity{ SrcItems::ItemByID((int)varTimer).GetAnimation(), -1 };
        entity.SetDimens( entity.GetSrcDimens() );
        vActivedIFL.insert( { varTimer, entity });

        bMustUpdate_PosIFL = true;
    }   
}
