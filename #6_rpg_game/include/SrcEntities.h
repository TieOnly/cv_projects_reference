#pragma once
#include "Entity.h"

class SrcEntities
{
public:
    SrcEntities();
    void AddEntity( 
        const std::string& fileSheet, const Vec2& srcPos, const Vec2& dimension, 
        const int frame, const float holdTime 
    );
    int IndexById( const int id );
    const Entity& EntityByIndex( const int index ) const;

    bool LoadSrcEntities( const std::string& fileName );
    Vec2 CreateVecByData( const std::string& data, const char delim );
    const int GetAmountEntity() const;
    const std::string& GetDataBuff() const;
private:
    void InitSrcEntities();
    void InitSrcEntities_list();
private:
    std::vector<Entity> entities;
    
    FileIO data;
    int amountEntity = 0;
    int iEntity = 0;
    const Texture2D* aniSheet;
    std::vector<Vec2> aniSrcPos;
    Vec2 aniSrcDimens = {0.0f, 0.0f};
    std::vector<Vec2> aniSrcDimens_list;
    std::vector<int> aniFrames;
    std::vector<float> aniHoldTime;    

    std::set<std::string> filesLoaded;
};