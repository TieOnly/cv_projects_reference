#include "raylibCpp.h"
#include "Sprite.h"

Sprite::Sprite( const int _id, const std::vector<Animation>& _animations )
    :
    id(_id)
{
    for( auto a : _animations )
    {
        animations.emplace_back( a );
    }
    sizeAnimated = (int)animations.size();
}
void Sprite::SetSprite( const Vec2& dimens, const Vei2& _pos, const Vei2& _boundary )
{
    rng = std::mt19937(std::random_device()());
    dimension = dimens;
    posSpawn = Vec2{ 
        _pos.x * settings::tileSize, 
        (_pos.y+1)*settings::tileSize + settings::deltaTopTile - dimension.y
    };
    pos = posSpawn;
    boundary = RectF{ 
        pos + Vec2{0.0f, dimension.y - settings::tileSize*_boundary.y},
        settings::tileSize*_boundary.x, settings::tileSize*_boundary.y
    };
    dir = Vei2{ dirDist( rng )?-1:1, 0};
    duraWalking = duraDist( rng );
    duraIdle = duraDist( rng );

    hpLineW = dimension.x;
    hpLineH = 10.0f;
}
void Sprite::SetCurSequence( const Sequence& s ) { curSequence = s; }
void Sprite::SetDirX( const int _dir ) { dir.x = _dir; }
void Sprite::Update( const float dTime, const RectF& levelRect )
{  
    //Process Before Update ( Set Sequence Default )
    if( curSequence == Sequence::Idle )
    {
        timerIdle += dTime;
        if( timerIdle >= duraIdle )
        {
            timerIdle = 0.0f;
            duraWalking = duraDist( rng );
            curSequence = Sequence::Walking;
        }
    }
    else if( curSequence == Sequence::Walking )
    {
        timerWalking += dTime;
        if( timerWalking >= duraWalking )
        {
            timerWalking = 0.0f;
            duraIdle = duraDist( rng );
            curSequence = Sequence::Idle;
            if( isAttack )
            {
                timerIdle = duraIdle/2;
                isSliding = true;
            }
        }
        //Check boundary
        if( pos.x < boundary.left )
        {
            pos.x = boundary.left + 2.f;
            dir.x = 1;
        }
        else if( pos.x + dimension.x > boundary.right )
        {
            pos.x = boundary.right - dimension.x - 2.0f;
            dir.x = -1;
        }
    }

    //Update
    if( curSequence == Sequence::Walking ) 
    {
        if( !isAttack ) pos.x += dir.x * speed * dTime;
    }
    else if ( curSequence == Sequence::Dead )
    {
        if( !isDisappear && GetPosBox().IsOverLapping( levelRect ) )
        {
            if( vel.y <= speedFall - gravity ) vel.y += gravity;
            pos.y += vel.y * dTime;
            pos.x += -dir.x * speed * dTime;
        }
        else isDisappear = true;
    }

    //Update Id sequence in animations
    if( 2*(int)curSequence + 1 < sizeAnimated )
    {
        if( dir.x > 0 ) iSequence = 2*(int)curSequence + 1;
        else iSequence = 2*(int)curSequence;
    }
    animations[iSequence].Update( dTime );
    
    //Process After Update ( Set TickEff, Some Active )
    if( curSequence == Sequence::Hurt )
    {
        hasTickEff = true;
        if( dir.x > 0 ) tickEffActives[(int)StateEff::BleedLeft] = 1;
        else tickEffActives[(int)StateEff::BleedRight] = 1;
        if( animations[iSequence].IsDonePeriod() )
        {
            curSequence = Sequence::Idle;
            timerIdle = duraIdle / 2;
        }
    }
    else if( curSequence == Sequence::Dead )
    {
        timerDead += dTime;
        if( timerDead >= duraDead )
        {
            isDisappear = false;
            timerDead = 0.0f;
            curSequence = Sequence::Walking;
            tickEffActives[(int)StateEff::Spawn] = 1;
            pos = posSpawn;
            curHp = max_hp;
            hasTickEff = true;
        }
        else hasTickEff = false;
    }
    else hasTickEff = false;
}
void Sprite::InjureCurHp( const int amount )
{
    isEnmity = true;
    curHp -= amount;
    //Process
    if( curHp <= 0 )
    {
        curHp = 0;
        curSequence = Sequence::Dead;
        isEnmity = false;
        isAttack = false;
        vel.y = -speedFall;
    }
    else if( curHp >= max_hp ) curHp = max_hp;
    font.AddEffText( 
        std::to_string(-amount), pos,
        Color{245, 144, 66, 255}, Vec2{ 16.0f, 20.0f } 
    );
}
bool Sprite::AttackToChar( const float& dTime, const float posXChar )
{
    //Not attack if Dead
    if( curSequence == Sequence::Dead ) return false;

    bool flag = false;
    //Set direct to Character
    GetCenter().x > posXChar ? dir.x = -1 : dir.x = 1;
    
    //Process Attack when hurt
    if( timerAttack <= 0.0f ) duraAttack = duraBootsAttackDist( rng );
    timerAttack += dTime;
    if( timerAttack >= duraAttack )
    {
        if( curSequence == Sequence::Hurt )
        {
            flag = true;
            timerAttack = 0.0f;
            timerIdle = 0.0f;
        }
    }

    //Process Attack when don't hurt
    if( timerIdle <= 0.0f  )
    {
        curSequence = Sequence::Walking;
        duraWalking = duraBootsAttackDist( rng );
        timerWalking += dTime;
        dirSilde = -dir.x;
    }
    if( isSliding )
    {
        //Start Sliding And Fire Attack
        if( timerSliding <= 0.0f )
        {
            flag = true;
            duraSliding = duraSlidingDist( rng );
            timerAttack = 0.0f;
        }

        //Update
        timerSliding += dTime;
        if( timerSliding >= duraSliding )
        {
            timerSliding = 0.0f;
            isSliding = false;
        }

        //Check boundary And Update pos
        if( pos.x < boundary.left )
        {
            dirSilde = 1;
        }
        else if( pos.x + dimension.x > boundary.right )
        {
            dirSilde = -1;
        }
        pos.x += dirSilde * (speed + 100.0f) * dTime;
    }

    return flag;
}
const int Sprite::CalculateDamage()
{
    std::uniform_int_distribution<int> damageDist{ min_damage, max_damage };

    return damageDist( rng );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void Sprite::Draw() const
{
    if( !isDisappear ) animations[iSequence].Draw( pos, dimension );
}
void Sprite::DrawHPLine() const
{
    const RectF HpLineOl = RectF{ pos + Vec2{0.0, -hpLineH}, hpLineW, hpLineH };
    const RectF HpLine = RectF{ pos + Vec2{1.0, -hpLineH}, hpLineW - 1.0f, hpLineH };
    const float rateHp = float(curHp) / float(max_hp);
    const RectF curHpLine = RectF{ pos + Vec2{1.0, -hpLineH}, (float(hpLineW) * rateHp) - 1.0f, hpLineH };
    rayCpp::DrawRectFill( HpLine, WHITE );
    rayCpp::DrawRectFill( curHpLine, RED );
    rayCpp::DrawRectThin( HpLineOl, BLACK, 2.0f );
}

void Sprite::SetDimens( const Vec2& dimens ) { dimension = dimens; }
const int Sprite::GetIds() const { return id; }
const std::vector<Animation>& Sprite::GetAnimations() const { return animations; }
const Vec2& Sprite::GetPos() const { return pos; }
const Sprite::Sequence& Sprite::GetState() const { return curSequence; }
const Vec2 Sprite::GetCenter() const { return pos + (dimension / 2); }
const RectF& Sprite::GetBoundary() const
{   
    return boundary;
}
const RectF Sprite::GetPosBox() const
{
    return RectF{ pos, dimension.x, dimension.y };
}
///////////////////////////////////////////////////////////////////////////////////////////////
void Sprite::LoadEffects( const std::string& fileName, const FontC& _font )
{
    if( EffActive.LoadSrcEntities( fileName ) )
    {
        for( ;iEffSprite < EffActive.GetAmountEntity();iEffSprite++ )
        {
            effSprites.emplace_back( EffActive.EntityByIndex(iEffSprite) );
        }
    }
    font = _font;
}
void Sprite::UpdateEff( const float dTime )
{
    if( hasTickEff || hasEffActiving )
    {
        hasEffActiving = false;
        for ( int i = 0; i < (int)StateEff::Count; i++)
        {
            if( tickEffActives[i] )
            {
                Entity& e = effSprites[i];
                //Ignore SetEff when IsDonePeriod Not Yet
                if( e.IsDonePeriod() ) SetEffByIndex( e, i );
                e.Update( dTime );
                //Reset Tick When DonePeriod
                if( e.IsDonePeriod() ) tickEffActives[i] = 0;
                else hasEffActiving = true;
            }
        }
    }
}
void Sprite::SetEffByIndex( Entity& e, const int index )
{
    switch (index)
    {
    case 0:     //Spawn
        e.SetPos( pos );
        e.SetDimens( dimension );
        break;
    case 1:     //BleedingLeft
        e.SetPos( pos + Vec2{-10.0f, 0.0f} );
        e.SetDimens( dimension );
        break;
    case 2:     //BleedingRight
        e.SetPos( pos + Vec2{10.0f, 0.0f} );
        e.SetDimens( dimension );
        break;
    default:
        break;
    }
}
void Sprite::UpdateEffText( const float dTime )
{
    font.UpdateEffText( dTime );
}
void Sprite::DrawEffText() const
{
    font.DrawTextEff();
}
void Sprite::DrawEff() const
{
    if( hasTickEff || hasEffActiving )
    {
        for ( int i = 0; i < (int)StateEff::Count; i++)
        {
            if( tickEffActives[i] )
            {
                effSprites[i].Draw();
            }
        }
    }
}
