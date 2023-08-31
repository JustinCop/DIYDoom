#pragma once
#include <vector>
#include <string>
#include "DataTypes.h"

class Map
{
public:
    Map(std::string name) :m_name(name) {}
    ~Map() {}

    std::string GetName() const { return m_name; }
    void AddVertex(const Vertex& v) { m_vertices.push_back(v); }
    void AddLineDef(const LineDef& l) { m_lineDefs.push_back(l); }

private:
    std::string m_name;
    std::vector<Vertex> m_vertices;
    std::vector<LineDef> m_lineDefs;
};

