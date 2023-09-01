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

enum LINE_DEF_FLAGS
{
    eBLOCKING = 0,
    eBLOCK_MONSTERS = 0x1,
    eTWO_SIDED = 0x2,
    eDONT_PEG_TOP = 0x4,
    eDONT_PEG_BOTTOM = 0x8,
    eSECRET = 0x10,
    eBLOCK_SOUND = 0x20,
    eDONT_DRAW = 0x40,
    eDRAW = 0x80
};

struct LineDef
{
    uint16_t startVertex;
    uint16_t endVertex;
    uint16_t lineDefFlags;
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

#define str_THINGS "THINGS"
#define str_LINEDEFS "LINEDEFS"
#define str_SIDEDDEFS "SIDEDDEFS"
#define str_VERTEXES "VERTEXES"
#define str_SEAGS "SEAGS"
#define str_SSECTORS "SSECTORS"
#define str_NODES "NODES"
#define str_SECTORS "SECTORS"
#define str_REJECT "REJECT"
#define str_BLOCKMAP "BLOCKMAP"
