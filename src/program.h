#pragma once
#include "terminal.h"

class program
{
protected:
	terminal *term;

public:
	bool*** pixelMatrixPointer;
	int sizeX, sizeY, bufferSize;
	const char* name;
	std::function<void(int, int, int, int, int)> keyHandler;


	virtual void tick(float) = 0;
	virtual int start() = 0;
};

