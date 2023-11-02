#pragma once
#include <cstdint>
#include <map>
#include <SDL.h>
#include <memory>

#define RENDER_WIDTH 320
#define RENDER_HEIGHT 200
#define WINDOW_WIDTH (RENDER_WIDTH * 4)
#define WINDOW_HEIGHT (RENDER_HEIGHT * 4)
#define AUTO_MAP_SCALE_FACTOR 15

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

struct Thing
{
    int16_t xPosition;
    int16_t yPosition;
    uint16_t direction;
    uint16_t type;
    uint16_t flags;
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

struct Node
{
    int16_t xPartition;
    int16_t yPartition;
    int16_t changeXPartition;
    int16_t changeYPartition;

    int16_t rightBoxTop;
    int16_t rightBoxBottom;
    int16_t rightBoxLeft;
    int16_t rightBoxRight;

    int16_t leftBoxTop;
    int16_t leftBoxBottom;
    int16_t leftBoxLeft;
    int16_t leftBoxRight;

    uint16_t rightChildID;
    uint16_t leftChildID;
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

// Define a smart pointer for SDL
struct SDLRendererDestroyer
{
    void operator()(SDL_Renderer* r) const
    {
        SDL_DestroyRenderer(r);
    }
};

using SDLRendererPtr = std::shared_ptr<SDL_Renderer>;

struct SDLWindowDestroyer
{
    void operator()(SDL_Window* w) const
    {
        SDL_DestroyWindow(w);
    }
};

using SDLWindowPtr = std::shared_ptr<SDL_Window>;