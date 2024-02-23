#include "raylibCpp.h"
#include "GUI/common.h"

/////////////////////////////////////////////////////////////////////////////////////
//GUI Common
void GuiCommon::_LoadVariable( const std::string& fileName )
{
    if( !filesLoaded.count(fileName) )
    {
        filesLoaded.insert(fileName);
        data_var.NewBuff();
        if( data_var.LoadFromFile(fileName) )
        {
            std::stringstream buff( data_var.GetBuff() );
            std::string token;
            while (std::getline(buff, token) && token != "#")
            {
                if( token == "[Text_Confirm]" )
                {
                    std::getline( buff, token );
                    text_confirm = token;
                }
            }
            std::cout<<"[GuiCommon]: Load Success From "<<fileName<<std::endl;
        }
        else std::cout<<"[GuiCommon]: Faild Load From "<<fileName<<std::endl;
    }
    else std::cout<<"[GuiCommon]: Data: "<<fileName<<"is loaded, ignore this time"<<std::endl;
}
/////////////////////////////////////////////////////////////////////////////////////
//Note Action
void NoteAction::_Init( const FontC& _font )
{
    font = _font;
    pos_center = Vec2{ float(settings::screenW/2), float(settings::screenH/4) };
}
void NoteAction::_Push( const std::string& text )
{
    show.push_back( std::pair{dura, text} );
    amount++;
}
void NoteAction::_Update( const float& dTime )
{
    if( amount > 0 )
    {
        for( auto& i : show )
        {
            i.first -= dTime;
        }
        if( show.begin()->first <= 0.0f )
        {
            --amount;
            show.erase( show.begin() );
        }
    } 
}
void NoteAction::_Draw()
{
    if( amount > 0 )
    {
        for( auto itr = show.begin(); itr != show.end(); itr++ )
        {
            if( itr == show.begin() || (itr-1)->first < dura/4 )
            {
                const Vec2 pos_draw = pos_center - Vec2{ 0.0f, distanceY*(1 - (itr->first/dura)) };
                rayCpp::DrawRectFill( RectF::FromCenter( 
                    pos_draw, 
                    font.WidthOf((int)show.begin()->second.length())/2 + 20.0f,
                    font.GetGlyphHeight()/2
                ), BLACK_TRANS_100 );
                font.DrawTextCenter(
                    show.begin()->second,
                    pos_draw, WHITE
                );
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
//Component
void Component::LoadSrc( const std::string& fileName )
{
    Sentities.LoadSrcEntities( fileName );
}

/////////////////////////////////////////////////////////////////////////////////////
//Button
void Button::Update()
{
    Entity::ProcessInput();
}
const bool Button::IsClicked() const { return GetStateEvent() == Entity::StateEvent::Clicked; }
const bool Button::IsReleased() const { return GetStateEvent() == Entity::StateEvent::Released; }
void Button::Draw( const FontC& font ) const
{
    if( IsReleased() )
        Entity::DrawHasColor( Color{217, 215, 215, 255} );
    else Entity::Draw();

    font.DrawTextCenter( title, GetCenter(), WHITE_TRANS_1 );
}

/////////////////////////////////////////////////////////////////////////////////////
//ListStyle
void ListStyle::LoadData( const std::string& fileName )
{
    Component::LoadSrc( fileName );
    //LoadVariable
    std::stringstream buff( Sentities.GetDataBuff() );
    std::string token;
    while (std::getline(buff, token) && token != "##" )
    {
        if( token == "[Boundary_Frame]" )
        {
            std::vector<float> angle_rect;
            while ( std::getline( buff, token, ' ' ) && (token != "") )
            {
                angle_rect.push_back(std::stoi(token));
            }
            boundary_frame = RectF{ angle_rect[0], angle_rect[1], angle_rect[2], angle_rect[3] };
        }
        else if( token == "[Scale]" )
        {
            std::getline( buff, token );    
            scale = std::stoi(token);
        }
        else if( token == "[Boundary_Dimens]" )
        {
            std::getline( buff, token );    
            boundary_dimens = rayCpp::CreateVecByData(token, ' ');
        }
        else if( token == "[Padding_Corner_Vs_PosMainTable]" )
        {
            std::getline( buff, token );    
            padcorner_vs_maintable = rayCpp::CreateVecByData(token, ' ');
        }
        else if( token == "[Element_Margin]" )
        {
            std::getline( buff, token);
            elem_margin = std::stof( token );
        }
        else if( token == "[Element_Select_Surface_ID]" )
        {
            std::getline( buff, token);
            const int id = std::stoi( token );
            if( id >= 0 && id < Sentities.GetAmountEntity() ) 
                elem_select = Sentities.EntityByIndex(std::stoi(token));
        }
    }

    ScaleVariable();
}
void ListStyle::ScaleVariable()
{   
    boundary_dimens = Vec2{ boundary_dimens * scale };
    padcorner_vs_maintable = Vec2{ padcorner_vs_maintable * scale };
    elem_size = Sentities.EntityByIndex(0).GetSrcDimens().x * scale;    //Cell src dimension
    elem_margin *= scale;
}
void ListStyle::Update()
{
    ListStyle::ProcessInput();

    //Flag express for list is moving
    isMoving = false;
    //Update Pos Element By Event Mouse Released
    if( delta_move != 0.0f || inertia != 0.0f )
    {
        for( int i = 0; i < amount_elem_int; i++ )
        {
            entities[i].SetPos( pos_default[i] + Vec2{0.0f, offsetY} );
        }
        top_elems = entities[0].GetPos().y;
        bottom_elems = entities[amount_elem_int - 1].GetPos().y + elem_size;
        elem_select.SetPos( entities[elem_select.GetId()].GetPos() );
    
        isKeyControl = false;
        isMoving = true;
    }
    else if( IsMouseButtonUp( MOUSE_BUTTON_LEFT ) || !boundary_frame.IsCover( mousePos ) )
    {   
        //Lerp Element Table
        const float dis_top_t = top_elems - boundary_frame.top;
        const float dis_bottom_t = bottom_elems - boundary_frame.bottom;
        if( !isKeyControl )
        {
            if( dis_top_t * dis_bottom_t >= 0.0f )
            {
                if( dis_top_t > 2.0f ) LerpVerticalList( dis_top_t );
                else if( dis_bottom_t < 2.0f ) LerpVerticalList( dis_bottom_t );   
            }
        }

        //Lerp Element Table By Element Select
        if( isKeyControl )
        {
            const float elem_slt_posY = elem_select.GetPos().y;
            //Check with one line element and inside boundary
            const float dis_top_c = elem_slt_posY - (boundary_frame.top + elem_size );
            const float dis_bottom_c = elem_slt_posY + elem_size - (boundary_frame.bottom - elem_size);
            if( dis_top_c < -2.0f && dis_top_t < 0.0f ) 
                LerpVerticalList( dis_top_c );
            else if( dis_bottom_c > 2.0f && dis_bottom_t > 0.0f ) 
                LerpVerticalList( dis_bottom_c );
        }
    }
}
void ListStyle::ProcessInput()
{
    //Mouse Event
    mousePos = rayCpp::GetMousePos();
    isNewPick = false;
    if( boundary_frame.IsCover( mousePos ) )
    {
        //Check for id valid or be releasing... if not, let active new element 
        const int last_idActive = id_elemActive;
        if( id_elemActive < 0 || id_elemActive > amount_elem_int || entities[id_elemActive].GetStateEvent() != Entity::StateEvent::Released )
        {
            const Vec2&& delta = (mousePos - boundary_frame.topleft - Vec2{0.0f, offsetY});
            id_elemActive = (
                (int)(delta.x / (elem_size + elem_margin)) 
                + (int)(delta.y / (elem_size + elem_margin))*amount_elem_hori
            );
        }

        if( id_elemActive >= 0 && id_elemActive < amount_elem_int )
        {
            Entity& e = entities[id_elemActive];
            e.ProcessInput();
            if( e.GetStateEvent() == Entity::StateEvent::Clicked )
            {
                if( std::fabs( delta_move ) < 10.0f && elem_select.GetId() != e.GetId() )
                {
                    isKeyControl = false;
                    elem_select.SetEntity( e.GetPos(), e.GetDimens(), e.GetId(), 0 );
                    isNewPick = true;
                    return;
                }
            }
            else ProcessEventRelease();
        }
        else id_elemActive = last_idActive;
    }
    else if( id_elemActive != -1 )
    {
        entities[id_elemActive].ProcessInput();
        ProcessEventRelease();
    }

    //Keyboard Event
    int id = elem_select.GetId();
    if( bAllowKeyControl )
    {
        if( !isKeyControl && GetKeyPressed() != 0 ) isKeyControl = true;
        isNewPick = true;
        if( IsKeyPressed( KEY_UP ) || IsKeyPressed( KEY_W ) )
        {
            if( id >= amount_elem_hori ) id -= amount_elem_hori;
        }
        else if( IsKeyPressed( KEY_DOWN ) || IsKeyPressed( KEY_S ) )
        {
            if( id == -1 ) id = 0;
            else if( id < amount_elem_int - amount_elem_hori ) id += amount_elem_hori;
        }
        else if( IsKeyPressed( KEY_LEFT ) || IsKeyPressed( KEY_A ) )
        {
            if( --id < 0 ) id = amount_elem_int - 1; 
        }
        else if( IsKeyPressed( KEY_RIGHT ) || IsKeyPressed( KEY_D ) )
        {
            if( ++id >= amount_elem_int ) id = 0;
        }
        else isNewPick = false;
        if( isNewPick && id != -1 ) elem_select.SetEntity( entities[id].GetPos(), entities[id].GetDimens(), entities[id].GetId(), 0 );
    }
}
void ListStyle::ProcessEventRelease()
{
    Entity& e = entities[id_elemActive];
    if( e.GetStateEvent() == Entity::StateEvent::Released )
    {
        const int newDis = (mousePos.y - e.GetPosBegin().y) * 0.6f;     //60% resistance compare with mouse distance released
        inertia = newDis - delta_move;      //distance mouse pos glide
        offsetY += inertia;    
        delta_move = newDis;
    }
    else if( delta_move != 0.0f && IsMouseButtonUp( MOUSE_BUTTON_LEFT ) )   //When exit released
    {
        //No inertia when over bouadary
        if( top_elems - boundary_frame.top > 0.0f || bottom_elems - boundary_frame.bottom < 0.0f )
        {
            inertia = 0.0f;
            delta_move = 0.0f;
        }
        //Apply inertia
        else
        {
            inertia -= inertia*0.1f;
            offsetY += inertia;
            if( std::fabs( inertia ) < 2.0f 
                || top_elems-boundary_frame.top >= 0.0f 
                || bottom_elems-boundary_frame.bottom <= 0.0f )
            {
                inertia = 0.0f;
                delta_move = 0.0f;
            }
        }
    }
}
void ListStyle::LerpVerticalList( const float distance, const float speed )
{
    for( int i = 0; i < amount_elem_int; i++ )
    {
        entities[i].SetPos( entities[i].GetPos() - Vec2{ 0.0f, distance * speed } );
    }
    offsetY -= distance * speed;
    top_elems = entities[0].GetPos().y;
    bottom_elems = entities[amount_elem_int - 1].GetPos().y + elem_size;
    elem_select.SetPos( entities[elem_select.GetId()].GetPos() );

    isMoving = true;
}
void ListStyle::Draw( const FontC& font ) const
{
    for( const auto& e : entities )
    {
        if( e.GetRect().IsInsideOf( boundary_frame ) )
            e.Draw();
        else if( e.GetRect().IsOverLapping( boundary_frame ) ) 
            e.DrawInsideOf( boundary_frame );
    }
    if( elem_select.GetId() != -1 )
    {
        if( elem_select.GetRect().IsInsideOf( boundary_frame ) ) 
            elem_select.Draw();
        else if( elem_select.GetRect().IsOverLapping( boundary_frame ) )
            elem_select.DrawInsideOf( boundary_frame );
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
//Area Description Item
AreaLerp::AreaLerp( const Entity& entity )
    :
    Entity( entity )
{}
AreaLerp::AreaLerp( const Vec2& pos, const Vec2& dimens )
{
    Entity::SetEntity( pos, dimens, -1, 0 );
    Init();
}
void AreaLerp::SetNewDest( const Vec2& pos, const Vec2& dimens )
{
    Entity::SetEntity( pos, dimens, -1, 0 );
}
void AreaLerp::Init()
{
    bound_frame = Entity::GetRect();
    top = Entity::pos.y;
    bottom = top + Entity::dimension.y;
    height_bound = bound_frame.height;
}
void AreaLerp::Update()
{
    AreaLerp::ProcessInput();
    if( delta_move != 0.0f || inertia != 0.0f )
    {
        SetPos( bound_frame.topleft + Vec2{0.0f, offsetY} );
        top = GetPos().y;
        bottom = top + height_bound;
    }
    else if( IsMouseButtonUp( MOUSE_BUTTON_LEFT ) || !bound_frame.IsCover( mousePos ) )
    {   
        const float dis_top_t = top - bound_frame.top;
        const float dis_bottom_t = bottom - bound_frame.bottom;
        if( dis_top_t * dis_bottom_t >= 0.0f )
        {
            if( dis_top_t > 2.0f ) LerpVertical( dis_top_t );
            else if( dis_bottom_t < 2.0f ) LerpVertical( dis_bottom_t );   
        }
    }
}
void AreaLerp::ProcessInput()
{
    Entity::ProcessInput( bound_frame );
    mousePos = rayCpp::GetMousePos();
    if( GetStateEvent() == Entity::StateEvent::Released )
    {
        const int newDis = (mousePos.y - GetPosBegin().y) * 0.6f;     //60% resistance compare with mouse distance released
        inertia = newDis - delta_move;      //distance mouse pos glide
        offsetY += inertia;    
        delta_move = newDis;
    }
    else if( delta_move != 0.0f && IsMouseButtonUp( MOUSE_BUTTON_LEFT ) )   //When exit released
    {
        //No inertia when over bouadary
        if( top - bound_frame.top > 0.0f || bottom - bound_frame.bottom < 0.0f )
        {
            inertia = 0.0f;
            delta_move = 0.0f;
        }
        //Apply inertia
        else
        {
            inertia -= inertia*0.1f;
            offsetY += inertia;
            if( std::fabs( inertia ) < 1.0f || top-bound_frame.top >= 0.0f || bottom-bound_frame.bottom <= 0.0f )
            {
                inertia = 0.0f;
                delta_move = 0.0f;
            }
        }
    }
}
void AreaLerp::LerpVertical( const float distance, const float speed )
{
    SetPos( GetPos() - Vec2{ 0.0f, distance * speed } );
    offsetY -= distance * speed;
    top = GetPos().y;
    bottom = top + height_bound;
}

////////////////////////////////////////////////////////////////////////////////////////////
//Area Description Item
void AreaDesc::Init( const Type& type )
{
    this->type = type;
    AreaLerp::Init();
    area_desc_separate = RectF{ 
        Vec2{ bound_frame.left + padding_hori, bound_frame.bottom }, 
        bound_frame.width - padding_hori*2, 2.0f 
    };
}
void AreaDesc::SetItemDesc( const Item& _item )
{
    item = _item;

    const float width_bound = bound_frame.width - 2*padding_hori;
    item_desc.name = std::move( font.MatchingTextHori(item.GetName(), width_bound ) );
    item_desc.info = std::move( font.MatchingTextHori(item.GetInfo(), width_bound ) );
    item_desc.cost_sell = "0";

    height_bound = font.HeigthOf(
        pad_info, font.GetGlyphHeight(),
        item_desc.name, item_desc.info
    );

    if( height_bound < bound_frame.height ) height_bound = bound_frame.height;
    bottom = top + height_bound;
}
void AreaDesc::SetTextDesc( const std::string& text )
{
    const float width_bound = bound_frame.width - 2*padding_hori;
    text_desc = std::move( font.MatchingTextHori( text, width_bound ) );

    height_bound = font.HeigthOf( pad_header, font.GetGlyphHeight(), text_desc );

    if( height_bound < bound_frame.height ) height_bound = bound_frame.height;
    bottom = top + height_bound;
}
void AreaDesc::Update()
{
    if( type == Type::Desc_Item )
    {
        if( item.GetId() != -1 ) AreaLerp::Update();
    }
    else AreaLerp::Update();
}
void AreaDesc::Draw() const
{
    rayCpp::DrawRectFill( area_desc_separate, WHITE_TRANS);
    if( type == Type::Desc_Item )
    {
        if( item.GetId() != -1 )
        {
            font.DrawTextInBound(
                item_desc.name, pos + Vec2{padding_hori, pad_header},
                bound_frame, YELLOW
            );
            font.DrawTextInBound(
                item_desc.info, pos + Vec2{padding_hori, pad_info},
                bound_frame, RAYWHITE
            );
        }
        else font.DrawTextCenter( "Pick anything!", bound_frame.GetCenterPoint(), WHITE_TRANS );
    }
    else
    {
        font.DrawTextInBound( text_desc, pos + Vec2{ padding_hori, pad_header }, bound_frame, WHITE );
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
//Area Action
void AreaAction::Load( const std::string& data )
{
    bUseDataByLoad = true;
    std::stringstream buff( data );
    std::string token;
    while ( std::getline( buff, token ) )
    {
        if( token == "[Area_Action]" )
        {
            while( std::getline( buff, token ) && token != "#" )
            {
                if( token == "{Button_Name}" )
                {
                    while( std::getline( buff, token, ' ' ) && token != "" )
                    {
                        vBtnName.push_back( token );
                    }
                }
                else if( token == "{Button_Scale}" )
                {
                    std::getline( buff, token );
                    scale = std::stoi(token);
                }
                else if( token == "{Padding_Hori}" )
                {
                    std::getline( buff, token );
                    pad_hori = std::stoi(token);
                }
                else if( token == "{Padding_Vert}" )
                {
                    std::getline( buff, token );
                    pad_vert = std::stoi(token);
                }
            }

            break;
        }
    }
    amount_action = (int)vBtnName.size();
}
void AreaAction::SetAreaAction( const Vec2& _pos_center, const Entity& _btn_surf, const int _scale )
{
    pos_center = _pos_center;
    btn_surf = _btn_surf;
    if( !bUseDataByLoad ) scale = _scale;
}
void AreaAction::Init( const std::vector<std::string>& vActions, Layer::Var toggle_var )
{
    if( toggle_var != Layer::Var::Default ) pToggle_KBC = &Layer::CallVar( toggle_var );
    if( !bUseDataByLoad )
    {
        amount_action = (int)vActions.size();
        vBtnName = vActions;
    }
    const Vec2 pos_begin = Vec2{
        pos_center.x - (amount_action*(btn_surf.GetSrcDimens().x*scale) + pad_hori*(amount_action-1)) / 2,
        pos_center.y - (btn_surf.GetSrcDimens().y*scale) / 2 + pad_vert
    };
    for( int i = 0; i < amount_action; i++ )
    {
        buttons.emplace_back( Button{} );
        Button& b = buttons.back();
        b.SetAnimation( btn_surf.GetAnimation() );
        b.SetEntity( 
            pos_begin + Vec2{ i*(btn_surf.GetSrcDimens().x*scale + pad_hori), 0.0f },
            btn_surf.GetSrcDimens()*scale, i, 0
        );
        b.SetTitle( vBtnName[i] );
    }
    if( amount_action > 0 )
    {
        curBtn_select_surf = GuiCommon::GetEntity( GuiCommon::SeqID::Btn_Frame_Slt );
        curBtn_select_surf.SetPos( buttons[0].GetPos() );
        curBtn_select_surf.SetDimens( buttons[0].GetDimens() );
    }
}
void AreaAction::Update()
{
    for( int i = 0; i < (int)buttons.size(); i++ )
    {
        buttons[i].Update();
    }   

    //Allow key control
    if( bAllowKeyControl )
    {
        if( IsKeyPressed( KEY_RIGHT ) )
        {
            if( ++curBtn_select >= amount_action ) curBtn_select = 0;
            curBtn_select_surf.SetPos( buttons[curBtn_select].GetPos() );
        }
        else if( IsKeyPressed( KEY_LEFT ) )
        {
            if( --curBtn_select < 0 ) curBtn_select = amount_action - 1;
            curBtn_select_surf.SetPos( buttons[curBtn_select].GetPos() );
        }
        else if( IsKeyPressed( KEY_F2 ) && pToggle_KBC != nullptr) *pToggle_KBC = true;
    }
    else isNewPress = false;
    if( pToggle_KBC != nullptr ) bAllowKeyControl = !*pToggle_KBC;
}
void AreaAction::Draw( const FontC& font ) const
{
    for( int i = 0; i < (int)buttons.size(); i++ )
    {
        buttons[i].Draw( font );
    }
    if( bAllowKeyControl ) curBtn_select_surf.Draw();
}
const bool AreaAction::IsBtnClicked( const int index ) const
{
    if( index >= 0 && index < amount_action )
    {
        return buttons[index].IsClicked();
    }
    return false;
}
const bool AreaAction::IsBtnPressed( const int index )
{
    if( index == curBtn_select )
    {
        if( isNewPress || (!IsKeyDown( KEY_ENTER ) && !IsKeyDown( KEY_F1 )) )
        {
            isNewPress = true;
            if( IsKeyDown( KEY_ENTER ) || IsKeyDown( KEY_F1 ) )
            {
                isNewPress = false;
                return true;
            }
        }
    }
    return false;
}
////////////////////////////////////////////////////////////////////////////////////////////
//Area Notify
AreaConfirm::AreaConfirm( const Entity& surf )
{
    Entity{ surf };
}
void AreaConfirm::SetSurf( const Animation& surf )
{
    Entity::SetAnimation( surf );    
}
void AreaConfirm::SetWrapDest( const Vec2& pos, const int scale )
{
    Entity::SetPos( pos );
    Entity::SetDimens( Entity::GetDimens() * scale );
}
void AreaConfirm::Init( const Entity& _surf, const Vec2& _pos, const int _scale )
{
    id_modal = Modal::TakeID();
    font.LoadSurface("../assets/font/fontWhite.png");

    Entity::SetAnimation( _surf.GetAnimation() ); 
    Entity::SetPos( _pos );
    Entity::SetDimens( _surf.GetSrcDimens() * _scale );

    area_desc.SetFont( font );
    area_desc.SetNewDest( pos + pad_desc, dimension - pad_desc*2 - Vec2{0.0f, 40.0f} );
    area_desc.Init( AreaDesc::Type::Desc_Text );

    area_action.pad_hori = 20.0f;
    area_action.pad_vert = 20.0f;
    area_action.SetAreaAction(
        Vec2{ area_desc.GetCenter().x, area_desc.GetRect().bottom },
        GuiCommon::GetEntity( GuiCommon::SeqID::Button_1 ), 2
    );
    area_action.Init( std::vector<std::string>{ "Yes", "No" } );
}
void AreaConfirm::ContentNotify( const std::string& text )
{
    area_desc.SetTextDesc( text );
}
void AreaConfirm::Update()
{
    display = false;
    if( Modal::HasModal() && Modal::HasModal( id_modal ) )
    {
        display = true;
        area_desc.Update();
        area_action.Update();
        if( IsKeyPressed( KEY_RIGHT ) || IsKeyPressed( KEY_LEFT ) )
        {
            area_action.SetAllowKeyControl( true );
        }
        if( area_action.IsBtnClicked(0) || IsBtnPressed(0) )
        {
            Modal::PushDone( id_modal, true );
            area_action.SetAllowKeyControl( false );
        }
        else if( area_action.IsBtnClicked(1) || IsBtnPressed(1) )
        {
            Modal::PushDone( id_modal, false );
            area_action.SetAllowKeyControl( false );
        }
    }
}
void AreaConfirm::Draw() const
{
    if( display )
    {
        Entity::Draw();
        area_desc.Draw();
        area_action.Draw( font );
    }
}
const bool AreaConfirm::IsYesPressed() { return area_action.IsBtnPressed( 0 ); }
const bool AreaConfirm::IsNoPressed() { return area_action.IsBtnPressed( 1 ); }
const bool AreaConfirm::IsBtnPressed( const int id ) { return area_action.IsBtnPressed( id ); }
