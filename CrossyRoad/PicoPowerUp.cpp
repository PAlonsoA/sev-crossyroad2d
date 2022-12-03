#include "PicoPowerUp.h"
#include "GameLayer.h"

PicoPowerUp::PicoPowerUp(string filename, float x, float y, Game* game)
	: PowerUp(filename, x, y, game){
}

void PicoPowerUp::collision()
{
	((GameLayer*)game->gameLayer)->player->shots = 3;
	((GameLayer*)game->gameLayer)->textShots->content = to_string(((GameLayer*)game->gameLayer)->player->shots += 3);
}
