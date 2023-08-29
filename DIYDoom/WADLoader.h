#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "WADDecoder.h"

// WAD Loader for users.
class WADLoader
{
public:
    WADLoader(std::string WADFilePath);
    ~WADLoader();

    bool LoadWAD(); // interface to outside

private:
    // helper functions
    bool OpenAndLoad();     // Read and save raw data
    bool ReadDirectories();

    std::string m_WADFilePath;
    std::ifstream m_WADFile;
    uint8_t* m_pWADData;

    WADHeader m_WADHeader;
    std::vector<WADDirectory> m_WADDirectories;
};

