// World.h

#pragma once



#include "Game.h"

class Region;
class TermBase;
class Map;
class TileBase;
class TileGold;
class TileDoorBump;
class TileDoorLock;
class TileDoorKey;


class World
{
public:
	Region *wRegion;
	//Region *status;
	Region *R1, *R2;
	TermBase *Term;
	Map *map;
	int playerX, playerY;
	int playerGold;
	TileDoorKey *playerKey;
	int statusY;
	int infoY;
	int textXmax;
	char playerC;
	TileBase *tilePlayer;
	TileBase *tileWall;
	TileGold *tileGold1;
	TileDoorBump *tileDoorBump1;
	TileDoorLock *tileDoorLock;
	TileDoorKey *tileDoorKey1;
	TileDoorKey *tileDoorKey2;
	int restoreX, restoreY;
	TileBase *restoreObj;

	World(int x1, int y1, int x2, int y2, TermBase *term);

	// Deletes dynamic memory
	// - untested, probably incomplete
	~World();

	// Fills map with walls, gold, keys, door, etc
	// - calling map->putItem() populates Map and draws it
	void fillMap();

	// Adds Region1 to the game
	// - minimal features
	Region *addRegion1(int wx, int wy, int nx, int ny);

	// Adds a second region to the game
	// - minimal features
	Region *addRegion2(int wx, int wy, int nx, int ny);

	// Prints user's game status (gold, items collected, etc)
	// - doesn't fill remaining line with spaces
	void printStatus();


	// Prints info, typically regarding the current play
	void printInfo(const char *string);


	// Tries to move the player to new coordinates
	// - doesn't enforce step size
	void tryPlay(int tryx, int tryy);


	// Plays the game
	// - needs a way to exit this method
	void start(int x, int y);

};

