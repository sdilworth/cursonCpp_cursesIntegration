// TermBase.h

#pragma once

class TermBase
{
public:
	virtual int Startup() = 0;
	virtual void cursorXY(int x, int y) = 0;
	virtual int getKey() = 0;
	virtual void Beep() = 0;
	virtual void Shutdown() = 0;
};
