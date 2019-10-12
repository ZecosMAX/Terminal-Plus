#pragma once
#include "program.h"

class FontEdit : public program
{
private:
	int xOff = 200;
	int yOff = 42;

	int iDPP = 15;

	int CursorX = 0;
	int CursorY = 0;

	int editorCursorX = 0;
	int editorCursorY = 0;

	bool CharacterReady = false;
	bool isCharacterInput = true;

	std::ostringstream oss;

	uint8_t *currentChar;
	char currentCharCode;

public:
	FontEdit(bool**& pm, int sizeX, int sizeY, int bufferSize, terminal *term)
	{
		this->name = "FontEdit";
		this->pixelMatrixPointer = &pm;
		this->sizeX = sizeX;
		this->sizeY = sizeY;
		this->bufferSize = bufferSize;
		this->term = term;

		this->keyHandler = [this](int key, int scancode, int action, int mods, int cA) 
		{
			if (key == GLFW_KEY_Z && mods == 2)
			{
				this->term->restore("STOPPED FONTEDIT");
				return;
			}
			if (CharacterReady)
			{
				if (action == GLFW_PRESS)
				{
					std::cout << key << " -- " << mods << std::endl;

					if (key == GLFW_KEY_RIGHT && editorCursorX < 7)
					{
						editorCursorX++;
					}
					if (key == GLFW_KEY_LEFT && editorCursorX > 0)
					{
						editorCursorX--;
					}
					if (key == GLFW_KEY_UP && editorCursorY > 0)
					{
						editorCursorY--;
					}
					if (key == GLFW_KEY_DOWN && editorCursorY < 11)
					{
						editorCursorY++;
					}
					if (key == GLFW_KEY_SPACE)
					{
						currentChar[editorCursorY] ^= (1 << (7 - editorCursorX));
					}
					if (key == GLFW_KEY_ENTER)
					{
						this->term->chars->characters[currentCharCode] = currentChar;
						this->term->chars->Save();
						CharacterReady = false;
						currentCharCode = 0;
						currentChar = 0;
					}
				}
			}
			else
			{
				if (key == GLFW_KEY_C && mods == 2 && action == GLFW_PRESS)
				{
					isCharacterInput = !isCharacterInput;
					std::cout << isCharacterInput;
				}
				if (cA == 1 && isCharacterInput)
				{
					if (this->LoadChar(key))
					{
						CharacterReady = true;
						char ch[3];
						_itoa_s(key, ch, 10);
						InternalPrint(30 + 5*12, yOff + 12, ch);
					}
				}
				else if (cA == 1 && !isCharacterInput)
				{
					oss << (char)key;
					InternalPrint(30 + 5 * 12, yOff + 12, oss.str().c_str());
				}
				if (key == GLFW_KEY_ENTER && !isCharacterInput)
				{
					uint8_t code = atoi(oss.str().c_str());
					if (this->LoadChar(code))
					{
						CharacterReady = true;
						isCharacterInput = true;
					}
				}
			}

			Update();
		};
		iDPP = iDPP * (sizeY - yOff) / ((12 * iDPP) + 1);
		xOff = sizeX - (8 * iDPP) - 31;
		
		this->start();
	}

	int program::start()
	{
		CenteredPrint(0, "FONT EDIT BY ZECOSMAX");
		InternalPrint(30, yOff - 3, "PRESS A KEY");
		InternalPrint(30, yOff + 12, "CODE: ");

		InternalPrint(30, sizeY - 15, "CTRL + Z TO EXIT");

		CursorX = xOff;
		CursorY = yOff;

		for (size_t y = yOff; y < yOff + (12 * iDPP) + 1; y++)
		{
			for (size_t x = xOff; x < xOff + (8 * iDPP) + 1; x++)
			{
				if( (x - xOff) % iDPP == 0 || (y - yOff) % iDPP == 0)
					pixelMatrixPointer[0][y][x] = true;
			}
		}

		for (size_t y = 0; y < iDPP; y++)
		{
			for (size_t x = 0; x < iDPP; x++)
			{
				pixelMatrixPointer[0][yOff + editorCursorY * iDPP + y][xOff + editorCursorX * iDPP + x] = true;
			}
		}

		return 0;
	}

	void program::tick(float time)
	{

	}

	void CenteredPrint(int y, const char* string)
	{
		InternalPrint(sizeX / 2 - 12 * strlen(string) / 2, y, string);
	}

	void InternalPrint(int x, int y, const char* string)
	{
		term->setCursor(x, y);
		for (size_t i = 0; i < strlen(string); i++)
		{
			term->writeCharacterNew(term->chars->getCharacter(string[i]));
		}
	}

	void Update()
	{
		for (size_t y = yOff; y < yOff + (12 * iDPP) + 1; y++)
		{
			for (size_t x = xOff; x < xOff + (8 * iDPP) + 1; x++)
			{
				if ((x - xOff) % iDPP == 0 || (y - yOff) % iDPP == 0)
					pixelMatrixPointer[0][y][x] = true;
				else
					pixelMatrixPointer[0][y][x] = false;
			}
		}

		for (size_t y = 0; y < iDPP; y++)
		{
			for (size_t x = 0; x < iDPP; x++)
			{
				pixelMatrixPointer[0][yOff + editorCursorY * iDPP + y][xOff + editorCursorX * iDPP + x] = true;
			}
		}

		if (currentChar != 0)
		{
			for (size_t y = yOff; y < yOff + (12 * iDPP) + 1; y++)
			{
				for (size_t x = xOff; x < xOff + (8 * iDPP) + 1; x++)
				{
					int indexX = (x - xOff) / iDPP; // 0 -- 8
					int indexY = (y - yOff) / iDPP; // 0 -- 12

					if (currentChar[indexY] & (1 << (7 - indexX)))
						pixelMatrixPointer[0][y][x] = true;
				}
			}
		}
	}

	bool LoadChar(int key)
	{
		currentChar = term->chars->getCharacter(key);
		currentCharCode = key;

		if (currentChar != 0)
		{
			for (size_t y = yOff; y < yOff + (12 * iDPP) + 1; y++)
			{
				for (size_t x = xOff; x < xOff + (8 * iDPP) + 1; x++)
				{
					int indexX = (x - xOff) / iDPP; // 0 -- 8
					int indexY = (y - yOff) / iDPP; // 0 -- 12

					if (currentChar[indexY] & (1 << (7 - indexX)))
						pixelMatrixPointer[0][y][x] = true;
				}
			}
		}
		else
		{
			currentChar = new uint8_t[12];
			for (size_t i = 0; i < 12; i++)
			{
				currentChar[i] = 0;
			}
		}

		return true;
	}
};

