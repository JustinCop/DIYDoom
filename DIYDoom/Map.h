#pragma once
#include <vector>
#include <string>
#include "DataTypes.h"
#include "Player.h"
#include <memory>

class Map
{
public:
    Map(std::string name, std::shared_ptr<Player> pPlayer, SDLRendererPtr pRenderer);
    ~Map() {}

    std::string GetName() const { return m_name; }
    void AddVertex(const Vertex& v);
    void AddLineDef(const LineDef& l) { m_lineDefs.push_back(l); }
    void AddThing(const Thing& thing);
    void AddNode(const Node& node);
    void RenderAutoMap();
    void SetLumpIndex(int index) { m_iLumpIndex = index; }
    int GetLumpIndex() const { return m_iLumpIndex; }

protected:
    // Render function
    void RenderAutoMapPlayer();
    void RenderAutoMapWalls();
    void RenderBSPNodes();
    void RenderBSPNodes(int16_t iNodeID);   // Binary Space Partioning
    void RenderAutoMapNode(int16_t iNodeID);
    void RenderSubSector(int16_t iNodeID);

    bool IsPointOnLeftSide(int16_t xPosition, int16_t yPosition, int iNodeID);

    // Convert WAD File position data to screen coord.
    int16_t MapXToScreen(int16_t xMapPosition);
    int16_t MapYToScreen(int16_t yMapPosition);

    std::string m_name;
    std::vector<Vertex> m_vertices;
    std::vector<LineDef> m_lineDefs;
    std::vector<Thing> m_things;
    std::vector<Node> m_nodes;

    // The position data from WAD file.
    // Right: +X
    // Left: -X
    // Up: +Y
    // Dpwn: -Y
    // Example: E1M1: X in [-768, 3808], Y in [-4864, -2048]
    int16_t m_xMin_Left;    // The real position data from WAD file.
    int16_t m_xMax_Right;   // The real position data from WAD file.
    int16_t m_yMin_Down;    // The real position data from WAD file.
    int16_t m_yMax_Up;      // The real position data from WAD file.
    int m_iAutoMapScaleFactor;
    int m_iLumpIndex;   // Cache the lump index as soon as this map is found

    std::shared_ptr<Player> m_pPlayer;
    SDLRendererPtr m_pRenderer;
    int16_t m_xMax_Render;  // Render size of SDL window.
    int16_t m_yMax_Render;  // Render size of SDL window.
};

