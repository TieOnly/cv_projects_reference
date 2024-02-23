#pragma once
#include "Arduino.h"

/*
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))
*/

//Read Left -> Right increasing
#define bit16Read(value, bit)   (((value) << (bit)) & 0x8000)
#define bit16Set(value, bit)    ((value) |= (0x8000 >> (bit)))
#define bit16Clear(value, bit)  ((value) &= ~(0x8000 >> (bit))) 
#define bit16Write(value, bit, bitvalue)  ((bitvalue) ? bit16Set(value, bit) : bit16Clear(value, bit))

#define DATA    7
#define OE      5
#define ST_CP   4
#define SH_CP   3

#define BTN_COLS 2
#define BTN_ROWS 2
#define BTN_PORT_COL0 8
#define BTN_PORT_COL1 9
#define BTN_PORT_ROW0 10
#define BTN_PORT_ROW1 11

#define BUZZER 6

#define DISPLAY_X 16
#define DISPLAY_Y 8

typedef unsigned char   _uc;
typedef unsigned int    _ui;
typedef unsigned long   _ul;
typedef uint8_t         _u8;

namespace TieUtil
{

};

class Vec
{
public:
    Vec() = default;
    // constexpr Vec( char _x, char _y )
    //     :x(_x), y(_y)
    // {}
    template<typename T, typename S>
    constexpr Vec( T _x, S _y )
        :x((char)_x), y((char)_y)
    {}
    explicit constexpr Vec( const Vec& src )
        :x((char)src.x), y((char)src.y)
    {}
    constexpr Vec( Vec& src )
        :x((char)src.x), y((char)src.y)
    {}

    char x = 0;
    char y = 0;
public:
    constexpr bool operator == (const Vec& rhs) const
    {
        return (x == rhs.x && y == rhs.y);
    }
    constexpr bool operator != (const Vec& rhs) const
    {
        return !(*this == rhs);
    }
    constexpr bool operator < (const Vec& rhs ) const
    {
        return (x < rhs.x && y < rhs.y );
    }
    constexpr bool operator <= (const Vec& rhs) const
    {
        return (x <= rhs.x && y <= rhs.y);
    }
    constexpr bool operator > (const Vec& rhs ) const
    {
        return (x > rhs.x && y > rhs.y );
    }
    constexpr bool operator >= (const Vec& rhs) const
    {
        return (x >= rhs.x && y >= rhs.y);
    }
    constexpr Vec operator + (const Vec& rhs) const
    {
        return {(x + rhs.x), (y + rhs.y)};
    }
    constexpr Vec operator - (const Vec& rhs) const
    {
        return {(x - rhs.x), (y - rhs.y)};
    }
    constexpr Vec operator * (const Vec& rhs) const
    {
        return {(x * rhs.x), (y * rhs.y)};
    }
    constexpr Vec operator / (const Vec& rhs) const
    {
        return {(x / rhs.x), (y / rhs.y)};
    }
};

struct row_8
{
    _u8 bits;
    bool at( const _u8 pos ) const
    {
        if( pos < 0 || pos > 7 ) return 0;
        return ((bits << pos) & 0x80);
    }
    void set( const _u8 pos, bool value )
    {
        if( pos < 0 || pos > 7 ) return;
        value == 1 ? bits |= (0x80 >> pos) : bits &= ~(0x80 >> pos); 
    }
};
