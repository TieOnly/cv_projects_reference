#include "../include/App.h"

App::App() : TieSys()
{
    snek.Init( TieSys );
    teris.Init();
    TieSys.LED.Bright(0);
}
void App::Tick()
{
    App::Update();
    App::Draw();
}
void App::Update()
{
    TieSys.Update();
    if( TieSys.GetMode() == TieSystem::Mode::Main )
    {
        TieSys.LED.DrawEff1( 20 );
    }
    else if( TieSys.GetMode() == TieSystem::Mode::Snek )
    {
        snek.Update( TieSys );     
    }
    else if( TieSys.GetMode() == TieSystem::Mode::Teris )
    {
        teris.Update( TieSys, board );        
    }
    else if( TieSys.GetMode() == TieSystem::Mode::Scroll )
    {
        TieSys.LED.DrawScroll( TieSys.GetPText() );
        TieSys.PlayTheme();
    }
}
void App::Draw()
{
    if( TieSys.GetMode() == TieSystem::Mode::Teris )
    {
        board.Draw( TieSys );
    }
    if( TieSys.GetMode() == TieSystem::Mode::Snek )
        snek.Draw( TieSys ); 
    
    TieSys.LED.Draw();
}
