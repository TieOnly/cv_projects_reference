#include "raylibCpp.h"
#include "GUI.h"

void GUI::LoadAsset( const Character& tie )
{
    if( !isLoad )
    {
        Layer::Init();
        Layer::On( Layer::ID::Main );
        
        font.LoadSurface("../assets/font/fontWhite.png");
        fontNum.LoadSurface("../assets/font/number.png"); 
        

        stateChar.LoadSrc("../assets/data/ui/state_char.txt");
        stateChar.Init();
        pickSkill.LoadSrc("../assets/data/ui/pick_skill.txt");
        pickSkill.Init();
        pickSkill.SetAvatarSkill( tie );
        buttons.LoadSrc("../assets/data/ui/buttons.txt");
        buttons.Init();
        
        tables.LoadSrc("../assets/data/ui/tables.txt");
        tables.Init();

        isLoad = true;
    }
}
void GUI::Update( Character& tie )
{
    stateChar.Update( tie );
    if( Layer::BeUpdate( Layer::ID::Main ) )
    {
        pickSkill.Update( tie );
        buttons.Update();
        if( IsKeyPressed(KEY_F1) )
        {
            Layer::On( Layer::ID::Table_Tab );
            Layer::OffUpdate( Layer::ID::Main );
        }
    }
    else if( Layer::BeUpdate( Layer::ID::Table_Tab ) )
    {
        tables.Update();
    }
}
void GUI::Draw()
{
    stateChar.Draw( font );
    stateChar.Draw_IFL_Character( font );
    pickSkill.Draw();
    buttons.Draw();
    if( Layer::BeDraw( Layer::ID::Table_Tab ) )
    {
        tables.Draw( font );
    }
}
/////////////////////////////////////////////////////////////////
//StateCharUI
void GUI::StateCharUI::Init()
{
    entities.emplace_back(Sentities.EntityByIndex(0));
    Entity& e = entities[0];
    std::stringstream buff( Sentities.GetDataBuff() );
    std::string token;
    while (std::getline(buff, token) && token != "##" )
    {
        if( token == "[Pos]" )
        {
            std::getline( buff, token );
            e.SetPos( rayCpp::CreateVecByData(token, ' ') );
        }
        else if( token == "[Scale]" )
        {
            std::getline( buff, token );
            scale = std::stof(token);
            e.SetDimens( Vec2{ e.GetSrcDimens().x*scale, e.GetSrcDimens().y*scale } );
        }
        else if( token == "[PosCircle]" )
        {
            std::getline( buff, token );
            pos_circle = rayCpp::CreateVecByData(token, ' ') * scale;
        }
        else if( token == "[Max_HP_Rect]" )
        {
            std::vector<float> angle_rect;
            while ( std::getline( buff, token, ' ' ) && (token != "") )
            {
                angle_rect.push_back(std::stoi(token));
            }
            maxHp_rect = RectF{ angle_rect[0]*scale, angle_rect[1]*scale, angle_rect[2]*scale, angle_rect[3]*scale };
        }
        else if( token == "[Max_MP_Rect]" )
        {
            std::vector<float> angle_rect;
            while ( std::getline( buff, token, ' ' ) && (token != "") )
            {
                angle_rect.push_back(std::stoi(token));
            }
            maxMp_rect = RectF{ angle_rect[0]*scale, angle_rect[1]*scale, angle_rect[2]*scale, angle_rect[3]*scale };
        }
        else if( token == "[Rank]" )
        {
            std::getline( buff, token );
            rank = std::stoi(token);
        }
        else if( token == "[Pos_Origin_IFL]" )
        {
            std::getline( buff, token );
            pos_origin_ifl = rayCpp::CreateVecByData( token, ' ' ) * scale;
        }
        else if( token == "[Width_Each_IFL]" )
        {
            std::getline( buff, token );
            width_each_ifl = std::stof(token);
        }
        else if( token == "[Padding_IFL_Duration]" )
        {
            std::getline( buff, token );
            pad_ifl_dura = std::stof(token);
        }
    }
}
void GUI::StateCharUI::Update( const Character& tie )
{
    // rank = tie.GetRank();
    rateRank = tie.GetRateRank();

    cur_hp = tie.GetCurHP();
    cur_mp = tie.GetCurMP();
    max_hp = tie.GetMaxHP();
    max_mp = tie.GetMaxMP();
    rate_hp = (float)cur_hp / (float)max_hp;
    rate_mp = (float)cur_mp / (float)max_mp;
    curHp_rect = RectF{ maxHp_rect.topleft, maxHp_rect.width * rate_hp, maxHp_rect.height };
    curMp_rect = RectF{ maxMp_rect.topleft, maxMp_rect.width * rate_mp, maxMp_rect.height };
}
void GUI::StateCharUI::Draw( const FontC& font ) const
{
    rayCpp::DrawRectFill( maxHp_rect, BLACK_TRANS );
    rayCpp::DrawRectFill( maxMp_rect, BLACK_TRANS );
    rayCpp::DrawRectFill( curHp_rect, RED );
    rayCpp::DrawRectFill( curMp_rect, BLUE );
    entities[0].Draw();
    font.DrawText( std::to_string(int(rate_hp*100)) + "%", Vec2{maxHp_rect.right + 4.0f, maxHp_rect.top + 2.0f}, {235, 208, 52, 255} );
    font.DrawText( std::to_string(int(rate_mp*100)) + "%", Vec2{maxMp_rect.right + 4.0f, maxMp_rect.top}, {235, 208, 52, 255} );
    font.DrawText( std::to_string(cur_hp), maxHp_rect.topleft + Vec2{12.0f, 2.0f}, WHITE );
    font.DrawText( std::to_string(cur_mp), maxMp_rect.topleft + Vec2{12.0f, 0.0f}, WHITE );
    font.DrawTextCenter( std::to_string(rank), pos_circle, {235, 208, 52, 255}, 2.0f );
}
void GUI::StateCharUI::Draw_IFL_Character( const FontC& font )
{
    if( VarTimer::BoolMustUpdate_PosDrawIFL() )
    {
        int i = 0;
        vPosDrawIFLDura.clear();
        for( auto& ifl : VarTimer::GetActiveIFL() )
        {
            ifl.second.SetPos( pos_origin_ifl + Vec2{ (i++)*width_each_ifl, 0.0f } );
            vPosDrawIFLDura.push_back( 
                ifl.second.GetCenter() 
                + Vec2{ 0.0f, font.GetGlyphHeight() + ifl.second.GetDimens().y/2 }
            );
        }
    }
    int i = 0;
    for( auto& ifl : VarTimer::GetActiveIFL() )
    {
        ifl.second.Draw();
        font.DrawTextCenter(
            std::to_string((int)VarTimer::GetTimer(ifl.first)), 
            vPosDrawIFLDura[i++], WHITE_TRANS
        );
    }
}
/////////////////////////////////////////////////////////////////
//SkillUI
void GUI::SkillUI::Init()
{
    //LoadVariable
    std::stringstream buff( Sentities.GetDataBuff() );
    std::string token;
    while (std::getline(buff, token) && token != "##" )
    {
        if( token == "[Pos_Origin]" )
        {
            std::getline( buff, token );
            pos_origin = rayCpp::CreateVecByData(token, ' ');
        }
        else if( token == "[Cell_Dimens]" )
        {
            std::getline( buff, token );
            cell_dimens = rayCpp::CreateVecByData(token, ' ');
        }
        else if( token == "[Cell_Content_Dimens]" )
        {
            std::getline( buff, token );
            const Vec2& v = rayCpp::CreateVecByData(token, ' ');
            rect_content_default = RectF::FromCenter( pos_origin + (cell_dimens/2.0f), v.x/2.0f, v.y/2.0f );
        }
        else if( token == "[Cell_Padding]" )
        {
            std::getline( buff, token );
            cell_padding = std::stof(token);
        }
        else if( token == "[Id_Cell]" )
        {
            std::getline( buff, token );
            idCell = std::stoi(token);
        }
    }
    amountEntity = Sentities.GetAmountEntity();

    //Init
    if( idCell >= 0 && idCell < 6 )
    {
        for( int i = 0; i < amountCell; i++ )
        {
            //Push Default Cell (ID: idCell)
            entities.emplace_back(Sentities.EntityByIndex(idCell));
            entities.back().SetEntity( 
                pos_origin + Vec2{i*(cell_dimens.x + cell_padding), 0.0f},
                cell_dimens,
                i, 0
            );
        }
    }
    for( int i = 0; i < amountCell; i++ )
    {
        //Push "Avatar Skill in Cell"
        entities.emplace_back(Entity{});
        Entity& e = entities[i];
        entities.back().SetEntity(
            e.GetPos(), e.GetDimens(), e.GetId() + amountCell, 0
        );
        dura_rects.emplace_back( RectF{
            rect_content_default.left + i*(cell_dimens.x + cell_padding), 
            rect_content_default.right + i*(cell_dimens.x + cell_padding),
            rect_content_default.top,
            rect_content_default.bottom
        } );
        dura_isDones.emplace_back(true);
    }
    for( int i = 0; i < amountCell; i++ )
    {
        //Push Frame Cell (ID: 1)
        entities.emplace_back(entities[i]);
        entities.back().SetAnimation(Sentities.EntityByIndex(1).GetAnimation());
        cell_isPicks.emplace_back(false);
    }
}
void GUI::SkillUI::SetAvatarSkill( const Character& tie )
{
    listPickSkill = tie.GetIdPickSkills();
    for( int i = amountCell, iList = 0; i < amountCell + 5; i++, iList++ )
    {
        const int id = listPickSkill[iList];
        if( id >= 0 && id < amountEntity )
        {
            entities[i].SetAnimation(Sentities.EntityByIndex(id + 6).GetAnimation());//6 entity once row
        }
    }
}
void GUI::SkillUI::Update( Character& tie )
{
    for( int i = 0; i < amountCell; i++ )
    {
        //Update dura skill, frame pick skill
        const int id = listPickSkill[i];
        if( id != -1 )
        {
            Entity& e = entities[i];
            const Character::InfoChar::Skill& sk = tie.GetSkillById(id);
            if( !sk.isDonePeriod )
            {
                const float rateDura = sk.GetTimerPeriod() / sk.GetDuraPeriod();
                dura_rects[i].top = rect_content_default.top + rateDura*rect_content_default.height;
                dura_isDones[i] = false;
            }
            else dura_isDones[i] = true;

            //Process Event to Pick SKill
            e.ProcessInput();
            if( e.GetStateEvent() == Entity::StateEvent::Clicked )
            {
                tie.PickSkillById( id );
                for( int k = 0; k < amountCell; k++ ) cell_isPicks[k] = (listPickSkill[k] == id);
            }
        }
    }
}
void GUI::SkillUI::Draw() const
{
    for( int i = 0; i < amountCell; i++ ) 
    {
        entities[i].Draw();                                             //Index In Row 0

        const int id = listPickSkill[i];
        if( id != -1 )
        {
            entities[i + amountCell].Draw();                            //Index In Row 1
            if( cell_isPicks[i] ) entities[i + 2*amountCell].Draw();    //Index In Row 2
            if( !dura_isDones[i] ) rayCpp::DrawRectFill( dura_rects[i], BLACK_TRANS );
        }
    }
}
/////////////////////////////////////////////////////////////////

//Map
// void GUI::Map::Init()
// {
//     //LoadVariable
//     std::stringstream buff( Sentities.GetDataBuff() );
//     std::string token;
//     while (std::getline(buff, token) && token != "##" )
//     {
//         if( token == "[Dimens_Mini_Map]" )
//         {
//             std::getline( buff, token );
//             dimens_mini_map = CreateVecByData(token, ' ');
//         }
//         else if( token == "[Title_Header]" )
//         {
//             std::getline(buff, token);
//             title_header = token;
//         }
//     }
//     pos_mini_map = Vec2{settings::screenW - dimens_mini_map.x, 0.0f};
//     center_header = pos_mini_map + Vec2{dimens_mini_map.x/2.0f, 10.0f};
//
//     //Init
//     entities.emplace_back(Sentities.EntityByIndex(0));
//     entities[0].SetEntity( pos_mini_map, dimens_mini_map, 0, 0 );
// }
// void GUI::Map::Update()
// {
//     for( auto& e : entities )
//     {
//         e.ProcessInput();
//         if( e.GetStateEvent() == Entity::StateEvent::Clicked )
//         {
//             std::cout<<e.GetId()<<": Click"<<std::endl;
//         }
//     }
// }
// void GUI::Map::Draw( const FontC& font ) const
// {
//     for( const auto& e : entities ) e.Draw();
//     font.DrawTextCenter( title_header, center_header, WHITE_TRANS );
// }

/////////////////////////////////////////////////////////////////
//Buttons
void GUI::Buttons::Init()
{
    //LoadVariable
    std::stringstream buff( Sentities.GetDataBuff() );
    std::string token;
    while (std::getline(buff, token) && token != "##" )
    {
        if( token == "[Amount]" )
        {
            std::getline( buff, token );
            amount = std::stoi(token);
        }
        else if( token == "[Seq_Pos]" )
        {
            while ( std::getline( buff, token, '|' ) && (token != "") )
            {
                seq_pos.push_back(rayCpp::CreateVecByData(token, ' '));
            }
        }
        else if( token == "[Seq_Dimens]" )
        {
            while ( std::getline( buff, token, '|' ) && (token != "") )
            {
                seq_dimens.push_back(rayCpp::CreateVecByData(token, ' '));
            }
        }
    }

    //Init
    for( int i = 0; i < amount; i++ )
    {
        entities.emplace_back(Sentities.EntityByIndex(i));
        entities.back().SetEntity( seq_pos[i], seq_dimens[i], i, 0 );
    }
}
void GUI::Buttons::Update()
{
    for( auto& e : entities )
    {
        e.ProcessInput();
        if( e.GetStateEvent() == Entity::StateEvent::Clicked )
        {
            if( e.GetId() == (int)SeqID::Menu ) 
            {
                // GUI::SetLayer( GUI::Layer::Table );
                Layer::On( Layer::ID::Table_Tab );
                Layer::OffUpdate( Layer::ID::Main );
            }
        }
    }
}
void GUI::Buttons::Draw() const
{
    for( const auto& e : entities ) e.Draw();
}
/////////////////////////////////////////////////////////////////
//Tables
void GUI::Tables::Init()
{
    //LoadVariable
    std::stringstream buff( Sentities.GetDataBuff() );
    std::string token;
    while (std::getline(buff, token) && token != "##" )
    {
        if( token == "[Amount_Tab]" )
        {
            std::getline( buff, token );
            amount_tab = std::stoi(token);
        }
        else if( token == "[Scale]" )
        {
            std::getline( buff, token );
            scale = std::stoi(token);
        }
        else if( token == "[PosY_MainTable]" )
        {
            std::getline( buff, token );
            posY_mainTable = std::stof(token);
        }
        else if( token == "[Distance_Match_Tabs_With_MainTable]" )
        {
            std::getline( buff, token );
            distance_match = std::stof(token);
        }
        else if( token == "[Tab_Names]" )
        {
            while ( std::getline( buff, token, '|' ) && (token != "") )
            {
                tab_names.push_back(token);
            }
        }
    }

    /////////////////////////////////////////////////
    //Init
    int countID = 0;
    ids[(int)SeqID::Main_Table] = countID;
    ids[(int)SeqID::Front_Tab] = ++countID;
    ids[(int)SeqID::Behind_Tab] = ++countID;
    ids[(int)SeqID::Close_Btn] = countID + (amount_tab - 1); countID += (amount_tab - 1);
    ids[(int)SeqID::Count] = ++countID;

    for( int idEntity = 0; idEntity < (int)SeqID::Count; idEntity++ )
    {
        for( int y = ids[idEntity]; y < ids[idEntity + 1]; y++ )
        {
            seq_pos_default.push_back(Vec2{});
            seq_dimens.push_back( Sentities.EntityByIndex(idEntity).GetSrcDimens() * scale );
        }
    }

    InitDefault();
}
void GUI::Tables::InitDefault()
{   
    //Pos MainTable
    seq_pos_default[ids[(int)SeqID::Main_Table]].x = center_screen.x - seq_dimens[ids[(int)SeqID::Main_Table]].x/2.0f;
    if( posY_mainTable == -1.0f )
        seq_pos_default[ids[(int)SeqID::Main_Table]].y = center_screen.y - seq_dimens[ids[(int)SeqID::Main_Table]].y/2.0f;
    else 
        seq_pos_default[ids[(int)SeqID::Main_Table]].y = posY_mainTable;

    //Add distance match between tabs and maintable
    seq_pos_default[ids[(int)SeqID::Main_Table]].y += distance_match * scale;
    
    //Set pos Tabs
    seq_pos_default[ids[(int)SeqID::Front_Tab]].x = 
        seq_pos_default[ids[(int)SeqID::Main_Table]].x;
    seq_pos_default[ids[(int)SeqID::Front_Tab]].y = 
        seq_pos_default[ids[(int)SeqID::Main_Table]].y - seq_dimens[ids[(int)SeqID::Front_Tab]].y;
    for( int i = 0; i < amount_tab - 1; i++ )
    {
        seq_pos_default[ids[(int)SeqID::Behind_Tab] + i].x = 
            seq_pos_default[ids[(int)SeqID::Front_Tab]].x + seq_dimens[ids[(int)SeqID::Front_Tab]].x
            + i*seq_dimens[ids[(int)SeqID::Behind_Tab]].x;
        
        seq_pos_default[ids[(int)SeqID::Behind_Tab] + i].y =
            seq_pos_default[ids[(int)SeqID::Main_Table]].y - seq_dimens[ids[(int)SeqID::Behind_Tab]].y;
    }
    
    seq_pos_default[ids[(int)SeqID::Close_Btn]].x = 
        seq_pos_default[ids[(int)SeqID::Main_Table]].x + seq_dimens[ids[(int)SeqID::Main_Table]].x 
        - 2*seq_dimens[ids[(int)SeqID::Close_Btn]].x;
    seq_pos_default[ids[(int)SeqID::Close_Btn]].y = 
        seq_pos_default[ids[(int)SeqID::Main_Table]].y - seq_dimens[ids[(int)SeqID::Close_Btn]].y;

    //Reset to pos default for maintable
    seq_pos_default[ids[(int)SeqID::Main_Table]].y -= distance_match * scale;

    //Init entities
    for( int idEntity = 0; idEntity < (int)SeqID::Count; idEntity++ )
    {
        for( int y = ids[idEntity]; y < ids[idEntity + 1]; y++ )
        {
            entities.emplace_back(Sentities.EntityByIndex(idEntity));
            entities.back().SetEntity( seq_pos_default[y], seq_dimens[y], y, 0 );
        }
    }
}
void GUI::Tables::Update()
{
    for( auto& e : entities )
    {
        e.ProcessInput();
        if( e.GetStateEvent() == Entity::StateEvent::Clicked )
        {
            const int id = e.GetId();
            if( id >= ids[(int)SeqID::Behind_Tab] &&
                id < ids[(int)SeqID::Behind_Tab] + (amount_tab - 1) )
            {
                if( (id - ids[(int)SeqID::Behind_Tab]) >= curTab )
                    curTab = id - ids[(int)SeqID::Behind_Tab] + 1;
                else
                    curTab = id - ids[(int)SeqID::Behind_Tab];
                SetTab();
                Layer::ToggleVar_ToFirst( Layer::Var::Tab_Luggage );
            }
            else if( id == ids[(int)SeqID::Front_Tab] )
            {
                Layer::ToggleVar_ToFirst( Layer::Var::Tab_Luggage );
            }
            else if( id == ids[(int)SeqID::Close_Btn] )
            {
                return ExitTable();
            }
            break;
        }
    }

    //Process input keyboard
    if( IsKeyPressed(KEY_F2) ) return ExitTable();
    //When luggage dont want control, so Tab will control
    if( !Layer::CallVar( Layer::Var::Tab_Luggage ) )
    {
        if( IsKeyPressed( KEY_D ) || IsKeyPressed( KEY_RIGHT ) )
        {
            if( ++curTab == amount_tab ) curTab = 0;
            SetTab();
        }
        else if( IsKeyPressed( KEY_A ) || IsKeyPressed( KEY_LEFT ) )
        {
            if( --curTab == -1 ) curTab = amount_tab - 1;
            SetTab();
        }
    }

    //Display content
    if( lastTab != curTab )
    {
        switch (lastTab)
        {
        case (int)Tables::Tab::Luggage: Layer::Off( Layer::ID::Luggage ); break;
        case (int)Tables::Tab::Skill: Layer::Off( Layer::ID::Skill ); break;
        case (int)Tables::Tab::Intrinsic: Layer::Off( Layer::ID::Intrinsic ); break;
        case (int)Tables::Tab::Info: Layer::Off( Layer::ID::Info ); break;
        default: break;
        }
        OnChildContent();
        lastTab = curTab;
    }
}
void GUI::Tables::SetTab()
{
    entities[ids[(int)SeqID::Front_Tab]].SetPos(
        Vec2 {
            seq_pos_default[ids[(int)SeqID::Main_Table]].x + curTab*seq_dimens[ids[(int)SeqID::Behind_Tab]].x ,
            seq_pos_default[ids[(int)SeqID::Front_Tab]].y
        }
    );
    for( int i = 0; i < amount_tab - 1; i++ )
    {
        float posX = seq_pos_default[ids[(int)SeqID::Main_Table]].x + i*seq_dimens[ids[(int)SeqID::Behind_Tab]].x;
        if( i >= curTab ) 
            posX += seq_dimens[ids[(int)SeqID::Front_Tab]].x;
        
        entities[ids[(int)SeqID::Behind_Tab] + i].SetPos( 
            Vec2 { posX, seq_pos_default[ids[(int)SeqID::Behind_Tab]].y } 
        );
    }
}
void GUI::Tables::OnChildContent()
{
    switch (curTab)
    {
    case (int)Tables::Tab::Luggage: Layer::On( Layer::ID::Luggage ); break;
    case (int)Tables::Tab::Skill: Layer::On( Layer::ID::Skill ); break;
    case (int)Tables::Tab::Intrinsic: Layer::On( Layer::ID::Intrinsic ); break;
    case (int)Tables::Tab::Info: Layer::On( Layer::ID::Info ); break;
    default: ; break;
    }
}
void GUI::Tables::ExitTable()
{
    Layer::ToggleVar_ToFirst( Layer::Var::Tab_Luggage );
    Layer::OffAll();
    Layer::On( Layer::ID::Main );
    lastTab = -1;
}
void GUI::Tables::Draw( const FontC& font ) const
{
    for( const auto& e : entities ) e.Draw();
    for( int i = 0; i < amount_tab; i++ )
    {
        if( i == curTab )
            font.DrawTextCenter( tab_names[curTab], entities[ids[(int)SeqID::Front_Tab]].GetCenter(), WHITE );
        else if( i < curTab )
            font.DrawTextCenter( tab_names[i], entities[ids[(int)SeqID::Behind_Tab] + i].GetCenter(), WHITE_TRANS );
        else
            font.DrawTextCenter( tab_names[i], entities[ids[(int)SeqID::Behind_Tab] + (i-1)].GetCenter(), WHITE_TRANS );
    }
    font.DrawTextCenter( "x", entities[ids[(int)SeqID::Close_Btn]].GetCenter(), WHITE_TRANS );
}
const int& GUI::Tables::GetCurTab() const { return curTab; }