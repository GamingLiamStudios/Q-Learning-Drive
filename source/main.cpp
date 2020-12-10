#include "game.h"

#include <iostream>

int main()
{
    Game game;
    if (game.Construct(WIDTH, HEIGHT, 1, 1, false, true)) game.Start();

    return 0;
}