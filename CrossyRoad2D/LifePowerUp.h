#pragma once
#include "PowerUp.h"
class LifePowerUp :
    public PowerUp
{
public:
	LifePowerUp(string filename, float x, float y, Game* game);
	void collision() override;
};

