#include "DoomEngine.h"

DoomEngine::DoomEngine():
    m_pRenderer(nullptr),
    m_bIsOver(false),
    m_pMap(nullptr),
    m_iRenderWidth(RENDER_WIDTH),
    m_iRenderHeight(RENDER_HEIGHT)
{
}

DoomEngine::~DoomEngine()
{
}

void DoomEngine::Render()
{
    SDL_SetRenderDrawColor(m_pRenderer.get(), 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(m_pRenderer.get());
    m_pMap->RenderAutoMap();
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

bool DoomEngine::Init(SDLRendererPtr pRenderer)
{
    m_pRenderer = pRenderer;
    m_pPlayer = std::make_shared<Player>(1);
    m_pMap = std::make_shared<Map>("E1M1", m_pPlayer, pRenderer);

    m_WADLoader.SetWADFilePath(GetWADFileName());
    m_WADLoader.LoadWAD();
    m_WADLoader.LoadMapData(m_pMap);
    return true;
}
