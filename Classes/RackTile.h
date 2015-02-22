#ifndef _RackTile_H_
#define _RackTile_H_

#include "cocos2d.h"
#include "GameHumanScene.h"

USING_NS_CC;
using namespace std;
typedef enum tagRackTileState 
{
    kRackTileStateGrabbed,
    kRackTileStateUngrabbed
} RackTileState; 

class RackTile : public CCSprite, public CCTargetedTouchDelegate
{
    RackTileState        m_state;

public:
    RackTile(void);
    virtual ~RackTile(void);

    CCRect rect();
	GameHumanScene* area;
	CCInteger* thingId;
	string scene;
	int tile;

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

	void setArea(GameHumanScene* scene);
	void setId(CCInteger* id);
	void setTile(int t);
	CCInteger* getId();

    static RackTile* RackTileWithTexture(CCTexture2D* aTexture);
};

#endif
