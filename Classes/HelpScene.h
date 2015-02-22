#ifndef _HELPSCENE_H_
#define _HELPSCENE_H_

#include "cocos2d.h"
#include <string>
USING_NS_CC;
using namespace std;

class HelpScene : public CCLayer
{
	
	
public:
	static CCScene *scene();
	bool init();
	void menuExitCallBack(CCObject *pSender);
	CREATE_FUNC(HelpScene);
};

#endif