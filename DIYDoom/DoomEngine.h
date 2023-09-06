#pragma once
#include <string>
#include "WADLoader.h"

// DoomEngine class will be a class that knows the internals of our 
// Doom clone.It will keep track of the player, the game status, what 
// map is loaded etc.The list will grow as we implement more of the game.
class DoomEngine
{
public:
    DoomEngine();
    ~DoomEngine();

    virtual void Render(SDL_Renderer *pRenderer); // Draw something to show on screen
    virtual void KeyPressed(SDL_Event &event); // Which keys are pressed?
    virtual void KeyReleased(SDL_Event &event); // Which keys are released?
    virtual void Quit(); // Close and shutdown the game
    virtual void Update();

    virtual bool IsOver(); // Did the game end?
    virtual bool Init(); // Initialize game object

    virtual int GetRenderWidth();
    virtual int GetRenderHeight();
    virtual int GetTimePerFrame();

    virtual std::string GetName();
    virtual std::string GetWADFileName();

protected:
    int m_iRenderWidth;
    int m_iRenderHeight;

    bool m_bIsOver;

    WADLoader m_WADLoader; // Now the game engine will own the loader
    Map *m_pMap; // Also the map will be owned by the engine.
};

