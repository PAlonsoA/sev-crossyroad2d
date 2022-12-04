#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"

#include "Enemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Tile.h"

#include "Audio.h" 
#include "Space.h"

#include <fstream> // Leer ficheros
#include <sstream> // Leer líneas / String
#include <list>

#include "River.h"
#include "Log.h"
#include "PowerUp.h"
#include "CoinPowerUp.h"
#include "LifePowerUp.h"
#include "SpeedPowerUp.h"
#include "PicoPowerUp.h"
#include "Obstacle.h"

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);
	void mouseToControls(SDL_Event event); // USO DE MOUSE
	void loadMap(string name);
	void loadMapObject(char character, float x, float y);
	int mapWidth;
	void calculateScroll();
	Actor* message;
	bool pause;
	Space* space;
	float scrollX;
	list<Tile*> tiles;

	Audio* audioBackground;
	Text* textCoins;
	int coins;
	int maxCoins = 0;
	int newEnemyTime = 0;
	Player* player;
	Background* background;
	Actor* backgroundPoints;
	list<Enemy*> enemies;
	list<Projectile*> projectiles;

	bool controlContinue = false;
	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;
	bool moving = false;
	int timeNotMoving = 60;

	list<River*> river;
	list<Log*> logs;
	list<PowerUp*> powerUps;
	list<Obstacle*> obstacles;

	Text* textLifes;
	Actor* backgroundLifes;
	Text* textShots;
	Actor* backgroundShots;
};

