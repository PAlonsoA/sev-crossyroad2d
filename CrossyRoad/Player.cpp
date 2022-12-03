#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador.png", x, y, 40, 40, game) {
	onAir = false;
	orientation = game->orientationRight;
	state = game->stateMoving;
	audioShoot = Audio::createAudio("res/efecto_disparo.wav", false);

	aJumpingRight = new Animation("res/jugador_saltando_derecha.png",
		width, height, 160, 40, 6, 4, true, game);
	aJumpingLeft = new Animation("res/jugador_saltando_izquierda.png",
		width, height, 160, 40, 6, 4, true, game);
	aShootingRight = new Animation("res/jugador_disparando_derecha.png",
		width, height, 160, 40, 6, 4, false, game);
	aShootingLeft = new Animation("res/jugador_disparando_izquierda.png",
		width, height, 160, 40, 6, 4, false, game);
	aIdleRight = new Animation("res/jugador_idle_derecha.png", width, height,
		320, 40, 6, 8, true, game);
	aIdleLeft = new Animation("res/jugador_idle_izquierda.png", width, height,
		320, 40, 6, 8, true, game);
	aRunningRight = new Animation("res/jugador_corriendo_derecha.png", width, height,
		320, 40, 6, 8, true, game);
	aRunningLeft = new Animation("res/jugador_corriendo_izquierda.png", width, height,
		320, 40, 6, 8, true, game);
	animation = aIdleRight;
}

void Player::update() {
	if (invulnerableTime > 0) {
		invulnerableTime--;
	}

	bool endAnimation = animation->update();

	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba disparando
		if (state == game->stateShooting) {
			state = game->stateMoving;
		}
	}

	// Establecer orientación
	if (vx > 0) {
		orientation = game->orientationRight;
	}
	if (vx < 0) {
		orientation = game->orientationLeft;
	}

	// Selección de animación basada en estados
	if (state == game->stateShooting) {
		if (orientation == game->orientationRight) {
			animation = aShootingRight;
		}
		if (orientation == game->orientationLeft) {
			animation = aShootingLeft;
		}
	}

	if (state == game->stateMoving) {
		if (vx != 0) {
			if (orientation == game->orientationRight) {
				animation = aRunningRight;
			}
			if (orientation == game->orientationLeft) {
				animation = aRunningLeft;
			}
		}
		if (vx == 0) {
			if (orientation == game->orientationRight) {
				animation = aIdleRight;
			}
			if (orientation == game->orientationLeft) {
				animation = aIdleLeft;
			}
		}
	}

	if (shootTime > 0) {
		shootTime--;
	}
	if (moveTimeX > 0) {
		moveTimeX--;
	}
	if (moveTimeY > 0) {
		moveTimeY--;
	}
}

void Player::moveX(float axis) {
	if (moveTimeX == 0 && axis != 0) {
		vx = axis * 40;
		moveTimeX = 5;
	}
	else
		vx = 0;
}

void Player::moveY(float axis) {
	if (moveTimeY == 0 && axis != 0) {
		vy = axis * 40;
		moveTimeY = 5;
	}
	else
		vy = 0;
}

void Player::draw(float scrollX) {
	if (invulnerableTime == 0) {
		animation->draw(x - scrollX, y);
	}
	else {
		if (invulnerableTime % 10 >= 0 && invulnerableTime % 10 <= 5) {
			animation->draw(x - scrollX, y);
		}
	}
}

void Player::loseLife(int lifes) {
	if (invulnerableTime <= 0) {
		if (this->lifes > 0) {
			this->lifes -= lifes;
			invulnerableTime = 100;
			// 100 actualizaciones 
		}
	}
}

Projectile* Player::shoot() {
	if (shootTime == 0) {
		state = game->stateShooting;
		audioShoot->play();
		shootTime = shootCadence;
		aShootingLeft->currentFrame = 0; //"Rebobinar" animación
		aShootingRight->currentFrame = 0; //"Rebobinar" animación
		Projectile* projectile = new Projectile(x, y, game);
		if (orientation == game->orientationLeft) {
			projectile->vx = projectile->vx * -1; // Invertir
		}
		return projectile;
	}
	else {
		return NULL;
	}
}
