#include "EndGameScene.h"
#include "MenuScene.h"

CCScene *EndGameScene::scene(vector<string> track)
{
	CCScene *scene = CCScene::create();
	EndGameScene *layer = EndGameScene::create(track);
	scene->addChild(layer);

	return scene;
}

EndGameScene* EndGameScene::create(vector<string> track) {

	EndGameScene *es = new EndGameScene();

	if (es->init()) {
		es->autorelease();
		es->track_vector = track;
	} else
		es = NULL;

	return es;

}


bool EndGameScene::init()
{


	CCSize screenSize = CCDirector::sharedDirector()->getWinSizeInPixels();
	
	CCSprite *_background;
	_background = CCSprite::create("assets/bg.png");
	_background->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	addChild(_background,0);

	//title
	CCSprite *_title = CCSprite::create("assets/title.png");
	_title->setPosition(ccp(screenSize.width*0.5,screenSize.height * 0.9));
	_title->setScale(0.5);
	addChild(_title,1);

	//membuat button
	CCMenuItem *_exitButton = CCMenuItemImage::create("assets/exit_btn.png","assets/exit_btn.png",this,menu_selector(EndGameScene::exit));
	_exitButton->setPosition(ccp(screenSize.width * 95 /100,screenSize.height * 95 /100));

	//membuat menu
	CCMenu *_menu;
	_menu = CCMenu::create(_exitButton,NULL);
	_menu->setPosition(CCPointZero);
	addChild(_menu,1);

	_tracker = CCLabelTTF::create("","Arial",16,CCSizeZero,kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
	_tracker->setPosition(ccp(screenSize.width * 0.40,screenSize.height * 0.45));
	addChild(_tracker);
	_tracker2 = CCLabelTTF::create("","Arial",16,CCSizeZero,kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
	_tracker2->setPosition(ccp(screenSize.width * 0.60,screenSize.height * 0.45));
	addChild(_tracker2);
	_tracker3 = CCLabelTTF::create("","Arial",16,CCSizeZero,kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
	_tracker3->setPosition(ccp(screenSize.width * 0.80,screenSize.height * 0.45));
	addChild(_tracker3);

	//_logFile = "HUMAN/Sat_Sep_13_20;42;28_2014.txt";
	scheduleOnce(schedule_selector(EndGameScene::setScene),0.5);

	return true;
}

void EndGameScene::setScene(float dt)
{
	std::thread t(&EndGameScene::loadScene,this);
	t.join();
	_tracker->setString(_trackerResult.c_str());
	_tracker2->setString(_trackerResult2.c_str());
	_tracker3->setString(_trackerResult3.c_str());
}

void EndGameScene::loadScene(){
	mtx.lock();

	/*ifstream file;
	file.open(_logFile.c_str());*/
	string str; 
	_trackerResult = "";
	_trackerResult2 = "";
	int i = 0;
	size_t pos; 
	while (i<track_vector.size())
	{
		str = track_vector.at(i);
		if(i < 25)
		{
			_trackerResult += "\n" + str;
		}
		else if(i < 50)
		{
			_trackerResult2 += "\n" + str;
		}
		else
		{
			_trackerResult3 += "\n" + str;
		}
		i++;
	}
	//file.close();

	mtx.unlock();
}

void EndGameScene::exit(CCObject *pSender)
{
	CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInL::create(1,MenuScene::scene()) );

}
