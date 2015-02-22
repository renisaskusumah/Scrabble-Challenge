#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include "cocos2d.h"
#include <string>
USING_NS_CC;
using namespace std;

class GameScene : public CCLayer
{
	CCSprite *_board[15][15];
	CCSprite *_background;
	CCSprite *_tiles[7];
	CCSprite *_possWord;
	int currTiles;
	int _arrTiles[7];
	int letterBag[27][2];
	CCLabelTTF* letterBagLabel;

public:
	static CCScene *scene();
	bool init();
	void menuStartCallBack(CCObject *pSender);
	void menuPauseCallBack(CCObject *pSender);
	void menuExchangeCallBack(CCObject *pSender);\
	void setLetterBag();
	void updateLetterBag();
	CREATE_FUNC(GameScene);
};

#endif