// TermWin10.h

#pragma once

#ifdef TERM_WIN_10_H

#define _CRTDBG_MAP_ALLOC  
#include <Windows.h>
#include <conio.h>

// https://docs.microsoft.com/en-us/windows/console/setconsolemode
// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#example
// Try some Set Graphics Rendition (SGR) terminal escape sequences
/*
wprintf(L"\x1b[31mThis text has a red foreground using SGR.31.\r\n");
wprintf(L"\x1b[1mThis text has a bright (bold) red foreground using SGR.1 to affect the previous color setting.\r\n");
wprintf(L"\x1b[mThis text has returned to default colors using SGR.0 implicitly.\r\n");
wprintf(L"\x1b[34;46mThis text shows the foreground and background change at the same time.\r\n");
wprintf(L"\x1b[0mThis text has returned to default colors using SGR.0 explicitly.\r\n");
wprintf(L"\x1b[31;32;33;34;35;36;101;102;103;104;105;106;107mThis text attempts to apply many colors in the same command. Note the colors are applied from left to right so only the right-most option of foreground cyan (SGR.36) and background bright white (SGR.107) is effective.\r\n");
wprintf(L"\x1b[39mThis text has restored the foreground color only.\r\n");
wprintf(L"\x1b[49mThis text has restored the background color only.\r\n");
*/

// these are from getKey()
// - char vals are experimental
// - currently only console output is virtual
// - they differ from virtual keys (with escape)
static const int KEY_UP = (0x80 | 'H');
static const int KEY_DOWN = (0x80 | 'P');
static const int KEY_RIGHT = (0x80 | 'M');
static const int KEY_LEFT = (0x80 | 'K');
static const int KEY_DIAG_NE = ('9');
static const int KEY_DIAG_SE = ('3');
static const int KEY_DIAG_SW = ('1');
static const int KEY_DIAG_NW = ('7');
static const int KEY_NORTH = ('8');
static const int KEY_SOUTH = ('2');
static const int KEY_EAST = ('6');
static const int KEY_WEST = ('4');

class TermWin10 : public TermBase
{
public:

	//https://docs.microsoft.com/en-us/windows/console/setconsolecursorinfo
	//https://docs.microsoft.com/en-us/windows/console/console-cursor-info-str
	int Startup()
	{
		DWORD err = ERROR_SUCCESS;
		
		_CONSOLE_CURSOR_INFO  cursor;
		cursor.dwSize = 1;
		cursor.bVisible = false;

		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
		return err;
	}

	void Shutdown()
	{
		_CrtDumpMemoryLeaks();
		cout << "Any Memory Leaks are in VS Output Tab" << endl;
	}


	void cursorXY(int x, int y)
	{

			COORD coord;
			coord.X = x;
			coord.Y = y;
			SetConsoleCursorPosition(
				GetStdHandle(STD_OUTPUT_HANDLE),
				coord
			);
		//wprintf(L"\x1b[%d;%dH", y, x);		// note (y,x), not (x,y);
	}

	int getKey()
	{
		int key = 0;

		while (1)
		{
			key = _getch();
			if (key == 0)
				continue;			// skip null keys
			if (key == 0xE0)
			{
				key = _getch();
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
