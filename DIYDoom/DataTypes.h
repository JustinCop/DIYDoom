#pragma once
#include <cstdint>
#include <map>

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

struct Vertex
{
    int16_t x;
    int16_t y;
};

struct LineDef
{
    uint16_t startVertex;
    uint16_t endVertex;
    uint16_t flags;
    uint16_t lineTypeAction;
    uint16_t sectorTag;
    uint16_t frontSideDef;  // 0xFFFF side not present
    uint16_t backSideDef;   // 0xFFFF side not present
};

enum MAP_LUMPS_INDEX
{
    eTHINGS = 1, // The WAD file is organized in this order: First the Map name (e.g. E1M1), then THINGS, then other stuffs.
    eLINEDEFS,
    eSIDEDDEFS,
    eVERTEXES,
    eSEAGS,
    eSSECTORS,
    eNODES,
    eSECTORS,
    eREJECT,
    eBLOCKMAP,

    eCOUNT
};

static const std::map<MAP_LUMPS_INDEX, std::string> LumpIndexNames =
{
    {eTHINGS, "THINGS"},
    {eLINEDEFS, "LINEDEFS"},
    {eSIDEDDEFS, "SIDEDDEFS"},
    {eVERTEXES, "VERTEXES"},
    {eSEAGS, "SEAGS"},
    {eSSECTORS, "SSECTORS"},
    {eNODES, "NODES"},
    {eSECTORS, "SECTORS"},
    {eREJECT, "REJECT"},
    {eBLOCKMAP, "BLOCKMAP"}
};