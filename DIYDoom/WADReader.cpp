#include "WADReader.h"
#include <cstring>

void WADReader::ReadHeaderData(const uint8_t* pWADData, WADHeader& header)
{
    // first 4 bytes for "IWAD" or "PWAD"
    memcpy(header.type, pWADData, 4);
    header.type[4] = '\0';

    //0x04 to 0x07
    header.directoryCount = Read4Bytes(pWADData, 4);
    
    //0x08 to 0x0b
    header.directoryOffset = Read4Bytes(pWADData, 8);
}

void WADReader::ReadDirectoryData(const uint8_t* pWADData, uint32_t offset, WADDirectory& directory)
{
    //0x00 to 0x03
    directory.lumpOffset = Read4Bytes(pWADData, offset);

    //0x04 to 0x07
    directory.lumpSize = Read4Bytes(pWADData, offset + 4);

    //0x08 to 0x0F
    memcpy(directory.lumpName, pWADData + offset + 8, 8);
    directory.lumpName[8] = '\0';
}

void WADReader::ReadVertexData(const uint8_t* pWADData, uint32_t offset, Vertex& vertex)
{
    vertex.x = Read2Bytes(pWADData, offset);
    vertex.y = Read2Bytes(pWADData, offset + 2);
}

void WADReader::ReadLineDefData(const uint8_t* pWADData, uint32_t offset, LineDef& lineDef)
{
    lineDef.startVertex = Read2Bytes(pWADData, offset);
    lineDef.endVertex = Read2Bytes(pWADData, offset + 2);
    lineDef.lineDefFlags = Read2Bytes(pWADData, offset + 4);
    lineDef.lineTypeAction = Read2Bytes(pWADData, offset + 6);
    lineDef.sectorTag = Read2Bytes(pWADData, offset + 8);
    lineDef.frontSideDef = Read2Bytes(pWADData, offset + 10);
    lineDef.backSideDef = Read2Bytes(pWADData, offset + 12);
}

void WADReader::ReadThingData(const uint8_t* pWADData, uint32_t offset, Thing& thing)
{
    thing.xPosition = Read2Bytes(pWADData, offset);
    thing.yPosition = Read2Bytes(pWADData, offset + 2);
    thing.direction = Read2Bytes(pWADData, offset + 4);
    thing.type = Read2Bytes(pWADData, offset + 6);
    thing.flags = Read2Bytes(pWADData, offset + 8);
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
