#ifndef _TILEONBOARD_H_
#define _TILEONBOARD_H_

#include "cocos2d.h"


#define COCOS2D_DEBUG 1
USING_NS_CC;
using namespace std;
typedef enum tagTileOnBoardState 
{
    kTileOnBoardStateGrabbed,
    kTileOnBoardStateUngrabbed
} TileOnBoardState; 

class TileOnBoard : public CCSprite, public CCTargetedTouchDelegate
{
    TileOnBoardState        m_state;

public:
    TileOnBoard(void);
    virtual ~TileOnBoard(void);

    CCRect rect();
	CCLayer* area;
	CCInteger* thingId;
	int position;
	//sudah dirangkai ke dalam kata
	bool isSet;
	bool isBlankTile;
	string oriTile;
	int tileType;
	int gameType;
	char letter;

    bool initWithTexture(CCTexture2D* aTexture);
    virtual void onEnter();
    virtual void onExit();
    bool containsTouchLocation(CCTouch* touch);
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual CCObject* copyWithZone(CCZone *pZone);
	//virtual void coba();

    virtual void touchDelegateRetain();
    virtual void touchDelegateRelease();

	void setArea(CCLayer* scene);
	void setId(CCInteger* id);
	void setOrderFromRack(int p);
	void setOriTile(string file);
	CCInteger* getId();
	void setTileType(int tt);
	int getTileType();
	void setIsSetTile(bool set);
	void setGameType(int gt);
	void setIsBlankTile(bool b);
	bool getBlankTileStatus();
	void changeTileImage(int index);
	void changeBlankTileImage(int index);
	void setLetter(char l);
	bool getSetStatus();
	char getLetter();
	void takeBack();
	int getTilePosition();

    static TileOnBoard* TileOnBoardWithTexture(CCTexture2D* aTexture);
};

#endif
