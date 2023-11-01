#include "Map.h"

Map::Map(std::string sName, std::shared_ptr<Player> pPlayer) :
    m_name(sName),
    m_xMin(_I16_MAX),
    m_xMax(_I16_MIN),
    m_yMin(_I16_MAX),
    m_yMax(_I16_MIN),
    m_iAutoMapScaleFactor(15),
    m_iLumpIndex(-1),
    m_pPlayer(pPlayer)
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

void Map::AddThing(const Thing& thing)
{
    if (thing.type == m_pPlayer->GetID())
    {
        m_pPlayer->SetXPosition(thing.xPosition);
        m_pPlayer->SetYPosition(thing.yPosition);
        m_pPlayer->SetDirection(thing.direction);
    }
    m_things.push_back(thing);
}

void Map::RenderAutoMap(SDL_Renderer* pRenderer)
{
    int16_t xShift = -m_xMin;
    int16_t yShift = -m_yMin;

    RenderAutoMapWalls(pRenderer, xShift, yShift);
    RenderAutoMapPlayer(pRenderer, xShift, yShift);
}

void Map::RenderAutoMapPlayer(SDL_Renderer* pRenderer, int iXShift, int iYShift)
{
    int iRenderXSize;
    int iRenderYSize;

    SDL_RenderGetLogicalSize(pRenderer, &iRenderXSize, &iRenderYSize);

    --iRenderXSize;
    --iRenderYSize;

    SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

    std::pair<int, int> Direction[] = {
    std::make_pair(-1, -1), std::make_pair(0, -1), std::make_pair(+1, -1),
    std::make_pair(-1, 0), std::make_pair(0, 0), std::make_pair(+1, 0),
    std::make_pair(-1, +1), std::make_pair(0, +1), std::make_pair(+1, +1)
    };

    for (int i = 0; i < 9; ++i)
    {
        SDL_RenderDrawPoint(pRenderer,
                            (m_pPlayer->GetXPosition() + iXShift) / m_iAutoMapScaleFactor + Direction[i].first,
                            iRenderYSize - (m_pPlayer->GetYPosition() + iYShift) / m_iAutoMapScaleFactor + Direction[i].second);
    }

}

void Map::RenderAutoMapWalls(SDL_Renderer* pRenderer, int iXShift, int iYShift)
{
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
                           (vStart.x + iXShift) / m_iAutoMapScaleFactor,
                           yRenderSize - (vStart.y + iYShift) / m_iAutoMapScaleFactor,
                           (vEnd.x + iXShift) / m_iAutoMapScaleFactor,
                           yRenderSize - (vEnd.y + iYShift) / m_iAutoMapScaleFactor);
    }

}
