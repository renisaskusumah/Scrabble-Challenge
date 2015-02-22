#include "HelpScene.h"
#include "MenuScene.h"

CCScene *HelpScene::scene()
{
	CCScene *scene = CCScene::create();
	HelpScene *layer = HelpScene::create();
	scene->addChild(layer);
	
	return scene;
}

bool HelpScene::init()
{
	
	
	CCSize screenSize = CCDirector::sharedDirector()->getWinSizeInPixels();
	
	CCSprite *_background;
	CCSprite *_help;
	_background = CCSprite::create("assets/bg.png");
	_background->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	addChild(_background,0);

	//title
	CCSprite *_title = CCSprite::create("assets/title.png");
	_title->setPosition(ccp(screenSize.width/2,screenSize.height * 80/100));
	addChild(_title,1);
	
	//help
	_help = CCSprite::create("assets/help.png");
	_help->setPosition(ccp(screenSize.width/2,screenSize.height * 0.4));
	addChild(_help,1);

	//membuat button
	CCMenuItem *_exitButton = CCMenuItemImage::create("assets/exit_btn.png","assets/exit_btn.png",this,menu_selector(HelpScene::menuExitCallBack));
	_exitButton->setPosition(ccp(screenSize.width * 0.9 , screenSize.height * 0.9));


	//membuat menu
	CCMenu *_menu = CCMenu::create(_exitButton,NULL);
	_menu->setPosition(CCPointZero);
	addChild(_menu,1);
	
	
	return true;
}

void HelpScene::menuExitCallBack(CCObject *pSender)
{
	CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(1,MenuScene::scene()) );
	
}
