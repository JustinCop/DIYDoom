#include "WADLoader.h"
#include <iostream>

WADLoader::WADLoader():
    m_pWADData(nullptr)
{
}

WADLoader::~WADLoader()
{
    delete[] m_pWADData;
}

bool WADLoader::LoadWAD()
{
    if (!OpenAndLoad())
    {
        return false;
    }

    if (!ReadHeaderAndDirectories())
    {
        return false;
    }

    return true;
}

bool WADLoader::LoadMapData(Map* pMap)
{
    std::cout << "Info: Parsing Map: " << pMap->GetName() << std::endl;

    std::cout << "Info: Processing Map Vertex" << std::endl;
    if (!ReadMapVertex(pMap))
    {
        std::cout << "Error: Failed to load map vertex data MAP: " << pMap->GetName() << std::endl;
        return false;
    }
    std::cout << "Info: Processing Map Linedef" << std::endl;
    if (!ReadMapLineDef(pMap))
    {
        std::cout << "Error: Failed to load map linedef data MAP: " << pMap->GetName() << std::endl;
        return false;
    }

    std::cout << "Info: Processing Map Things" << std::endl;
    if (!ReadMapThing(pMap))
    {
        std::cout << "Error: Failed to load map thing data MAP: " << pMap->GetName() << std::endl;
        return false;
    }

    return true;
}

int WADLoader::FindMapIndex(Map* pMap)
{
    if (pMap->GetLumpIndex() > -1)
    {
        // The map is alreay found
        return pMap->GetLumpIndex();
    }

    for (size_t i = 0; i < m_WADDirectories.size(); ++i)
    {
        if (m_WADDirectories[i].lumpName == pMap->GetName())
        {
            pMap->SetLumpIndex((int)i);
            return (int)i;
        }
    }
    return -1;
}

bool WADLoader::ReadMapVertex(Map* pMap)
{
    int iMapIndex = FindMapIndex(*pMap);
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
        WADReader::ReadVertexData(m_pWADData, vtxDir.lumpOffset + i * vtxSize, vtx);
        pMap->AddVertex(vtx);
    }

    return true;
}

bool WADLoader::ReadMapLineDef(Map* pMap)
{
    int iMapIndex = FindMapIndex(*pMap);
    if (iMapIndex == -1)
    {
        return false;
    }

    const WADDirectory &lineDir = m_WADDirectories[iMapIndex + eLINEDEFS];
    if (strcmp(lineDir.lumpName, str_LINEDEFS) != 0)
    {
        return false;
    }

    const uint32_t lineSize = sizeof(LineDef);
    const uint32_t lineCount = lineDir.lumpSize / lineSize;

    for (uint32_t i = 0; i < lineCount; ++i)
    {
        LineDef line;
        WADReader::ReadLineDefData(m_pWADData, lineDir.lumpOffset + i * lineSize, line);
        pMap->AddLineDef(line);
    }

    return true;
}

bool WADLoader::ReadMapThing(Map* pMap)
{
    int iMapIndex = FindMapIndex(*pMap);
    if (iMapIndex == -1)
    {
        return false;
    }

    const WADDirectory& thingDir = m_WADDirectories[iMapIndex + eTHINGS];
    if (strcmp(thingDir.lumpName, str_THINGS) != 0)
    {
        return false;
    }

    const uint32_t thingSize = sizeof(Thing);
    const uint32_t thingCount = thingDir.lumpSize / thingSize;

    for (uint32_t i = 0; i < thingCount; ++i)
    {
        Thing thing;
        WADReader::ReadThingData(m_pWADData, thingDir.lumpOffset + i * thingSize, thing);
        pMap->AddThing(thing);
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

bool WADLoader::ReadHeaderAndDirectories()
{
    WADReader::ReadHeaderData(m_pWADData, m_WADHeader);

    for (uint32_t i = 0; i < m_WADHeader.directoryCount; ++i)
    {
        WADDirectory dir;
        WADReader::ReadDirectoryData(m_pWADData, m_WADHeader.directoryOffset + i * 16, dir);
        m_WADDirectories.push_back(dir);
    }

    return true;
}

