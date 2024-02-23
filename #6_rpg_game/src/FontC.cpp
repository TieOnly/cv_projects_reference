#include "raylibCpp.h"
#include "FontC.h"
#include "assert.h"

void FontC::LoadSurface( const std::string& fileName )
{
    surf = Surface::Retrieve( fileName );
    glyphWidth = surf->width / colums;
    glyphHeight = surf->height / rows;
    assert( glyphWidth * colums == surf->width );
    assert( glyphHeight * rows == surf->height );
}
RectF FontC::MapGlyphRectF( char c ) const
{
    assert( c >= fisrtChar && c <= lastChar );

    const int glyphIndex = c - ' ';

    const int xGlyph = glyphIndex % colums; 
    const int yGlyph = glyphIndex / colums;

    return RectF{ {float(xGlyph * glyphWidth), float(yGlyph * glyphHeight)}, (float)glyphWidth, (float)glyphHeight }; 
}
void FontC::DrawText( const std::string& text, const Vec2& pos, Color color, const Vec2& measure ) const
{
    Vec2 curPos = pos;
    for( char c : text )
    {
        if(c == '\n')
        {
            curPos.x = pos.x;
            curPos.y += measure.y;
            continue;
        }
        else if(c >= fisrtChar + 1 && c <= lastChar)
        {
            rayCpp::DrawSprite( *surf, curPos, MapGlyphRectF( c ), measure.x, measure.y, color );
        }
        curPos.x += measure.x;
    }
}
void FontC::DrawText( const std::string& text, const Vec2& pos, Color color ) const
{
    DrawText( text, pos, color, Vec2{(float)glyphWidth, (float)glyphHeight} );
}
void FontC::DrawTextInBound( 
    const std::string& text, 
    const Vec2& pos, 
    const RectF& bound, 
    Color color,
    const Vec2& measure ) const
{
    Vec2 curPos = pos;
    for( char c : text )
    {
        if(c == '\n')
        {
            curPos.x = pos.x;
            curPos.y += measure.y;
            continue;
        }
        else if(c >= fisrtChar + 1 && c <= lastChar)
        {
            rayCpp::DrawSpriteInside(
                *surf, curPos, MapGlyphRectF( c ), RectF{curPos, measure.x, measure.y}, bound, color
            );
        }
        curPos.x += measure.x;
    }
}
void FontC::DrawTextInBound( 
    const std::string& text, 
    const Vec2& pos, 
    const RectF& bound, 
    Color color ) const
{
    DrawTextInBound( text, pos, bound, color, Vec2{(float)glyphWidth, (float)glyphHeight} );
}
void FontC::DrawTextCenter( 
        const std::string& text, 
        const Vec2& pos_center, 
        Color color,
        const float _scale,
        const Vec2& measure 
) const
{
    DrawText( text, pos_center - (GetDest(text, measure, _scale)/2), color, measure*_scale );
}
void FontC::DrawTextCenter( const std::string& text, const Vec2& pos_center, Color color, const float scale ) const
{
    DrawTextCenter( text, pos_center, color, scale, Vec2{(float)glyphWidth, (float)glyphHeight} );
}
void FontC::AddEffText( 
    const std::string& text, 
    const Vec2& posSpawn, 
    const Color color, 
    const Vec2& mearure, 
    const float _yScroll 
)
{
    effTexts.emplace_back( EffText{ text, posSpawn, color, mearure, _yScroll } );
}
void FontC::AddEffText( const std::string& text, const Vec2& posSpawn, const Color color, const float _yScroll )
{
    effTexts.emplace_back( EffText{ text, posSpawn, color, Vec2{(float)glyphWidth, (float)glyphHeight}, _yScroll } );
}
void FontC::UpdateEffText( const float dTime )
{
    std::vector<EffText>::iterator it = effTexts.begin();
    for ( ; it < effTexts.end(); it++)
    {
        it->Update( dTime );
        if( it->isDonePeriod )
        {
            it->text.clear();
            effTexts.erase(it);
        }
    }
}
void FontC::DrawTextEff() const
{
    for( auto& e : effTexts ) DrawText( e.text, e.pos, e.color, e.measure );
}

FontC::EffText::EffText( 
    const std::string& _text, 
    const Vec2& _posSpawn, 
    const Color& _color, 
    const Vec2& _measure,
    const float _yScroll 
)
    :
    text( _text ), posSpawn( _posSpawn ), pos( _posSpawn ),
    measure( _measure ), yScroll( _yScroll ),
    posYMax( _posSpawn + Vec2{0.0f, -_yScroll} ),
    color(_color)
{}
void FontC::EffText::Update( const float dTime )
{
    if( pos.y > posYMax.y )
    {
        pos.y += -speed * dTime;
    }
    else isDonePeriod = true;
}
const Vec2 FontC::GetDest( const std::string& text, const Vec2& measure, const float _scale ) const
{
    return Vec2{ (int)text.size()*measure.x*_scale, measure.y*_scale };
}
const std::string FontC::MatchingTextHori( const std::string& text, const float& width_bound )
{
    if( width_bound <= 0.0f )
    {
        std::cout << "[FontC](MatchingTextHori): Error width bound" << std::endl;
        return "";
    }
    std::string text_n;
    int last_space = 0;
    int pos_char_of_line = 1;
    for( int i = 0; i < (int)text.size(); i++ )
    {
        if( text[i] == ' ' ) last_space = i;
        else if( text[i] == '\n' ) pos_char_of_line = 1;
        if( float(pos_char_of_line * glyphWidth) > width_bound )
        {
            while ( text_n.back() != ' ' ) { text_n.pop_back(); }
            text_n.push_back('\n');
            i = last_space;
            pos_char_of_line = 1;
        }
        else 
        {
            text_n.push_back(text[i]);
            ++pos_char_of_line;
        }
    }
    return text_n;
}
const float FontC::WidthOf( const int& lenght ) const
{
    return lenght * glyphWidth;
}
const float FontC::WidthOf( const std::string& text ) const
{
    return (int)text.length() * glyphWidth;
}
