#pragma once

#include "Tile.h"

class Log : public Tile
{
public:
	Log(string filename, float x, float y, Game* game);
	void update();
};
