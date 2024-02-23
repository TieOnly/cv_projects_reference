#include "raylibCpp.h"
#include "SrcEntities.h"
#include "assert.h"

SrcEntities::SrcEntities(){};
void SrcEntities::AddEntity( 
    const std::string& fileSheet, const Vec2& srcPos, const Vec2& dimension, 
    const int frame, const float holdTime 
)
{
    entities.emplace_back( 
        Entity( 
            Animation{ 
                srcPos.x, srcPos.y, 
                dimension.x, dimension.y, 
                frame, 
                Surface::Retrieve( fileSheet ), 
                holdTime 
            }, 
            amountEntity++
        ) 
    );
}
int SrcEntities::IndexById( const int id )
{
    for( size_t i = 0u; i < entities.size(); i++ )
    {
        if( entities[i].GetId() == id ) return i;
    }
    return -1;
}
const Entity& SrcEntities::EntityByIndex( const int index ) const
{
    assert( index != -1 );
    return entities[index];
}
//////////////////////////////////////////////////////////////////////////////////////////
bool SrcEntities::LoadSrcEntities( const std::string& fileName )
{
    if( !filesLoaded.count( fileName ) )
    {
        filesLoaded.insert( fileName );
        data.NewBuff();
        if( data.LoadFromFile( fileName ) )
        {
            std::stringstream buff( data.GetBuff() );
            std::string token;
            while (std::getline(buff, token))
            {
                if( token == "[SurfaceSheet]" )
                {
                    std::getline( buff, token );
                    aniSheet = Surface::Retrieve( "../assets/img/" + token );
                }
                else if( token == "[AmountEntity]" )
                {
                    std::getline( buff, token );
                    amountEntity += std::stoi(token);
                }
                else if( token == "[InitAnimation]" )
                {
                    while (std::getline( buff, token ) && (token != "#"))
                    {
                        if( token == "{srcPos}" )
                        {
                            while ( std::getline( buff, token, '|' ) && (token != "") )
                            {
                                aniSrcPos.push_back(rayCpp::CreateVecByData(token, ' '));
                            }
                        }
                        else if( token == "{srcDimens}" )
                        {
                            std::getline( buff, token );
                            aniSrcDimens = rayCpp::CreateVecByData(token, ' ');
                        }
                        else if( token == "{srcDimens_list}" )
                        {
                            while ( std::getline( buff, token, '|' ) && (token != "") )
                            {
                                aniSrcDimens_list.push_back(rayCpp::CreateVecByData(token, ' '));
                            }
                        }
                        else if( token == "{frame}" )
                        {
                            while ( std::getline( buff, token, ' ' ) && (token != "") )
                            {
                                aniFrames.push_back(std::stoi(token));
                            }
                        }
                        else if( token == "{holdTime}" )
                        {
                            while ( std::getline( buff, token, ' ' ) && (token != "") )
                            {
                                aniHoldTime.push_back(std::stof(token));
                            }
                        }
                    }
                }
            }
            if( aniSrcDimens == 0.0f ) InitSrcEntities_list();
            else InitSrcEntities();
            std::cout<<"Data: Load Success From "<<fileName<<std::endl;
            return true;
        }
        else std::cout<<"Data: Faild Load From "<<fileName<<std::endl;
    }
    else std::cout<<"Data: "<<fileName<<"is loaded, ignore this time"<<std::endl;
    
    return false;
}
void SrcEntities::InitSrcEntities()
{
    for( ; iEntity < amountEntity; iEntity++)
    {
        entities.emplace_back( 
            Entity( 
                Animation{ 
                    aniSrcPos[iEntity].x, aniSrcPos[iEntity].y, 
                    aniSrcDimens.x, aniSrcDimens.y, 
                    aniFrames[iEntity], 
                    aniSheet, 
                    aniHoldTime[iEntity] 
                }, iEntity 
            ) 
        );
    }
}
void SrcEntities::InitSrcEntities_list()
{
    for( ; iEntity < amountEntity; iEntity++)
    {
        entities.emplace_back( 
            Entity( 
                Animation{ 
                    aniSrcPos[iEntity].x, aniSrcPos[iEntity].y, 
                    aniSrcDimens_list[iEntity].x, aniSrcDimens_list[iEntity].y, 
                    aniFrames[iEntity], 
                    aniSheet, 
                    aniHoldTime[iEntity] 
                }, iEntity 
            ) 
        );
    }
}
const int SrcEntities::GetAmountEntity() const
{
    return amountEntity;
}
const std::string& SrcEntities::GetDataBuff() const
{
    return data.GetBuff();
}
