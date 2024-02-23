#include "raylibCpp.h"
#include "InfoChar.h"

//Get
const int InfoChar::CalculateDamage()
{
    std::uniform_int_distribution<int> damageDist{ attack_min, attack_max };

    return skills[(int)iCurSkill].GetDamageBuff() * damageDist( rng );
}
const int& InfoChar::GetRank() const { return rank; }
const float& InfoChar::GetRateRank() const { return rateRank; }
const int& InfoChar::GetCurHP() const { return cur_hp; }
const int& InfoChar::GetCurMP() const { return cur_mp; }
const int& InfoChar::GetMaxHP() const { return max_hp; }
const int& InfoChar::GetMaxMP() const { return max_mp; }
const std::vector<int>& InfoChar::GetIdPickSkills() const { return idPickSkills; }
const InfoChar::Skill& InfoChar::GetSkillById( const int id ) const 
{ 
    if( id >= 0 && id < amountSkill ) 
        return skills[id];
    return skills.back();
}
//Set

//////////////////////////////////////////////////////////////////////////////////////////
InfoChar::Skill::Skill(
    const int _id,
    const std::string _name,
    const int _rank,
    const float _damageBuff,
    const float _duraPeriod
)
    :
    id(_id),
    name( _name ),
    rank( _rank ),
    damageBuff( _damageBuff ),
    duraPeriod( _duraPeriod ),
    timerPeriod( _duraPeriod )
{}
void InfoChar::Skill::Fire()
{
    timerPeriod = 0.0f;
    isDonePeriod = false;
}
void InfoChar::Skill::Update( const float dTime )
{
    if( timerPeriod < duraPeriod )
    {
        timerPeriod += dTime;
        isDonePeriod = false;
    }
    else isDonePeriod = true;
}
const int& InfoChar::Skill::GetId() const { return id; }
const std::string& InfoChar::Skill::GetName() const { return name; }
const int& InfoChar::Skill::GetRank() const { return rank; }
const float InfoChar::Skill::GetDamageBuff() const { return damageBuff / 100.0f; }
const float& InfoChar::Skill::GetTimerPeriod() const { return timerPeriod; }
const float& InfoChar::Skill::GetDuraPeriod() const { return duraPeriod; }
//////////////////////////////////////////////////////////////////////////////////////////
bool InfoChar::Luggage::LoadData( const std::string& fileName )
{
    data.NewBuff();
    if( data.LoadFromFile( fileName ) )
    {
        std::stringstream buff( data.GetBuff() );
        std::string token;
        while( std::getline( buff, token ) )
        {
            if( token == "[Amount_Cell]" )
            {
                std::getline( buff, token );
                amountCell = std::stoi( token );
            }
            else if( token == "[Amount_Item]")
            {
                std::getline( buff, token );
                amountItem = std::stoi( token );
            }
            else if( token == "[Items_ID_EachOfThem]" )
            {
                while( std::getline( buff, token, ' ' ) && (token != "") )
                {
                    items_id.push_back( std::stoi( token ) );
                }
            }
            else if( token == "[Items_Amount_EachOfThem]" )
            {
                while( std::getline( buff, token, ' ' ) && (token != "") )
                {
                    items_amount.push_back( std::stoi( token ) );
                }
            }
            else if( token == "[Items_Order_EachOfThem]" )
            {
                while( std::getline( buff, token, ' ' ) && (token != "") )
                {
                    items_order.push_back( std::stoi( token ) );
                }
            }
            else if( token == "[Items_IsLocked_EachOfThem]" )
            {
                while( std::getline( buff, token, ' ' ) && (token != "") )
                {
                    items_isLocked.push_back( (bool)std::stoi( token ) );
                }
            }
        }
        InitItems();
        std::cout<<"Data: Load Success From "<<fileName<<std::endl;
        return true;
    }
    else std::cout<<"Data: Faild Load From "<<fileName<<std::endl;
    return false;
}
void InfoChar::Luggage::InitItems()
{
    ItemDesc item_desc{};
    for( int i = 0; i < amountItem; i++ )
    {
        item_desc.id = items_id[i];
        item_desc.amount = items_amount[i];
        item_desc.order = items_order[i];
        item_desc.isLocked = items_isLocked[i];
        items.emplace_back( item_desc );
    }
}
void InfoChar::Luggage::PushItem( const int& id_item )
{   

}
void InfoChar::Luggage::RemoveItem( const int& id_item )
{
    for( auto itr = items.begin(); itr != items.end(); itr++ )
    {
        if( itr->id == id_item )
        {
            if( --(itr->amount) <= 0 )
            {
                items.erase( itr );
                break;
            }
        }
    }
}
