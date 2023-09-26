#pragma once
#include <vector>
#include <string>
#include "DataTypes.h"
#include <SDL.h>
#include "Player.h"

class Map
{
public:
    Map(std::string name, Player* pPlayer);
    ~Map() {}

    std::string GetName() const { return m_name; }
    void AddVertex(const Vertex& v);
    void AddLineDef(const LineDef& l) { m_lineDefs.push_back(l); }
    void AddThing(const Thing& thing);
    void RenderAutoMap(SDL_Renderer* pRenderer);
    void SetLumpIndex(int index) { m_iLumpIndex = index; }
    int GetLumpIndex() const { return m_iLumpIndex; }

protected:
    void RenderAutoMapPlayer(SDL_Renderer* pRenderer, int iXShift, int iYShift);
    void RenderAutoMapWalls(SDL_Renderer* pRenderer, int iXShift, int iYShift);
    
    std::string m_name;
    std::vector<Vertex> m_vertices;
    std::vector<LineDef> m_lineDefs;
    std::vector<Thing> m_things;

    int16_t m_xMin;
    int16_t m_xMax;
    int16_t m_yMin;
    int16_t m_yMax;
    int m_iAutoMapScaleFactor;
    int m_iLumpIndex;   // Cache the lump index as soon as this map is found

    Player* m_pPlayer;
};

