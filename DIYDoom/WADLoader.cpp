#include "WADLoader.h"
#include <iostream>

WADLoader::WADLoader(std::string WADFilePath) :
    m_WADFilePath(WADFilePath),
    m_pWADData(nullptr)
{
}

WADLoader::~WADLoader()
{
}

bool WADLoader::LoadWAD()
{
    if (!OpenAndLoad())
    {
        return false;
    }

    if (!ReadDirectories())
    {
        return false;
    }

    return true;
}


bool WADLoader::OpenAndLoad()
{
    m_WADFile.open(m_WADFilePath, std::ifstream::binary);
    if (!m_WADFile.is_open())
    {
        std::cout << "Error: Failed to open WAD file" << m_WADFilePath << std::endl;
        return false;
    }

    // Move file ptr to the end, to check file size
    m_WADFile.seekg(0, m_WADFile.end);
    size_t length = m_WADFile.tellg();
    m_pWADData = new uint8_t[length];

    // Move back to beginning to copy data
    m_WADFile.seekg(0, m_WADFile.beg);
    m_WADFile.read((char*)m_pWADData, length);
    m_WADFile.close();

    return true;
}

bool WADLoader::ReadDirectories()
{
    WADHeader header;
    WADDecoder::ReadHeaderData(m_pWADData, header);

    std::cout << header.type << std::endl;
    std::cout << header.directoryCount << std::endl;
    std::cout << header.directoryOffset << std::endl;
    std::cout << std::endl << std::endl;

    for (uint32_t i = 0; i < header.directoryCount; ++i)
    {
        WADDirectory dir;
        WADDecoder::ReadDirectoryData(m_pWADData, header.directoryOffset + i * 16, dir);
        m_WADDirectories.push_back(dir);

        std::cout << dir.lumpOffset << std::endl;
        std::cout << dir.lumpSize << std::endl;
        std::cout << dir.lumpName << std::endl;
        std::cout << std::endl << std::endl;

    }

    return true;
}
