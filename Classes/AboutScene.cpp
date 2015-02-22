#include "AboutScene.h"
#include "MenuScene.h"

CCScene *AboutScene::scene()
{
	CCScene *scene = CCScene::create();
	AboutScene *layer = AboutScene::create();
	scene->addChild(layer);

	return scene;
}

bool AboutScene::init()
{
	CCSprite *_background;
	CCSprite *_about;
	CCSize screenSize = CCDirector::sharedDirector()->getWinSizeInPixels();
	
	_background = CCSprite::create("assets/bg.png");
	_background->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	addChild(_background,0);

	//title
	CCSprite *_title = CCSprite::create("assets/title.png");
	_title->setPosition(ccp(screenSize.width/2,screenSize.height * 80/100));
	addChild(_title,1);
	
	//about
	_about = CCSprite::create("assets/about.png");
	_about->setPosition(ccp(screenSize.width/2,screenSize.height * 0.5));
	addChild(_about,1);

	//membuat button
	CCMenuItem *_exitButton = CCMenuItemImage::create("assets/exit_btn.png","assets/exit_btn.png",this,menu_selector(AboutScene::menuExitCallBack));
	_exitButton->setPosition(ccp(screenSize.width * 0.9 , screenSize.height * 0.9));


	//membuat menu
	CCMenu *_menu = CCMenu::create(_exitButton,NULL);
	_menu->setPosition(CCPointZero);
	addChild(_menu,1);
	
	
	return true;
}

void AboutScene::menuExitCallBack(CCObject *pSender)
{
	CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(1,MenuScene::scene()) );
	
}
