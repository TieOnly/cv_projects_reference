#include "raylibCpp.h"
#include "Character.h"

Character::Character( const Vec2& pos )
    :
    pos( pos ),
    stepSound( LoadSound("../assets/audio/step.mp3") )
{
    SMoves.LoadSrcEntities( "../assets/data/char/char_moves.txt" );
    sizeAmoves = SMoves.GetAmountEntity();
    for( int i = 0; i < sizeAmoves; i++ )
    {
       Amoves.emplace_back( SMoves.EntityByIndex(i).GetAnimation() ); 
    }

    SActions.LoadSrcEntities( "../assets/data/char/char_actions.txt" );
    sizeAactions = SActions.GetAmountEntity();
    for( int i = 0; i < sizeAactions; i++ )
    {
       Aactions.emplace_back( SActions.EntityByIndex(i).GetAnimation() ); 
    }

    SSkills.LoadSrcEntities( "../assets/data/char/char_attacks.txt" );
    sizeAskill = SSkills.GetAmountEntity();
    for( int i = 0; i < sizeAskill; i++ )
    {
       Askills.emplace_back( SSkills.EntityByIndex(i).GetAnimation() ); 
    }
    for( int i = 0; i < 5; i++ )
    {
        idPickSkills.emplace_back(-1);
    }
    skills.emplace_back( Skill{
        (int)SkillID::Default, "Punch", 1, 110.0f, 0.3f
    } );
    skills.emplace_back( Skill{
        (int)SkillID::Water1, "PunchRed", 1, 150.0f, 0.3f
    } );
    amountSkill = (int)skills.size();
    idPickSkills[0] = skills[0].GetId();
    idPickSkills[1] = skills[1].GetId();
    idPickSkills[2] = skills[0].GetId();

    luggage.LoadData( "../assets/data/char/char_luggage.txt" );
}
void Character::ProcessInput( const bool allow_control )
{
    direct = { 0.0f, 1.0f };
    if( !isLanding )
    {   
        if( vel.y < 0.0f ) direct.y = -1.0f;
        if( vel.x > 0.0f ) direct.x = 1.0f;
        else if( vel.x < 0.0f ) direct.x = -1.0f;
    }
    else direct.y = 0.0f;

    if( allow_control )
    {
        if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
        {
            direct.x = -1.0f;
        }
        if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        {
            direct.x = 1.0f;
        }
        if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
        {
            if( isLanding && iCurAction != Actions::Dead )
            {
                vel.y = -speedJump;
                isLanding = false;
                direct.y = -1.0f;
            }
        }
        if( IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) ) isActiving = true;
    }
}
void Character::SetDirection()
{
    //Set after input, before collision
    if( direct.x > 0.0f ) isLeft = false;
    else if( direct.x < 0.0f ) isLeft = true;
    if( isLanding )
    {
        if( direct.x > 0.0f ) iCurMove = Moves::Walking;
        else if( direct.x < 0.0f ) iCurMove = Moves::Walking;
        else iCurMove = Moves::Standing;
    }
    else
    {
        if( direct.y < 0.0f) iCurMove = Moves::Upping;
        else if( direct.y > 0.0f ) iCurMove = Moves::Downing;
        if( vel.y  > -400.0f && vel.y < 0.0f ) iCurMove = Moves::Round;
    }
}
void Character::ProcessActiveSprite( Sprite& sprite, const RectF& camRect )
{
    const RectF& sBox = sprite.GetPosBox();
    const Vec2& sCenter = sBox.GetCenterPoint();

    if( isActiving && iCurAction != Actions::Dead )
    {
        if( (pos.x - range) - sCenter.x > 0.0f ) direct.x = -1.0f;
        else if( (pos.x + range) - sCenter.x < 0.0f ) direct.x = 1.0f;
        else
        {
            isActiving = false;
            //Add Attack When Period of active skill done
            if( skills[(int)iCurSkill].isDonePeriod )
            {
                //Check for Overlap with RangeDame
                if( rayCpp::CheckCollidedRectCir( sBox, pos, range ) )
                {
                    if( sCenter.x > pos.x ) isLeft = false;
                    else isLeft = true;
                    iAskill = GetISkill();
                    
                    //Set Anamated
                    iCurAction = Actions::Attack;
                    skills[(int)iCurSkill].Fire();
                    Askills[iAskill].ForceBeginPeriod();
                    sprite.SetCurSequence( Sprite::Sequence::Hurt );
                    sprite.SetDirX( isLeft ? 1 : -1 );

                    //Set Info
                    const int damege = CalculateDamage();
                    IFL_Add_HP_Cur( int((float)damege * dame_to_hp) );
                    sprite.InjureCurHp( CalculateDamage() );
                }
            }
        }
    }
}
void Character::IgnoreActive() { isActiving = false; }
void Character::ProcessCollision( const TilesMap& tileMap, const float dTime )
{
    std::vector<Vei2> listPosCollieds;
    const RectF& curRect = GetPosBox();
    const RectF& nextRect = GetPosBoxNext( direct, dTime );
    const float tileSize = tileMap.GetTileSize();

    const int xStart = int(curRect.left / tileSize);
    const int xEnd = int(curRect.right / tileSize);
    const int yStart = int(curRect.top / tileSize);
    const int yEnd = int(curRect.bottom / tileSize);

    const int xStartNext = int(nextRect.left / tileSize);
    const int xEndNext = int(nextRect.right / tileSize);
    const int yStartNext = int(nextRect.top / tileSize);
    const int yEndNext = int(nextRect.bottom / tileSize);

    if( direct.x < 0.0f )
    {
        for(int iY = yStart; iY <= yEnd; iY++)
        {
            const TilesMap::Tile::Type& typeTile = tileMap.TileAt( xStartNext, iY ).CurType();
            if( typeTile == TilesMap::Tile::Type::Thick ||
                curRect.left < tileMap.GetOffsetX() )
            {
                direct.x = 0.0f;
                break;
            }
            else if( typeTile == TilesMap::Tile::Type::Anima ) 
                listPosCollieds.push_back({xStartNext, iY});
        }
    }
    else if( direct.x > 0.0f )
    {
        for(int iY = yStart; iY <= yEnd; iY++)
        {
            const TilesMap::Tile::Type& typeTile = tileMap.TileAt( xEndNext, iY ).CurType();
            if( typeTile == TilesMap::Tile::Type::Thick ||
                curRect.right > tileMap.GetRect().right)
            {
                direct.x = 0.0f;
                break;
            }
            else if( typeTile == TilesMap::Tile::Type::Anima )
                listPosCollieds.push_back({xEndNext, iY});
        }
    }

    if( direct.y < 0.0f )
    {
        isLanding = false;
        for(int iX = xStart; iX <= xEnd; iX++)
        {
            const TilesMap::Tile::Type& typeTile = tileMap.TileAt( iX, yStartNext ).CurType();
            if( typeTile == TilesMap::Tile::Type::Thick  )
            {
                vel.y = vel.y * -1.0f;
                direct.y = 1.0f;
                //Fix distance touch
                const float touchZone = curRect.top - (yStartNext + 1)*tileSize;
                if( touchZone <= 0.0f ) pos.y -= (touchZone - 0.1f);
                else if( touchZone > 3.0f ) pos.y -= (touchZone - 1.0f);
                break;
            }
            else if( typeTile == TilesMap::Tile::Type::Anima )
                listPosCollieds.push_back({iX, yStartNext});
        }
    }
    else
    {
        for(int iX = xStart; iX <= xEnd; iX++)
        {
            const TilesMap::Tile::Type& typeTile = tileMap.TileAt( iX, yEndNext ).CurType();
            if( typeTile == TilesMap::Tile::Type::Thick || curRect.bottom > tileMap.GetRect().bottom)
            {
                direct.y = 0.0f;
                isLanding = true;
                //Fix distance touch
                pos.y += (yEndNext*tileSize - curRect.bottom - 1.0f);
                break;
            }
            else if( typeTile == TilesMap::Tile::Type::Behind )
            {
                if( curRect.bottom <= yEndNext*tileSize )
                {
                    direct.y = 0.0f;
                    isLanding = true;
                    //Fix distance touch
                    pos.y += (yEndNext*tileSize - curRect.bottom - 1.0f);
                }
                break;
            }
            else if( typeTile == TilesMap::Tile::Type::Anima )
            {
                listPosCollieds.push_back({iX, yEndNext});
            }
            isLanding = false;
        }
    }
    //Set Effects
    if( iCurAction != Actions::Dead )
    {
        for( const auto& pos : listPosCollieds)
        {
            const int index = rayCpp::BinarySerch_IdInVei2(tileMap.GetLevelEffPos(), pos);
            if( index != -1 )
            {
                const int id = tileMap.GetLevelEffIds()[index];
                if( id == 0 )   //Water
                {
                    if( curRect.bottom < pos.y*tileSize )   //First Touch Water
                    {
                        tickEffActives[(int)StateEff::Fall_Down_Water] = 1;
                    }
                    else if( iCurMove == Moves::Walking )
                    {
                        tickEffActives[(int)StateEff::Under_Water] = 1;
                    }
                }
            }
        }
    }
}
void Character::Update( float dTime )
{
    //Update With State Action ( animations, direct, pos... )
    iAction = GetIAction();

    if( iCurAction == Actions::None )
    {
        iMove = GetIMove();

        if( iCurMove == Character::Moves::Walking )
            Amoves[iMove].Update( dTime, stepSound, nFramePerSound );
        else
            Amoves[iMove].Update( dTime );
    }
    else if( iCurAction == Actions::Attack )
    {
        direct.x = 0.0f;
        Askills[iAskill].Update( dTime );
        //Reset back to Moves
        if( Askills[iAskill].IsDonePeriod() ) iCurAction = Actions::None;
    }
    else if( iCurAction == Actions::Hurt )
    {
        Aactions[iAction].Update( dTime );
        if( Aactions[iAction].IsDonePeriod() ) iCurAction = Actions::None;
    }
    else if( iCurAction == Actions::Dead )
    {
        direct.x = 0.0f;
        Aactions[iAction].Update( dTime );
    }

    //Update Timer Skill
    for( int i = 0; i < amountSkill; i++ )
    {
        skills[i].Update( dTime );
    }

    //Update Pos
    vel.x = direct.x * speed;
    if( direct.y != 0.0f )
    {
        if( vel.y <= speedJump - gravity ) vel.y += gravity;
    }
    else vel.y = 0.0f;

    pos += vel * dTime;

    //Update Influence Item TimeUp
    ProcessIFL_BeTimeUp();
}
void Character::Draw() const
{
    if( iCurAction == Actions::None )
    {
        Amoves[iMove].Draw( (Vec2)(pos + draw_offset ), dimension);
    }
    else if( iCurAction == Actions::Attack )
    {
        Askills[iAskill].Draw( (Vec2)(pos + draw_offset ), dimension);
    }
    else
    {
        Aactions[iAction].Draw( (Vec2)(pos + draw_offset ), dimension);
    }
}
///////////////////////////////////////////////////////////////////////////////////////
const Vec2& Character::GetVel() const { return vel; }
const float Character::GetSpeed() const { return speed; }
const Vec2& Character::GetPos() const { return pos; }
const float Character::GetRange() const { return range; }
const float Character::GetVision() const { return vision; }
const RectF Character::GetRectVision() const { return RectF::FromCenter( pos, vision, vision ); }
const RectF Character::GetPosBox() const { return RectF::FromCenter( (pos + posBoxOffset), hwPosBox, hhPosBox ); }
const RectF Character::GetPosBoxNext( const Vec2& dir, const float dTime ) const
{
    const float velYNext = isLanding ? speedJump : vel.y + gravity;
    const Vec2 nextPos = pos + (Vec2{ dir.x * speed, velYNext } * dTime);   //next vel fexiable
    return RectF::FromCenter( (nextPos + posBoxOffset), hwPosBox, hhPosBox );
}
const bool Character::IsLeft() const { return isLeft; }
const Character::Moves& Character::GetCurMove() const { return iCurMove; }
const Character::Actions& Character::GetCurActions() const { return iCurAction; }
bool Character::IsLanding() const { return isLanding; }
const int Character::GetIMove() const 
{ 
    const int res = isLeft 
        ? (int)iCurMove + (int)Moves::Count
        : (int)iCurMove;
    return res < sizeAmoves ? res : iMove;
}
const int Character::GetIAction() const 
{ 
    const int res = isLeft 
        ? 2*(int)iCurAction + 1
        : 2*(int)iCurAction;
    return res < sizeAactions ? res : iAction;
}
const int Character::GetISkill() const 
{ 
    const int res = isLeft 
        ? 2*(int)iCurSkill + 1
        : 2*(int)iCurSkill;
    return res < sizeAskill ? res : iAskill;
}

///////////////////////////////////////////////////////////////////////////////////////
bool Character::UseItem( const int& id_item )
{
    const int index = SrcItems::IndexById( id_item );
    if( index != -1 )
    {
        int count = 0;
        for( auto& eff : SrcItems::ItemByIndex(index).GetEffects() )
        {
            ++count;
            switch ( eff.first )
            {
            case Item::Influence::TimeIFL: IFL_Tick_VarTimer( id_item, eff.second ); break;

            case Item::Influence::Attack_Add: IFL_Atk_Add( eff.second ); break;
            case Item::Influence::Attack_Add_PC: IFL_Atk_Add_PC( eff.second ); break;
            case Item::Influence::Attack_Add_VT: IFL_Atk_Add_VT( eff.second ); break;
            case Item::Influence::Attack_Add_PC_VT: IFL_Atk_Add_PC_VT( eff.second ); break;

            case Item::Influence::Add_HP_Cur: IFL_Add_HP_Cur( eff.second ); break;
            case Item::Influence::HP_Add: IFL_HP_Add( eff.second ); break;
            case Item::Influence::HP_Add_PC: IFL_HP_Add_PC( eff.second ); break;
            case Item::Influence::HP_Add_VT: IFL_HP_Add_VT( eff.second ); break;
            case Item::Influence::HP_Add_PC_VT: IFL_HP_Add_PC_VT( eff.second ); break;

            case Item::Influence::Add_MP_Cur: IFL_Add_MP_Cur( eff.second ); break;
            case Item::Influence::MP_Add: IFL_MP_Add( eff.second ); break;
            case Item::Influence::MP_Add_PC: IFL_MP_Add_PC( eff.second ); break;
            case Item::Influence::MP_Add_VT: IFL_MP_Add_VT( eff.second ); break;
            case Item::Influence::MP_Add_PC_VT: IFL_MP_Add_PC_VT( eff.second ); break;

            case Item::Influence::Armor_Add: IFL_Armor_Add( eff.second ); break;
            case Item::Influence::Armor_Add_PC: IFL_Armor_Add_PC( eff.second ); break;
            case Item::Influence::Armor_Add_VT: IFL_Armor_Add_VT( eff.second ); break;
            case Item::Influence::Armor_Add_PC_VT: IFL_Armor_Add_PC_VT( eff.second ); break;

            case Item::Influence::Speed: IFL_Speed( eff.second ); break;
            default: --count ; break;
            }
        }
        if( count > 0 )     //Be added effects
        {
            luggage.RemoveItem( id_item );
            return true;
        }
    }  
    return false;
}
void Character::InjureCurHp( const int amount )
{
    const int sub_hp = amount - armor_max;
    cur_hp -= sub_hp;
    if( cur_hp <= 0 )
    {
        cur_hp = 0;
        iCurAction = Actions::Dead;
    }
    else if( cur_hp > max_hp ) cur_hp = max_hp;
    else
    {
        if( iCurAction != Actions::Attack )
        {
            iCurAction = Actions::Hurt;
            Aactions[GetIAction()].ForceBeginPeriod();
        }
        font.AddEffText( std::to_string(-sub_hp), pos, RED, Vec2{12.0, 16.0} );
    }
}
void Character::PickSkillById( const int id )
{
    for( auto& s : skills )
    {
        if( s.GetId() == id )
        {
            switch (id)
            {
            case 0: iCurSkill = SkillID::Default; break;
            case 1: iCurSkill = SkillID::Water1; break;
            case 2: iCurSkill = SkillID::Wind1; break;
            case 3: iCurSkill = SkillID::Fire1; break;
            default: break;
            }
            iAskill = GetISkill();
            break;
        }
    } 
}
///////////////////////////////////////////////////////////////////////////////////////
void Character::LoadEffects( const std::string& fileName, const FontC& _font )
{
    if( EffActive.LoadSrcEntities( fileName ) )
    {
        for( ;iEffChar < EffActive.GetAmountEntity();iEffChar++ )
        {
            effChars.emplace_back( EffActive.EntityByIndex(iEffChar) );
        }
    }
    font = _font;
    fontNum.LoadSurface("../assets/font/number.png"); 
}
void Character::UpdateEff( const float dTime )
{
    for( int i = 0; i < (int)StateEff::Count; i++ )
    {
        if( tickEffActives[i] )
        {
            Entity& e = effChars[i];
            //Ignore SetEff when IsDonePeriod Not Yet
            if( e.IsDonePeriod() ) SetEffByIndex( e, i );
            e.Update( dTime );
            //Reset Tick When DonePeriod
            if( e.IsDonePeriod() ) tickEffActives[i] = 0;
        }
    }
    font.UpdateEffText( dTime );
    fontNum.UpdateEffText( dTime );
}
void Character::SetEffByIndex( Entity& e, const int index )
{
    switch (index)
    {
    case 0:     //Fall_Down_Water
        e.SetPos( pos + Vec2{-18.0f, -24.0f} );
        e.SetDimens( {64.0f, 64.0f} );
        e.SetType(Entity::Type::Transparent);
        break;
    case 1:     //Under_Water
        e.SetPos( pos + Vec2{0.0f, -46.0f} );
        e.SetDimens( {48.0f, 48.0f} );
        e.SetType(Entity::Type::Transparent);
        break;
    default:
        break;
    }
}
void Character::DrawEff() const
{
    for( int i = 0; i < (int)StateEff::Count; i++ )
    {
        if( tickEffActives[i] )
        {
            effChars[i].Draw();
        }
    }
    font.DrawTextEff();
    fontNum.DrawTextEff();
}