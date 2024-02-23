#include "raylibCpp.h"
#include "Enemies.h"

void Enemies::LoadSprites( const std::string& fileName )
{
    if( spriteAnimas.LoadSrcEntities( fileName ) )
    {
        data.NewBuff();
        if( data.LoadFromFile( fileName ) )
        {
            std::stringstream buff( data.GetBuff() );
            std::string token;
            while (std::getline(buff, token))
            {
                if( token == "[InitSprite]" )
                {
                    while (std::getline( buff, token ) && (token != "#"))
                    {
                        if( token == "{amountSprite}" )
                        {
                            std::getline( buff, token );
                            amountSprite += std::stoi(token);
                        }
                        else if( token == "{EntityPerSprite}" )
                        {
                            while ( std::getline( buff, token, ' ' ) && (token != "") )
                            {
                                ettPerSprite.push_back(std::stoi(token));
                            }
                        }
                    }
                }
            }
            InitSprites();
            std::cout<<"SpritesSize: "<<sprites.size()<<std::endl;
        }
        else std::cout<<"Faild Load From: "<<fileName<<std::endl;
    }
}
void Enemies::InitSprites()
{
    for ( ; iSprite < amountSprite; iSprite++)
    {
        for( int t = 0; t < ettPerSprite[iSprite]; t++, iEntity++ )
        {
            buff.emplace_back( spriteAnimas.EntityByIndex(iEntity).GetAnimation() );
        }
        sprites.emplace_back( Sprite{ iSprite, buff } );
        ResetBuff();
    }
}
void Enemies::ResetBuff()
{
    for( auto& b : buff ) b.Destroy();
    buff.clear();
}
void Enemies::SetSpritesLevel( const LevelPro& level, const RectF& _levelRect )
{
    curSprites.clear();
    curAmountSprite = level.GetEnemyAmount();
    for (int i = 0; i < curAmountSprite; i++)
    {
        const int id = level.GetEnemyIds()[i];
        //Here: id equal to index
        if( id < amountSprite )
        {
            curSprites.emplace_back( sprites[id] );
            curSprites.back().SetSprite(
                level.GetEnemyDimens()[i],
                level.GetEnemyPos()[i],
                level.GetEnemyBoundary()
            );
            enemiesBoundary.push_back( curSprites.back().GetBoundary() );
        }
    }
    levelRect = _levelRect;
}
void Enemies::Update( const RectF& _camRect, const float dTime )
{
    camRect = _camRect;
    for( auto& s : curSprites )
    {
        if( s.GetBoundary().IsOverLapping(_camRect) )
        {
            s.Update( dTime, levelRect );
            s.UpdateEff( dTime );
            s.UpdateEffText( dTime );
        }
    }
}
void Enemies::ProcessTargetByChar( const Vec2& charPos, const float vision )
{
    const RectF charRectVision = RectF::FromCenter( charPos, vision, vision );
    if( targetedId != -1 )
    {
        if( curSprites[targetedId].GetState() == Sprite::Sequence::Dead
            || !rayCpp::CheckCollidedRectCir( curSprites[targetedId].GetPosBox(), charPos, vision ) 
        ) 
            targetedId = -1;
    }
    else
    {
        const std::vector<int>& listIdsInRectVision = rayCpp::BS_PosInBoundary( enemiesBoundary, charRectVision);
        for( const auto& id : listIdsInRectVision )
        {
            if( curSprites[id].GetState() != Sprite::Sequence::Dead 
                && rayCpp::CheckCollidedRectCir( curSprites[id].GetPosBox(), charPos, vision ) 
            ) 
                targetedId = id;
        }
    }
}
void Enemies::ProcessAttackToChar( Character& tie, const float dTime )
{
    if( tie.GetCurActions() != Character::Actions::Dead )
    {
        for( auto& s : curSprites )
        {
            if( s.isEnmity && s.isAttack )
            {
                if( !s.GetBoundary().IsOverLapping( camRect ) )
                {
                    s.isAttack = false;
                    s.isEnmity = false;
                }
                else if( !s.GetBoundary().IsOverLapping( tie.GetPosBox() ) )
                {
                    if( s.AttackToChar( dTime, tie.GetPos().x ) )
                        tie.InjureCurHp( s.CalculateDamage() );
                }
            }
            else s.isAttack = false;
        }

        const std::vector<int>& idsSpriteAttack = rayCpp::BS_PosInBoundary( enemiesBoundary, tie.GetPosBox());
        for( const auto& id : idsSpriteAttack )
        {
            curSprites[id].isAttack = true;
            if( curSprites[id].AttackToChar( dTime, tie.GetPos().x ) )
                tie.InjureCurHp( curSprites[id].CalculateDamage() );
        }
        if( tie.GetCurActions() == Character::Actions::Dead )
        {
            for( auto& s : curSprites )
            {
                s.isAttack = false;
                s.isEnmity = false;
            }
        }
    }
}
void Enemies::Draw() const  
{
    for( const auto& s : curSprites ) 
    {
        if( s.GetPosBox().IsOverLapping( camRect ) ) 
        {
            s.Draw();
            s.DrawEff();
            rayCpp::DrawRectThin( s.GetPosBox(), GREEN );
            s.DrawEffText();
        }
    }
    if( targetedId != -1 ) curSprites[targetedId].DrawHPLine();
}
const int Enemies::GetTargetId() const { return targetedId; }
Sprite& Enemies::SpriteById( const int id )
{
    return curSprites[id];
}
//////////////////////////////////////////////////////////////////////////////////////////////////
void Enemies::LoadSpriteEffs( const std::string& fileName, const FontC& font )
{
    for( auto& s : sprites ) s.LoadEffects( fileName, font );
}
