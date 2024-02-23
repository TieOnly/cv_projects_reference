#include "raylibCpp.h"
#include "Animation.h"

Animation::Animation(){};
Animation::Animation(float x, float y, float width, float height, int count, const Texture2D* sprite, float holdTime)
    :
    sprite( sprite ),
    holdTime( holdTime )
{
    sizeFrames = count;
    frames.clear();
    for(int i = 0; i < count; i++)
    {
        frames.emplace_back( x + i * width, x + (i + 1) * width, y, y + height );
    }
    dimens = Vec2{ width, height };
}
void Animation::Destroy()
{
    sprite = nullptr;
    delete sprite;
}
void Animation::Update( float dTime )
{
    curFrameTime += dTime;
    isDonePeriod = false;
    while( curFrameTime >= holdTime )
    {
        if( ++iCurFrame >= sizeFrames )
        {
            iCurFrame = 0;
            isDonePeriod = true;
        }
        curFrameTime -= holdTime;
    }
}
void Animation::Update( float dTime, const Sound& sound, const int FramePerSound )
{
    curFrameTime += dTime;
    isDonePeriod = false;
    while( curFrameTime >= holdTime )
    {
        if( iCurFrame % FramePerSound == 0) PlaySound(sound);
        if( ++iCurFrame >= sizeFrames )
        {
            iCurFrame = 0;
            isDonePeriod = true;
        }
        curFrameTime -= holdTime;
    }
}
void Animation::ForceBeginPeriod()
{
    isDonePeriod = false;
    curFrameTime = 0.0f;
    iCurFrame = 0;
}
void Animation::ForceDonePeriod()
{
    isDonePeriod = true;
    curFrameTime = 0.0f;
    iCurFrame = 0;
}
void Animation::Draw( const Vec2& pos ) const
{
    rayCpp::DrawSprite( *sprite, pos, frames[iCurFrame] );
}
void Animation::Draw( const Vec2& pos, const Vec2& dimension ) const
{
    rayCpp::DrawSprite( *sprite, pos, frames[iCurFrame], dimension.x, dimension.y);
}
void Animation::Draw( const Vec2& pos, const Vec2& dimension, Color color ) const
{
    rayCpp::DrawSprite( *sprite, pos, frames[iCurFrame], dimension.x, dimension.y, color);
}
void Animation::Draw( const Vec2& pos, const Vec2& dimension, Color color, const RectF& boundary ) const
{
    rayCpp::DrawSpriteInside( 
        *sprite, pos, frames[iCurFrame], RectF{ pos, dimension.x, dimension.y }, boundary );
}
bool Animation::IsDonePeriod() const
{
    return isDonePeriod;
}
const Texture2D* Animation::GetSurf() const { return sprite; }
const Vec2& Animation::GetDimens() const { return dimens; }
