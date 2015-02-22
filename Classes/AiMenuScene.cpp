#include "AiMenuScene.h"
#include "MenuScene.h"
#include "GameAiScene.h"

CCScene *AiMenuScene::scene()
{
	CCScene *scene = CCScene::create();
	AiMenuScene *layer = AiMenuScene::create();
	scene->addChild(layer);

	return scene;
}

bool AiMenuScene::init()
{
	
	

	CCSize screenSize = CCDirector::sharedDirector()->getWinSizeInPixels();
	
	_background = CCSprite::create("bg.png");
	_background->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	addChild(_background,0);

	//title
	CCSprite *_title = CCSprite::create("title.png");
	_title->setPosition(ccp(screenSize.width/2,screenSize.height * 85/100));
	addChild(_title,1);

	//label vs
	CCSprite *_vs = CCSprite::create("vs_label.png");
	_vs->setPosition(ccp(screenSize.width*50/100,screenSize.height * 56/100));
	addChild(_vs);

	//membuat button
	CCMenuItem *_backButton = CCMenuItemImage::create("back_btn.png","back_hover.png",this,menu_selector(AiMenuScene::backMenu));
	_backButton->setPosition(ccp(screenSize.width * 5 / 100, screenSize.height * 92 /100));
	CCMenuItem *_testButton = CCMenuItemImage::create("test_btn.png","test_hover.png",this,menu_selector(AiMenuScene::testMenu));
	_testButton->setPosition(ccp(screenSize.width/2,screenSize.height * 20 / 100));
	
	//membuat menu
	CCMenu *_menu = CCMenu::create(_backButton,_testButton,NULL);
	_menu->setPosition(CCPointZero);
	addChild(_menu,1);

	//P1 dan P2

	_player1 = CCSprite::create("p1_btn.png");
	_player1->setPosition(ccp(screenSize.width*40/100,screenSize.height * 55/100));
	_player1->setTag(1);
	addChild(_player1);
	
	_player2 = CCSprite::create("p2_btn.png");
	_player2->setPosition(ccp(screenSize.width*60/100,screenSize.height * 55/100));
	_player2->setTag(2);
	addChild(_player2);
	
	
	//sprite jenis AI

	_ai1 = CCSprite::create("AI_1.png");
	_ai1->setPosition(ccp(screenSize.width*35/100,screenSize.height * 35/100));
	addChild(_ai1);

	_ai2 = CCSprite::create("AI_2.png");
	_ai2->setPosition(ccp(screenSize.width*45/100,screenSize.height * 35/100));
	addChild(_ai2);

	_ai3 = CCSprite::create("AI_3.png");
	_ai3->setPosition(ccp(screenSize.width*55/100,screenSize.height * 35/100));
	addChild(_ai3);

	_ai4 = CCSprite::create("AI_4.png");
	_ai4->setPosition(ccp(screenSize.width*65/100,screenSize.height * 35/100));
	addChild(_ai4);

	activePlayer = 0;
	this->setTouchEnabled(true);

	return true;
}

void AiMenuScene::backMenu(CCObject *pSender)
{
	CCDirector::sharedDirector()->pushScene(CCTransitionSlideInL::create(1,MenuScene::scene()) );
}

void AiMenuScene::testMenu(CCObject *pSender)
{
	CCDirector::sharedDirector()->pushScene(CCTransitionSlideInR::create(1,GameAiScene::scene()) );
}

void AiMenuScene::ccTouchesBegan(CCSet *pTouches,CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch *) pTouches->anyObject();
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);

	if(_player1->boundingBox().containsPoint(location))
	{
		if(activePlayer == 1)
		{
			_player1->setTexture(CCTextureCache::sharedTextureCache()->addImage("p1_btn.png"));
			activePlayer = 0;
		}
		else
		{
			_player1->setTexture(CCTextureCache::sharedTextureCache()->addImage("p1_hover.png"));
			_player2->setTexture(CCTextureCache::sharedTextureCache()->addImage("p2_btn.png"));
			activePlayer = 1;
		}
	}
	else if(_player2->boundingBox().containsPoint(location))
	{
		if(activePlayer == 2)
		{
			_player2->setTexture(CCTextureCache::sharedTextureCache()->addImage("p2_btn.png"));
			activePlayer = 0;
		}
		else
		{
			_player2->setTexture(CCTextureCache::sharedTextureCache()->addImage("p2_hover.png"));
			_player1->setTexture(CCTextureCache::sharedTextureCache()->addImage("p1_btn.png"));
			activePlayer = 2;
		}
	}

	if(_ai1->boundingBox().containsPoint(location))
	{
		if(activePlayer == 1 || activePlayer == 0)
		{
			_ai1->runAction(CCMoveTo::create(1,_player1->getPosition()));
		}
		else
		{
			_ai1->runAction(CCMoveTo::create(1,_player2->getPosition()));
		}
	}
	else if(_ai2->boundingBox().containsPoint(location))
	{
		if(activePlayer == 1 || activePlayer == 0)
		{
			_ai2->runAction(CCMoveTo::create(1,_player1->getPosition()));
		}
		else
		{
			_ai2->runAction(CCMoveTo::create(1,_player2->getPosition()));
		}
	}
	else if(_ai3->boundingBox().containsPoint(location))
	{
		if(activePlayer == 1 || activePlayer == 0)
		{
			_ai3->runAction(CCMoveTo::create(1,_player1->getPosition()));
		}
		else
		{
			_ai3->runAction(CCMoveTo::create(1,_player2->getPosition()));
		}
	}
	else if(_ai4->boundingBox().containsPoint(location))
	{
		if(activePlayer == 1 || activePlayer == 0)
		{
			_ai4->runAction(CCMoveTo::create(1,_player1->getPosition()));
		}
		else
		{
			_ai4->runAction(CCMoveTo::create(1,_player2->getPosition()));
		}
	}
}