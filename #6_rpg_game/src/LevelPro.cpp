#include "raylibCpp.h"
#include "LevelPro.h"

void LevelPro::LoadLevel( const std::string& path )
{
    if( isBuffEmpty )
    {
        if( data.LoadFromFile( path ) )
        {
            WriteData();
            isBuffEmpty = false;
        }
        else std::cout << "Data: Faile load file from: "<< path << std::endl;
    }
}

void LevelPro::WriteData()
{
    //Reset Variable

    std::stringstream buff( data.GetBuff() );
    std::string token = "";
    while (std::getline( buff, token ))
    {
        if( token == "[Width-Height]" )
        {
            std::getline( buff, token );
            std::stringstream ss( token );
            std::string s;
            std::getline( ss, s, ' ' );
            width = std::stoi( s );
            std::getline( ss, s, ' ' );
            height = std::stoi( s );
        }
        else if( token == "[Type]" )
        {
            while ( std::getline( buff, token ) && (token != "") )
            {
                std::stringstream ss( token );
                std::string s;
                while (std::getline(ss, s, ' '))
                {
                    typeTiles.push_back(std::stoi( s ));
                }
            }            
        }
        else if( token == "[ID]" )
        {
            while ( std::getline( buff, token ) && (token != "") )
            {
                std::stringstream ss( token );
                std::string s;
                while (std::getline(ss, s, ' '))
                {
                    idTiles.push_back(std::stoi( s ));
                }
            }  
        }
        else if( token == "[TileSize]" )
        {
            std::getline( buff, token );
            std::stringstream ss( token );
            std::string s;
            std::getline( ss, s, ' ' );
            tileSize.x = std::stof( s );
            std::getline( ss, s, ' ' );
            tileSize.y = std::stof( s );
        }
        else if( token == "[FileSheet]" )
        {
            std::getline( buff, token );
            tileSheet = Surface::Retrieve( "../assets/tile/" + token );
        }
        else if( token == "[InitEntity]" )
        {
            while (std::getline( buff, token ) && (token != "#"))
            {
                if( token == "{id}" )
                {
                    while ( std::getline( buff, token, ' ' ) && (token != "") )
                    {
                        effIds.push_back(std::stoi(token));
                    } 
                }
                else if( token == "{pos}" )
                {
                    while ( std::getline( buff, token, '|' ) && (token != "") )
                    {
                        Vei2 s_pos = Vei2(CreateVecByData(token, ' '));
                        effPos.push_back(s_pos);
                    }
                }
                else if( token == "{dimension}" )
                {
                    std::getline( buff, token );
                    effDimension = CreateVecByData(token, ' ');
                }
                else if( token == "{layers}" )
                {
                    while ( std::getline( buff, token, ' ' ) && (token != "") )
                    {
                        effLayers.push_back(std::stoi(token));
                    } 
                }
                else if( token == "{types}" )
                {
                    while ( std::getline( buff, token, ' ' ) && (token != "") )
                    {
                        effTypes.push_back(std::stoi(token));
                    } 
                }
                else if( token == "{ThemeIds}" )
                {
                    while ( std::getline( buff, token, ' ' ) && (token != "") )
                    {
                        themeIds.push_back(std::stoi(token));
                    }
                }
                else if( token == "{ThemeSize}" )
                {
                    std::getline( buff, token );
                    themSize = std::stoi(token);
                }
            }
        }
        else if( token == "[Enemies]" )
        {
            while (std::getline( buff, token ) && (token != "#"))
            {
                if( token == "{amount}" )
                {
                    std::getline(buff, token);
                    enemyAmount = std::stoi(token);
                }
                else if( token == "{ids}" )
                {
                    while ( std::getline( buff, token, ' ' ) && (token != "") )
                    {
                        enemiesIds.push_back(std::stoi(token));
                    } 
                }
                else if( token == "{pos}" )
                {
                    while ( std::getline( buff, token, '|' ) && (token != "") )
                    {
                        Vei2 s_pos = Vei2(CreateVecByData(token, ' '));
                        enemiesPos.push_back(s_pos);
                    }
                }
                else if( token == "{dimensions}" )
                {
                    while ( std::getline( buff, token, ' ' ) && (token != "") )
                    {
                        if( token == "l" ) enemiesDimens.push_back(Vec2{64.0f, 64.0f});
                        else if( token == "m" ) enemiesDimens.push_back(Vec2{50.0f, 50.0f});
                        else enemiesDimens.push_back(Vec2{32.0f, 32.0f});
                    }
                }
                else if( token == "{boundary}" )
                {
                    std::getline(buff, token);
                    enemiesBoundary = Vei2(CreateVecByData(token, ' '));
                }
            }
        }
    }
}
Vec2 LevelPro::CreateVecByData( const std::string& data, const char delim )
{
    Vec2 v;
    std::stringstream ss( data );
    std::string s;
    std::getline( ss, s, ' ' );
    v.x = std::stof( s );
    std::getline( ss, s, ' ' );
    v.y = std::stof( s );
    return v;
}
/////////////////////////////////////////////////////////////////////////////////////////////
const int& LevelPro::GetWidth() const { return width; }
const int& LevelPro::GetHeight() const { return height; }
const Vec2& LevelPro::GetTileSize() const { return tileSize; }
const std::vector<int>& LevelPro::GetTypeTiles() const { return typeTiles; }
const std::vector<int>& LevelPro::GetIdTiles() const { return idTiles; }
const Texture2D* LevelPro::GetSurface() const { return tileSheet; }
/////////////////////////////////////////////////////////////////////////////////////////////////
const std::vector<int>& LevelPro::GetEffIds() const { return effIds; }
const std::vector<Vei2>& LevelPro::GetEffPos() const { return effPos; }
const Vec2& LevelPro::GetEffDimension() const { return effDimension; }
const std::vector<int>& LevelPro::GetEffLayer() const { return effLayers; }
const std::vector<int>& LevelPro::GetEffType() const { return effTypes; }
const std::vector<int>& LevelPro::GetThemeIds() const { return themeIds; }
const int& LevelPro::GetSizeOfTheme() const { return themSize; }
/////////////////////////////////////////////////////////////////////////////////////////////////
const int LevelPro::GetEnemyAmount() const { return enemyAmount; }
const std::vector<int>& LevelPro::GetEnemyIds() const { return enemiesIds; }
const std::vector<Vei2>& LevelPro::GetEnemyPos() const { return enemiesPos; }
const std::vector<Vec2>& LevelPro::GetEnemyDimens() const { return enemiesDimens; }
const Vei2& LevelPro::GetEnemyBoundary() const { return enemiesBoundary; }
