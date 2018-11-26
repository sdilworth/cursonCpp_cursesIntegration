// World.cpp
// TRL4, 11/24/2018, DSD

#include "World.h"

World::World(int x1, int y1, int x2, int y2, TermBase *term)
{
	Term = term;

	// setup status and text space
	textXmax = x2;
	statusY = y2;
	infoY = statusY + 1;

	// create the map
	map = new Map(x2, y2, statusY, term);	// oversized by x1,y1
	// create objects for the map

	tilePlayer = new TileBase(TILE_PLAYER);
	tileGold1 = new TileGold(42, TILE_GOLD);
	tileDoorBump1 = new TileDoorBump(TILE_DOOR_BUMP);
	tileDoorLock = new TileDoorLock(DOOR_CODE_1, TILE_DOOR_LOCK);
	tileDoorKey1 = new TileDoorKey(DOOR_CODE_1, TILE_DOOR_KEY);
	tileDoorKey2 = new TileDoorKey(DOOR_CODE_2, TILE_DOOR_KEY);

	tileWall = new TileBase(TILE_WALL);
	wRegion = new Region(x1, y1, x2, y2, map, tileWall);			// call after tileWall created

	// abandoned allocate status area as a Region
	//status = new Region(wx, statusY, nx, 3, map,tileWall);
	
	R1 = nullptr;					// regions will be created in fillMap()
	R2 = nullptr;
	playerX = playerY = -1;			// player doesn't exist yet
	playerC = '0';
	playerGold = 0;
	playerKey = nullptr;

	restoreX = restoreY = -1;		// restore a Tile that user stand on (key swap)
	restoreObj = nullptr;
}

// Deletes dynamic memory
// - untested, probably incomplete
World::~World()
{
	if (map != nullptr)
	{
		delete map;
		map = nullptr;
	}
	if (wRegion != nullptr)
	{
		delete wRegion;
		wRegion = nullptr;
	}
	if (R1 != nullptr)
	{
		delete R1;
		R1 = nullptr;
	}
	if (R2 != nullptr)
	{
		delete R2;
		R2 = nullptr;
	}
}

// Fills map with walls, gold, keys, door, etc
// - calling map->putItem() populates Map and draws it
void World::fillMap()
{
	wRegion->draw();
	//status->draw();
	if (R1 != nullptr)
		R1->draw();
	if (R2 != nullptr)
		R2->draw();
	map->putItem(8, 10, tileDoorKey1);						// good key
	map->putItem(27, 16, tileDoorKey2);						// decoy key
	map->putItem(30, 5, tileGold1);
	map->putItem(17, 14, tileDoorBump1);
	map->putItem(34, 14, tileDoorLock);
}

// Adds Region1 to the game
// - minimal features
Region *World::World::addRegion1(int wx, int wy, int nx, int ny)
{
	R1 = new Region(wx, wy, nx, ny, map, tileWall);
	return R1;
}

// Adds a second region to the game
// - minimal features
Region *World::addRegion2(int wx, int wy, int nx, int ny)
{
	R2 = new Region(wx, wy, nx, ny, map, tileWall);
	return R2;
}

// Prints user's game status (gold, items collected, etc)
// - doesn't fill remaining line with spaces (may get artifacts)
void World::printStatus()
{
	Term->cursorXY(4, statusY);
	printf("(%2d,%2d)", playerX, playerY);
	if (playerKey != nullptr)
		printf("  Key:%d", playerKey->doorCode);
	if (playerGold > 0)
		printf("  Gold:%d", playerGold);
}

// Prints info, typically regarding the current play
// - pads the remaining line
void World::printInfo(const char *string)
{
	int i;
	Term->cursorXY(4, infoY);
	printf("%s", string);
	int len = strlen(string);

	for (i = 0; i < textXmax - len - 5; i++)
		printf(" ");
}

// Tries to move the player to new coordinates
// - doesn't enforce step size
// - in PROCESS, set item=0 to move to (tryx,tryy), else return early
// test function for getting name of an object
// - const char *t = typeid(*tilePlayer).name();
// - way to have different types of objects in the same Tile character
void World::tryPlay(int tryx, int tryy)
{
	char text[TEXT_BUFFER_MAX];
	char item;
	printInfo("");
	TileBase *tile = map->getItem(tryx, tryy);
	if (tile == nullptr)
		item = 0;
	else
		item = tile->drawChar;

	// PROCESS GOLD
	if (item == TILE_GOLD)
	{
		//TileGold *gold = (TileGold*)tile;					// avoid avoid c-style cast
		TileGold *gold = dynamic_cast<TileGold*>(tile);		// dynamic_cast is "more better"
		playerGold += gold->Value;
		sprintf_s(text, TEXT_BUFFER_MAX, "Got the gold %d", gold->Value);
		printInfo(text);
		gold->Value = 0;									// deplete gold from tile
		item = 0;
	}

	// PROCESS TILE_DOOR_BUMP
	if (item == TILE_DOOR_BUMP)
	{
		TileDoorBump *door = dynamic_cast<TileDoorBump*>(tile);
		if (door->doorOpen == false)
		{
			door->doorOpen = true;				// mark open		
			map->clearItem(tryx, tryy);			// deletes tile (didn't need to mark open)
			printInfo("OK, door is open now");
			return;								// it will make the move next (cleared)
		}
	}

	// PROCESS TILE_DOOR_KEY
	if (item == TILE_DOOR_KEY)
	{
		TileDoorKey *key = dynamic_cast<TileDoorKey*>(tile);
		if (playerKey != nullptr)					// does player have a key?
		{
			// we will restore this tile when we move off of it
			restoreX = tryx;
			restoreY = tryy;
			restoreObj = playerKey;
			// swap the keys and move
			sprintf_s(text, TEXT_BUFFER_MAX, "Drop #%d", playerKey->doorCode);
			printInfo(text);
			playerKey = key;
			printStatus();
			item = 0;
		}
		else										// take the key
		{
			playerKey = key;
			//map->clearItem(tryx, tryy);
			sprintf_s(text, TEXT_BUFFER_MAX, "Got key #%d", key->doorCode);
			printInfo(text);
			item = 0;								// make the move
		}
	}
	// PROCESS TILE_DOOR_LOCK
	if (item == TILE_DOOR_LOCK)
	{
		TileDoorLock *door = dynamic_cast<TileDoorLock*>(tile);
		if (playerKey != nullptr)							// do we have any key
		{
			if (playerKey->doorCode == door->doorCode)		// do we have the right key
			{
				printInfo("Opened door");
				item = 0;									// make the move
			}
			else
			{
				sprintf_s(text, TEXT_BUFFER_MAX, "You have key #%d, you need key #%d", playerKey->doorCode, door->doorCode);
				printInfo(text);
				return;								// return w/o moving
			}
		}
		else
		{
			sprintf_s(text, TEXT_BUFFER_MAX, "Need key #%d", door->doorCode);
			printInfo(text);
			return;
		}
	}
	// PROCESS item==0, catchall to move to (tryx,tryy)
	// - move to the try tile if item==0
	if (item == 0)									// no item is at (tryx,tryy)
	{
		map->clearItem(playerX, playerY);			// clear player tile
		playerX = tryx;
		playerY = tryy;
		map->putItem(playerX, playerY, tilePlayer);	// move to try tile
		printStatus();
		if (restoreObj != nullptr)					// restore what we standing on
		{
			if (playerX != restoreX || playerY != restoreY)
			{
				map->putItem(restoreX, restoreY, restoreObj);
				restoreObj = nullptr;
			}
		}
	}
	else									// can't move to the try tile
	{
		Term->Beep();
		printInfo("Only ghosts can walk through walls");
	}
}

// Plays the game
// - place the player, then loop through keys
// - ok to add more synonym keys
// - needs a way to exit this method
void World::start(int x, int y)
{
	playerX = x;
	playerY = y;
	int key;
	int xo, yo;

	map->putItem(playerX, playerY, tilePlayer);
	printStatus();

	while (1)
	{
		key = Term->getKey();
		switch (key)
		{

		case KEY_UP:
		case KEY_NORTH:
			tryPlay(playerX, playerY - 1);
			break;

		case KEY_DOWN:
		case KEY_SOUTH:
			tryPlay(playerX, playerY + 1);
			break;

		case KEY_RIGHT:
		case KEY_EAST:
			tryPlay(playerX + 1, playerY);
			break;

		case KEY_LEFT:
		case KEY_WEST:
			tryPlay(playerX - 1, playerY);
			break;

		case KEY_DIAG_NE:
			tryPlay(playerX + 1, playerY - 1);
			break;

		case KEY_DIAG_SE:
			tryPlay(playerX + 1, playerY + 1);
			break;

		case KEY_DIAG_SW:
			tryPlay(playerX - 1, playerY + 1);
			break;

		case KEY_DIAG_NW:
			tryPlay(playerX - 1, playerY - 1);
			break;

		default:
			break;

		}
	}
}


