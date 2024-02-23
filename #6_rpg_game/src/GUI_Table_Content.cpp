#include "raylibCpp.h"
#include "GUI_Table_Content.h"

void GUI_Table_Content::LoadAsset( const Character& tie,const FontC& _font, const Vec2& pos_main_table )
{
    if( !isLoad )
    {
        font = _font;

        //Load Luggage
        Layer::CallVar( Layer::Var::Tab_Luggage ) = false;
        Layer::CallVar( Layer::Var::LugDesc_Luggage ) = true;
        luggage.LoadData("../assets/data/ui/luggage.txt");
        luggage.InitPosOrigin( pos_main_table );
        luggage.InitData( 
            tie.luggage.GetAmountCell(), tie.luggage.GetAmountItem(),
            tie.luggage.GetItemsDesc()
        );
        
        //Load area descripton
        lug_area_desc.SetNewDest( luggage.GetAreaDesc().GetPos(), luggage.GetAreaDesc().GetDimens() );
        lug_area_desc.Init( AreaDesc::Type::Desc_Item );
        lug_area_desc.SetFont( font );

        lug_action_item.Load( luggage.GetDataBuff() );
        lug_action_item.SetAreaAction( luggage.GetPosAreaAction(), GuiCommon::GetEntity(GuiCommon::SeqID::Button_0) );
        lug_action_item.Init( std::vector<std::string>{ 
            "Use", "Remove"
        }, Layer::Var::LugDesc_Luggage );

        lug_area_notify.Init(
            GuiCommon::GetEntity(GuiCommon::SeqID::Area_Notify_Vert),
            luggage.GetPosAreaConfirm(), 
            luggage.GetScale()
        );
        lug_area_notify.ContentNotify( GuiCommon::Get().text_confirm );
        lug_action_item.id_modal_own = lug_area_notify.GetIDModal();

        isLoad = true;
    }
}
void GUI_Table_Content::Update( Character& tie )
{
    if( Layer::BeUpdate( Layer::ID::Luggage ) )
    {     
        luggage.Update( tie.luggage.GetItemsDesc() );
        
        lug_area_desc.Update();
        if( luggage.IsNewPickCell() ) 
            lug_area_desc.SetItemDesc( std::move(luggage.ItemPick()) );
        if( luggage.idItemPick != -1 ) 
            lug_action_item.Update( tie, luggage, luggage.idItemPick );
    }

    lug_area_notify.Update();
}
void GUI_Table_Content::Draw() const
{
    if( Layer::BeDraw( Layer::ID::Luggage ) )
    {
        luggage.Draw( font );
        lug_area_desc.Draw();
        if( luggage.idItemPick != -1 ) lug_action_item.Draw( font );
    }
    lug_area_notify.Draw();
}
////////////////////////////////////////////////////////////////
//Luggage
void GUI_Table_Content::Luggage::LoadData( const std::string& fileName )
{
    ListStyle::LoadData( fileName );
    std::stringstream buff( Sentities.GetDataBuff() );
    std::string token;
    while( std::getline( buff, token ) )
    {
        if( token == "[Area_Description_Init]" )
        {
            while( std::getline( buff, token ) && token != "#" )
            {
                if( token == "{Dimension}" )
                {
                    std::getline( buff, token );
                    area_desc.SetDimens( rayCpp::CreateVecByData( token, ' ' ) * scale );
                }
            }

            break;
        }
    }
}
void GUI_Table_Content::Luggage::InitPosOrigin( const Vec2& _pos_main_table )
{
    pos_main_table = _pos_main_table;
    boundary_frame = RectF{ pos_main_table + padcorner_vs_maintable, boundary_dimens.x, boundary_dimens.y };
    area_desc.SetPos( Vec2{ boundary_frame.right + 1*scale, boundary_frame.top } );
}
void GUI_Table_Content::Luggage::InitData( 
    const int amountCell, const int amountItem,
    const std::vector<Character::Luggage::ItemDesc>& vItemsDesc 
)
{
    InitTabControl( std::vector<Layer::Var>{
        Layer::Var::Tab_Luggage, Layer::Var::LugDesc_Luggage
    } );
    amount_elem_int = amountCell;
    amount_elem_hori = 6;
    for( int i = 0; i < amountCell; i++ )
    {
        entities.emplace_back(Sentities.EntityByIndex((int)SeqID::Cell_LK));
        entities.back().SetEntity(
            boundary_frame.topleft + Vec2{ 
                (i % amount_elem_hori)*(elem_size + elem_margin), 
                (i / amount_elem_hori)*(elem_size + elem_margin) 
            },
            Vec2{ elem_size, elem_size }, i, 0
        );
        pos_default.emplace_back( entities.back().GetPos() );
    }
    top_elems = pos_default.front().y;
    bottom_elems = pos_default.back().y + elem_size;
    elem_select.SetEntity(
        entities[0].GetPos(), entities[0].GetDimens(), -1, 0
    );

    //Init Item inside cell
    for( int i = 0; i < amountItem; i++ )
    {
        const int index = SrcItems::IndexById( vItemsDesc[i].id );
        if( index != -1 )
        {
            items_desc.emplace_back( SrcItems::ItemByIndex( index ) );
            items_desc[i].text_desc = vItemsDesc[i];
            items_desc[i].SetDimens(Vec2{ elem_size, elem_size });
            const int indexCell = vItemsDesc[i].order;
            if( indexCell > -1 && indexCell < amountCell )
            {   
                items_desc[i].SetPos( pos_default[indexCell] );
                const int indexEntity = vItemsDesc[i].isLocked 
                    ? (int)SeqID::Cell_LK
                    : (int)SeqID::Cell_ULK;
                entities[vItemsDesc[i].order].SetAnimation( Sentities.EntityByIndex(indexEntity).GetAnimation() );
            }
            items_desc[i].pos_draw_amount = items_desc[i].GetPos() + Vec2{ elem_size - 6.0f, elem_size - 15.0f };
            items_desc[i].color_amount = CELL_SELECT;
        }
    }
}
void GUI_Table_Content::Luggage::InitTabControl( const std::vector<Layer::Var>& var_tabs )
{
    amount_tab_control = (int)var_tabs.size();
    for( auto& v : var_tabs ) vBeAllowKeyControl.insert( {v, true} );
}
void GUI_Table_Content::Luggage::RemoveItem( const int id_item_active )
{
    auto itr = items_desc.begin();
    for( ; itr != items_desc.end(); itr++ ) 
    {
        if( id_item_active == itr->text_desc.id ) break;
    }

    if( itr != items_desc.end() )
    {
        if( --(itr->text_desc.amount) <= 0 )
        {
            entities[itr->text_desc.order].SetAnimation( Sentities.EntityByIndex((int)SeqID::Cell_LK).GetAnimation() );
            items_desc.erase( itr );
            idItemPick = -1;
            TakeBackPermissionKeyControl();
        }
    }
}
void GUI_Table_Content::Luggage::AddItem( const int id_item_active )
{
    auto itr = items_desc.begin();
    for( ; itr != items_desc.end(); itr++ ) 
    {
        if( id_item_active == itr->text_desc.id ) break;
    }

    if( itr != items_desc.end() )
    {
        itr->text_desc.amount++;
    }
    else
    {

    }
}
void GUI_Table_Content::Luggage::Update( const std::vector<Character::Luggage::ItemDesc>& vItemsDesc )
{       
    if( ListStyle::bAllowKeyControl )   //Is this luggage got control 
    {
        if( IsKeyPressed( KEY_UP ) || IsKeyPressed( KEY_W ) )
        {
            const int id = elem_select.GetId();
            if( id < amount_elem_hori )
            {
                elem_select.SetEntity( entities[id].GetPos(), entities[id].GetDimens(), -1, 0 );
                Layer::CallVar( Layer::Var::Tab_Luggage ) = false;
            }
        }
        else if( IsKeyPressed( KEY_TAB ) || IsKeyPressed( KEY_ENTER ) )
        {
            if( idItemPick != -1 ) Layer::CallVar( Layer::Var::LugDesc_Luggage ) = false;
        }
    }
    else 
    {
        for( auto& var : vBeAllowKeyControl )
        {
            if( !var.second )   //Is Other got control 
            {
                switch ( var.first )
                {
                case Layer::Var::Tab_Luggage:
                {
                    if( IsKeyPressed( KEY_DOWN ) || IsKeyPressed( KEY_S ) )
                    {
                        TakeBackPermissionKeyControl();
                    }
                } break;

                case Layer::Var::LugDesc_Luggage:
                {
                    if( IsKeyPressed( KEY_TAB ) )
                    {
                        TakeBackPermissionKeyControl();
                    }
                } break;

                default: break;
                }
            }
        }
    }

    for( auto& var : vBeAllowKeyControl ) var.second = Layer::CallVar( var.first );

    ListStyle::bAllowKeyControl = 
        vBeAllowKeyControl[Layer::Var::Tab_Luggage] &&
        vBeAllowKeyControl[Layer::Var::LugDesc_Luggage];

    ListStyle::Update();

    if( isNewPick && !isKeyControl ) 
    {
        //Give back all permission keycontrol to this luggage 
        TakeBackPermissionKeyControl();
    }

    if( ListStyle::isMoving )
    {
        for( int i = 0; i < (int)items_desc.size(); i++ )
        {
            items_desc[i].SetPos( entities[items_desc[i].text_desc.order].GetPos() );
            items_desc[i].pos_draw_amount = items_desc[i].GetPos() + Vec2{ elem_size - 6.0f, elem_size - 15.0f };
        }
    }
}
void GUI_Table_Content::Luggage::TakeBackPermissionKeyControl()
{
    for( auto& var : vBeAllowKeyControl ) 
    {
        Layer::CallVar( var.first ) = true;
    }
}
void GUI_Table_Content::Luggage::Draw( const FontC& font ) const
{
    ListStyle::Draw( font );
    for( int i = 0; i < (int)items_desc.size(); i++ )
    {
        items_desc[i].DrawInsideOf( boundary_frame );
        if( items_desc[i].text_desc.amount > 1 )
        {
            const std::string& text_amount = std::to_string(items_desc[i].text_desc.amount);
            font.DrawTextInBound(
                text_amount, 
                items_desc[i].pos_draw_amount - Vec2{font.WidthOf(text_amount) , 0.0f},
                boundary_frame,
                items_desc[i].color_amount
            );
        }
    }
}
const Item GUI_Table_Content::Luggage::ItemPick()
{
    Item it{};
    idItemPick = -1;
    for( int i = 0; i < (int)items_desc.size(); i++ )
    {
        if( elem_select.GetId() == items_desc[i].text_desc.order )
        {
            it = items_desc[i];
            idItemPick = items_desc[i].text_desc.id;
            break;
        }
    }
    return it;
}
const Vec2 GUI_Table_Content::Luggage::GetPosAreaAction() const
{ 
    return Vec2{ area_desc.GetCenter().x, area_desc.GetRect().bottom };
}
const std::string& GUI_Table_Content::Luggage::GetDataBuff() const
    { return ListStyle::Component::Sentities.GetDataBuff(); }
const Vec2 GUI_Table_Content::Luggage::GetPosAreaConfirm() const
{
    return Vec2{ area_desc.GetRect().right + 10.0f*scale, area_desc.GetRect().top }; 
}
////////////////////////////////////////////////////////////////
void GUI_Table_Content::Lug_Action::Update( Character& tie, GUI_Table_Content::Luggage& gui_lug, const int id_item_active )
{
    AreaAction::Update();

    //Key control
    if( bAllowKeyControl )
    {
        if( IsBtnPressed((int)Lug_Action::SeqID::Use) )
        {
            Use( tie, gui_lug, id_item_active );
        }
        else if( IsBtnPressed((int)Lug_Action::SeqID::Remove) )
        {
            Modal::PushWait( id_modal_own );
            if( id_modal_own == -1 )
            {   
                //ToDo
                RemoveItem( tie, gui_lug, id_item_active );
            }
        }
    }

    //Mouse event
    if( IsBtnClicked((int)Lug_Action::SeqID::Use) )
    {
        //ToDo
        Use( tie, gui_lug, id_item_active );
    }
    else if( IsBtnClicked((int)Lug_Action::SeqID::Remove) )
    {
        Modal::PushWait( id_modal_own );
        //No confirm, so...
        if( id_modal_own == -1 )
        {   
            //ToDo
            RemoveItem( tie, gui_lug, id_item_active );
        }
    }

    //Has confirm and ...
    if( Modal::HasDone() && Modal::HasDone( id_modal_own ) )
    {
        if( Modal::TakeConfirm( id_modal_own ) )
        {
            RemoveItem( tie, gui_lug, id_item_active );
        }
    }
}
void GUI_Table_Content::Lug_Action::Use( Character& tie, GUI_Table_Content::Luggage& gui_lug, const int _id_item_active )
{
    if( tie.UseItem( _id_item_active ) )
    {
        gui_lug.RemoveItem( _id_item_active );
        const int index = SrcItems::IndexById( _id_item_active );
        if( index != -1 )
            NoteAction::Push( "You already use " + SrcItems::ItemByIndex(index).GetName() + "!" );
    }   
}
void GUI_Table_Content::Lug_Action::RemoveItem( Character& tie, GUI_Table_Content::Luggage& gui_lug, const int _id_item_active )
{
    gui_lug.RemoveItem( _id_item_active );
    tie.luggage.RemoveItem( _id_item_active );
    const int index = SrcItems::IndexById( _id_item_active );
    if( index != -1 )
        NoteAction::Push( "You already remove " + SrcItems::ItemByIndex(index).GetName() + "!" );
}
///////////////////////////////////////////////////////////////////////////////////////////////
void GUI_Table_Content::Lug_Confirm::Update()
{
    AreaConfirm::Update();

    if( AreaConfirm::display )
    {
        if( IsYesPressed() ) std::cout << "Btn Yes" << std::endl;
        else if( IsNoPressed() ) std::cout << "No" << std::endl;
    }
}
