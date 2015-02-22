#include "TileOnBoard.h"
#include "GameHumanScene.h"

TileOnBoard::TileOnBoard(void)
{
		position = -1;
		isSet = false;
		isBlankTile = false;
		//area = NULL;
}

TileOnBoard::~TileOnBoard(void)
{
}

CCRect TileOnBoard::rect()
{
	CCSize s = getContentSize();
    return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}

TileOnBoard* TileOnBoard::TileOnBoardWithTexture(CCTexture2D* aTexture)
{
    TileOnBoard* pTileOnBoard = new TileOnBoard();
    pTileOnBoard->initWithTexture( aTexture );
    pTileOnBoard->autorelease();

    return pTileOnBoard;
}


bool TileOnBoard::initWithTexture(CCTexture2D* aTexture)
{
    if( CCSprite::initWithTexture(aTexture) ) 
    {
        m_state = kTileOnBoardStateUngrabbed;
    }

    
    return true;
}

void TileOnBoard::onEnter()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCSprite::onEnter();
}

void TileOnBoard::onExit()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}    

bool TileOnBoard::containsTouchLocation(CCTouch* touch)
{
    return rect().containsPoint(convertTouchToNodeSpaceAR(touch));
}

bool TileOnBoard::ccTouchBegan(CCTouch* touch, CCEvent* event)
{

    if (m_state != kTileOnBoardStateUngrabbed) return false;
    if ( !containsTouchLocation(touch) ) return false;
    
    m_state = kTileOnBoardStateGrabbed;
	if(gameType == 1 && isSet == false)
	{
		if(position != -1)
		{
			this->setTexture(CCTextureCache::sharedTextureCache()->addImage(oriTile.c_str()));
			((GameHumanScene*)area)->putTileToRack(position,getTag());
			((GameHumanScene*)area)->closeBlank();
			letter = '-';
			position = -1;
			isBlankTile = false;
			((GameHumanScene*)area)->setBlankTag(-1);
		}
		else if(((GameHumanScene*)area)->getHoldTile() != -1)
		{
			changeTileImage(((GameHumanScene*)area)->getHoldTile());
			if(((GameHumanScene*)area)->getHoldTile() < 26)
			{
				((GameHumanScene*)area)->setHoldTile(-1);
				position = ((GameHumanScene*)area)->getHoldPosition();
				((GameHumanScene*)area)->takeTileFromRack(position,getTag());
				letter = ((GameHumanScene*)area)->getLetterOnRack(position);
				isBlankTile = false;
				((GameHumanScene*)area)->setBlankTag(-1);
			}
			else
			{
				if(((GameHumanScene*)area)->getBlankTag() == -1)
				{
					((GameHumanScene*)area)->setBlankTag(getTag());
					((GameHumanScene*)area)->changeBlank();
					((GameHumanScene*)area)->setHoldTile(-1);
					position = ((GameHumanScene*)area)->getHoldPosition();
					((GameHumanScene*)area)->takeTileFromRack(position,getTag());
					isBlankTile = true;
				}
			}
		}
	}
    return true;
}



CCObject* TileOnBoard::copyWithZone(CCZone *pZone)
{
    this->retain();
    return this;
}

void TileOnBoard::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    CCAssert(m_state == kTileOnBoardStateGrabbed, "TileOnBoard - Unexpected state!");    
    m_state = kTileOnBoardStateUngrabbed;
	
	//area->addHighligth(this->getPositionX(),this->getPositionY(),CCInteger::create(getTag()),thingId);

	
} 

void TileOnBoard::touchDelegateRetain()
{
    this->retain();
}

void TileOnBoard::touchDelegateRelease()
{
    this->release();
}

void TileOnBoard::setArea(CCLayer* scene)
{
	area = scene;
}

void TileOnBoard::setGameType(int gt)
{
	gameType = gt;
}

void TileOnBoard::setId(CCInteger* id)
{
	thingId = id;
}


CCInteger* TileOnBoard::getId()
{
	return thingId;
}

void TileOnBoard::setOriTile(string file)
{
	oriTile = file;
}

void TileOnBoard::setOrderFromRack(int p)
{
	position = p;
}

void TileOnBoard::setTileType(int tt)
{
	tileType = tt;
}

int TileOnBoard::getTileType()
{
	if (isSet != false)
	{
		tileType = 1;
	}
	return tileType;
}

void TileOnBoard::setIsSetTile(bool set)
{
	isSet = set;
}

bool TileOnBoard::getSetStatus()
{
	return isSet;
}

void TileOnBoard::setLetter(char l)
{
	letter = l;
}

char TileOnBoard::getLetter()
{
	return letter;
}

void TileOnBoard::setIsBlankTile(bool b)
{
	isBlankTile = b;
}

bool TileOnBoard::getBlankTileStatus()
{
	return isBlankTile;
}

void TileOnBoard::changeTileImage(int index)
{
	char* _alphabetSmall[27] = {"assets/A_small.png","assets/B_small.png","assets/C_small.png","assets/D_small.png","assets/E_small.png","assets/F_small.png","assets/G_small.png","assets/H_small.png","assets/I_small.png","assets/J_small.png","assets/K_small.png","assets/L_small.png","assets/M_small.png","assets/N_small.png","assets/O_small.png","assets/P_small.png","assets/Q_small.png","assets/R_small.png","assets/S_small.png","assets/T_small.png","assets/U_small.png","assets/V_small.png","assets/W_small.png","assets/X_small.png","assets/Y_small.png","assets/Z_small.png","assets/blank_small.png"}; 
	this->setTexture(CCTextureCache::sharedTextureCache()->addImage(_alphabetSmall[index]));
}

void TileOnBoard::changeBlankTileImage(int index)
{
	char* fileName[26] = {"assets/A_blank.png","assets/B_blank.png","assets/C_blank.png","assets/D_blank.png","assets/E_blank.png","assets/F_blank.png","assets/G_blank.png","assets/H_blank.png","assets/I_blank.png","assets/J_blank.png","assets/K_blank.png","assets/L_blank.png","assets/M_blank.png","assets/N_blank.png","assets/O_blank.png","assets/P_blank.png","assets/Q_blank.png","assets/R_blank.png","assets/S_blank.png","assets/T_blank.png","assets/U_blank.png","assets/V_blank.png","assets/W_blank.png","assets/X_blank.png","assets/Y_blank.png","assets/Z_blank.png"}; 
	this->setTexture(CCTextureCache::sharedTextureCache()->addImage(fileName[index]));
}

void TileOnBoard::takeBack()
{
	this->setTexture(CCTextureCache::sharedTextureCache()->addImage(oriTile.c_str()));
	((GameHumanScene*)area)->putTileToRack(position,getTag());
	((GameHumanScene*)area)->closeBlank();
	letter = '-';
	position = -1;
	isBlankTile = false;
	((GameHumanScene*)area)->setBlankTag(-1);
}

int TileOnBoard::getTilePosition()
{
	return position;
}