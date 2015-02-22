#ifndef _GAMEHUMANSCENE_H_
#define _GAMEHUMANSCENE_H_


#include "cocos2d.h"
#include "GameAlgorithm.cpp"
//#include "BasicTiles.cpp"
#include "AiPlayer.cpp"
#include <cstdio>
#include <ctime>
#include <thread>
#include <mutex>
#include <math.h>

#define COCOS2D_DEBUG 1
USING_NS_CC;
using namespace std;

class GameHumanScene : public CCLayer
{
	
	CCSprite *_tiles[7];
	CCSprite *_trackerBox;
	CCSprite *_hintBox;
	CCSprite *_blankBox;
	CCSize screenSize;
	CCMenuItem *_startButton;
	CCMenuItem *_executeButton;
	CCMenuItem *_exchangeButton;
	CCMenu *_exchangeMenu;
	CCMenu *_menu; 
	CCMenu *_resumeMenu;
	CCSprite *_paused;
	CCSprite *_exchangeBox;
	CCSprite *_exchangeTile[7];
	CCSprite *_blankTile[26];
	CCSprite *_winnerBg;
	int totalTiles;
	vector<vector<int>> letterBag;
	int scorePlayer1;
	int scorePlayer2;
	float timePlayer1;
	float timePlayer2;
	bool isPaused;
	int _arrTiles[7];
	int _exchangeIndex[7];
	int holdTile;
	int holdPosition;
	int totalPass;
	int blankTag;
	float sec;
	clock_t _start_human;
	float _durasi_human;
	vector<int> exchangeTile;
	CCLabelTTF* letterBagLabel;
	CCLabelTTF* countdownPlayer1;
	CCLabelTTF* countdownPlayer2;
	CCLabelTTF* scoreLabelPlayer1;
	CCLabelTTF* scoreLabelPlayer2;
	CCLabelTTF* guideLabel;
	CCLabelTTF* wordLabel;
	CCLabelTTF* scoreLabel;
	CCLabelTTF* winnerLabel;
	CCLabelTTF* hintLabel;
	CCLabelTTF* trackerLabel;
	int playerTurn;
	GameAlgorithm algorithm;
	BasicTiles basicTiles;
	AiPlayer playerTwo;
	vector<int> tileOnBoardTag;
	string saveFile;
	string logFile;
	vector<string> hintResult;
	vector<string> tracker;
	mutex mtx;
	float stop;
	int thingkingIndex;
	
	
public:
	static CCScene *scene();
	virtual bool init();
	bool ccTouchBegan(CCTouch *pTouches,CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	void randomRack();
	void exitGame(CCObject *pSender);
	int getHoldTile();
	void setHoldTile(int tile);
	int getHoldPosition();
	void setHoldPosition(int p);
	void takeTileFromRack(int position,int tag);
	void putTileToRack(int position, int tag);
	void update(float dt);
	void startGame(CCObject *pSender);
	void playTile(CCObject *pSender);
	void pass(CCObject *pSender);
	void showExchange(CCObject *pSender);
	void cancelExchange(CCObject *pSender);
	void exchange(CCObject *pSender);
	void pause(CCObject *pSender);
	void resume(CCObject *pSender);
	void hint(CCObject *pSender);
	void setLetterBag();
	void updateLetterBag(int index);
	char getLetterOnRack(int index);
	void computerPlayTurn();
	void setComputerPlay(float dt);
	void setEndgame();
	void changeBlank();
	void closeBlank();
	void updateTracker();
	void setBlankTag(int t);
	int getBlankTag();
	void goToEndGame(float dt);
	void takeBackTileOnBoard();
	void endPlayComputer();
	void updateLoading();
	void callBackStopStatus();
	CREATE_FUNC(GameHumanScene);
};

#endif