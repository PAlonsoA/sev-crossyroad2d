#include "CoinPowerUp.h"
#include "GameLayer.h"

CoinPowerUp::CoinPowerUp(string filename, float x, float y, Game* game)
	: PowerUp(filename, x, y, game){
	audio = Audio::createAudio("res/sounds/coin_sound.wav", false);
}

void CoinPowerUp::collision()
{
	((GameLayer*)game->gameLayer)->coins += 1;
	((GameLayer*)game->gameLayer)->textCoins->content = to_string(((GameLayer*)game->gameLayer)->coins) + " I " + to_string(((GameLayer*)game->gameLayer)->maxCoins);
	audio->play();
}
