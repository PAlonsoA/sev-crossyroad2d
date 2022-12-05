#pragma once

#include "Actor.h"
#include "Animation.h" 

class Car : public Actor
{
public:
	Car(float x, float y, Game* game);
	void draw(float scrollX = 0) override; // Va a sobrescribir
	void update();
	void impacted(); // Recibe impacto y pone animaci�n de morir
	int state;
	Animation* aDying;
	Animation* aMoving;
	Animation* animation; // Referencia a la animaci�n mostrada
	int speed[4] = {-2, -1, 1, 2};
};
