#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "WADDecoder.h"
#include "Map.h"

// WAD Loader for users.
class WADLoader
{
public:
    WADLoader(std::string WADFilePath);
    ~WADLoader();

    bool LoadWAD(); // interface to outside
    bool LoadMapData(Map &map);

private:
    // helper functions
    bool OpenAndLoad();     // Read and save raw data
    bool ReadDirectories();

    int FindMapIndex(const Map& map);  // Find the lump index of a map
    bool ReadMapVertex(Map& map);
    bool ReadMapLineDef(Map &map);

    std::string m_WADFilePath;
    std::ifstream m_WADFile;
    uint8_t* m_pWADData;

    WADHeader m_WADHeader;
    std::vector<WADDirectory> m_WADDirectories;
};

