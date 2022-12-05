#pragma once

#include "Tile.h"

class River : public Tile
{
public:
	River(string filename, float x, float y, Game* game);
	bool hasLog;
};

