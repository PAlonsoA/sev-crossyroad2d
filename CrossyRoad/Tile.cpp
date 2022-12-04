#include "Tile.h"

Tile::Tile(string filename, float x, float y, Game* game)
	: Actor(filename, x, y, 40, 40, game) {
	steppedTimes = 0;
}
