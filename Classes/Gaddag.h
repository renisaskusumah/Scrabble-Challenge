#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include "Papan.cpp"
#include "cocos2d.h"
#include "TileOnBoard.h"
#include <ctype.h>

#define COCOS2D_DEBUG 1
USING_NS_CC;

using namespace std;

typedef struct
{
	int direction;
	string word;
	int startRow;
	int startCol;
	int score;
	string all;
	int rowAnchor;
	int colAnchor;
	string usedTiles;
	float heuristic;
}possible;

typedef struct simpul *alamatsimpul;

typedef struct jlr *alamatjalur;

typedef struct jlr
{
	alamatjalur nextjalur;
	alamatsimpul tujuan;
}jalur;

//huruf bisa berupa huruf biasa atau delimiter
typedef struct simpul
{
	char huruf;
	//untuk menandai apakah simpul tersebut membentuk kata atau tidak jika ditelusur dari root
	int eow;
	int jmlAnak; //jumlah jalur yang dimiliki
	//int toFile;
	//jika eow maka disimpan index kata tersebut di file kamus ->keperluan definition
	alamatsimpul next;
	alamatsimpul prev;
	jalur *arc;
}node;

typedef struct
{
	node *start;
}graf;