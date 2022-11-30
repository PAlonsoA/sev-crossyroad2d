#include "Log.h"

Log::Log(string filename, float x, float y, Game* game)
	: Tile(filename, x, y, game)
{
	vy = -1;
}

void Log::update()
{
	y += vy;
	
	if (y < -40 && vy < 0) {
		y = HEIGHT + 40;
	}
}
