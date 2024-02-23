#include "raylibCpp.h"
#include "Entity.h"

Entity::Entity(){};
Entity::Entity( Animation _animation, const int _id )
    :
    animation( _animation ),
    id( _id )
{}
void Entity::Destroy()
{
    animation.Destroy();
}
void Entity::SetAnimation( 
    const Texture2D* surf, const Vec2& srcPos, const Vec2& _dimension,
    const int frame, const float holdTime 
)
{
    animation = Animation{ srcPos.x, srcPos.y, _dimension.x, _dimension.y, frame, surf, holdTime };
}
void Entity::SetAnimation( const Animation& _animation) 
{
    animation = _animation;
}
void Entity::SetEntity( 
    const Vec2& _pos, const Vec2& _dimension, 
    const int _id, const Entity::Type _type )
{
    pos = _pos;
    dimension = _dimension;
    id = _id;
    type = _type;
}
void Entity::SetEntity( 
    const Vec2& _pos, const Vec2& _dimension, 
    const int _id, const int _type )
{
    pos = _pos;
    dimension = _dimension;
    id = _id;
    switch (_type)
    {
    case 0: type = Entity::Type::Normal;break;
    case 1: type = Entity::Type::Transparent;break;
    default: break;
    }
}
///////////////////////////////////////////////////////////////////////////////////////
void Entity::SetPos( const Vec2& _pos ) { pos = _pos; }
void Entity::SetVel( const Vec2& _vel ) { vel = _vel; }
void Entity::SetDimens( const Vec2& _dimension ) { dimension = _dimension; }
void Entity::SetType( const Entity::Type& _type ) { type = _type; }
///////////////////////////////////////////////////////////////////////////////////////
const int Entity::GetId() const { return id; }
const Animation& Entity::GetAnimation() const { return animation; }
const RectF Entity::GetRect() const { return RectF{pos, dimension.x, dimension.y}; }
const Vec2& Entity::GetPos() const { return pos; }
const Vec2& Entity::GetSrcDimens() const { return animation.GetDimens(); }
const Vec2& Entity::GetDimens() const { return dimension; }
const Vec2 Entity::GetCenter() const { return  pos + (dimension/2); }
const Vec2& Entity::GetVel() const { return vel; }
const Entity::Type& Entity::GetType() const { return type; }
const bool Entity::IsDonePeriod() const { return animation.IsDonePeriod(); }
void Entity::Update( const float dTime )
{
    animation.Update( dTime );
}
void Entity::Draw() const
{
    Color c{};
    switch (type)
    {
        case Entity::Type::Normal: c = WHITE;break;
        case Entity::Type::Transparent: c = WHITE_TRANS;break;
        case Entity::Type::Disappear: c = {0,0,0,0};break;
        default: break;
    }
    animation.Draw( pos, dimension, c );
}
void Entity::DrawHasColor( Color c ) const
{
    animation.Draw( pos, dimension, c );
}
void Entity::DrawInsideOf( const RectF& boundary ) const
{
    Color c;
    switch (type)
    {
        case Entity::Type::Normal: c = WHITE;break;
        case Entity::Type::Transparent: c = WHITE_TRANS;break;
        default: c = {0,0,0,0};break;
    }
    animation.Draw( pos, dimension, c, boundary );
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Event
void Entity::ProcessInput()
{
    if( curSEvent != StateEvent::Disable )
    {
        mousePos = rayCpp::GetMousePos();
        if( GetRect().IsCover( mousePos ) )
        {
            if( isNew || IsMouseButtonUp( MOUSE_BUTTON_LEFT ) )
            {
                isNew = true;
                curSEvent = StateEvent::Normal;
                if( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) )
                {
                    curSEvent = StateEvent::Released;
                    if( !IsValidPosBegin() ) pos_begin = mousePos;
                }
                else if( IsMouseButtonUp( MOUSE_BUTTON_LEFT ) )
                {
                    if( IsValidPosBegin() ) 
                    {
                        isNew = false;
                        pos_begin.x = -1.0f;
                        curSEvent = StateEvent::Clicked;
                    }
                }
            }
        }
        else if( !IsMouseButtonDown( MOUSE_BUTTON_LEFT ) )
        {
            isNew = false;
            pos_begin.x = -1.0f;
            curSEvent = StateEvent::Normal; 
        }
    }
}
void Entity::ProcessInput( const RectF& area_allow_process )
{
    if( curSEvent != StateEvent::Disable )
    {
        mousePos = rayCpp::GetMousePos();
        if( area_allow_process.IsCover( mousePos ) )
        {
            if( isNew || IsMouseButtonUp( MOUSE_BUTTON_LEFT ) )
            {
                isNew = true;
                curSEvent = StateEvent::Normal;
                if( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) )
                {
                    curSEvent = StateEvent::Released;
                    if( !IsValidPosBegin() ) pos_begin = mousePos;
                }
                else if( IsMouseButtonUp( MOUSE_BUTTON_LEFT ) )
                {
                    if( IsValidPosBegin() ) 
                    {
                        isNew = false;
                        pos_begin.x = -1.0f;
                        curSEvent = StateEvent::Clicked;
                    }
                }
            }
        }
        else if( !IsMouseButtonDown( MOUSE_BUTTON_LEFT ) )
        {
            isNew = false;
            pos_begin.x = -1.0f;
            curSEvent = StateEvent::Normal; 
        }
    }
}
const Entity::StateEvent& Entity::GetStateEvent() const { return curSEvent; }
const bool Entity::IsValidPosBegin() const { return pos_begin.x != -1.0f; }
const Vec2& Entity::GetPosBegin() const { return pos_begin; }