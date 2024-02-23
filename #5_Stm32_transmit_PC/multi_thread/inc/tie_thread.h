#pragma once
#include <thread>
#include <chrono>

namespace tie
{
namespace thread
{
    const int LEFT_CONTROL = 162;

    template <typename _Func, typename... _Args>
    static void AddThread(_Func &&func, _Args &&...args)
    {
        std::thread newThread(std::forward<_Func>(func), std::forward<_Args>(args)...);
        if (newThread.joinable()) newThread.detach();
    }

    template <typename _Func, typename... _Args>
    static void AddThreadPause(_Func &&func, _Args &&...args)
    {
        std::thread newThread(std::forward<_Func>(func), std::forward<_Args>(args)...);
        if (newThread.joinable()) newThread.join();
    }

    template<typename... _Args>
    static void MakeLoop( int fps, int (*func)(_Args...), _Args&&... args )
    {
        auto next_frame = std::chrono::steady_clock::now();
        bool bBreak = false;
        while ( !bBreak )
        {
            next_frame += std::chrono::microseconds(1000000 / fps);
            
            bBreak = func( std::forward<_Args>(args)... );

            std::this_thread::sleep_until(next_frame);
        }
    }

    template<typename... _Args>
    static void MakeLoopVoid( int fps, void (*func)(_Args...), _Args&&... args )
    {
        using frame = std::chrono::duration<std::chrono::steady_clock::rep, std::ratio<1, 60>>;
        auto next_frame = std::chrono::steady_clock::now() + frame{1};
        while ( true )
        {
            func( std::forward<_Args>(args)... );
            std::this_thread::sleep_until(next_frame);
            next_frame += frame{1};
        }
    }

    template<typename... _Args>
    static void MakeLoopThread( int fps, int (*func)(_Args...), _Args&&... args )
    {
        constexpr int max_rate = 10000;
        fps = fps > max_rate ? max_rate : fps;
        const int dura = int(max_rate / fps);
        using frame = std::chrono::duration<std::chrono::steady_clock::rep, std::ratio<1, max_rate>>;
        auto next_frame = std::chrono::steady_clock::now() + frame{dura};
        while( !func( std::forward<_Args>(args)... ) ) 
        {
            std::this_thread::sleep_until(next_frame);
            next_frame += frame{dura};
        }
    }
}
}