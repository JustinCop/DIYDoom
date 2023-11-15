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

void Map::AddNode(const Node& node)
{
    m_nodes.push_back(node);
}

void Map::RenderAutoMap()
{
    RenderAutoMapWalls();
    RenderAutoMapPlayer();
    RenderBSPNodes();
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

void Map::RenderBSPNodes()
{
    RenderBSPNodes(m_nodes.size() - 1);
}

void Map::RenderAutoMapNode(int16_t iNodeID)
{
    Node node = m_nodes[iNodeID];
    SDL_Rect rightRect =
    {
        MapXToScreen(node.rightBoxLeft),
        MapYToScreen(node.rightBoxTop),
        MapXToScreen(node.rightBoxRight) - MapXToScreen(node.rightBoxLeft) + 1,
        MapYToScreen(node.rightBoxBottom) - MapYToScreen(node.rightBoxTop) + 1
    };

    SDL_Rect leftRect =
    {
        MapXToScreen(node.leftBoxLeft),
        MapYToScreen(node.leftBoxTop),
        MapXToScreen(node.leftBoxRight) - MapXToScreen(node.leftBoxLeft) + 1,
        MapYToScreen(node.leftBoxBottom) - MapYToScreen(node.leftBoxTop) + 1
    };

    // Draw right rect as green
    SDL_SetRenderDrawColor(m_pRenderer.get(), 0, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(m_pRenderer.get(), &rightRect);

    // draw left rect as red
    SDL_SetRenderDrawColor(m_pRenderer.get(), 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(m_pRenderer.get(), &leftRect);

    // draw the partition line
    SDL_SetRenderDrawColor(m_pRenderer.get(), 0, 0, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(m_pRenderer.get(),
                       MapXToScreen(node.xPartition),
                       MapYToScreen(node.yPartition),
                       MapXToScreen(node.xPartition + node.changeXPartition),
                       MapYToScreen(node.yPartition + node.changeYPartition));

    // draw the partition origin
    SDL_SetRenderDrawColor(m_pRenderer.get(), 100, 100, 100, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(m_pRenderer.get(),
                        MapXToScreen(node.xPartition),
                        MapYToScreen(node.yPartition));
}

void Map::RenderBSPNodes(int16_t iNodeID)
{
    // Check most significant bit
    const bool bIsLeafSector = iNodeID & SUBSECTOR_IDENTIFIER;
    iNodeID = iNodeID & (~SUBSECTOR_IDENTIFIER);
    
    if (bIsLeafSector)
    {
        RenderSubSector(iNodeID);
        return;
    }

    /////////////////////////////////
    // Debug: Render the Node square.
    // Cannot call RenderAutoMapNode before RenderSubSector.
    // For example, in E1M1, left child of Node 231 is -32530, in next call to RenderBSPNodes(-32530),
    // -32530 & (~SUBSECTOR_IDENTIFIER) == 238 == m_nodes.size(), calling RenderAutoMapNode(238) causes out_of_range error. 
    // Please call RenderAutoMapNode() after the "if (bIsLeafSector)" statement.
    RenderAutoMapNode(iNodeID);
    //////////////////////////

    bool isOnLeft = IsPointOnLeftSide(m_pPlayer->GetXPosition(), m_pPlayer->GetYPosition(), iNodeID);
    const Node& curNode = m_nodes[iNodeID];
    if (isOnLeft)
    {
        RenderBSPNodes(curNode.leftChildID);
        RenderBSPNodes(curNode.rightChildID);
    }
    else
    {
        RenderBSPNodes(curNode.rightChildID);
        RenderBSPNodes(curNode.leftChildID);
    }
}

void Map::RenderSubSector(int16_t iNodeID)
{
}

bool Map::IsPointOnLeftSide(int16_t xPosition, int16_t yPosition, int iNodeID)
{
    // v1: vector from partition origin to target point
    const int16_t v1x = xPosition - m_nodes[iNodeID].xPartition;
    const int16_t v1y = yPosition - m_nodes[iNodeID].yPartition;

    // v2: vector of the partition
    const int16_t v2x = m_nodes[iNodeID].changeXPartition;
    const int16_t v2y = m_nodes[iNodeID].changeYPartition;

    // check cross product: v2 x v1
    return (v2x * v1y - v2y * v1x >= 0);
}

int16_t Map::MapXToScreen(int16_t xMapPosition)
{
    return (xMapPosition - m_xMin_Left) / m_iAutoMapScaleFactor;
}

int16_t Map::MapYToScreen(int16_t yMapPosition)
{
    return m_yMax_Render - (yMapPosition - m_yMin_Down) / m_iAutoMapScaleFactor;
}
