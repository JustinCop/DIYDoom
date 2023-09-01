#include "WADLoader.h"
#include <iostream>

WADLoader::WADLoader(std::string WADFilePath) :
    m_WADFilePath(WADFilePath),
    m_pWADData(nullptr)
{
}

WADLoader::~WADLoader()
{
}

bool WADLoader::LoadWAD()
{
    if (!OpenAndLoad())
    {
        return false;
    }

    if (!ReadDirectories())
    {
        return false;
    }

    return true;
}

bool WADLoader::LoadMapData(Map& map)
{
    if (!ReadMapVertex(map))
    {
        std::cout << "Error: Failed to load map vertex data MAP: " << map.GetName() << std::endl;
        return false;
    }
    if (!ReadMapLineDef(map))
    {
        std::cout << "Error: Failed to load map linedef data MAP: " << map.GetName() << std::endl;
        return false;
    }

    return true;
}

int WADLoader::FindMapIndex(const Map& map)
{
    for (size_t i = 0; i < m_WADDirectories.size(); ++i)
    {
        if (m_WADDirectories[i].lumpName == map.GetName())
            return (int)i;
    }
    return -1;
}

bool WADLoader::ReadMapVertex(Map& map)
{
    int iMapIndex = FindMapIndex(map);
    if (iMapIndex == -1)
    {
        return false;
    }

    const WADDirectory& vtxDir = m_WADDirectories[iMapIndex + eVERTEXES];

    if (strcmp(vtxDir.lumpName, str_VERTEXES) != 0)
    {
        return false;
    }

    const uint32_t vtxSize = sizeof(Vertex);
    const uint32_t vtxCount = vtxDir.lumpSize / vtxSize;
    for (uint32_t i = 0; i < vtxCount; ++i)
    {
        Vertex vtx;
        WADDecoder::ReadVertexData(m_pWADData, vtxDir.lumpOffset + i * vtxSize, vtx);
        map.AddVertex(vtx);

        //std::cout << vtx.x << std::endl;
        //std::cout << vtx.y << std::endl;
        //std::cout << std::endl;
    }

    return true;
}

bool WADLoader::ReadMapLineDef(Map &map)
{
    int iMapIndex = FindMapIndex(map);
    if (iMapIndex == -1)
    {
        return false;
    }

    const WADDirectory &lineDir = m_WADDirectories[iMapIndex += eLINEDEFS];
    if (strcmp(lineDir.lumpName, str_LINEDEFS) != 0)
    {
        return false;
    }

    const uint32_t lineSize = sizeof(LineDef);
    const uint32_t lineCount = lineDir.lumpSize / lineSize;

    for (uint32_t i = 0; i < lineCount; ++i)
    {
        LineDef line;
        WADDecoder::ReadLineDefData(m_pWADData, lineDir.lumpOffset + i * lineSize, line);
        map.AddLineDef(line);

        //std::cout << line.startVertex << std::endl;
        //std::cout << line.endVertex << std::endl;
        //std::cout << line.flags << std::endl;
        //std::cout << line.lineTypeAction << std::endl;
        //std::cout << line.sectorTag << std::endl;
        //std::cout << line.frontSideDef << std::endl;
        //std::cout << line.backSideDef << std::endl;
        //std::cout << std::endl;
    }

    return true;
}


bool WADLoader::OpenAndLoad()
{
    m_WADFile.open(m_WADFilePath, std::ifstream::binary);
    if (!m_WADFile.is_open())
    {
        std::cout << "Error: Failed to open WAD file" << m_WADFilePath << std::endl;
        return false;
    }

    // Move file ptr to the end, to check file size
    m_WADFile.seekg(0, m_WADFile.end);
    size_t length = m_WADFile.tellg();
    m_pWADData = new uint8_t[length];

    // Move back to beginning to copy data
    m_WADFile.seekg(0, m_WADFile.beg);
    m_WADFile.read((char*)m_pWADData, length);
    m_WADFile.close();

    return true;
}

bool WADLoader::ReadDirectories()
{
    WADHeader header;
    WADDecoder::ReadHeaderData(m_pWADData, header);

    //std::cout << header.type << std::endl;
    //std::cout << header.directoryCount << std::endl;
    //std::cout << header.directoryOffset << std::endl;
    //std::cout << std::endl << std::endl;

    for (uint32_t i = 0; i < header.directoryCount; ++i)
    {
        WADDirectory dir;
        WADDecoder::ReadDirectoryData(m_pWADData, header.directoryOffset + i * 16, dir);
        m_WADDirectories.push_back(dir);

        //std::cout << dir.lumpOffset << std::endl;
        //std::cout << dir.lumpSize << std::endl;
        //std::cout << dir.lumpName << std::endl;
        //std::cout << std::endl << std::endl;
    }

    return true;
}

