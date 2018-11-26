// TermCurses.h

#pragma once

#ifdef TERM_CURSES_H

//#define _CRTDBG_MAP_ALLOC  
#include <curses.h>

//static const int KEY_UP = (0x80 | 'H');
//static const int KEY_DOWN = (0x80 | 'P');
//static const int KEY_RIGHT = (0x80 | 'M');
//static const int KEY_LEFT = (0x80 | 'K');
static const int KEY_DIAG_NE = ('9');
static const int KEY_DIAG_SE = ('3');
static const int KEY_DIAG_SW = ('1');
static const int KEY_DIAG_NW = ('7');
static const int KEY_NORTH = ('8');
static const int KEY_SOUTH = ('2');
static const int KEY_EAST = ('6');
static const int KEY_WEST = ('4');

class TermCurses : public TermBase
{
public:
	int Startup()
	{
		(void)initscr();                  /* initialize the curses library*/
		keypad(stdscr, TRUE);             /* enable keyboard mapping */
		(void)nonl();                     /* tell curses not to do NL->CR/NL to output */
		(void)cbreak();                   /* take input chars one at a time, no for wor \n */
		(void)echo();                     /* echo input - in color */

		return 0;
	}

	void Shutdown()
	{
		endwin();
		_CrtDumpMemoryLeaks();
		cout << "Any Memory Leaks are in VS Output Tab" << endl;
	}


	void cursorXY(int x, int y)
	{
		move(y, x);
		refresh();
		//wprintf(L"\x1b[%d;%dH", y, x);		// note (y,x), not (x,y);
	}

	int getKey()
	{
		int key = 0;

		while (1)
		{
			key = getch();
			if (key == 0)
				continue;			// skip null keys
			if (key == 0xE0)
			{
				key = getch();
				key |= 0x80;
			}
			break;
		}

		return key;
	}
	void Beep()
	{
		//::Beep(1000, 100);			// call windows beep function
		cout << '7';					// beep
		//cout << "\a";					// a windows sound
	}

};

#endif // TERM_WIN_10_H
