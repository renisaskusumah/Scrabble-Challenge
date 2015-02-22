#include "GameScene.h"

CCScene *GameScene::scene()
{
	CCScene *scene = CCScene::create();
	GameScene *layer = GameScene::create();
	scene->addChild(layer);

	return scene;
}

bool GameScene::init()
{
	int i,j,posX,posY;
	currTiles = 7;
	CCSprite *_rack;
	//simpan file gambar tiles
	char* _alphabet[26] = {"A.png","B.png","C.png","D.png","E.png","F.png","G.png","H.png","I.png","J.png","K.png","L.png","M.png","N.png","O.png","P.png","Q.png","R.png","S.png","T.png","U.png","V.png","W.png","X.png","Y.png","Z.png"}; 

	//set letter bag (jumlah kemunculan , bobot)
	//int letterBag[26][2] = {{9,1},{2,3},{2,3},{4,2},{12,1},{2,4},{3,2},{2,4},{9,1},{1,8},{1,5},{4,1},{2,3},{6,1},{8,1},{2,3},{1,10},{6,1},{4,1},{6,1},{4,1},{2,4}};
	setLetterBag();

	//set tiles yang ada di rack
	int _arrTiles[7] = {0,0,0,0,0,0,0};

	//kondisi awal papan kosong
	int arrPapan[15][15] = {
		{6,0,0,0,0,0,0,6,0,0,0,0,0,0,6},
		{0,4,0,2,0,3,0,0,0,3,0,2,0,4,0},
		{0,0,4,0,0,0,2,0,2,0,0,0,4,0,0},
		{2,0,0,4,0,0,0,2,0,0,0,4,0,0,2},
		{0,0,0,0,4,0,0,0,0,0,4,0,0,0,0},
		{0,3,0,0,0,3,0,0,0,3,0,0,0,3,0},
		{0,0,2,0,0,0,2,0,2,0,0,0,2,0,0},
		{6,0,0,2,0,0,0,-1,0,0,0,2,0,0,6},
		{0,0,2,0,0,0,2,0,2,0,0,0,2,0,0},
		{0,3,0,0,0,3,0,0,0,3,0,0,0,3,0},
		{0,0,0,0,4,0,0,0,0,0,4,0,0,0,0},
		{2,0,0,4,0,0,0,2,0,0,0,4,0,0,2},
		{0,0,4,0,0,0,2,0,2,0,0,0,4,0,0},
		{0,4,0,0,0,3,0,0,0,3,0,0,0,4,0},
		{6,0,0,2,0,0,0,6,0,0,0,2,0,0,6}
	};

	CCSize screenSize = CCDirector::sharedDirector()->getWinSizeInPixels();

	_background = CCSprite::create("assets/bg.png");
	_background->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	addChild(_background,0);

	_rack = CCSprite::create("assets/rack.png");
	_rack->setPosition(ccp((screenSize.width/2)+65,screenSize.height * 8/100));
	addChild(_rack,1);

	_possWord = CCSprite::create("assets/possWord.png");
	_possWord->setPosition(ccp(screenSize.width - 150 ,screenSize.height - 300));
	addChild(_possWord);

	CCSprite *_scoreBoard = CCSprite::create("assets/scoreboard.png");
	_scoreBoard->setPosition(ccp(screenSize.width * 15 / 100,screenSize.height * 70 / 100));
	addChild(_scoreBoard);

	CCSprite *_letterBag = CCSprite::create("assets/letter_bag.png");
	_letterBag->setPosition(ccp(screenSize.width * 15/100,screenSize.height * 25 / 100));
	addChild(_letterBag,1);

	letterBagLabel = CCLabelTTF::create("Letter", "Arial", 16);
	letterBagLabel->setPosition(ccp(screenSize.width * 15/100,screenSize.height * 20 / 100));
	addChild(letterBagLabel, 2);
	updateLetterBag();


	/*CCSprite *_scoreBg1 = CCSprite::create("score_bg.png");
	_scoreBg1->setPosition(ccp(screenSize.width * 8 / 100,screenSize.height * 40 / 100));
	addChild(_scoreBg1);
	CCSprite *_scoreBg2 = CCSprite::create("score_bg.png");
	_scoreBg2->setPosition(ccp((screenSize.width * 8 / 100) + (_scoreBg1->getContentSize().width *7/4) ,screenSize.height * 40 / 100));
	addChild(_scoreBg2);*/


	//inisialisasi papan
	posX = (screenSize.width/3) + 35;
	posY = screenSize.height - 35;
	for(i = 0 ; i < 15 ; i++)
	{
		for(j = 0 ; j < 15 ; j++)
		{
			if(arrPapan[i][j] == 0)
			{
				_board[i][j] = CCSprite::create("board_tile.png");
			}
			else if(arrPapan[i][j] == 2)
			{
				_board[i][j] = CCSprite::create("board_2L.png");
			}
			else if(arrPapan[i][j] == 3)
			{
				_board[i][j] = CCSprite::create("board_3L.png");
			}
			else if(arrPapan[i][j] == 4)
			{
				_board[i][j] = CCSprite::create("board_2w.png");
			}
			else if(arrPapan[i][j] == 6)
			{
				_board[i][j] = CCSprite::create("board_3w.png");
			}
			else
			{
				_board[i][j] = CCSprite::create("board_center.png");
			}
			_board[i][j]->setPosition(ccp(posX,posY));
			addChild(_board[i][j]);
			posX+=35;
		}
		posX = (screenSize.width/3) + 35;
		posY -= 35;
	}

	//membuat button
	CCMenuItem *_startButton = CCMenuItemImage::create("start_btn.png","start_hover.png",this,menu_selector(GameScene::menuStartCallBack));
	_startButton->setPosition(ccp(screenSize.width * 83 / 100,screenSize.height *93/100));
	CCMenuItem *_pauseButton = CCMenuItemImage::create("pause_btn.png","pause_hover.png",this,menu_selector(GameScene::menuPauseCallBack));
	_pauseButton->setPosition(ccp((screenSize.width * 83/ 100) + (_startButton->getContentSize().width + 10),screenSize.height *93/100));
	CCMenuItem *_exchangeButton = CCMenuItemImage::create("exchange_btn.png","exchange_hover.png",this,menu_selector(GameScene::menuExchangeCallBack));
	_exchangeButton->setPosition(ccp(screenSize.width * 85 / 100,screenSize.height * 7 / 100));

	//membuat menu
	CCMenu *_menu = CCMenu::create(_startButton,_pauseButton,_exchangeButton,NULL);
	_menu->setPosition(CCPointZero);
	addChild(_menu,1);

	//simpan tiles di rack 
	int tempX = (screenSize.width/3) + 80;
	for(i = 0 ; i < 7 ; i++)
	{
		_tiles[i] = CCSprite::create("A.png");
		_tiles[i]->setPosition(ccp(tempX,screenSize.height * 8/100));
		addChild(_tiles[i],2);
		tempX += 65;
	}

	//random tiles rack
	srand(time(NULL));
	for(i = 0 ; i < 7;i++)
	{
		if(_arrTiles[i] == 0)
		{
			_arrTiles[i] = (rand() % 26);
			CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addImage(_alphabet[_arrTiles[i]]);
			_tiles[i]->setTexture(tex);
		}
		tempX += 65;
	}

	return true;
}

void GameScene::menuStartCallBack(CCObject *pSender)
{

}

void GameScene::menuPauseCallBack(CCObject *pSender)
{

}

void GameScene::menuExchangeCallBack(CCObject *pSender)
{

}

void GameScene::setLetterBag()
{
	//int letterBag[26][2] = {{9,1},{2,3},{2,3},{4,2},{12,1},{2,4},{3,2},{2,4},{9,1},{1,8},{1,5},{4,1},{2,3},{6,1},{8,1},{2,3},{1,10},{6,1},{4,1},{6,1},{4,1},{2,4},{2,4},{1,8},{2,4},{1,10}};
	
	letterBag[0][0] = 9; letterBag[0][1] = 1;
	letterBag[1][0] = 2; letterBag[1][1] = 3;
	letterBag[2][0] = 2; letterBag[2][1] = 3;
	letterBag[3][0] = 4; letterBag[3][1] = 2;
	letterBag[4][0] = 12; letterBag[4][1] = 1;
	letterBag[5][0] = 2; letterBag[5][1] = 4;
	letterBag[6][0] = 3; letterBag[6][1] = 2;
	letterBag[7][0] = 2; letterBag[7][1] = 4;
	letterBag[8][0] = 9; letterBag[8][1] = 1;
	letterBag[9][0] = 1; letterBag[9][1] = 8;
	letterBag[10][0] = 1; letterBag[10][1] = 5;
	letterBag[11][0] = 4; letterBag[11][1] = 1;
	letterBag[12][0] = 2; letterBag[12][1] = 3;
	letterBag[13][0] = 6; letterBag[13][1] = 1;
	letterBag[14][0] = 8; letterBag[14][1] = 1;
	letterBag[15][0] = 2; letterBag[15][1] = 3;
	letterBag[16][0] = 1; letterBag[16][1] = 10;
	letterBag[17][0] = 6; letterBag[17][1] = 1;
	letterBag[18][0] = 4; letterBag[18][1] = 1;
	letterBag[19][0] = 6; letterBag[19][1] = 1;
	letterBag[20][0] = 4; letterBag[20][1] = 1;
	letterBag[21][0] = 2; letterBag[21][1] = 4;
	letterBag[22][0] = 2; letterBag[22][1] = 4;
	letterBag[23][0] = 1; letterBag[23][1] = 8;
	letterBag[24][0] = 2; letterBag[24][1] = 4;
	letterBag[25][0] = 1; letterBag[25][1] = 10;
	letterBag[26][0] = 2; letterBag[26][1] = 0;


}

void GameScene::updateLetterBag()
{
	
	string bagStatus = "Ax" + to_string(letterBag[0][0]) + "   Bx" + to_string(letterBag[1][0]) + "   Cx" + to_string(letterBag[2][0]) 
						+ "   Dx" + to_string(letterBag[3][0]) + "   Ex" + to_string(letterBag[4][0]) + "   Fx" + to_string(letterBag[5][0])+ "\n"
						+ "   Gx" + to_string(letterBag[6][0])  + "   Hx" + to_string(letterBag[7][0]) + "   Ix" + to_string(letterBag[8][0]) 
						+ "   Jx" + to_string(letterBag[9][0]) + "   Kx" + to_string(letterBag[10][0]) + "   Lx" + to_string(letterBag[11][0]) + "\n"
						+ "   Mx" + to_string(letterBag[12][0]) + "   Nx" + to_string(letterBag[13][0]) + "   Ox" + to_string(letterBag[14][0])
						+ "   Px" + to_string(letterBag[15][0])  + "   Qx" + to_string(letterBag[16][0]) + "   Rx" + to_string(letterBag[17][0])+ "\n"
						+ "   Sx" + to_string(letterBag[18][0]) + "   Tx" + to_string(letterBag[19][0]) + "   Ux" + to_string(letterBag[20][0]) 
						+ "   Vx" + to_string(letterBag[21][0]) + "   Wx" + to_string(letterBag[22][0]) + "   Xx" + to_string(letterBag[23][0])+ "\n"
						+ "   Yx" + to_string(letterBag[24][0])  + "   Zx" + to_string(letterBag[25][0]) +  "  Blank" + " x" + to_string(letterBag[26][0]) +"\n";



	letterBagLabel->setString(bagStatus.c_str());

}