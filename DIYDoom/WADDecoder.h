#pragma once
#include "DataTypes.h"

class WADDecoder
{
public:
    static void ReadHeaderData(const uint8_t* pWADData, WADHeader& header);
    static void ReadDirectoryData(const uint8_t* pWADData, uint32_t offset, WADDirectory& directory);
private:
    static uint16_t Read2Bytes(const uint8_t* pWADData, uint32_t offset);
    static uint32_t Read4Bytes(const uint8_t* pWADData, uint32_t offset);
    static uint64_t Read8Bytes(const uint8_t* pWADData, uint32_t offset);
};

