#pragma once
#include "SrcEntities.h"
#include "LevelPro.h"

class TilesMap
{
public:
    class Tile
    {
    public:
        enum class Type
        {
            Thick = 0,
            Behind,
            Anima,
            Solid
        };
    public:
        Tile( const Vec2& pos_in, const Texture2D* surf_in, const Vec2& srcSize, const int& id, const int& type_in );
        void IsSolid() const;
        const Type& CurType() const; 
        void Draw( const Vec2& offset_in ) const;
    private:
        Vec2 pos{};
        const Texture2D* surf = nullptr;
        RectF srcRect{};
        int id = -1;
        Type type = Type::Solid;
    };
public:
    TilesMap( const Vec2& offset_in );
    void SetLevel( const LevelPro& level_in );
    const Tile& TileAt( const int x, const int y ) const;
    const RectF& GetRect() const;
    const Vec2& GetOffset() const;
    const float& GetOffsetX() const;
    const float& GetOffsetY() const;
    const float GetTileSize() const;
    void UpdateCam( const RectF& camRect );
    void Draw() const;
private:
    void Genarate();
private:
    Vec2 offset = {0.0f, 0.0f};
    RectF dimension{};
    RectF camRect{};
    static constexpr float tileSize = settings::tileSize;
    std::vector<Tile> tiles;
    const LevelPro* level = nullptr;
//////////////////////////////////////////////////////////////////////////////////
public:
    enum class Layer
    {
        Behind = 0,
        Front,
        BehindTile
    };
public:
    void LoadEffects( const std::string& fileName );
    void SetEffLevel();
    void SetEffTheme();
    void UpdateTheme( const float dTime );
    void UpdateEff( const float dTime );
    void DrawBehind() const;
    void DrawFront() const;
    void DrawBehindTile() const;
    void DrawTheme() const;

    const std::vector<Vei2>& GetLevelEffPos() const;
    const std::vector<int>& GetLevelEffIds() const;
private:
    void ResetVariable();
private:
    SrcEntities EffActive;

    std::vector<Entity> effLevels;
    std::vector<int> index_behinds;
    std::vector<int> index_fronts;
    std::vector<int> index_behindTiles;

    std::vector<Entity> theme;
    std::vector<int> id_themes;
    int sizeOfThemes = 0;
    std::mt19937 rng = std::mt19937(std::random_device()());
};