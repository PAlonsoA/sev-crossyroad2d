#include "Enemy.h"

Enemy::Enemy(float x, float y, Game* game)
	: Actor("res/cars/coche_rojo_1.png", x, y, 40, 60, game) {

	vy = speed[rand() % 4];

	state = game->stateMoving;

	if (vy < 0) {
		aDying = new Animation("res/cars/coche_rojo_1_impacto.png", width, height,
			160, 60, 1, 4, false, game);
		aMoving = new Animation("res/cars/coche_rojo_1_movimiento.png", width, height,
			160, 60, 1, 4, true, game);
	}
	else if (vy > 0) {
		aDying = new Animation("res/cars/coche_rojo_2_impacto.png", width, height,
			160, 60, 1, 4, false, game);
		aMoving = new Animation("res/cars/coche_rojo_2_movimiento.png", width, height,
			160, 60, 1, 4, true, game);
	}

	animation = aMoving;
}

void Enemy::update() {
	// Actualizar la animación
	bool endAnimation = animation->update();

	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba muriendo
		if (state == game->stateDying) {
			state = game->stateDead;
		}
	}

	if (state == game->stateMoving) {
		animation = aMoving;
	}
	if (state == game->stateDying) {
		animation = aDying;
	}

	// Establecer velocidad
	if (state == game->stateDying) {
		vy = 0;
	}

	if (y > HEIGHT + 50 && vy > 0) {
		y = -50;
	}
	else if (y < -50 && vy < 0) {
		y = HEIGHT + 50;
	}
}

void Enemy::impacted() {
	if (state != game->stateDying) {
		state = game->stateDying;
	}
}

void Enemy::draw(float scrollX) {
	animation->draw(x - scrollX, y);
}

