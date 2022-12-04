#include "Projectile.h"

Projectile::Projectile(float x, float y, Game* game) :
	Actor("res/tiles/tile_pico.png", x, y, 40, 40, game) {
	vx = 9;
	vy = 0;
	initialPosition = x;
}
