#include "DoomEngine.h"

DoomEngine::DoomEngine():
    m_pRenderer(nullptr),
    m_bIsOver(false),
    m_pMap(nullptr),
    m_iRenderWidth(320),
    m_iRenderHeight(200)
{
}

DoomEngine::~DoomEngine()
{
    delete m_pMap;
}

void DoomEngine::Render()
{
    SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(m_pRenderer);
    m_pMap->RenderAutoMap(m_pRenderer);
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

bool DoomEngine::Init(SDL_Renderer* pRenderer)
{
    m_pRenderer = pRenderer;
    m_pPlayer = std::make_shared<Player>(1);
    m_pMap = new Map("E1M1", m_pPlayer);

    m_WADLoader.SetWADFilePath(GetWADFileName());
    m_WADLoader.LoadWAD();
    m_WADLoader.LoadMapData(m_pMap);
    return true;
}
