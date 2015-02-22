#ifndef _MENUSCENE_H_
#define _MENUSCENE_H_

#include "cocos2d.h"
#include <string>
USING_NS_CC;
using namespace std;

class MenuScene : public CCLayer
{
	
	
public:
	static CCScene *scene();
	bool init();
	void menuAiCallBack(CCObject *pSender);
	void menuPlayCallBack(CCObject *pSender);
	void menuHelpCallBack(CCObject *pSender);
	void menuAboutCallBack(CCObject *pSender);
	CREATE_FUNC(MenuScene);
};

#endif