#pragma once

#include "fileHandler.h"

class Characters
{
private:
	fileHandler* fh;
public:
	uint8_t** characters;

	Characters()
	{
		fh = new fileHandler("termfont.zf");
		characters = new uint8_t* [256];
		for (size_t i = 0; i < 256; i++)
		{
			characters[i] = 0;
		}
		bool flag = true;
		while (flag)
		{
			//auto a = fh->loadTypeFromFile<uint8_t>(fh->fp); // size

			//std::cout << (int)a << std::endl;
				auto code = fh->loadTypeFromFile<uint8_t>(fh->fp); // code
				if (code == 255)
				{
					flag = false;
					break;
				}
				else
				{
					characters[code] = new uint8_t[12];
					characters[code][0] = fh->loadTypeFromFile<uint8_t>(fh->fp); // data 1 
					characters[code][1] = fh->loadTypeFromFile<uint8_t>(fh->fp); // data 2 
					characters[code][2] = fh->loadTypeFromFile<uint8_t>(fh->fp); // data 3 
					characters[code][3] = fh->loadTypeFromFile<uint8_t>(fh->fp); // data 4 
					characters[code][4] = fh->loadTypeFromFile<uint8_t>(fh->fp); // data 5 
					characters[code][5] = fh->loadTypeFromFile<uint8_t>(fh->fp); // data 6 
					characters[code][6] = fh->loadTypeFromFile<uint8_t>(fh->fp); // data 7 
					characters[code][7] = fh->loadTypeFromFile<uint8_t>(fh->fp); // data 8
					characters[code][8] = fh->loadTypeFromFile<uint8_t>(fh->fp); // data 9 
					characters[code][9] = fh->loadTypeFromFile<uint8_t>(fh->fp); // data 10 
					characters[code][10] = fh->loadTypeFromFile<uint8_t>(fh->fp); // data 11 
					characters[code][11] = fh->loadTypeFromFile<uint8_t>(fh->fp); // data 12 	
				}
		}
		fh->~fileHandler();
	}

	void Save()
	{
		fh = new fileHandler("termfont.zf");
		for (int code = 0; code < 256; code++)
		{
			std::cout << "saved: " << code << std::endl;
			if (characters[code] != 0)
			{
				fh->saveTypeToFile<uint8_t>(code, fh->fp); // code
				fh->saveTypeToFile<uint8_t>(characters[code][0], fh->fp); // data 1 
				fh->saveTypeToFile<uint8_t>(characters[code][1], fh->fp); // data 2
				fh->saveTypeToFile<uint8_t>(characters[code][2], fh->fp); // data 3 
				fh->saveTypeToFile<uint8_t>(characters[code][3], fh->fp); // data 4 
				fh->saveTypeToFile<uint8_t>(characters[code][4], fh->fp); // data 5 
				fh->saveTypeToFile<uint8_t>(characters[code][5], fh->fp); // data 6 
				fh->saveTypeToFile<uint8_t>(characters[code][6], fh->fp); // data 7 
				fh->saveTypeToFile<uint8_t>(characters[code][7], fh->fp); // data 8 
				fh->saveTypeToFile<uint8_t>(characters[code][8], fh->fp); // data 9 
				fh->saveTypeToFile<uint8_t>(characters[code][9], fh->fp); // data 10 
				fh->saveTypeToFile<uint8_t>(characters[code][10], fh->fp); // data 11 
				fh->saveTypeToFile<uint8_t>(characters[code][11], fh->fp); // data 12
			}
			else
			{
				continue;
			}
		}
		fh->~fileHandler();
	}

	uint8_t* getCharacter(char code)
	{
		return characters[code];
	}
};