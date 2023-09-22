#pragma once
#include <vector>
#include <string>
#include "DataTypes.h"
#include <SDL.h>

class Map
{
public:
    Map(std::string name);
    ~Map() {}

    std::string GetName() const { return m_name; }
    void AddVertex(const Vertex& v);
    void AddLineDef(const LineDef& l) { m_lineDefs.push_back(l); }
    void RenderAutoMap(SDL_Renderer* pRenderer);
private:
    std::string m_name;
    std::vector<Vertex> m_vertices;
    std::vector<LineDef> m_lineDefs;

    int16_t m_xMin;
    int16_t m_xMax;
    int16_t m_yMin;
    int16_t m_yMax;
    int m_iAutoMapScaleFactor;
};

