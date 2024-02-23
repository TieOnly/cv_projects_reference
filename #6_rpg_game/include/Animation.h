#pragma once

class Animation
{
public:
    Animation(); 
    Animation(float x, float y, float width, float height, int count, const Texture2D* sprite, float holdTime);
    void Destroy();
    void Update( float dTime );
    void Update( float dTime, const Sound& sound, const int nFramePerSound );
    void ForceBeginPeriod();
    void ForceDonePeriod();
    bool IsDonePeriod() const;
    void Draw( const Vec2& pos ) const;
    void Draw( const Vec2& pos, const Vec2& dimension ) const;
    void Draw( const Vec2& pos, const Vec2& dimension, Color color ) const;
    void Draw( const Vec2& pos, const Vec2& dimension, Color color, const RectF& boundary ) const;

    const Texture2D* GetSurf() const;
    const Vec2& GetDimens() const;
private:
    const Texture2D* sprite = nullptr;
    std::vector<RectF> frames{};
    Vec2 dimens = {0.0f, 0.0f};
    int iCurFrame = 0;
    int sizeFrames = 0;
    float curFrameTime = 0.0f;
    float holdTime = 0.0f;
    bool isDonePeriod = true;
};