#pragma once
#include "DoomEngine.h"

class Game
{
public:
    Game();
    virtual ~Game();

    void ProcessInput(); // Read user input
    void Render(); // Draw on the screen
    void Update(); // Update the world
    void Delay(); // A function that would just wait for an amount of time

    bool IsOver(); // Check if game is over
    bool Init(); // Initialize our objects

protected:
    int m_iWindowWidth;
    int m_iWindowHeight;

    SDLWindowPtr m_pWindow; // The windows we will create
    SDLRendererPtr m_pRenderer; // Where we will render our frames
    std::unique_ptr<DoomEngine> m_pDoomEngine; // This is an object that knows more details about game internal
};

