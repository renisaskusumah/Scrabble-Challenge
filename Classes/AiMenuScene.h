#ifndef _AIMENUSCENE_H_
#define _AIMENUSCENE_H_

#include "cocos2d.h"
#include <string>
USING_NS_CC;
using namespace std;

class AiMenuScene : public CCLayer
{
	
	CCSprite *_background;
	CCSprite *_player1;
	CCSprite *_player2;
	CCSprite *_ai1; //GADDAG
	CCSprite *_ai2; //GADDAG + Strategy
	CCSprite *_ai3; //GADDAG + Nega
	CCSprite *_ai4; //GADDAG + Nega + Strategy
	int activePlayer;

public:
	static CCScene *scene();
	bool init();
	void playerClicked(CCObject *pSender);
	void backMenu(CCObject *pSender);
	void testMenu(CCObject *pSender);
	void ccTouchesBegan(CCSet *pTouches,CCEvent *pEvent);
	CREATE_FUNC(AiMenuScene);
};

#endif