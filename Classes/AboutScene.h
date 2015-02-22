#ifndef _ABOUTSCENE_H_
#define _ABOUTSCENE_H_

#include "cocos2d.h"
#include <string>
USING_NS_CC;
using namespace std;

class AboutScene : public CCLayer
{
	
public:
	static CCScene *scene();
	bool init();
	void menuExitCallBack(CCObject *pSender);
	CREATE_FUNC(AboutScene);
};

#endif