#pragma once

#include "Tile.h"

class PowerUp : public Tile
{
public:
	PowerUp(string filename, float x, float y, Game* game);
	virtual void collision();
};
