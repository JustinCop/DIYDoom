#include "WADLoader.h"

int main()
{
    WADLoader wadLoader("../assets/doom.wad");
    wadLoader.LoadWAD();

    Map map("E1M1");
    wadLoader.LoadMapData(map);

    return 0;
}