#include "raylibCpp.h"
#include "Cam2D.h"

Cam2D::Cam2D( const Vec2& target_in )
    :
    target( target_in )
{
    cam.target = (Vector2){ target.x, target.y};
    cam.offset = (Vector2){ float(settings::screenW/2.0f), float(settings::screenH/2.0f) };
    cam.rotation = 0.0f;
    cam.zoom = settings::camZoom;
}
void Cam2D::ProcessMovement( Vec2& newTarget, const RectF& rectMap )
{
    isBoundary = false;
    //this not work with camZoom < 1.0f;
    if( newTarget.x - float(settings::screenW / (2*settings::camZoom)) < rectMap.left)
    {
        newTarget.x = rectMap.left + float(settings::screenW / (2*settings::camZoom));
        isBoundary = true;
    }
    else if( newTarget.x + float(settings::screenW / (2*settings::camZoom)) > rectMap.right )
    {
        newTarget.x = rectMap.right - float(settings::screenW / (2*settings::camZoom));
        isBoundary = true;
    }
    if( newTarget.y - float(settings::screenH / (2*settings::camZoom)) < rectMap.top )
    {
        newTarget.y = rectMap.top + float(settings::screenH / (2*settings::camZoom));
    }
    else if( newTarget.y + float(settings::screenH / (2*settings::camZoom)) > rectMap.bottom )
    {
        newTarget.y = rectMap.bottom - float(settings::screenH / (2*settings::camZoom));
    }
    if( std::fabs( target.x - newTarget.x ) > 2.0f)
    {
        target.x += (newTarget.x - target.x) * 0.08f;
    }
    if( std::fabs( target.y - newTarget.y ) > 2.0f ) 
    {
        target.y += (newTarget.y - target.y) * 0.08f;
    }
    cam.target = (Vector2){ target.x, target.y };
}
const RectF Cam2D::GetRect() const
{
    //Could custome, not only format with screen
    return RectF::FromCenter( { target.x, target.y }, 
        float(settings::screenW / (2*settings::camZoom)), 
        float(settings::screenH / (2*settings::camZoom)) 
    );
}
const RectF Cam2D::GetRectTarget() const
{
    return RectF::FromCenter( { target.x, target.y }, 20.0f, 20.0f );
}
const Camera2D& Cam2D::GetRayCam() const
{
    return cam;
}
void Cam2D::AddLayerBackGround( 
    const std::string& path, 
    const Vec2& offset, 
    const float dRoll, const float scale, const float autoRoll )
{
    increaseRolling += dRoll;
    layers.emplace_back( BackGround{
        Surface::Retrieve(path), 
        offset, 
        increaseRolling, 
        scale, 
        autoRoll} 
    );
}
void Cam2D::UpdateBackGround( const Vec2& dir )
{
    if( isBoundary )
        for( auto& l : layers ) l.Update( target, Vec2{0.0f, 0.0f} );
    else
        for( auto& l : layers ) l.Update( target, dir );
}
void Cam2D::DrawBackGround() const
{
    for( auto& l : layers ) l.Draw( GetRect() );
}
Cam2D::BackGround::BackGround( 
        const Texture2D* surf_in, 
        const Vec2& offset_in, 
        const float dRoll_in, 
        const float scale,
        const float autoRoll_in
    )
    :
    surf( surf_in ),
    offset( offset_in ),
    dRoll( dRoll_in ),
    autoRoll( autoRoll_in )
{
    widthPerFrame = (float)surf->width*( heightPerFrame / surf->height ) + 20.0f;
    widthPerFrame *= scale;
    heightPerFrame *= scale;
}
void Cam2D::BackGround::Update( const Vec2& target, const Vec2& dir )
{
    if( dir.x > 0.0f )
    {
        rolling -= dRoll;
    }
    else if( dir.x < 0.0f )
    {
        rolling += dRoll; 
    }
    rolling += autoRoll;
    pos.x = offset.x + target.x - (float)settings::screenW / 2.0f - 10.0f + rolling;
    pos.y = offset.y + target.y - (float)settings::screenH / 2.0f - 10.0f;
}
void Cam2D::BackGround::Draw( const RectF& rectCam ) const
{
    int i = 0;
    while ( pos.x + i*widthPerFrame >= rectCam.left )
    {
        --i;
    }
    while ( pos.x + i*widthPerFrame <= rectCam.right )
    {
        RectF rectDest = RectF{
            Vec2{pos.x + i*widthPerFrame, pos.y}, 
            widthPerFrame, heightPerFrame
        };
        DrawTexturePro( 
            *surf, 
            Rectangle{ 0.0f, 0.0f, (float)surf->width, (float)surf->height }, 
            rectDest.rayRect(), { 0.0f, 0.0f }, 0.0f, WHITE );
        ++i;
    }
}
