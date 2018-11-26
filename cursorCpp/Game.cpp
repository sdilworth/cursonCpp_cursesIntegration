// Game.cpp

#include "Game.h"

int main()
{
	//TermBase *Term = new TermWin10;
	int dbg = -1;
	TermBase *Term = new TermCurses;
	Term->Startup();
	World world(3, 2, 44, 23, Term);
	world.addRegion1(4, 3, 24,15);
	world.addRegion2(25,3,43, 15);
	world.fillMap();
	world.start(20, 18);

	Term->Shutdown();
}

