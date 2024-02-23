#pragma once
#include "Animation.h"

class Entity
{
public:
    enum class Type
    {
        Normal = 0,
        Transparent,
        Disappear
    };
public:
    Entity();
    Entity( Animation animation, const int id );
    void Destroy();
    void SetAnimation( 
        const Texture2D* surf, const Vec2& srcPos, const Vec2& dimension, 
        const int frame, const float holdTime 
    );
    void SetAnimation( const Animation& animation);
    void SetEntity( 
        const Vec2& pos, const Vec2& dimension, 
        const int id, const Entity::Type type = Entity::Type::Normal
    ); 
    void SetEntity( 
        const Vec2& pos, const Vec2& dimension, 
        const int id, const int type = 0
    );
    void SetPos( const Vec2& pos );
    void SetVel( const Vec2& vel );
    void SetDimens( const Vec2& dimension );
    void SetType( const Type& type );

    const int GetId() const;
    const Animation& GetAnimation() const;
    const RectF GetRect() const;
    const Vec2& GetPos() const;
    const Vec2& GetSrcDimens() const;
    const Vec2& GetDimens() const;
    const Vec2 GetCenter() const;
    const Vec2& GetVel() const;
    const Type& GetType() const;
    const bool IsDonePeriod() const;
    void Update( const float dTime );
    void Draw() const;
    void DrawHasColor( Color c ) const;
    void DrawInsideOf( const RectF& boundary ) const;

protected:
    Animation animation{};
    Vec2 pos = {0.0f, 0.0f};
    Vec2 dimension = { 0.0f, 0.0f };
    Type type = Type::Normal;
    int id = -1;
    Vec2 vel = {0.0f, 0.0f};
///////////////////////////////////////////////////////////////////////////
//Event
public:
    enum class StateEvent
    {
        Normal,
        Disable,
        Clicked,
        Released
    };
    void ProcessInput();
    void ProcessInput( const RectF& area_allow_process );
    const StateEvent& GetStateEvent() const;
    const bool IsValidPosBegin() const;
    const Vec2& GetPosBegin() const;
private:
    StateEvent curSEvent = StateEvent::Normal;
    Vec2 mousePos{};
    bool isNew = false;
    Vec2 pos_begin = {-1.0f, 0.0f};
};