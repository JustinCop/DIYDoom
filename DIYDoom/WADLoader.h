#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "WADReader.h"
#include "Map.h"

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
    bool LoadMapData(Map* pMap);

private:
    // helper functions
    
    // Read and save raw data's pointer.
    bool OpenAndLoad();
    
    // save Header and Directories.
    bool ReadHeaderAndDirectories();

    // Find the lump index of a named map, i.e. the lump index to m_WADDirectories.
    int FindMapIndex(const Map& map);
    
    // Read map vertex data and save into a named map.
    bool ReadMapVertex(Map* pMap);
    
    // Read map lineDef data and save into a named map.
    bool ReadMapLineDef(Map* pMap);

    std::string m_WADFilePath;
    std::ifstream m_WADFile;
    const uint8_t* m_pWADData;

    WADHeader m_WADHeader;
    std::vector<WADDirectory> m_WADDirectories;
};

