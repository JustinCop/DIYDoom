#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include "Map.h"


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

// WAD Loader for users.
class WADLoader
{
public:
    WADLoader();
    ~WADLoader();

    void SetWADFilePath(std::string sWADFilePath) { m_WADFilePath = sWADFilePath; }

    // Initialize WAD raw data, decide Header and Directory.
    bool LoadWAD();

    // Given a map initialized with map name, find the map and retrieve vertex/lineDef data.
    bool LoadMapData(std::shared_ptr<Map> pMap);

private:
    // helper functions
    
    // Read and save raw data.
    bool OpenAndLoad();
    
    // save Header and Directories.
    bool ReadHeaderAndDirectories();

    // Find the lump index of a named map, i.e. the lump index to m_WADDirectories.
    int FindMapIndex(std::shared_ptr<Map> pMap);
    
    // Read map vertex data and save into a named map.
    bool ReadMapVertex(std::shared_ptr<Map> pMap);
    
    // Read map lineDef data and save into a named map.
    bool ReadMapLineDef(std::shared_ptr<Map> pMap);

    // Read map Thing struct
    bool ReadMapThing(std::shared_ptr<Map> pMap);

    std::string m_WADFilePath;
    std::ifstream m_WADFile;
    std::unique_ptr<const uint8_t[]> m_pWADData;

    WADHeader m_WADHeader;
    std::vector<WADDirectory> m_WADDirectories;
};

