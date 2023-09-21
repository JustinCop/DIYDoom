#include "DoomEngine.h"

DoomEngine::DoomEngine():
    m_bIsOver(false),
    m_iRenderWidth(320),
    m_iRenderHeight(200)
{
    m_pMap = new Map("E1M1");
}

DoomEngine::~DoomEngine()
{
    delete m_pMap;
}

void DoomEngine::Render(SDL_Renderer* pRenderer)
{
    SDL_SetRenderDrawColor(pRenderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(pRenderer);
}

void DoomEngine::KeyPressed(const SDL_Event& event)
{
    switch (event.key.keysym.sym)
    {
    case SDLK_UP:
        break;
    case SDLK_DOWN:
        break;
    case SDLK_LEFT:
        break;
    case SDLK_RIGHT:
        break;
    case SDLK_ESCAPE:
        Quit();
        break;
    default:
        break;
    }
}

void DoomEngine::KeyReleased(const SDL_Event& event)
{
}

void DoomEngine::Quit()
{
    m_bIsOver = true;
}

void DoomEngine::Update()
{
}

std::string DoomEngine::GetWADFileName()
{
    return "../assets/doom.wad";
}

bool DoomEngine::Init()
{
    m_WADLoader.SetWADFilePath(GetWADFileName());
    m_WADLoader.LoadWAD();
    m_WADLoader.LoadMapData(m_pMap);
    return true;
}
