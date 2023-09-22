#include "Map.h"

Map::Map(std::string sName) :
    m_name(sName),
    m_xMin(_I16_MAX),
    m_xMax(_I16_MIN),
    m_yMin(_I16_MAX),
    m_yMax(_I16_MIN),
    m_iAutoMapScaleFactor(15)
{

}

void Map::AddVertex(const Vertex& v)
{
    m_vertices.push_back(v);
    m_xMin = std::min(m_xMin, v.x);
    m_xMax = std::max(m_xMax, v.x);
    m_yMin = std::min(m_yMin, v.y);
    m_yMax = std::max(m_yMax, v.y);
}

void Map::RenderAutoMap(SDL_Renderer* pRenderer)
{
    int16_t xShift = -m_xMin;
    int16_t yShift = -m_yMin;

    int xRenderSize;
    int yRenderSize;

    SDL_RenderGetLogicalSize(pRenderer, &xRenderSize, &yRenderSize);

    --xRenderSize;
    --yRenderSize;

    SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    for (const LineDef& l : m_lineDefs)
    {
        Vertex vStart = m_vertices[l.startVertex];
        Vertex vEnd = m_vertices[l.endVertex];

        SDL_RenderDrawLine(pRenderer,
                           (vStart.x + xShift) / m_iAutoMapScaleFactor,
                           yRenderSize - (vStart.y + yShift) / m_iAutoMapScaleFactor,
                           (vEnd.x + xShift) / m_iAutoMapScaleFactor,
                           yRenderSize - (vEnd.y + yShift) / m_iAutoMapScaleFactor);
    }
}