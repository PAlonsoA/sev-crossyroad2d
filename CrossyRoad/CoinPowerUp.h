#pragma once
#include "PowerUp.h"
#include "Audio.h"
class CoinPowerUp :
    public PowerUp
{
public:
	CoinPowerUp(string filename, float x, float y, Game* game);
	void collision() override;
	Audio* audio;
};

