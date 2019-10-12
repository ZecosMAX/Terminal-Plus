#include "terminal.h"
#include "programs.h"

terminal::terminal(bool**& pM, int sizeX, int sizeY, int bufferSize)
{
	chars = new Characters();

	this->pixelMatrixPointer = &pM;
	this->internalBuffer;
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->bufferSize = bufferSize;
	this->cursorPosX = 0;
	this->cursorPosY = 0;
	this->screenPosY = 0;

	this->keyHandler = [this](int key, int scancode, int action, int mods, int cA) {
		this->handleKey(key, scancode, action, mods, cA);
	};

	initPixelMatrix(internalBuffer);
	*pixelMatrixPointer = &internalBuffer[screenPosY];

	writeCharacterNew(chars->getCharacter('>'));
	writeCharacterNew(chars->getCharacter('_'), false);
}

void terminal::handleKey(int key, int scancode, int action, int mods, int cA)
{
	if (cA == 0) 
	{
		if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
		{
			if (cursorPosX - 11 >= 0)
			{
				writeCharacterNew(chars->getCharacter(' '));
				moveCursor(-2, 0);
				writeCharacterNew(chars->getCharacter('_'), false);

				command.str(command.str().erase(command.str().length() - 1, 1));
				command.seekp(0, std::ios::end);
			}
		}
		else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
		{
			std::cout << command.str() << std::endl;

			writeCharacterNew(chars->getCharacter(' '));
			setCursor(0, cursorPosY + 15);
			this->handleCommands();
			
		}
		else if (key == GLFW_KEY_PAGE_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
		{
			moveScreen(15);
		}
		else if (key == GLFW_KEY_PAGE_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
		{
			moveScreen(-15);
		}
	}
	else if (cA == 1) //if that is a Unicode Character
	{
		this->command << (char)key;

		writeCharacterNew(chars->getCharacter((char)toupper(key)));
		writeCharacterNew(chars->getCharacter('_'), false);
	} 
}

void terminal::tick(float time)
{

}

void terminal::moveCursor(int x, int y)
{
	this->cursorPosX += 10 * x;
	this->cursorPosY += 15 * y;

	if (cursorPosY + 16 > sizeY)
	{
		moveScreen(0);
	}
}

void terminal::setCursor(int x, int y)
{
	this->cursorPosX = x;
	this->cursorPosY = y;
}

void terminal::writeCharacterNew(uint8_t *character, bool move)
{
	if (character != 0)
	{
		for (size_t j = 0; j < 12; j++)
		{
			this->internalBuffer[3 + cursorPosY + j][2 + cursorPosX + 0] = character[j] & (1 << 7);
			this->internalBuffer[3 + cursorPosY + j][2 + cursorPosX + 1] = character[j] & (1 << 6);
			this->internalBuffer[3 + cursorPosY + j][2 + cursorPosX + 2] = character[j] & (1 << 5);
			this->internalBuffer[3 + cursorPosY + j][2 + cursorPosX + 3] = character[j] & (1 << 4);
			this->internalBuffer[3 + cursorPosY + j][2 + cursorPosX + 4] = character[j] & (1 << 3);
			this->internalBuffer[3 + cursorPosY + j][2 + cursorPosX + 5] = character[j] & (1 << 2);
			this->internalBuffer[3 + cursorPosY + j][2 + cursorPosX + 6] = character[j] & (1 << 1);
			this->internalBuffer[3 + cursorPosY + j][2 + cursorPosX + 7] = character[j] & (1 << 0);
		}

		if (move)
		{
			if (cursorPosX > sizeX - 29)
			{
				setCursor(0, cursorPosY + 15);
			}
			else
			{
				moveCursor(1, 0);
			}

			if (cursorPosY > screenPosY + sizeY - 16)
			{
				moveScreen(15);
			}
		}
	}
	else
	{
		writeCharacterNew(chars->getCharacter(0), move);
	}
}

void terminal::restore(const char* str)
{
	this->cursorPosX = 0;
	this->cursorPosY = 0;
	this->screenPosY = 0;

	clearScreen();

	this->keyHandler = [this](int key, int scancode, int action, int mods, int cA) {
		this->handleKey(key, scancode, action, mods, cA);
	};

	println(str);

	writeCharacterNew(chars->getCharacter('>'));
	writeCharacterNew(chars->getCharacter('_'), false);
}

void terminal::handleCommands()
{
	auto a = (int)'0';
	if (!_strcmpi(command.str().c_str(), "help"))
	{
		println("HELP: GET HELP");
		println("CLEAR: CLEAR SCREEN");
		println("LOAD FE: FONTEDITOR");
		writeCharacterNew(chars->getCharacter('>'));
		writeCharacterNew(chars->getCharacter('_'), false);
	}
	else if (!_strcmpi(command.str().c_str(), "clear"))
	{
		clearScreen();
		writeCharacterNew(chars->getCharacter('>'));
		writeCharacterNew(chars->getCharacter('_'), false);
	}
	else if (!_strcmpi(command.str().c_str(), "load fe"))
	{
		clearScreen();
		FontEdit *fe = new FontEdit(this->internalBuffer, sizeX, sizeY, bufferSize, this);
		this->keyHandler = fe->keyHandler;
	}
	else
	{
		println("UNKNOWN COMMAND");
		writeCharacterNew(chars->getCharacter('>'));
		writeCharacterNew(chars->getCharacter('_'), false);
	}

	command.str(std::string());
}

void terminal::initPixelMatrix(bool**& matrix)
{
	matrix = new bool* [bufferSize];
	for (size_t i = 0; i < bufferSize; i++)
	{
		matrix[i] = new bool[sizeX];
		for (size_t j = 0; j < sizeX; j++)
		{
			matrix[i][j] = false;
		}
	}
}

void terminal::moveScreen(int rows)
{
	if (screenPosY + rows >= 0 && screenPosY + rows <= sizeY)
	{
		this->screenPosY += rows;
		*pixelMatrixPointer = &internalBuffer[screenPosY];
	}
}

void terminal::clearScreen()
{
	this->cursorPosX = 0;
	this->cursorPosY = 0;
	this->screenPosY = 0;

	for (size_t i = 0; i < bufferSize; i++)
	{
		for (size_t j = 0; j < sizeX; j++)
		{
			internalBuffer[i][j] = false;
		}
	}

	*pixelMatrixPointer = &internalBuffer[screenPosY];
}

void terminal::print(const char* string)
{
	for (size_t i = 0; i < strlen(string); i++)
	{
		if (string[i] == '\n')
		{
			setCursor(0, cursorPosY + 15);
		}
		else
		{
			writeCharacterNew(chars->getCharacter(string[i]));
		}
	}
}

void terminal::println(const char* string)
{
	for (size_t i = 0; i < strlen(string); i++)
	{
		if (string[i] == '\n')
		{
			setCursor(0, cursorPosY + 15);
		}
		else
		{
			writeCharacterNew(chars->getCharacter(string[i]));
		}
	}
	setCursor(0, cursorPosY + 15); // a.k.a print("\n")
}

char* terminal::tolowerstr(const char* string)
{
	char* stringResult = new char[strlen(string) + 1];
	for (size_t i = 0; i < strlen(string); i++)
	{
		stringResult[i] = (char)tolower(string[i]);
	}
	stringResult[strlen(string)] = '\0';
	return stringResult;
}