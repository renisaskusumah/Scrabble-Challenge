#include<malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <ctype.h>
#include <sstream>

using namespace std;
typedef struct smp * alamat;
typedef struct smp{
	//possible isi;
	int eval;
	int endEval;
	int depth;
	int totalChild;
	string word;
	int wordLength;
	int direction;
	int score;
	int row;
	int col;
	int rowAnchor;
	int colAnchor;
	int player;
	int opponent;
	int leftTiles;
	alamat sibling;
	alamat child;
	alamat parent;
	int usedTiles;
	float heuristic;
}nodetree;
typedef struct{
	nodetree *root;
}tree;