#include "RackTile.h"

RackTile::RackTile(void)
{
}

RackTile::~RackTile(void)
{
}

CCRect RackTile::rect()
{
	CCSize s = getContentSize();
    return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}

RackTile* RackTile::RackTileWithTexture(CCTexture2D* aTexture)
{
    RackTile* pRackTile = new RackTile();
    pRackTile->initWithTexture( aTexture );
    pRackTile->autorelease();

    return pRackTile;
}


bool RackTile::initWithTexture(CCTexture2D* aTexture)
{
    if( CCSprite::initWithTexture(aTexture) ) 
    {
        m_state = kRackTileStateUngrabbed;
    }

    
    return true;
}

void RackTile::onEnter()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCSprite::onEnter();
}

void RackTile::onExit()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}    

bool RackTile::containsTouchLocation(CCTouch* touch)
{
    return rect().containsPoint(convertTouchToNodeSpaceAR(touch));
}

bool RackTile::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (m_state != kRackTileStateUngrabbed) return false;
    if ( !containsTouchLocation(touch) ) return false;
    
    m_state = kRackTileStateGrabbed;
	
	//area->setHoldTile(tile);
    return true;
}



CCObject* RackTile::copyWithZone(CCZone *pZone)
{
    this->retain();
    return this;
}

void RackTile::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    CCAssert(m_state == kRackTileStateGrabbed, "RackTile - Unexpected state!");    
    m_state = kRackTileStateUngrabbed;
	
	//area->addHighligth(this->getPositionX(),this->getPositionY(),CCInteger::create(getTag()),thingId);

	
} 

void RackTile::touchDelegateRetain()
{
    this->retain();
}

void RackTile::touchDelegateRelease()
{
    this->release();
}

void RackTile::setArea(GameHumanScene* scene)
{
	area = scene;
}

void RackTile::setId(CCInteger* id)
{
	thingId = id;
}

void RackTile::setTile(int t)
{
	tile = t;
}


CCInteger* RackTile::getId()
{
	return thingId;
}