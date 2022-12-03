#include "LifePowerUp.h"
#include "GameLayer.h"

LifePowerUp::LifePowerUp(string filename, float x, float y, Game* game)
	: PowerUp(filename, x, y, game){
}

void LifePowerUp::collision()
{
	((GameLayer*)game->gameLayer)->player->lifes += 1;
	((GameLayer*)game->gameLayer)->textLifes->content = to_string(((GameLayer*)game->gameLayer)->player->lifes);
}
