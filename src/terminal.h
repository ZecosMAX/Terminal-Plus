#pragma once
#include <string.h>
#include <sstream>
#include <iostream>
#include <Windows.h>
#include <functional>
#include <GLFW/glfw3.h>

#include "resources.h"

class terminal
{
private:
	bool ***pixelMatrixPointer;
	bool **internalBuffer;
	int screenPosY;
	int sizeX, sizeY, bufferSize;

	
	void handleCommands();
	void handleKey(int, int, int, int, int);
	char* tolowerstr(const char*);
	void initPixelMatrix(bool**&);

	std::ostringstream command;

public:
	Characters* chars;
	int cursorPosX, cursorPosY;

	std::function<void(int, int, int, int, int)> keyHandler;

	terminal(bool**&, int, int, int);
	
	void writeCharacterNew(uint8_t*, bool move = true);
	void clearScreen();
	void moveScreen(int rows);
	void moveCursor(int, int);
	void setCursor(int, int);

	void restore(const char*);

	void print(const char*);
	void println(const char*);

	void tick(float);
};

//8x12