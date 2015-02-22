#ifndef _ENDGAMESCENE_H_
#define _ENDGAMESCENE_H_

#include "cocos2d.h"
#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include <algorithm>

USING_NS_CC;
using namespace std;

class EndGameScene : public CCLayer
{
	
	//string _logFile;
	CCLabelTTF* _tracker;
	CCLabelTTF* _tracker2;
	CCLabelTTF* _tracker3;
	mutex mtx;
	string _trackerResult;
	string _trackerResult2;
	string _trackerResult3;
	vector<string> track_vector;
	
public:
	static CCScene *scene(vector<string> track);
	static EndGameScene* create(vector<string> track);
	bool init();
	void setScene(float dt);
	void loadScene();
	void exit(CCObject *pSender);
	CREATE_FUNC(EndGameScene);
};

#endif