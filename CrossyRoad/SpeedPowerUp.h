#pragma once
#include "PowerUp.h"
class SpeedPowerUp :
    public PowerUp
{
public:
	SpeedPowerUp(string filename, float x, float y, Game* game);
	void collision() override;
};

