// Game.h

#pragma once

//#define TERM_WIN_10_H			// compile for Windows 10
#define TERM_CURSES_H

// C includes
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>    
#include <typeinfo>				// operator typeid
// C++ includes
#include <ctime>
#include <ioStream>
using namespace std;			// avoid this in .h, not everyone wants it

#include "TermBase.h"
//#include "TermWin10.h"
#include "TermCurses.h"
#include "World.h"


const int TEXT_BUFFER_MAX = 100;		// !! longer than widest text message?

const char TILE_PLAYER = '@';
const char TILE_GOLD = 'G';
const char TILE_WALL = '*';
const char TILE_DOOR_BUMP = '+';
const char TILE_DOOR_LOCK = 'L';
const char TILE_DOOR_KEY = 'K';
const int DOOR_CODE_1 = 123;
const int DOOR_CODE_2 = 456;

class TileBase
{
public:
	char drawChar;

	TileBase(const char drawAs)
	{
		drawChar = drawAs;
	}
	virtual void dummy()			// dynamic_cast requires on virtual in base class!
	{

	}

};

class TileGold : public TileBase
{
public:
	int Value;
	TileGold(int value, const char drawAs) : TileBase(drawAs)
	{
		Value = value;
	}
};


class TileDoorBump : public TileBase
{
public:
	bool doorOpen;
	TileDoorBump(const char drawAs) : TileBase(drawAs)
	{
		doorOpen = false;
	}
};

class TileDoorKey : public TileBase
{
public:
	int doorCode;

	TileDoorKey(int door_code, const char drawAs) : TileBase(drawAs)
	{
		doorCode = door_code;
	}
};

class TileDoorLock : public TileDoorBump
{
public:
	int doorCode;

	TileDoorLock(int door_code, const char drawAs) : TileDoorBump(drawAs)
	{
		doorCode = door_code;
	}
};

class Map
{
public:
	TermBase * Term;
	int nx, ny, nxy;
	int statusY;
	TileBase **surface;

	Map(int nx, int ny, int statusY, TermBase *term)
	{
		this->nx = nx;
		this->ny = ny;
		this->statusY = statusY;
		this->Term = term;
		nxy = nx * ny;
		surface = new TileBase*[nxy];
		for (int i = 0; i < nxy; i++)
			surface[i] = nullptr;
	}
	int getIndex(int x, int y)
	{
		int index = -3;
		// check bounds
		if (x >= 0 && x < nx)
		{
			if (y >= 0 && y < ny)
			{
				index = y * nx + x;
			}
			else
				assert(TRUE);
		}
		else
			assert(TRUE);

		return index;
	}
	void clearItem(int x, int y)
	{
		int index = getIndex(x, y);
		surface[index] = 0;
		Term->cursorXY(x, y);
		printf("%c", ' ');
	}
	void putItem(int x, int y, TileBase *tile)
	{
		int index = getIndex(x, y);
		surface[index] = tile;
		Term->cursorXY(x, y);
		printf("%c", tile->drawChar);
	}
	TileBase *getItem(int x, int y)
	{
		int index = getIndex(x, y);
		TileBase *tile = surface[index];
		return tile;
	}
};

class Region
{
public:
	Map * map;
	int x1, y1, x2, y2;

	TileBase *tile;

	Region(int x1, int y1, int x2, int y2, Map *map, TileBase *tile)
	{

		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
		this->map = map;
		this->tile = tile;
	}
	void draw()
	{
		int x, y;

		for (y = y1; y < y2; y++)
		{
			map->putItem(x1, y, tile);
		}
		for (y = y1; y < y2; y++)
		{
			map->putItem(x2 - 1, y, tile);
		}

		for (x = x1; x < x2; x++)
		{
			map->putItem(x, y1, tile);
		}

		for (x = x1; x < x2; x++)
		{
			map->putItem(x, y2 - 1, tile);
		}
	}
};
