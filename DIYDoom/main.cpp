#include <iostream>
#include "Game.h"

// SDL2 will define main as SDL_main.
// must add the arguments (int argc, char* argv[]) to keep consistance
int main(int argc, char* argv[])
{
    Game game;
    game.Init();

    while (!game.IsOver())
    {
        game.ProcessInput();
        game.Update();
        game.Render();
        game.Delay();
    }

    return 0;
}