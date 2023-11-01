#include "Map.h"

Map::Map(std::string sName, std::shared_ptr<Player> pPlayer, SDLRendererPtr pRenderer) :
    m_name(sName),
    m_xMin_Left(_I16_MAX),
    m_xMax_Right(_I16_MIN),
    m_yMin_Down(_I16_MAX),
    m_yMax_Up(_I16_MIN),
    m_iAutoMapScaleFactor(AUTO_MAP_SCALE_FACTOR),
    m_iLumpIndex(-1),
    m_pPlayer(pPlayer),
    m_pRenderer(pRenderer)
{
    SDL_RenderGetLogicalSize(m_pRenderer.get(), (int*)(&m_xMax_Render), (int*)(&m_yMax_Render));

    --m_xMax_Render;
    --m_yMax_Render;

}

void Map::AddVertex(const Vertex& v)
{
    m_vertices.push_back(v);
    m_xMin_Left = std::min(m_xMin_Left, v.x);
    m_xMax_Right = std::max(m_xMax_Right, v.x);
    m_yMin_Down = std::min(m_yMin_Down, v.y);
    m_yMax_Up = std::max(m_yMax_Up, v.y);
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

void Map::RenderAutoMap()
{
    RenderAutoMapWalls();
    RenderAutoMapPlayer();
}

void Map::RenderAutoMapPlayer()
{
    SDL_SetRenderDrawColor(m_pRenderer.get(), 255, 0, 0, SDL_ALPHA_OPAQUE);

    std::pair<int, int> Direction[] = {
    std::make_pair(-1, -1), std::make_pair(0, -1), std::make_pair(+1, -1),
    std::make_pair(-1, 0), std::make_pair(0, 0), std::make_pair(+1, 0),
    std::make_pair(-1, +1), std::make_pair(0, +1), std::make_pair(+1, +1)
    };

    for (int i = 0; i < 9; ++i)
    {
        SDL_RenderDrawPoint(m_pRenderer.get(),
                            MapXToScreen(m_pPlayer->GetXPosition()) + Direction[i].first,
                            MapYToScreen(m_pPlayer->GetYPosition()) + Direction[i].second);
    }

}

void Map::RenderAutoMapWalls()
{
    SDL_SetRenderDrawColor(m_pRenderer.get(), 255, 255, 255, SDL_ALPHA_OPAQUE);
    for (const LineDef& l : m_lineDefs)
    {
        Vertex vStart = m_vertices[l.startVertex];
        Vertex vEnd = m_vertices[l.endVertex];

        SDL_RenderDrawLine(m_pRenderer.get(),
                           MapXToScreen(vStart.x),
                           MapYToScreen(vStart.y),
                           MapXToScreen(vEnd.x),
                           MapYToScreen(vEnd.y));
    }

}

int16_t Map::MapXToScreen(int16_t xMapPosition)
{
    return (xMapPosition - m_xMin_Left) / m_iAutoMapScaleFactor;
}

int16_t Map::MapYToScreen(int16_t yMapPosition)
{
    return m_yMax_Render - (yMapPosition - m_yMin_Down) / m_iAutoMapScaleFactor;
}
