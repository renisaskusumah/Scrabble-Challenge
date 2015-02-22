#ifndef _GAMEAISCENE_H_
#define _GAMEAISCENE_H_

#include "cocos2d.h"
//#include "BasicTiles.cpp"
#include "AiPlayer.cpp"
#include "GameAlgorithm.cpp"
#include <string>
#include <stdio.h>
#include <iostream>
#include <time.h> 
#include <fstream>
#include <algorithm>

#define COCOS2D_DEBUG
USING_NS_CC;
using namespace std;
class GameAiScene : public CCLayer
{
	
	CCSprite *_background;
	CCSprite *_possWord;
	CCSprite *_loading;
	CCSprite *_winnerBg;
	CCSize screenSize;
	CCMenuItem *_startButton;
	int papan[15][15];
	int scorePlayer1;
	int scorePlayer2;
	int timePlayer1;
	int timePlayer2;
	int currTiles;
	/*
		Ai combination adalah jenis algoritma yg dilibatkan untuk pemain
		1 = GADDAG
		2 = GADDAG + Strategy
		3 = GADDAG + NegaScout
		4 = GADDAG + Strategy + NegaScout
	*/
	int aiCombination1;
	vector<vector<int>> letterBag;
	float sec;
	int mTime;
	bool isPaused;
	CCLabelTTF* letterBagLabel;
	CCLabelTTF* rackLabel;
	CCLabelTTF* wordLabel;
	CCLabelTTF* scoreLabel;
	CCLabelTTF* countdownPlayer1;
	CCLabelTTF* countdownPlayer2;
	CCLabelTTF* scoreLabelPlayer1;
	CCLabelTTF* scoreLabelPlayer2;
	CCLabelTTF* trackerLabel;
	CCLabelTTF* winnerLabel;
	int playerTurn;
	int totalTiles;
	string saveFile;
	BasicTiles basicTiles;
	AiPlayer playerOne;
	AiPlayer playerTwo;
	GameAlgorithm algorithm;
	
public:
	static CCScene *scene();
	bool init();
	void randomRack();
	void ccTouchesBegan(CCSet *pTouches,CCEvent *pEvent);
	void exitGame(CCObject *pSender);
	void startGame(CCObject *pSender);
	void playTile(CCObject *pSender);
	void pause(CCObject *pSender);
	void setLetterBag();
	void updateLetterBag(int index);
	void iseng();
	vector<vector<int>> getLetterBag();
	void playGame(CCObject *pSender); //memainkan huruf yang ada di rack
	void showPlayerTurn(float dt);
	BasicTiles getBasicTiles();
	CREATE_FUNC(GameAiScene);
};

#endif