#pragma once
#include "SrcEntities.h"
#include "FontC.h"

class Sprite
{
public:
    enum class Sequence
    {
        Idle = 0,
        Walking,
        Hurt,
        Dead
    };
public:
    Sprite() = default;
    Sprite( const int id, const std::vector<Animation>& animations );
    void SetSprite( const Vec2& dimens, const Vei2& pos, const Vei2& boundary );
    void SetDimens( const Vec2& dimens );
    void SetCurSequence( const Sequence& s );
    void SetDirX( const int dir );

    void Update( const float dTime, const RectF& levelRect );
    void Draw() const;
    void DrawHPLine() const;

    const int GetIds() const;
    const std::vector<Animation>& GetAnimations() const;
    const Vec2& GetPos() const;
    const Sequence& GetState() const;
    const Vec2 GetCenter() const;
    const RectF GetPosBox() const;
    const RectF& GetBoundary() const;

private:
    int id = -1;
    std::vector<Animation> animations;
    int sizeAnimated = 0;
    Sequence curSequence = Sequence::Idle;
    int iSequence = 0;
    bool isDisappear = false;
    bool isSliding = false;
    int dirSilde = 1;

    Vec2 pos{};
    Vec2 posSpawn{};
    RectF boundary{};
    Vec2 dimension = { 48.0f, 48.0f };
    Vei2 dir = { 0, 0 };
    Vec2 vel = { 0.0f, 0.0f };
    
    float timerIdle = 0.0f;
    float timerWalking = 0.0f;
    float timerDead = 0.0f;
    float timerAttack = 0.0f;
    float timerSliding = 0.0f;
    float duraIdle = 3.0f;
    float duraWalking = 3.0f;
    float duraDead = 10.0f;
    float duraAttack = 1.0f;
    float duraSliding = 1.0f;

    float hpLineW = 0.0f;
    float hpLineH = 0.0f;

    std::mt19937 rng;
    std::uniform_int_distribution<int> dirDist{ 0, 1 };
    std::uniform_real_distribution<float> duraDist{ 2.0f, 6.0f };
    std::uniform_real_distribution<float> duraBootsAttackDist{ 1.0f, 3.0f };
    std::uniform_real_distribution<float> duraSlidingDist{ 0.1f, 0.6f };
public:
    void InjureCurHp( const int amount );
    bool AttackToChar( const float& dTime, const float posXChar );
    const int CalculateDamage();
    bool isAttack = false;
    bool isEnmity = false;
private:
    float speed = 40.0f;
    float speedFall = 400.0f;
    float gravity = 18.0f;
    int max_hp = 400;
    int curHp = max_hp;
    int min_damage = 7;
    int max_damage = 20;
////////////////////////////////////////////////////////////////////////
public:
    enum class StateEff
    {
        Spawn = 0,
        BleedLeft,
        BleedRight,
        Count
    };
public:
    void LoadEffects( const std::string& fileName, const FontC& font );
    void UpdateEff( const float dTime );
    void UpdateEffText( const float dTime );
    void DrawEff() const;
    void DrawEffText() const;
private:
    void SetEffByIndex( Entity& e, const int index );
private:
    SrcEntities EffActive;
    FontC font;

    std::vector<Entity> effSprites;
    int iEffSprite = 0;
    bool tickEffActives[(int)StateEff::Count] = {false};
    bool hasTickEff = true;
    bool hasEffActiving = false;
};