#include "WADLoader.h"

int main()
{
    WADLoader wadLoader("../assets/doom.wad");
    wadLoader.LoadWAD();

    return 0;
}