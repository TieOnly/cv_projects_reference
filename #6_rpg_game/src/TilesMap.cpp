#include "raylibCpp.h"
#include "TilesMap.h"

TilesMap::TilesMap( const Vec2& offset_in )
    :
    offset( offset_in )
{}
void TilesMap::SetLevel( const LevelPro& level_in )
{
    level = &level_in;
    Genarate();
}
void TilesMap::Genarate()
{
    tiles.reserve( (level->GetWidth()) * (level->GetHeight()) );
    int i = 0;
    for(int iY = 0; iY < level->GetHeight(); iY++)
    {
        for(int iX = 0; iX < level->GetWidth(); iX++, i++)
        {
            const Tile& tile = Tile( 
                Vec2{ (float)iX * tileSize , (float)iY * tileSize }, 
                level->GetSurface(),
                level->GetTileSize(),
                level->GetIdTiles()[i],
                level->GetTypeTiles()[i]    
            );
            tiles.push_back( tile );
        }
    }
    dimension = RectF{ offset, (float)level->GetWidth() * tileSize, (float)level->GetHeight() * tileSize };
}
const TilesMap::Tile& TilesMap::TileAt( const int x, const int y ) const 
    { return tiles[x + y*level->GetWidth()]; }
const Vec2& TilesMap::GetOffset() const { return offset; }
const float& TilesMap::GetOffsetX() const { return offset.x; }
const float& TilesMap::GetOffsetY() const { return offset.y; }
const float TilesMap::GetTileSize() const { return tileSize; }
const RectF& TilesMap::GetRect() const { return dimension; }
void TilesMap::UpdateCam( const RectF& _camRect )
    { camRect = _camRect; }
void TilesMap::Draw() const
{
    for(int iY = 0; iY < level->GetHeight(); iY++)
    {
        //boundY
        if((iY + 2) * tileSize + offset.y < camRect.top || (iY - 1) * tileSize + offset.y >= camRect.bottom) continue;
        for(int iX = 0; iX < level->GetWidth(); iX++)
        {
            //boundX
            if((iX + 2) * tileSize + offset.x < camRect.left || (iX - 1) * tileSize + offset.x >= camRect.right) continue;
            TileAt( iX, iY ).Draw( offset );
        }
    }
}

TilesMap::Tile::Tile( const Vec2& pos_in, const Texture2D* surf_in, const Vec2& srcSize, const int& id_in, const int& type_in )
    :
    pos( pos_in ),
    surf( surf_in ),
    id( id_in )
{
    if( id != -1 )
    {
        const int xPos = id % int(surf->width / srcSize.x); 
        const int yPos = id / int(surf->width / srcSize.x); 
        srcRect = RectF{ Vec2{ xPos * srcSize.x, yPos * srcSize.y }, srcSize.x, srcSize.y };
    }
    else surf = nullptr;
    switch (type_in)
    {
    case 0:
        type = Type::Thick;
        break;
    case 1:
        type = Type::Behind;
        break;
    case 2:
        type = Type::Anima;
        break;
    case 3:
        type = Type::Solid;
        break;
    default:
        break;
    }
}
const TilesMap::Tile::Type& TilesMap::Tile::CurType() const { return type; }
void TilesMap::Tile::Draw(const Vec2& offset_in) const
{
    if( surf != nullptr)
    {
        rayCpp::DrawSprite(*surf, pos + offset_in, srcRect, tileSize, tileSize);
    }
}
///////////////////////////////////////////////////////////////////////////////////////
void TilesMap::LoadEffects( const std::string& fileName )
{
    EffActive.LoadSrcEntities( fileName );
}
void TilesMap::UpdateEff( const float dTime )
{
    for ( auto& e : effLevels ) e.Update( dTime );
}
void TilesMap::UpdateTheme( const float dTime )
{
    std::uniform_real_distribution<float> xDist{ camRect.left, camRect.right};
    std::uniform_real_distribution<float> vxDist{ 0, 2 };
    std::uniform_real_distribution<float> vyDist{ 0.5, 1 };
    for (int i = 0; i < (int)theme.size(); i++)
    {
        Entity& e = theme[i];
        if( e.GetVel() == 0.0f )
        {
            //Reset and random
            e.SetPos( {xDist(rng), camRect.top - e.GetDimens().y + 1.0f} );
            e.SetVel( {vxDist(rng), vyDist(rng)} );
        }
        else
        {
            //Update pos with curVel*dTime*speed And reVel
            e.SetPos( e.GetPos() + e.GetVel()*dTime*50.0f );
            if( e.GetRect().top > camRect.bottom )
            {
                e.SetVel( {0.0f, 0.0f} );
            }
        }
        e.Update( dTime );
    }
}
void TilesMap::SetEffTheme()
{
    if( (int)theme.size() <= sizeOfThemes )
    {
        for( size_t i = 0u; i < id_themes.size(); i++ )
        {
            const int index = EffActive.IndexById(id_themes[i]);
            if( index != -1 )
            {
                for(int k = 0; k < sizeOfThemes; k++)
                {
                    theme.emplace_back(EffActive.EntityByIndex(index));
                    theme[k].SetVel( {0.0f, 0.0f} );
                    theme[k].SetDimens( {48.0f, 48.0f} );
                }
            }
        }
    }
}
void TilesMap::SetEffLevel()
{
    ResetVariable();
    id_themes = level->GetThemeIds();
    sizeOfThemes = level->GetSizeOfTheme();
    const int amount = (int)level->GetEffIds().size();
    for(int i = 0; i < amount; i++)
    {
        const int index = EffActive.IndexById(level->GetEffIds()[i]);
        if( index != -1 )
        {
            //Create entity in this level
            effLevels.emplace_back( EffActive.EntityByIndex(index) );
            effLevels[i].SetEntity(
                Vec2{level->GetEffPos()[i] * settings::tileSize}, 
                level->GetEffDimension(), 
                level->GetEffIds()[i], 
                level->GetEffType()[i]
            );
            //Set id draw
            switch (level->GetEffLayer()[i])
            {
            case 0: index_behinds.push_back(i); break;
            case 1: index_fronts.push_back(i); break;
            case 2: index_behindTiles.push_back(i); break;
            default: break;
            }
        }
    }
    // std::cout<<<std::endl;
}
void TilesMap::ResetVariable()
{
    for (auto& e : effLevels) e.Destroy();
    effLevels.clear();
    index_behinds.clear();
    index_fronts.clear();
    index_behindTiles.clear();

    for (auto& t : theme) t.Destroy();
    theme.clear();
    id_themes.clear();
}
void TilesMap::DrawBehind() const
{
    for( const auto& i : index_behinds )
    {
        if( effLevels[i].GetRect().IsOverLapping( camRect ) ) effLevels[i].Draw();
    }
}
void TilesMap::DrawFront() const
{
    for( const auto& i : index_fronts )
    {
        if( effLevels[i].GetRect().IsOverLapping( camRect ) ) effLevels[i].Draw();
    }
}
void TilesMap::DrawBehindTile() const
{
    for( const auto& i : index_behindTiles )
    {
        if( effLevels[i].GetRect().IsOverLapping( camRect ) ) effLevels[i].Draw();
    }
}
void TilesMap::DrawTheme() const
{
    for( const auto& t : theme )
    {
        if( t.GetRect().IsOverLapping( camRect) ) t.Draw();
    }
}
const std::vector<Vei2>& TilesMap::GetLevelEffPos() const { return level->GetEffPos(); }   
const std::vector<int>& TilesMap::GetLevelEffIds() const { return level->GetEffIds(); }  
