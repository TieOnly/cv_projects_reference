#pragma once

class LevelPro
{
public:
    void LoadLevel( const std::string& path );
    const int& GetWidth() const;
    const int& GetHeight() const;
    const Vec2& GetTileSize() const;
    const std::vector<int>& GetTypeTiles() const;
    const std::vector<int>& GetIdTiles() const;
    const Texture2D* GetSurface() const;
public:
    const std::vector<int>& GetEffIds() const;
    const std::vector<Vei2>& GetEffPos() const;
    const Vec2& GetEffDimension() const;
    const std::vector<int>& GetEffLayer() const;
    const std::vector<int>& GetEffType() const;
    const std::vector<int>& GetThemeIds() const;
    const int& GetSizeOfTheme() const;
public:
    const int GetEnemyAmount() const;
    const std::vector<int>& GetEnemyIds() const;
    const std::vector<Vei2>& GetEnemyPos() const;
    const std::vector<Vec2>& GetEnemyDimens() const;
    const Vei2& GetEnemyBoundary() const;
private:
    void WriteData();
    Vec2 CreateVecByData( const std::string& data, const char delim );
private:
    int width = 0;
    int height = 0;
    std::vector<int> typeTiles;
    std::vector<int> idTiles;
    const Texture2D* tileSheet = nullptr;
    Vec2 tileSize{};
    FileIO data;
    bool isBuffEmpty = true;

    std::vector<int> effIds;
    std::vector<Vei2> effPos;
    Vec2 effDimension;
    std::vector<int> effLayers;
    std::vector<int> effTypes;
    std::vector<int> themeIds;
    int themSize;

    int enemyAmount;
    std::vector<int> enemiesIds;
    std::vector<Vei2> enemiesPos;
    std::vector<Vec2> enemiesDimens;
    Vei2 enemiesBoundary;    
};