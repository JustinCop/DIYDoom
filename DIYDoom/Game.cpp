#include "Game.h"
#include <iostream>
#include <string>

Game::Game() :
    m_iWindowWidth(WINDOW_WIDTH),
    m_iWindowHeight(WINDOW_HEIGHT),
    m_pRenderer(nullptr),
    m_pWindow(nullptr),
    m_pDoomEngine(nullptr)
{
}

Game::~Game()
{
    SDL_DestroyRenderer(m_pRenderer);
    SDL_DestroyWindow(m_pWindow);
    SDL_Quit();
}


bool Game::IsOver()
{
    return m_pDoomEngine->IsOver();
}

bool Game::Init()
{
    // Create a DoomEngine
    m_pDoomEngine = std::make_unique<DoomEngine>();

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL failed to initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create a window with a specific size, here you can set whatever resolution you prefer.
    m_pWindow = SDL_CreateWindow(m_pDoomEngine->GetName().c_str(),
                                 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                 m_iWindowWidth, m_iWindowHeight,
                                 SDL_WINDOW_SHOWN);
    if (m_pWindow == nullptr)
    {
        std::cout << "SDL failed to create window! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_SOFTWARE);
    if (m_pRenderer == nullptr)
    {
        std::cout << "SDL failed to create renderer! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetRenderDrawColor(m_pRenderer, 0xff, 0xff, 0xff, 0xff);

    // Sets the logical size of the screen, that means we can draw on a specific render size then, SDL will 
    // automatically stretch it to the window size. This will make the development independent from window resolution
    if (SDL_RenderSetLogicalSize(m_pRenderer, m_pDoomEngine->GetRenderWidth(), m_pDoomEngine->GetRenderHeight()) != 0)
    {
        std::cout << "SDL failed to set logical size! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Use the renderer to Init DoomEngine.
    // NOTE! Must initialize renderer before using it!
    if (!m_pDoomEngine->Init(m_pRenderer))
    {
        std::cout << m_pDoomEngine->GetName() << " failed to initialize!" << std::endl;
        return false;
    }

    return true;
}


// All what process input need to do is just forward the events to the 
// DoomEngine to take appropriate action
void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            m_pDoomEngine->KeyPressed(event);
            break;
        case SDL_KEYUP:
            m_pDoomEngine->KeyReleased(event);
            break;
        case SDL_QUIT:
            m_pDoomEngine->Quit();
            break;
        }
    }
}

void Game::Render()
{
    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 0xff);
    SDL_RenderClear(m_pRenderer);

    // GameObjects to draw themselves
    m_pDoomEngine->Render();

    SDL_RenderPresent(m_pRenderer);
}

void Game::Delay()
{
    SDL_Delay(m_pDoomEngine->GetTimePerFrame());
}

void Game::Update()
{
    m_pDoomEngine->Update();
}

