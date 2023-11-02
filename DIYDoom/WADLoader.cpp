#include "WADLoader.h"
#include <iostream>
#include <cstring>

void WADReader::ReadHeaderData(const uint8_t* pWADData, WADHeader* header)
{
    // first 4 bytes for "IWAD" or "PWAD"
    memcpy(header->type, pWADData, 4);
    header->type[4] = '\0';

    //0x04 to 0x07
    header->directoryCount = Read4Bytes(pWADData, 4);

    //0x08 to 0x0b
    header->directoryOffset = Read4Bytes(pWADData, 8);
}

void WADReader::ReadDirectoryData(const uint8_t* pWADData, uint32_t offset, WADDirectory* directory)
{
    //0x00 to 0x03
    directory->lumpOffset = Read4Bytes(pWADData, offset);

    //0x04 to 0x07
    directory->lumpSize = Read4Bytes(pWADData, offset + 4);

    //0x08 to 0x0F
    memcpy(directory->lumpName, pWADData + offset + 8, 8);
    directory->lumpName[8] = '\0';
}

void WADReader::ReadVertexData(const uint8_t* pWADData, uint32_t offset, Vertex* vertex)
{
    vertex->x = Read2Bytes(pWADData, offset);
    vertex->y = Read2Bytes(pWADData, offset + 2);
}

void WADReader::ReadLineDefData(const uint8_t* pWADData, uint32_t offset, LineDef* lineDef)
{
    lineDef->startVertex = Read2Bytes(pWADData, offset);
    lineDef->endVertex = Read2Bytes(pWADData, offset + 2);
    lineDef->lineDefFlags = Read2Bytes(pWADData, offset + 4);
    lineDef->lineTypeAction = Read2Bytes(pWADData, offset + 6);
    lineDef->sectorTag = Read2Bytes(pWADData, offset + 8);
    lineDef->frontSideDef = Read2Bytes(pWADData, offset + 10);
    lineDef->backSideDef = Read2Bytes(pWADData, offset + 12);
}

void WADReader::ReadThingData(const uint8_t* pWADData, uint32_t offset, Thing* thing)
{
    thing->xPosition = Read2Bytes(pWADData, offset);
    thing->yPosition = Read2Bytes(pWADData, offset + 2);
    thing->direction = Read2Bytes(pWADData, offset + 4);
    thing->type = Read2Bytes(pWADData, offset + 6);
    thing->flags = Read2Bytes(pWADData, offset + 8);
}

void WADReader::ReadNodeData(const uint8_t* pWADData, uint32_t offset, Node* node)
{
    node->xPartition = Read2Bytes(pWADData, offset);
    node->yPartition = Read2Bytes(pWADData, offset + 2);
    node->changeXPartition = Read2Bytes(pWADData, offset + 4);
    node->changeYPartition = Read2Bytes(pWADData, offset + 6);

    node->rightBoxTop = Read2Bytes(pWADData, offset + 8);
    node->rightBoxBottom = Read2Bytes(pWADData, offset + 10);
    node->rightBoxLeft = Read2Bytes(pWADData, offset + 12);
    node->rightBoxRight = Read2Bytes(pWADData, offset + 14);

    node->leftBoxTop = Read2Bytes(pWADData, offset + 16);
    node->leftBoxBottom = Read2Bytes(pWADData, offset + 18);
    node->leftBoxLeft = Read2Bytes(pWADData, offset + 20);
    node->leftBoxRight = Read2Bytes(pWADData, offset + 22);

    node->rightChildID = Read2Bytes(pWADData, offset + 24);
    node->leftChildID = Read2Bytes(pWADData, offset + 26);
}

uint16_t WADReader::Read2Bytes(const uint8_t* pWADData, uint32_t offset)
{
    uint16_t value;
    memcpy(&value, pWADData + offset, sizeof(uint16_t));
    return value;
}

uint32_t WADReader::Read4Bytes(const uint8_t* pWADData, uint32_t offset)
{
    uint32_t value;
    memcpy(&value, pWADData + offset, sizeof(uint32_t));
    return value;
}

uint64_t WADReader::Read8Bytes(const uint8_t* pWADData, uint32_t offset)
{
    uint64_t value;
    memcpy(&value, pWADData + offset, sizeof(uint64_t));
    return value;
}



WADLoader::WADLoader():
    m_pWADData(nullptr),
    m_WADHeader({})
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

    if (!ReadHeaderAndDirectories())
    {
        return false;
    }

    return true;
}

bool WADLoader::LoadMapData(std::shared_ptr<Map> pMap)
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

    std::cout << "Info: Processing Map Nodes" << std::endl;
    if (!ReadMapNode(pMap))
    {
        std::cout << "Error: Failed to load map node data MAP: " << pMap->GetName() << std::endl;
        return false;
    }

    return true;
}

int WADLoader::FindMapIndex(std::shared_ptr<Map> pMap)
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

bool WADLoader::ReadMapVertex(std::shared_ptr<Map> pMap)
{
    int iMapIndex = FindMapIndex(pMap);
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
        WADReader::ReadVertexData(m_pWADData.get(), vtxDir.lumpOffset + i * vtxSize, &vtx);
        pMap->AddVertex(vtx);
    }

    return true;
}

bool WADLoader::ReadMapLineDef(std::shared_ptr<Map> pMap)
{
    int iMapIndex = FindMapIndex(pMap);
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
        WADReader::ReadLineDefData(m_pWADData.get(), lineDir.lumpOffset + i * lineSize, &line);
        pMap->AddLineDef(line);
    }

    return true;
}

bool WADLoader::ReadMapThing(std::shared_ptr<Map> pMap)
{
    int iMapIndex = FindMapIndex(pMap);
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
        WADReader::ReadThingData(m_pWADData.get(), thingDir.lumpOffset + i * thingSize, &thing);
        pMap->AddThing(thing);
    }

    return true;
}

bool WADLoader::ReadMapNode(std::shared_ptr<Map> pMap)
{
    int iMapIndex = FindMapIndex(pMap);
    if (iMapIndex == -1)
    {
        return false;
    }

    const WADDirectory& nodeDir = m_WADDirectories[iMapIndex + eNODES];
    if (strcmp(nodeDir.lumpName, str_NODES) != 0)
    {
        return false;
    }

    const uint32_t nodeSize = sizeof(Node);
    const uint32_t nodeCount = nodeDir.lumpSize / nodeSize;

    for (uint32_t i = 0; i < nodeCount; i++)
    {
        Node node;
        WADReader::ReadNodeData(m_pWADData.get(), nodeDir.lumpOffset + i * nodeSize, &node);
        pMap->AddNode(node);
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
    m_pWADData = std::make_unique<const uint8_t[]>(length);

    // Move back to beginning to copy data
    m_WADFile.seekg(0, m_WADFile.beg);
    m_WADFile.read((char*)m_pWADData.get(), length);
    m_WADFile.close();

    return true;
}

bool WADLoader::ReadHeaderAndDirectories()
{
    WADReader::ReadHeaderData(m_pWADData.get(), &m_WADHeader);

    for (uint32_t i = 0; i < m_WADHeader.directoryCount; ++i)
    {
        WADDirectory dir;
        WADReader::ReadDirectoryData(m_pWADData.get(), m_WADHeader.directoryOffset + i * 16, &dir);
        m_WADDirectories.push_back(dir);
    }

    return true;
}

