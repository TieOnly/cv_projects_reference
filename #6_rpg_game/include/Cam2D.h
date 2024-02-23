#pragma once

class Cam2D
{
public:
    class BackGround
    {
    public:
        BackGround( 
            const Texture2D* surf_in, 
            const Vec2& offset_in, 
            const float dRoll_in, 
            const float scale = 1.0f, 
            const float autoRoll = 0.0f 
        );
        void Update( const Vec2& target, const Vec2& dir );
        void Update( const Vec2& target, const Vec2& dir, const float autoRoll );
        void Draw( const RectF& rectCam ) const;
    private:
        const Texture2D* surf = nullptr;
        Vec2 pos = {0.0f, 0.0f};
        Vec2 offset = {0.0f, 0.0f};
        float dRoll = 0.0f;
        float rolling = 0.0f;
        float autoRoll = 0.0f;
        float widthPerFrame;
        float heightPerFrame = float( settings::screenH + 20.0f );
    };
public:
    Cam2D( const Vec2& target_in);
    void ProcessMovement( Vec2& newTarget, const RectF& rectMap );
    const RectF GetRect() const;
    const RectF GetRectTarget() const;
    const Camera2D& GetRayCam() const;

    void AddLayerBackGround( 
        const std::string& path, 
        const Vec2& offset, 
        const float dRoll = 0.0f, 
        const float scale = 1.0f,
        const float autoRoll = 0.0f 
    );
    void SetBackGround();
    void UpdateBackGround( const Vec2& dir );
    void DrawBackGround() const;
private:
    Camera2D cam = { 0 };
    Vec2 target;
    bool isBoundary = false;
    std::vector<BackGround> layers;
    float increaseRolling = 0.0f;
};