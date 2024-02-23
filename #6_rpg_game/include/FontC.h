#pragma once

class FontC
{
private:
    class EffText
    {
    public:
        EffText( 
            const std::string& text, 
            const Vec2& posSpawn, 
            const Color& color,
            const Vec2& measure, 
            const float _yScroll );
        void Update( const float dTime );

        std::string text;
        Vec2 posSpawn{};
        Vec2 pos{};
        Vec2 measure{};
        float speed = 40.0;
        float yScroll = 50.0f;
        Vec2 posYMax{};
        Color color = WHITE;
        bool isDonePeriod = false;
    };
public:
    void LoadSurface( const std::string& fileName );
    void DrawText( 
        const std::string& text, 
        const Vec2& pos, 
        Color color, 
        const Vec2& measure ) const;
    void DrawText( const std::string& text, const Vec2& pos, Color color ) const;
    void DrawTextInBound( 
        const std::string& text, 
        const Vec2& pos, 
        const RectF& bound, 
        Color color,
        const Vec2& measure ) const;
    void DrawTextInBound( 
        const std::string& text, 
        const Vec2& pos, 
        const RectF& bound, 
        Color color ) const;
    void DrawTextCenter( 
        const std::string& text, 
        const Vec2& pos_center, 
        Color color,
        const float scale,
        const Vec2& measure ) const;
    void DrawTextCenter( 
        const std::string& text, 
        const Vec2& pos_center, 
        Color color,
        const float scale = 1.0f ) const;
    void AddEffText( 
        const std::string& text, 
        const Vec2& posSpawn, 
        const Color color,
        const Vec2& mearure, 
        const float _yScroll = 50.0f );
    void AddEffText( 
        const std::string& text, 
        const Vec2& posSpawn, 
        const Color color,
        const float _yScroll = 50.0f );
    void DrawTextEff() const;
    void UpdateEffText( const float dTime );
    const std::string MatchingTextHori( const std::string& text, const float& width_bound );
    const Vec2 GetDest( const std::string& text, const Vec2& measure, const float scale ) const;
    const float GetGlyphHeight() const { return (float)glyphHeight; }
    const float GetGlyphWidth() const { return (float)glyphWidth; }
public:
    const float WidthOf( const int& text_lenght ) const;
    const float WidthOf( const std::string& text ) const;
    template<typename... Args>
    const float HeigthOf( const float& height_stuff_default, const float fontHeight, const Args (&...args) )
    {
        std::vector<std::string> wrap = {args...};
        int total_lenght = 0;
        for( const std::string& s : wrap )
        {
            int count = 0;
            if( s.size() > 0 ) count = 1;
            for( const char c : s ) if( c == '\n' ) count++;
            total_lenght += count;
        }
        return (float)total_lenght*fontHeight + height_stuff_default;
    }
private:
    RectF MapGlyphRectF( char c ) const;
private:
    const Texture2D* surf;

    int glyphWidth; 
    int glyphHeight;

    static constexpr int colums = 32;
    static constexpr int rows = 3;

    static constexpr char fisrtChar = ' ';
    static constexpr char lastChar = '~';

    std::vector<EffText> effTexts;
};