#pragma once
#include "DataTypes.h"

class WADReader
{
public:
    static void ReadHeaderData(const uint8_t* pWADData, WADHeader& header);
    static void ReadDirectoryData(const uint8_t* pWADData, uint32_t offset, WADDirectory& directory);
    static void ReadVertexData(const uint8_t* pWADData, uint32_t offset, Vertex& vertex);
    static void ReadLineDefData(const uint8_t* pWADData, uint32_t offset, LineDef& lineDef);
    static void ReadThingData(const uint8_t* pWADData, uint32_t offset, Thing& thing);
private:
    static uint16_t Read2Bytes(const uint8_t* pWADData, uint32_t offset);
    static uint32_t Read4Bytes(const uint8_t* pWADData, uint32_t offset);
    static uint64_t Read8Bytes(const uint8_t* pWADData, uint32_t offset);
};

