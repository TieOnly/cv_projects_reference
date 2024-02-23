#pragma once
#include "raylibCpp.h"

class Layer
{
public:
    enum class ID
    {
        Main,
        Table_Tab,
        Luggage,
        Skill,
        Intrinsic,
        Info,
        Default
    };
    enum class Var
    {
        //Toggle control one of them ( false:first , true:second )
        Tab_Luggage,
        LugDesc_Luggage,
        Default
    };
public:
    static const bool BeUpdate( const ID& id )
    {
        return Get().vLayer[id].first;
    }
    static const bool BeDraw( const ID& id )
    {
        return Get().vLayer[id].second;
    }
    static void Init()
    {        
        Get().vLayer.insert( {ID::Main, {false, false}} );
        Get().vLayer.insert( {ID::Table_Tab, {false, false}} );
        Get().vLayer.insert( {ID::Luggage, {false, false}} );
        Get().vLayer.insert( {ID::Skill, {false, false}} );
        Get().vLayer.insert( {ID::Intrinsic, {false, false}} );
        Get().vLayer.insert( {ID::Info, {false, false}} );

        Get().vVar.insert( {Var::Tab_Luggage, false} );
        Get().vVar.insert( {Var::LugDesc_Luggage, false} );
    }
    static void OffAll()
    {
        for( auto& l : Get().vLayer )
        {
            l.second.first = false;
            l.second.second = false;
        }
    }
    static void On( const ID& id )
    {
        Get().vLayer[id].first = true;
        Get().vLayer[id].second = true;
    }
    static void Off( const ID& id )
    {
        Get().vLayer[id].first = false;
        Get().vLayer[id].second = false;
    }
    static void OnUpdate( const ID& id )
    {
        Get().vLayer[id].first = true;
    }
    static void OffUpdate( const ID& id )
    {
        Get().vLayer[id].first = false;
    }
    static void OnDraw( const ID& id )
    {
        Get().vLayer[id].second = true;
    }
    static void OffDraw( const ID& id )
    {
        Get().vLayer[id].second = false;
    }

    static bool& CallVar( const Var& var )
    {
        return Get().vVar[var];
    }
    static void ToggleVar_ToFirst( const Var& var )
    {
        Get().ToggleVar_AllBackToSecond();
        Get().vVar[var] = false;
    }
    static void ToggleVar_ToSecond( std::vector<Var>&& _vVar )
    {
        for( auto& v : _vVar ) Get().vVar[v] = true;
    }

    static void LockUpdate() { Get()._LockUpdate(); }
    static void UnLockUpdate() { Get()._UnLockUpdate(); }
private:
    static Layer& Get() 
    {
        static Layer layer;
        return layer;
    }
    void _LockUpdate()
    {
        vLastLayerBeUpdate.clear();
        for( auto& l : vLayer )
        {
            if( l.second.first )
            {
                vLastLayerBeUpdate.push_back( l.first );
                l.second.first = false;
            }
        }
    }
    void _UnLockUpdate()
    {
        for( auto& id : vLastLayerBeUpdate )
        {
            vLayer[id].first = true;
        }
    }
    void ToggleVar_AllBackToSecond()
    {
        for( auto& v : vVar ) v.second = true;
    }
private:
    //Map roster layer ( ID layer, <AllowUpdate, AllowDraw> )
    std::unordered_map<ID, std::pair<bool, bool>> vLayer;
    std::unordered_map<Var, bool> vVar;
    std::vector<ID> vLastLayerBeUpdate;
};

class Modal
{
public:
    static const bool HasModal()
    {
        return Get().vModalWait.size() > 0;
    }
    static const bool HasModal( const int id_modal )
    {
        return Get()._HasModal(id_modal);
    }
    static const bool HasDone()
    {
        return Get().vModalDone.size() > 0;
    }
    static const bool HasDone( const int id_modal )
    {
        return Get()._HasDone(id_modal);
    }
    static void PushWait( const int id )
    {
        Get()._PushWait(id);
    }
    static void PushDone( const int& id, const bool& result )
    {
        Get()._PushDone(id, result);
    }
    static const std::pair<int, bool> Pop()
    {
        return Get()._Pop();
    }
    static const bool TakeConfirm( const int id )
    {
        return Get()._TakeConfirm(id);
    }
    static const int TakeID()
    {
        return Get().id++;
    }
private:
    static Modal& Get()
    {
        static Modal modal;
        return modal;
    }
    const bool _HasModal( const int id )
    {
        return vModalWait.find(id) != vModalWait.end();
    }
    const bool _HasDone( const int id )
    {
        return vModalDone.find(id) != vModalDone.end();
    }
    void _PushWait( const int id )
    {
        if( id != -1 && vModalWait.find(id) == vModalWait.end() )
        {
            vModalWait.insert(id);
            Layer::LockUpdate();
        }
    }
    void _PushDone( const int& id, const bool& result )
    {
        vModalDone.insert_or_assign( id, result );
        vModalWait.erase(id);
        Layer::UnLockUpdate();
    }
    const std::pair<int, bool> _Pop()
    {
        std::pair<int, bool> res;
        res.first = vModalDone.begin()->first;
        res.second = vModalDone.begin()->second;
        vModalDone.erase( vModalDone.begin() );
        return res;
    }
    const bool _TakeConfirm( const int id )
    {
        bool res = false;
        if( _HasDone(id) )
        {
            res = vModalDone[id];
            vModalDone.erase( id );
        }
        return res;
    }
private:
    int id = 0;
    std::unordered_set<int> vModalWait;
    std::unordered_map<int, bool> vModalDone;
};

