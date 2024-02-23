#include "Game.h"

Game::Game(int fps)
    :
    rng( std::random_device()() ),
    tileMap( Vec2{ 0.0f, 0.0f }),
    tie( Vec2{ 1800.0f, 0.0f } ),
    cam( tie.GetPos() )
{
    SetTargetFPS(fps);
    targetCam = tie.GetPos();

    cam.AddLayerBackGround("../assets/img/clouds_bg.png", {0.0f, 0.0f}, 2.0f);
    cam.AddLayerBackGround("../assets/img/glacial_mountains.png", {0.0f, 0.0f}, 1.0f, 1.0f);
    cam.AddLayerBackGround("../assets/img/cloud_lonely.png", {0.0f, 50.0f}, 1.0f, 0.5f, 0.5f);
        
    font.LoadSurface( "../assets/font/fontWhite.png" );

    level.LoadLevel( "../assets/data/dataLevel.txt" );
    
    SrcItems::LoadSrcItems( "../assets/data/items.txt" );
    VarTimer::Init();

    tileMap.SetLevel( level );
    tileMap.LoadEffects( "../assets/data/eff_level.txt" );
    tie.LoadEffects( "../assets/data/char/effects_0.txt", font );
    
    tileMap.SetEffLevel();
    tileMap.SetEffTheme();

    enemies.LoadSprites( "../assets/data/enemies/enemies_0.txt" );
    enemies.LoadSprites( "../assets/data/enemies/enemies_1.txt" );
    enemies.LoadSpriteEffs( "../assets/data/enemies/effects_0.txt", font );
    enemies.SetSpritesLevel( level, tileMap.GetRect() );
    
    GuiCommon::LoadSrcEntity( "../assets/data/ui/gui_common/entity_0.txt" );
    GuiCommon::LoadVariable( "../assets/data/ui/gui_common/data_var_0.txt" );
    NoteAction::Init( font );
    gui.LoadAsset( tie );
    gui_table_content.LoadAsset( tie, font, gui.GetPosMainTable() );

    std::cout<<"Tiles: "<<sizeof( tileMap )<<std::endl;
    std::cout<<"Char: "<<sizeof( tie )<<std::endl;
    std::cout<<"Level: "<<sizeof( level )<<std::endl;
    std::cout<<"Enemies: "<<sizeof( enemies )<<std::endl;
    std::cout<<"GUI: "<<sizeof( gui )<<std::endl;

    std::cout<<"Item["<< SrcItems::ItemByIndex(3).GetName() << "]: "
        << (int)SrcItems::ItemByIndex(3).GetEffects()[0].first
        << ":"
        << (int)SrcItems::ItemByIndex(3).GetEffects()[0].second
    << std::endl;
}
Game::~Game() noexcept
{
    assert(GetWindowHandle());
    CloseWindow();
}
bool Game::GameShouldClose() const
{
    return WindowShouldClose();
}
void Game::Tick() 
{
    BeginDrawing();
    Update();
    Draw();
    EndDrawing();
}
void Game::Update()
{
    //Start Init
    float dTime = ft.Mark();
    if( dTime > 0.02f ) dTime = 1.0f / 60.0f;

    const Vec2& charPos = tie.GetPos();
    const Vec2& charVel = tie.GetVel();
    targetCam.y = charPos.y - 40.0f;
    if( charVel.x > 0.0f )
        targetCam.x = charPos.x + settings::vision;
    else if( charVel.x < 0.0f )
        targetCam.x = charPos.x - settings::vision;

    cam.ProcessMovement( targetCam, tileMap.GetRect() );
    cam.UpdateBackGround( tie.GetVel() );
    camRect = cam.GetRect();

    tileMap.UpdateCam( camRect );
    tileMap.UpdateTheme( dTime );
    tileMap.UpdateEff( dTime );

    enemies.Update( camRect, dTime );
    enemies.ProcessTargetByChar( tie.GetPos(), tie.GetVision() );
    enemies.ProcessAttackToChar( tie, dTime );

    tie.ProcessInput( Layer::BeUpdate( Layer::ID::Main ) );
    if( enemies.GetTargetId() != -1 ) 
        tie.ProcessActiveSprite( enemies.SpriteById(enemies.GetTargetId()), camRect );
    else tie.IgnoreActive();
    tie.SetDirection();
    tie.ProcessCollision( tileMap, dTime );
    tie.Update( dTime );
    tie.UpdateEff( dTime );

    gui.Update( tie );
    gui_table_content.Update( tie );
    NoteAction::Update( dTime );

    VarTimer::Timer_Update( dTime );
}
void Game::Draw()
{
    ClearBackground(SKYBLUE);
    BeginMode2D( cam.GetRayCam() );
    
    cam.DrawBackGround();

    tileMap.DrawBehindTile();
    tileMap.Draw();
    tileMap.DrawBehind();
    
    enemies.Draw();
    tie.DrawEff();
    tie.Draw();
    tileMap.DrawFront();
    tileMap.DrawTheme();
    // rayCpp::DrawRectThin( tie.GetPosBox(), GREEN );
    rayCpp::DrawCircleThin( tie.GetPos(), tie.GetRange(), SKYBLUE );
    rayCpp::DrawCircleThin( tie.GetPos(), tie.GetVision(), GREEN );
    // rayCpp::DrawRectThin( cam.GetRectTarget(), YELLOW );
    
    EndMode2D();

    gui.Draw();
    gui_table_content.Draw();
    NoteAction::Draw();
}
