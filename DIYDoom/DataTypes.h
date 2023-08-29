#pragma once
#include <cstdint>

struct WADHeader
{
    char type[5];    // either IWAD (official) or PWAD (mod)
    uint32_t directoryCount;
    uint32_t directoryOffset;   // offset to the WAD file start
};

struct WADDirectory
{
    uint32_t lumpOffset;
    uint32_t lumpSize;
    char lumpName[9];
};