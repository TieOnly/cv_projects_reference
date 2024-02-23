#pragma once
#include "Sprite.h"
#include "SrcEntities.h"
#include "LevelPro.h"
#include "FontC.h"
#include "Character.h"

class Enemies
{
public:
    void LoadSprites( const std::string& fileName );
    void SetSpritesLevel( const LevelPro& level, const RectF& levelRect );
    void Update( const RectF& camRect, const float dTime );
    void ProcessTargetByChar( const Vec2& charPos, const float vision );
    void ProcessAttackToChar( Character& tie, const float dTime );
    void Draw() const;

    const int GetTargetId() const;
    Sprite& SpriteById( const int id );
private:
    void InitSprites();
    void ResetBuff();
public:
    void LoadSpriteEffs( const std::string& fileName, const FontC& font );
    void UpdateEffs( const float dTime );
    void DrawEffs() const;
private:
    SrcEntities spriteAnimas;
    int iEntity = 0;

    FileIO data;
    std::vector<Sprite> sprites;
    int amountSprite = 0;
    int iSprite = 0;
    std::vector<Sprite> curSprites;
    int curAmountSprite = 0;
    std::vector<int> ettPerSprite;
    std::set<std::string> filesLoaded;
    
    std::vector<Animation> buff;
    RectF camRect = RectF{};
    RectF levelRect = RectF{};
    std::vector<RectF> enemiesBoundary;
    int targetedId = -1;
};