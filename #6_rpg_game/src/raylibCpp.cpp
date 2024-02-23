#include "raylibCpp.h"

void rayCpp::DrawSprite(const Texture2D& s, const Vec2& pos)
{
    DrawSprite(s, pos, 
        RectF{{0.0f, 0.0f}, (float)s.width, (float)s.height}, 
        RectF{ pos, (float)s.width, (float)s.height }, 
        { 0.0f, 0.0f }, 0.0f, WHITE 
    );
}
void rayCpp::DrawSprite(const Texture2D& s, const Vec2& pos, const RectF& srcRect)
{
    DrawSprite(s, pos, srcRect, RectF{ pos, srcRect.width, srcRect.height }, { 0.0f, 0.0f }, 0.0f, WHITE);
}
void rayCpp::DrawSprite(const Texture2D& s, const Vec2& pos, const RectF& srcRect, float scale)
{
    DrawSprite(s, pos, srcRect, RectF{ pos, srcRect.width*scale, srcRect.height*scale }, { 0.0f, 0.0f }, 0.0f, WHITE);
}
void rayCpp::DrawSprite(const Texture2D& s, const Vec2& pos, const RectF& srcRect, Color color)
{
    DrawSprite(s, pos, srcRect, RectF{ pos, srcRect.width, srcRect.height }, { 0.0f, 0.0f }, 0.0f, color);
}
void rayCpp::DrawSprite(const Texture2D& s, const Vec2& pos, const RectF& srcRect, const float wDest, const float hDest)
{
    DrawSprite(s, pos, srcRect, RectF{ pos, wDest, hDest }, { 0.0f, 0.0f }, 0.0f, WHITE);
}
void rayCpp::DrawSprite(const Texture2D& s, const Vec2& pos, const RectF& srcRect, const float wDest, const float hDest, Color c)
{
    DrawSprite(s, pos, srcRect, RectF{ pos, wDest, hDest }, { 0.0f, 0.0f }, 0.0f, c);
}
void rayCpp::DrawSprite(const Texture2D& s, const Vec2& pos, const RectF& srcRect, const RectF& dest, const Vec2& origin, const float rotation, Color color)
{
    // Rectangle rSrc = srcRect.rayRect();
    // Rectangle dSrc = dest.rayRect();
    // Vector2 ori = { origin.x, origin.y };
    DrawTexturePro(s, srcRect.rayRect(), dest.rayRect(), { origin.x, origin.y }, rotation, color);
}
void rayCpp::DrawSpriteInside(const Texture2D& s, const Vec2& pos, const RectF& srcRect, const RectF& dest, const RectF& boundary, Color color )
{
    RectF rebound = {};
    const float rateSurfDest = srcRect.width / dest.width;
    if( dest.top < boundary.top )
    {
        rebound.top = boundary.top - dest.top;
    }
    if( dest.left < boundary.left )
    {
        rebound.left = boundary.left - dest.left;
    }
    if( dest.bottom > boundary.bottom )
    {
        rebound.bottom = boundary.bottom - dest.bottom;
    }
    if( dest.right > boundary.right )
    {
        rebound.right = boundary.right - dest.right;
    }
    DrawTexturePro(s, (srcRect + rebound*rateSurfDest).rayRect(), (dest + rebound).rayRect(), {0.0f, 0.0f}, 0.0f, color);
}
void rayCpp::DrawRectThin(const RectF& rect, const Color& color)
{
    DrawRectangleLinesEx( rect.rayRect(), 2.0f, color );
}
void rayCpp::DrawRectThin(const RectF& rect, const Color& color, const float lineThick)
{
    DrawRectangleLinesEx( rect.rayRect(), lineThick, color );
}
void rayCpp::DrawRectPoint(const Vec2& pos, const Color& color)
{
    Rectangle rayRect = Rectangle{ pos.x, pos.y, 8.0f, 8.0f };
    DrawRectangleLinesEx( rayRect, 2.0f, color );
}
void rayCpp::DrawRectFill(const RectF& rect, const Color& color)
{
    DrawRectangle( (int)rect.left, (int)rect.top, int(rect.right - rect.left), int(rect.bottom - rect.top), color );
}
void rayCpp::DrawCircleThin(const Vec2& pos, const float radius, Color color)
{
    DrawCircleLines( (int)pos.x, (int)pos.y, radius, color );
}
RectF rayCpp::GetScreenRect(  )
{
    return RectF{ {0, 0}, settings::screenW, settings::screenH };
}
Vec2 rayCpp::GetCenterScreen()
{
    return GetScreenRect().GetCenterPoint();
}
Vec2 rayCpp::GetMousePos() { return Vec2{(float)GetMouseX(), (float)GetMouseY()}; }
Vec2 rayCpp::CreateVecByData( const std::string& data, const char delim )
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
Vei2 rayCpp::CreateVeiByData( const std::string& data, const char delim )
{
    Vei2 v;
    std::stringstream ss( data );
    std::string s;
    std::getline( ss, s, delim );
    v.x = std::stof( s );
    std::getline( ss, s, delim );
    v.y = std::stof( s );
    return v;
}
bool rayCpp::CheckCollidedRectCir( const RectF& rect, const Vec2& pos_cir, const float radius )
{
    const float disX = abs( pos_cir.x - (rect.left + (rect.width / 2)));
    const float disY = abs( pos_cir.y - (rect.top + (rect.height / 2)));

    if( disX > (radius + rect.width/2) ) { return false; }
    if( disY > (radius + rect.height/2) ) { return false; }
    if( disX <= (rect.width/2) ) { return true; }
    if( disY <= (rect.height/2) ) { return true; }

    const Vec2 corner = { disX - (rect.width/2), disY - (rect.height)/2 };
    return corner.GetLengthSq() <= ( radius * radius );
} 
///////////////////////////////////////////////////////////////////////////////////////////////
int rayCpp::BinarySerch_IdInVei2( const std::vector<Vei2>& vei, const Vei2 val )
{
    int id_x_first = -1;
    int id_x_last = -1;
    
    int l_first = 0;
    int r_first = (int)vei.size() - 1;
    int r_last = r_first;
    
    while( l_first <= r_first )
    {
        int m = (l_first + r_first) / 2;
        if( vei[m].x == val.x )
        {
            id_x_first = m;
            r_first = m - 1;
        }
        else if( vei[m].x < val.x )
        {
            l_first = m + 1;
        }
        else if( vei[m].x > val.x )
        {
            r_first = m - 1;
            r_last = r_first;
        }
    }  
    if( id_x_first == -1 ) return -1;
    
    int l_last = id_x_first;
    while ( l_last <= r_last )
    {
        int m = (l_last + r_last) / 2;
        if( vei[m].x == val.x )
        {
            id_x_last = m;
            l_last = m + 1;
        }
        else if( vei[m].x > val.x )
        {
            r_last = m - 1;
        }
    }
    if( id_x_last == -1 ) return -1;
    
    int l_y = id_x_first;
    int r_y = id_x_last;
    while (l_y <= r_y)
    {
        int m = (l_y + r_y) / 2;
        if( vei[m].y == val.y )
        {
            return m;
        }
        else if( vei[m].y < val.y )
        {
            l_y = m + 1;
        }
        else if( vei[m].y > val.y )
        {
            r_y = m - 1;
        }
    }
    return -1;
}
std::vector<int> rayCpp::BS_PosInBoundary( const std::vector<RectF>& listBox, const RectF& boundary )
{
    std::vector<int> list_found_ids;
    std::vector<int> ids_hori;

    // binary search for horizontal pos overlapping with boundary
    int l = 0;
    int r = (int)listBox.size() - 1;
    int id_first_found = -1;

    while ( l <= r )
    {
        int m = (l + r) / 2;
        if( listBox[m].right >= boundary.left && listBox[m].left <= boundary.right )
        {
            id_first_found = m;
            break;
        }
        else if( listBox[m].right < boundary.left )
        {
            l = m + 1;
        }
        else if( listBox[m].left > boundary.right )
        {
            r = m - 1;
        }
    }
    if( id_first_found == -1 ) return list_found_ids;
    else ids_hori.push_back( id_first_found );

    int l_check = id_first_found - 1;
    while ( l_check >= l )
    {
        if( listBox[l_check].right >= boundary.left )
        {
            ids_hori.push_back(l_check);
            l_check--;
        }
        else break;
    }
    int r_check = id_first_found + 1;
    while ( r_check <= r )
    {
        if( listBox[r_check].left <= boundary.right )
        {
            ids_hori.push_back(r_check);
            r_check++;
        }
        else break;
    }

    // binary search for vertical pos overlapping with boundary
    std::sort(ids_hori.begin(), ids_hori.end());
    l = ids_hori.front();
    r = ids_hori.back();
    id_first_found = -1;
    
    while (l <= r)
    {
        int m = (l + r) / 2;
        if( listBox[m].bottom >= boundary.top && listBox[m].top <= boundary.bottom )
        {
            id_first_found = m;
            break;
        }
        else if( listBox[m].bottom < boundary.top )
        {
            l = m + 1;
        }
        else if( listBox[m].top > boundary.bottom )
        {
            r = m - 1;
        }
    }
    if( id_first_found == -1 ) return list_found_ids;
    else list_found_ids.push_back( id_first_found );
    
    int t_check = id_first_found - 1;
    while ( t_check >= l )
    {
        if( listBox[t_check].bottom >= boundary.top )
        {
            list_found_ids.push_back(t_check);
            t_check--;
        }
        else break;
    }
    int b_check = id_first_found + 1;
    while ( b_check <= r )
    {
        if( listBox[b_check].top <= boundary.bottom )
        {
            list_found_ids.push_back(b_check);
            b_check++;
        }
        else break;
    }

    return list_found_ids;
}
