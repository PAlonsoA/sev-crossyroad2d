#pragma once
#include "Tile.h"
class Obstacle :
    public Tile
{
public:
    Obstacle(string filename, float x, float y, Game* game);
};

