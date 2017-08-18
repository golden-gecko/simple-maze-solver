/*
 *
 *	Maze 1.0
 *	Copyright (C) 2007  Wojciech Holisz (wojciech.holisz@gmail.com)
 *
 */

//------------------------------------------------------------------------------

#include <windows.h>
#include <stdio.h>
#include <conio.h>

#include <string>
#include <list>

using namespace std;

//------------------------------------------------------------------------------

#define EMPTY   -1
#define WALL    -2
#define START   -3
#define END     -4
#define WAY     -5

//------------------------------------------------------------------------------

short * maze	= NULL; //	Tutaj przechowywane sa dane labiryntu

WORD sizeX      = 0;    //	Wymiary labiryntu
WORD sizeY      = 0;

WORD startPos   = 0;    //	Pozycja poczatkowa
WORD endPos     = 0;    //	Pozycja koncowa

//------------------------------------------------------------------------------

// loadMaze - wczytuje labirynt z pliku tekstowego, w ktorym:
//
// 0 - oznacza przejscie,
// 1 - oznacza sciane,
// S - pozycje poczatkowa
// K - pozycje koncowa

//------------------------------------------------------------------------------

void loadMaze(string fileName){

	// Otworz plik.

	FILE * file;
	
	fopen_s(&file, fileName.c_str(), "rb");

	if (!file) {

		printf("File not found!");

		return;

	}


	// Odczytaj rozmiar pliku.

	fpos_t fileSize = 0;

	fseek(file, 0, SEEK_END);
	fgetpos(file, &fileSize);
	fseek(file, 0, SEEK_SET);	


	// Wczytaj dane do pamieci.

	BYTE * data = new BYTE[(unsigned int)fileSize];

	fread(data, (size_t)fileSize, 1, file);


	// Zamknij plik.

	fclose(file);


	// Odczytaj rozmiary labiryntu.

	for (WORD i = 0; i < fileSize; i++)

		if (data[i] == 13) {

			if (sizeY == 0)

				sizeX = i;

			sizeY += 1;

		}


	// Przetworz dane wczytane z pliku.

	maze = new short [sizeX * sizeY];

	WORD index = 0;

	for (WORD y = 0; y < sizeY; y++) {

		for (WORD x = 0; x < sizeX; x++)

			maze[y * sizeX + x] = data[index++];

		index += 2;

	}

	for (WORD i = 0; i < sizeX * sizeY; i++)

		switch (maze[i]) {

			case '0':
				maze[i] = EMPTY;
				break;

			case '1':
				maze[i] = WALL;
				break;

			case 'S':
				maze[i] = START;
				startPos = i;
				break;

			case 'K':
				maze[i] = END;
				endPos = i;
				break;

		}


	// Usun niepotrzebne dane.

	delete [] data;
}

//------------------------------------------------------------------------------

// showMaze - wyswietla labirynt na ekranie.

//------------------------------------------------------------------------------

void showMaze(){

	// Wyczysc ekran.

	system("cls");


	// Wyswietl aktualny stan labiryntu.

	for (WORD y = 0; y < sizeY; y++) {

		for (WORD x = 0; x < sizeX; x++)

			switch (maze[y * sizeX + x]) {

				case EMPTY:
					printf("%c", 255);
					break;

				case WALL:
					printf("%c", 219);
					break;

				case START:
					printf("S");
					break;

				case END:
					printf("K");
					break;

				case WAY:
					printf("%c", 178);
					break;

				default:
					printf("%c", 177);
					break;

			}

		printf("\n");

	}

}

//------------------------------------------------------------------------------

// deleteMaze - usuwa dane labiryntu z pamieci.

//------------------------------------------------------------------------------

void deleteMaze(){

	delete [] maze;

	maze		= NULL;
	sizeX		= 0;
	sizeY		= 0;
	startPos	= 0;
	endPos		= 0;

}

//------------------------------------------------------------------------------

// Ponizsze funckje sluza do okreslania drogi w labiryncie.

//------------------------------------------------------------------------------

short getLeft(WORD n){

	if (n % sizeX != 0)

		return maze[n - 1];

	return WALL;

}

//------------------------------------------------------------------------------

short getRight(WORD n){

	if (n % sizeX != sizeX - 1)

		return maze[n + 1];

	return WALL;

}

//------------------------------------------------------------------------------

short getUpper(WORD n){

	if (n >= sizeX)

		return maze[n - sizeX];

	return WALL;

}

//------------------------------------------------------------------------------

short getLower(WORD n){

	if (n < sizeX * (sizeY - 1))

		return maze[n + sizeX];

	return WALL;

}

//------------------------------------------------------------------------------

WORD set(list <short> pos, WORD level, BYTE progress){

	if (progress) {

		showMaze();

		Sleep(25);

	}

	list <short> posNext;

	for (list <short>::iterator i = pos.begin(); i != pos.end(); i++) {

		if (getLeft(*i) == END || getRight(*i) == END || getUpper(*i) == END || getLower(*i) == END) {

			while (posNext.size())

				posNext.pop_back();

			return level;

		}

		if (getLeft(*i) == EMPTY) {

			posNext.push_back(*i - 1);
			maze[*i - 1] = level;

		}

		if (getRight(*i) == EMPTY) {

			posNext.push_back(*i + 1);
			maze[*i + 1] = level;

		}

		if (getUpper(*i) == EMPTY) {

			posNext.push_back(*i - sizeX);
			maze[*i - sizeX] = level;

		}

		if (getLower(*i) == EMPTY) {

			posNext.push_back(*i + sizeX);
			maze[*i + sizeX] = level;

		}

	}

	if (posNext.size())

		return set(posNext, level + 1, progress);

	return 0;

}
//------------------------------------------------------------------------------

void findWay(WORD pos, WORD level, BYTE progress){

	if (progress) {

		showMaze();

		Sleep(25);

	}

	level -= 1;

	if (getLeft(pos) == level) {
	
		maze[pos - 1] = WAY;

		if (level > 1) {

			findWay(pos - 1, level, progress);

			return;

		}

	}

	else if (getRight(pos) == level) {

		maze[pos + 1] = WAY;

		if (level > 1) {

			findWay(pos + 1, level, progress);

			return;

		}

	}

	else if (getUpper(pos) == level) {
	
		maze[pos - sizeX] = WAY;

		if (level > 1) {

			findWay(pos - sizeX, level, progress);

			return;

		}

	}

	else if (getLower(pos) == level) {
	
		maze[pos + sizeX] = WAY;

		if (level > 1) {

			findWay(pos + sizeX, level, progress);

			return;

		}

	}

}

//------------------------------------------------------------------------------

// solveMaze - funckja znajduje droge w labiryncie.

//------------------------------------------------------------------------------

void solveMaze(BYTE progress){

	showMaze();

	list <short> pos;

	pos.push_back(startPos);

	WORD length = set(pos, 1, progress);

	findWay(endPos, length, progress);

	showMaze();

	printf("\nWymiary\t\t\t%i x %i", sizeX, sizeY);
	printf("\nPozycja startowa\tx = %i\ty = %i", startPos / sizeX + 1, startPos % sizeX + 1);
	printf("\nPozycja koncowa\t\tx = %i\ty = %i", endPos / sizeX + 1, endPos % sizeX + 1);
	printf("\nDlugosc trasy\t\t%i", length);
	printf("\n\n");

	printf("Nacisnij klawisz, aby kontynuowac...");

	_getch();

}

//------------------------------------------------------------------------------

// main - glowna funckja programu, petla wyswietlajaca menu.

//------------------------------------------------------------------------------

int main(){

	char c;

	do {

		system("cls");

		printf("\n");
		printf("Maze 1.0\n\n");
		printf("Wojciech Holisz\n\n");
		printf("1 - Wczytaj labirynt, wyszukaj droge\n");
		printf("2 - Wczytaj labirynt, wyszukaj droge, wyswietlaj postep\n");
		printf("3 - Koniec\n");

		c = _getch();

		switch (c) {

			case '1':
				loadMaze("maze.txt");
				solveMaze(FALSE);
				deleteMaze();
				break;

			case '2':
				loadMaze("maze.txt");
				solveMaze(TRUE);
				deleteMaze();
				break;

		}

	} while (c != '3') ;

	return 0;

}

//------------------------------------------------------------------------------
