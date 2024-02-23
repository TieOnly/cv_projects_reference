#pragma once
#include "tie_define.h"

// class FrameTime
// {
// public:
//     static void Tick()
//     { return Get()._Tick(); }
//     static const _ul& DTime()
//     { return Get().dTime; }
//     static const _ul Second( const _ul& second )
//     { return _ul(second * 1000UL); }
// private:
//     static FrameTime& Get()
//     {
//         static FrameTime ft;
//         return ft;
//     }
//     void _Tick()
//     {
//       _ul old = last;
//       last = millis();
//       dTime = last - old;
//     }
//     _ul last;
//     _ul dTime;
// };

class TieFT
{
public:
    TieFT() = default;
    void Tick()
    {
        _ul old = last;
        last = millis();
        dTime = last - old;
    }
    const _ul& DTime()
    {
        return dTime;
    }
private:
    _ul last = 0;
    _ul dTime;
};
