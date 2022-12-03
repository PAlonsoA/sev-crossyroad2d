#include "SpeedPowerUp.h"
#include "GameLayer.h"

SpeedPowerUp::SpeedPowerUp(string filename, float x, float y, Game* game)
	: PowerUp(filename, x, y, game){
}

void SpeedPowerUp::collision()
{
	((GameLayer*)game->gameLayer)->player->moveTime = 2;
}
