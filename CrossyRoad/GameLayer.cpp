#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
	//llama al constructor del padre : Layer(renderer)
	pause = true;
	message = new Actor("res/boton_pausa.png", WIDTH * 0.5, HEIGHT * 0.5,
		40, 40, game);
	init();
}

void GameLayer::init() {
	space = new Space(0);
	scrollX = 0;
	tiles.clear();

	audioBackground = Audio::createAudio("res/sounds/crossy_road_music.mp3", true);
	audioBackground->play();

	coins = 0;
	maxCoins = 0;
	textCoins = new Text("hola", WIDTH * 0.92, HEIGHT * 0.045, game);
	textCoins->content = "OI" + to_string(maxCoins);

	background = new Background("res/backgrounds/fondo_lava.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	backgroundPoints = new Actor("res/tiles/tile_coin.png",
		WIDTH * 0.82, HEIGHT * 0.05, 40, 40, game);

	cars.clear(); // Vaciar por si reiniciamos el juego
	projectiles.clear(); // Vaciar por si reiniciamos el juego

	river.clear();
	logs.clear();
	powerUps.clear();
	obstacles.clear();

	loadMap("res/levels/" + to_string(game->currentLevel) + ".txt");
	textLifes = new Text("O", WIDTH * 0.12, HEIGHT * 0.05, game);
	textLifes->content = to_string(player->lifes);
	backgroundLifes = new Actor("res/tiles/tile_corazon.png", WIDTH * 0.06, HEIGHT * 0.06, 44, 36, game);

	textShots = new Text("O", WIDTH * 0.28, HEIGHT * 0.045, game);
	backgroundShots = new Actor("res/tiles/tile_pico.png", WIDTH * 0.22, HEIGHT * 0.05, 40, 40, game);

	timeNotMoving = 60;
}
void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		// Cambio automático de input
		if (event.type == SDL_KEYDOWN) {
			game->input = game->inputKeyboard;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			game->input = game->inputMouse;
		}
		// Procesar teclas
		if (game->input == game->inputKeyboard) {
			keysToControls(event);
		}
		if (game->input == game->inputMouse) {
			mouseToControls(event);
		}
	}
	//procesar controles
	if (controlContinue) {
		pause = false;
		controlContinue = false;
	}
	// Disparar
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();
		if (newProjectile != NULL) {
			space->addDynamicActor(newProjectile);
			projectiles.push_back(newProjectile);
			if (player->shots != 0)
				textShots->content = to_string(player->shots);
			else
				textShots->content = "O";
		}
	}

	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
		moving = true;
		timeNotMoving = 60;
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
		moving = true;
		timeNotMoving = 60;
	}
	else {
		player->moveX(0);
		moving = false;
	}

	// Eje Y
	if (controlMoveY > 0) {
		player->moveY(1);
		moving = true;
		timeNotMoving = 60;
	}
	else if (controlMoveY < 0) {
		player->moveY(-1);
		moving = true;
		timeNotMoving = 60;
	}
	else {
		player->moveY(0);
		moving = false;
	}
}

void GameLayer::update() {
	if (pause)
		return;
	// Nivel superado
	if (coins == maxCoins) {
		game->currentLevel++;
		if (game->currentLevel > game->finalLevel) {
			game->currentLevel = 0;
		}
		message = new Actor("res/mensaje_ganar.png", WIDTH * 0.5, HEIGHT * 0.5,
			WIDTH, HEIGHT, game);
		pause = true;
		init();
	}
	// Jugador se sale del mapa
	if (player->y >= HEIGHT + 40 || player->y <= -40) {
		restart();
	}
	int posX = player->x;
	int posY = player->y;
	space->update();

	if (!moving) {
		timeNotMoving--;
		if (timeNotMoving == 0) {
			restart();
			return;
		}
	}

	background->update();
	player->update();
	for (auto const& enemy : cars) {
		enemy->update();
	}

	for (auto const& log : logs) {
		log->update();
		for (auto const& riverTile : river) {
			if (log->isOverlap(riverTile)) {
				riverTile->hasLog = true;
			}
		}
		if (log->isOverlap(player) && player->vy == 0) {
			player->y = log->y;
		}

	}

	// Colisiones
	for (auto const& car : cars) {
		if (player->isOverlap(car)) {
			player->loseLife();
			if (player->lifes <= 0) {
				restart();
				return;
			}
			textLifes->content = to_string(player->lifes);
		}
	}

	list<Projectile*> deleteProjectiles;
	list<PowerUp*> deletePowerUps;
	list<Obstacle*> deleteObstacles;

	for (auto const& projectile : projectiles) {
		bool lifeTime = projectile->x >= projectile->initialPosition + 60 || projectile->x <= projectile->initialPosition - 60;
		if (projectile->isInRender(scrollX) == false || projectile->vx == 0 || lifeTime) {
			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();

			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
		else {
			for (auto const& obstacle : obstacles)
				if (projectile->collisionWithStatic && obstacle->collisionWithStatic) {
					bool oInList = std::find(deleteObstacles.begin(),
						deleteObstacles.end(),
						obstacle) != deleteObstacles.end();

					if (!oInList)
						deleteObstacles.push_back(obstacle);

					bool pInList = std::find(deleteProjectiles.begin(),
						deleteProjectiles.end(),
						projectile) != deleteProjectiles.end();

					if (!pInList) {
						deleteProjectiles.push_back(projectile);
					}
				}
		}
	}

	for (auto const& powerUp : powerUps) {
		if (powerUp->isOverlap(player)) {
			powerUp->collision();
			bool pInList = std::find(deletePowerUps.begin(),
				deletePowerUps.end(),
				powerUp) != deletePowerUps.end();

			if (!pInList)
				deletePowerUps.push_back(powerUp);
		}
	}

	// Restart when player falls into the river
	for (auto const& riverTile : river) {
		if (!riverTile->hasLog && riverTile->isOverlap(player)) {
			player->loseLife(player->lifes);
			restart();
			return;
		}
		riverTile->hasLog = false;
	}

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		space->removeDynamicActor(delProjectile);
		delete delProjectile;
	}
	deleteProjectiles.clear();

	for (auto const& delPowerUp : deletePowerUps) {
		powerUps.remove(delPowerUp);
		space->removeDynamicActor(delPowerUp);
		delete delPowerUp;
	}
	deletePowerUps.clear();

	for (auto const& delObstacle : deleteObstacles) {
		obstacles.remove(delObstacle);
		space->removeStaticActor(delObstacle);
		delete delObstacle;
	}
	deleteObstacles.clear();
}

void GameLayer::draw() {
	calculateScroll();
	background->draw();
	
	for (auto const& tile : tiles) {
		tile->draw(scrollX);
	}

	for (auto const& riverTile : river) {
		riverTile->draw(scrollX);
	}
	
	for (auto const& obstacle : obstacles) {
		obstacle->draw(scrollX);
	}

	for (auto const& log : logs) {
		log->draw(scrollX);
	}

	for (auto const& powerUp : powerUps) {
		powerUp->draw(scrollX);
	}

	for (auto const& projectile : projectiles) {
		projectile->draw(scrollX);
	}
	player->draw(scrollX);
	for (auto const& car : cars) {
		car->draw(scrollX);
	}

	backgroundPoints->draw();
	textCoins->draw();
	backgroundLifes->draw();
	textLifes->draw();
	backgroundShots->draw();
	textShots->draw();

	// HUD

	if (pause)
		message->draw();

	SDL_RenderPresent(game->renderer); // Renderiza
}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_SPACE: // dispara
			controlShoot = true;
			break;
		}
	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		}
	}
}

void GameLayer::mouseToControls(SDL_Event event) {
	// Modificación de coordenadas por posible escalado
	float motionX = event.motion.x / game->scaleLower;
	float motionY = event.motion.y / game->scaleLower;
	// Cada vez que hacen click
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		controlContinue = true;
	}
}

void GameLayer::loadMap(string name) {
	char character;
	string line;
	ifstream streamFile(name.c_str());
	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de mapa" << endl;
		return;
	}
	else {
		// Por línea
		for (int i = 0; getline(streamFile, line); i++) {
			istringstream streamLine(line);
			mapWidth = line.length() * 40; // Ancho del mapa en pixels
			// Por carácter (en cada línea)
			for (int j = 0; !streamLine.eof(); j++) {
				streamLine >> character; // Leer character 
				cout << character;
				float x = 40 / 2 + j * 40; // x central
				float y = 40 + i * 40; // y suelo
				loadMapObject(character, x, y);
			}

			cout << character << endl;
		}
	}
	streamFile.close();
}

void GameLayer::loadMapObject(char character, float x, float y) {
	switch (character) {
		case 'C': {
			loadMapObject('X', x, y);
			PowerUp* coin = new CoinPowerUp("res/tiles/tile_coin.png", x, y, game);
			// modificación para empezar a contar desde el suelo.
			coin->y = coin->y - coin->height / 2;
			space->addDynamicActor(coin);
			powerUps.push_back(coin);
			maxCoins++;
			textCoins->content = "O I " + to_string(maxCoins);
			break;
		}
		case 'H': {
			loadMapObject('X', x, y);
			PowerUp* life = new LifePowerUp("res/tiles/tile_corazon.png", x, y, game);
			// modificación para empezar a contar desde el suelo.
			life->y = life->y - life->height / 2;
			space->addDynamicActor(life);
			powerUps.push_back(life);
			break;
		}
		case 'S': {
			loadMapObject('X', x, y);
			PowerUp* speed = new SpeedPowerUp("res/tiles/tile_speed_powerup.png", x, y, game);
			// modificación para empezar a contar desde el suelo.
			speed->y = speed->y - speed->height / 2;
			space->addDynamicActor(speed);
			powerUps.push_back(speed);
			break;
		}
		case 'P': {
			loadMapObject('X', x, y);
			PowerUp* pico = new PicoPowerUp("res/tiles/tile_pico.png", x, y, game);
			// modificación para empezar a contar desde el suelo.
			pico->y = pico->y - pico->height / 2;
			space->addDynamicActor(pico);
			powerUps.push_back(pico);
			break;
		}
		case 'E': {
			loadMapObject('.', x, y);
			Car* car = new Car(x, y, game);
			// modificación para empezar a contar desde el suelo.
			car->y = car->y - car->height / 2;
			cars.push_back(car);
			space->addDynamicActor(car);
			break;
		}
		case '1': {
			loadMapObject('X', x, y);
			player = new Player(x, y, game);
			// modificación para empezar a contar desde el suelo.
			player->y = player->y - player->height / 2;
			space->addDynamicActor(player);
			break;
		}
		case 'X': {
			Tile* tile = new Tile("res/tiles/tile_hierba.png", x, y, game);
			// modificación para empezar a contar desde el suelo.
			tile->y = tile->y - tile->height / 2;
			tiles.push_back(tile);
			break;
		};
		case 'O': {
			loadMapObject('X', x, y);
			string obst[2] = { "arbol", "piedra" };
			Obstacle* obstacle = new Obstacle("res/tiles/tile_" + obst[rand() % 2] + ".png", x, y, game);
			// modificación para empezar a contar desde el suelo.
			obstacle->y = obstacle->y - obstacle->height / 2;
			space->addStaticActor(obstacle);
			obstacles.push_back(obstacle);
			break;
		}
		case 'R': {
			River* riverTile = new River("res/tiles/tile_rio.png", x, y, game);
			// modificación para empezar a contar desde el suelo.
			riverTile->y = riverTile->y - riverTile->height / 2;
			river.push_back(riverTile);
			break;
		}
		case 'L': {
			loadMapObject('R', x, y);
			Log* log = new Log("res/tiles/tile_tronco.png", x, y, game);
			log->y = log->y - log->height / 2;
			logs.push_back(log);
			space->addDynamicActor(log);
			break;
		}
		case '.': {
			Tile* tile = new Tile("res/tiles/tile_carretera.png", x, y, game);
			// modificación para empezar a contar desde el suelo.
			tile->y = tile->y - tile->height / 2;
			tiles.push_back(tile);
			break;
		}

	}
}

void GameLayer::restart()
{
	init();
	message = new Actor("res/mensaje_perder.png", WIDTH * 0.5, HEIGHT * 0.5,
			WIDTH, HEIGHT, game);
	pause = true;
}

void GameLayer::calculateScroll() {
	// limite izquierda
	if (player->x > WIDTH * 0.3) {
		if (player->x - scrollX < WIDTH * 0.3) {
			scrollX = player->x - WIDTH * 0.3;
		}
	}

	// limite derecha
	if (player->x < mapWidth - WIDTH * 0.3) {
		if (player->x - scrollX > WIDTH * 0.7) {
			scrollX = player->x - WIDTH * 0.7;
		}
	}
}

