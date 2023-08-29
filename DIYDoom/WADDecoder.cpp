#include "WADDecoder.h"
#include <cstring>

void WADDecoder::ReadHeaderData(const uint8_t* pWADData, WADHeader& header)
{
    // first 4 bytes for "IWAD" or "PWAD"
    memcpy(header.type, pWADData, 4);
    header.type[4] = '\0';

    //0x04 to 0x07
    header.directoryCount = Read4Bytes(pWADData, 4);
    
    //0x08 to 0x0b
    header.directoryOffset = Read4Bytes(pWADData, 8);
}

void WADDecoder::ReadDirectoryData(const uint8_t* pWADData, uint32_t offset, WADDirectory& directory)
{
    //0x00 to 0x03
    directory.lumpOffset = Read4Bytes(pWADData, offset);

    //0x04 to 0x07
    directory.lumpSize = Read4Bytes(pWADData, offset + 4);

    //0x08 to 0x0F
    memcpy(directory.lumpName, pWADData + offset + 8, 8);
    directory.lumpName[8] = '\0';
}

uint16_t WADDecoder::Read2Bytes(const uint8_t* pWADData, uint32_t offset)
{
    uint16_t value;
    memcpy(&value, pWADData + offset, sizeof(uint16_t));
    return value;
}

uint32_t WADDecoder::Read4Bytes(const uint8_t* pWADData, uint32_t offset)
{
    uint32_t value;
    memcpy(&value, pWADData + offset, sizeof(uint32_t));
    return value;
}

uint64_t WADDecoder::Read8Bytes(const uint8_t* pWADData, uint32_t offset)
{
    uint64_t value;
    memcpy(&value, pWADData + offset, sizeof(uint64_t));
    return value;
}
