#include "raylibCpp.h"
#include "SrcItems.h"

Item::Item( const Entity& _surface )
    :
    Entity( _surface )
{}
void Item::SetItem(
    const std::string& _name,
    const std::string& _info,
    const bool _isLocked,
    const int _cost_sell )
{
    name = _name;
    info = _info;
    isLocked = _isLocked;
    cost_sell = _cost_sell;
}
void Item::SetEffects( const std::vector<Vei2>& effs )
{
    for( int i = 0; i < (int)effs.size(); i++ )
    {
        effects.push_back( {Influence{}, effs[i].y} );
        switch (effs[i].x)
        {
        case 0: effects[i].first = Influence::TimeIFL; break;
        
        case 1: effects[i].first = Influence::Attack_Add; break;
        case 2: effects[i].first = Influence::Attack_Add_PC; break;
        case 3: effects[i].first = Influence::Attack_Add_VT; break;
        case 4: effects[i].first = Influence::Attack_Add_PC_VT; break;
        
        case 5: effects[i].first = Influence::Add_HP_Cur; break;
        case 6: effects[i].first = Influence::HP_Add; break;
        case 7: effects[i].first = Influence::HP_Add_PC; break;
        case 8: effects[i].first = Influence::HP_Add_VT; break;
        case 9: effects[i].first = Influence::HP_Add_PC_VT; break;

        case 10: effects[i].first = Influence::Add_MP_Cur; break;
        case 11: effects[i].first = Influence::MP_Add; break;
        case 12: effects[i].first = Influence::MP_Add_PC; break;
        case 13: effects[i].first = Influence::MP_Add_VT; break;
        case 14: effects[i].first = Influence::MP_Add_PC_VT; break;

        case 15: effects[i].first = Influence::Armor_Add; break;
        case 16: effects[i].first = Influence::Armor_Add_PC; break;
        case 17: effects[i].first = Influence::Armor_Add_VT; break;
        case 18: effects[i].first = Influence::Armor_Add_PC_VT; break;

        case 19: effects[i].first = Influence::Speed; break;
        case 20: effects[i].first = Influence::Skill_CountDown; break;
        case 21: effects[i].first = Influence::Crit; break;
        case 22: effects[i].first = Influence::Dame_To_HP; break;
        case 23: effects[i].first = Influence::Dizzy; break;

        default: effects[i].first = Influence::Count; break;
        }
    }   
}
const std::vector<std::pair<Item::Influence, int>>& Item::GetEffects() const
{
    return effects;
}
///////////////////////////////////////////////////////////////////////
bool SrcItems::_LoadSrcItems( const std::string& fileName )
{   
    if( surfItems.LoadSrcEntities( fileName ) )
    {
        InitSrcItems();
        return true;
    }
    else return false;
}
void SrcItems::InitSrcItems()
{
    std::stringstream buff( surfItems.GetDataBuff() );
    std::string token;
    while ( std::getline( buff, token ) )
    {
        if( token == "[Name_Items]" )
        {
            while (std::getline( buff, token ) && (token != "#"))
            {
                nameItems.push_back( token );
            }
        }
        else if( token == "[Info_Items]" )
        {
            while( std::getline( buff, token, '|' ) && (token != "#") )
            {
                while( token.front() == '\n' ) token.erase( token.begin() );
                infoItems.push_back( token );
            }
        }
        else if( token == "[CostSell_Items]" )
        {
            while( std::getline( buff, token, ' ' ) && (token != "") )
            {
                costSellItems.push_back( std::stoi(token) );
            }
        }
        else if( token == "[Effects_Items]" )
        {
            while ( std::getline( buff, token ) && (token != "#") )
            {
                std::stringstream buff_n( token );
                std::string token_n;
                std::vector<Vei2> effs;
                while( std::getline( buff_n, token_n, ' ' ) && (token_n != "") )
                {
                    effs.push_back( rayCpp::CreateVeiByData( token_n, ':' ) );
                }
                effItems.push_back( std::move( effs ) );
            }
        }
    }
    //Init
    for( ; iItem < GetAmountEntity(); iItem++)
    {
        items.emplace_back( surfItems.EntityByIndex( iItem ) );
        items.back().SetItem(
            nameItems[iItem],
            infoItems[iItem],
            true,
            costSellItems[iItem]
        );
        items.back().SetEffects( effItems[iItem] );
    }
}
const int SrcItems::_GetAmountEntity() const
{
    return surfItems.GetAmountEntity();
}
const int SrcItems::_IndexById( const int id ) const
{
    for( int i = 0; i < (int)items.size(); i++ )
    {
        if( items[i].GetId() == id ) return i;
    }
    return -1;
}
const Item& SrcItems::_ItemByIndex( const int index ) const
{
    if( index > -1 && index < (int)items.size() )
        return items[index];
    return item_default;
}
const Item& SrcItems::_ItemByID( const int id ) const
{
    for( int i = 0; i < (int)items.size(); i++ )
    {
        if( items[i].GetId() == id ) return items[i];
    }
    return item_default;
}
