#pragma once
#include <string>
#include "WADLoader.h"
#include <SDL.h>

// DoomEngine class will be a class that knows the internals of our 
// Doom clone.It will keep track of the player, the game status, what 
// map is loaded etc.The list will grow as we implement more of the game.
class DoomEngine
{
public:
    DoomEngine();
    ~DoomEngine();

    virtual void Render(SDL_Renderer *pRenderer); // Draw something to show on screen
    virtual void KeyPressed(const SDL_Event &event); // Which keys are pressed?
    virtual void KeyReleased(const SDL_Event &event); // Which keys are released?
    virtual void Quit(); // Close and shutdown the game
    virtual void Update();

    virtual bool IsOver() const { return m_bIsOver; } // Did the game end?
    virtual bool Init(); // Initialize game object

    virtual int GetRenderWidth() const { return m_iRenderWidth; }
    virtual int GetRenderHeight() const { return m_iRenderHeight; }
    virtual int GetTimePerFrame() const { return 1000 / 60; }   // in micro-seconds.

    virtual std::string GetName() const { return "DIYDOOM"; }
    virtual std::string GetWADFileName() const { return "../assets/doom.wad"; }

protected:
    int m_iRenderWidth;
    int m_iRenderHeight;

    bool m_bIsOver;

    WADLoader m_WADLoader; // Now the game engine will own the loader
    Map *m_pMap; // Also the map will be owned by the engine.
    std::shared_ptr<Player> m_pPlayer;
};

