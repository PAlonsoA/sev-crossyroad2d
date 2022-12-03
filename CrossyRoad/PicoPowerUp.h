#pragma once
#include "PowerUp.h"
class PicoPowerUp :
    public PowerUp
{
public:
	PicoPowerUp(string filename, float x, float y, Game* game);
	void collision() override;
};

