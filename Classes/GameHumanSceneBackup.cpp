#include "GameHumanScene.h"
#include "MenuScene.h"

CCScene *GameHumanScene::scene()
{
	CCScene *scene = CCScene::create();
	GameHumanScene *layer = GameHumanScene::create();
	layer->setTag(999);
	scene->addChild(layer);

	return scene;
}

bool GameHumanScene::init()
{


	scorePlayer1 = 0;
	scorePlayer2 = 0;
	//25 menit
	timePlayer1 = 1500;
	timePlayer2 = 1500;
	TileOnBoard *_board[15][15];
	int i,j;
	int tempX,tempY; 
	screenSize = CCDirector::sharedDirector()->getWinSizeInPixels();

	_background = CCSprite::create("bg.png");
	_background->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	addChild(_background,0);
	
	//grid helper
	CCSprite *_grid_helper = CCSprite::create("grid_helper.png");
	_grid_helper->setPosition(ccp(screenSize.width*0.527,screenSize.height * 0.555));
	addChild(_grid_helper);

	//inisialisasi papan
	int posX = (screenSize.width/3) + 35;
	int posY = screenSize.height - (screenSize.height * 0.1);
	string fileName;
	for(i = 0 ; i < 15 ; i++)
	{
		for(j = 0 ; j < 15 ; j++)
		{
			if(basicTiles.getStatusBoardByPosition(i,j) == 0)
			{
				fileName = "board_tile.png";
			}
			else if(basicTiles.getStatusBoardByPosition(i,j) == 2)
			{
				fileName = "board_2L.png";
			}
			else if(basicTiles.getStatusBoardByPosition(i,j) == 3)
			{
				fileName = "board_3L.png";
			}
			else if(basicTiles.getStatusBoardByPosition(i,j) == 4)
			{
				fileName = "board_2w.png";
			}
			else if(basicTiles.getStatusBoardByPosition(i,j) == 6)
			{
				fileName = "board_3w.png";
			}
			else
			{
				fileName = "board_center.png";
			}
			_board[i][j] = TileOnBoard::TileOnBoardWithTexture(CCTextureCache::sharedTextureCache()->addImage(fileName.c_str()));
			_board[i][j]->setOriTile(fileName);
			_board[i][j]->setPosition(ccp(posX,posY));
			_board[i][j]->setTag((i * 15) + j);
			_board[i][j]->setArea(this);
			_board[i][j]->setGameType(1);
			_board[i][j]->setTileType(basicTiles.getStatusBoardByPosition(i,j));
			algorithm.getBoard().setTileType(basicTiles.getStatusBoardByPosition(i,j),i,j);
			addChild(_board[i][j]);
			posX+=35;
		}
		posX = (screenSize.width/3) + 35;
		posY -= 35;
	}



	//letter bag
	//CCSprite *_letterBag = CCSprite::create("letter_bag.png");
	//_letterBag->setPosition(ccp(screenSize.width * 15/100,screenSize.height * 45 / 100));
	//addChild(_letterBag,1);

	//score bg
	CCSprite *_scoreBg = CCSprite::create("score.png");
	_scoreBg->setPosition(ccp(screenSize.width * 15/100,screenSize.height * 75 / 100));
	addChild(_scoreBg,1);

	//time label
	countdownPlayer1 = CCLabelTTF::create("25:00","Arial",28);
	countdownPlayer1->setPosition(ccp(screenSize.width * 20/100,screenSize.height * 80 / 100));
	addChild(countdownPlayer1);
	countdownPlayer2 = CCLabelTTF::create("25:00","Arial",28);
	countdownPlayer2->setPosition(ccp(screenSize.width * 20/100,screenSize.height * 70 / 100));
	addChild(countdownPlayer2);

	//skor label
	scoreLabelPlayer1 = CCLabelTTF::create("0","Arial",28);
	scoreLabelPlayer1->setPosition(ccp(screenSize.width * 12/100,screenSize.height * 80 / 100));
	addChild(scoreLabelPlayer1);
	scoreLabelPlayer2 = CCLabelTTF::create("0","Arial",28);
	scoreLabelPlayer2->setPosition(ccp(screenSize.width * 12/100,screenSize.height * 70 / 100));
	addChild(scoreLabelPlayer2);


	//rack bg
	CCSprite *_rackBg = CCSprite::create("rack.png");
	_rackBg->setPosition(ccp((screenSize.width*0.535),screenSize.height * 8/100));
	addChild(_rackBg,1);

	//init tiles rack

	tempX = (screenSize.width/3) + 80;
	for(i = 0 ; i < 7 ; i++)
	{
		_tiles[i] = CCSprite::create("A.png");
		_tiles[i]->setPosition(ccp(tempX,screenSize.height * 8/100));
		addChild(_tiles[i],2);
		_arrTiles[i] = -1;
		_tiles[i]->setTag(300 + i);
		_tiles[i]->setVisible(false);
		tempX += 65;
	}
	//random tiles rack
	//randomRack();

	//title scrabble
	CCSprite* _title = CCSprite::create("title.png");
	_title->setPosition(ccp(screenSize.width *15 / 100 , screenSize.height * 92/100));
	_title->setScale(0.6);
	addChild(_title);

	//membuat button
	CCMenuItem *_exitButton = CCMenuItemImage::create("exit_btn.png","exit_btn.png",this,menu_selector(GameHumanScene::exitGame));
	_exitButton->setPosition(ccp(screenSize.width * 95 /100,screenSize.height * 95 /100));
	_startButton = CCMenuItemImage::create("start_btn.png","loading.png",this,menu_selector(GameHumanScene::startGame));
	_startButton->setPosition(ccp(screenSize.width *77 / 100 ,screenSize.height * 8/100));
	CCMenuItem *_pauseButton = CCMenuItemImage::create("pause_btn.png","pause_hover.png",this,menu_selector(GameHumanScene::pause));
	_pauseButton->setPosition(ccp(screenSize.width *10 / 100 , screenSize.height * 20/100));
	CCMenuItem *_passButton = CCMenuItemImage::create("pass_btn.png","pass_btn.png",this,menu_selector(GameHumanScene::pass));
	_passButton->setPosition(ccp(screenSize.width *25 / 100 , screenSize.height * 10/100));
	CCMenuItem *_hintButton = CCMenuItemImage::create("hint_btn.png","hint_hover.png",this,menu_selector(GameHumanScene::hint));
	_hintButton->setPosition(ccp(screenSize.width *25 / 100 , screenSize.height * 20/100));
	CCMenuItem *_exchangeButton = CCMenuItemImage::create("exchange_btn.png","exchange_btn.png",this,menu_selector(GameHumanScene::showExchange));
	_exchangeButton->setPosition(ccp(screenSize.width *10 / 100 , screenSize.height * 10/100));
	_executeButton = CCMenuItemImage::create("execute_btn.png","execute_btn.png",this,menu_selector(GameHumanScene::playTile));
	_executeButton->setPosition(ccp(screenSize.width *77 / 100 ,screenSize.height * 8/100));
	_executeButton->setVisible(false);

	//membuat menu
	_menu = CCMenu::create(_exitButton,_startButton,_pauseButton,_passButton,_hintButton,_exchangeButton,_executeButton,NULL);
	_menu->setPosition(CCPointZero);
	addChild(_menu,1);

	CCSprite *_box = CCSprite::create("box.png");
	_box->setPosition(ccp(screenSize.width *17 / 100 , screenSize.height * 35/100));
	addChild(_box);
	guideLabel = CCLabelTTF::create("Your guide is here","Arial",16);
	guideLabel->setPosition(ccp(screenSize.width * 17/100,screenSize.height * 35 / 100));
	addChild(guideLabel);


	letterBagLabel = CCLabelTTF::create("Left tiles : ","Arial",16);
	letterBagLabel->setPosition(ccp(screenSize.width * 15/100,screenSize.height * 60 / 100));
	addChild(letterBagLabel);
	wordLabel = CCLabelTTF::create("Word : ","Arial",16);
	wordLabel->setPosition(ccp(screenSize.width * 15/100,screenSize.height * 55 / 100));
	addChild(wordLabel);
	scoreLabel = CCLabelTTF::create("Score : ","Arial",16);
	scoreLabel->setPosition(ccp(screenSize.width * 15/100,screenSize.height * 50 / 100));
	addChild(scoreLabel);

	/*-----TRACKER BOX----*/

	_trackerBox = CCSprite::create("game_tracker.png");
	_trackerBox->setPosition(ccp(screenSize.width * 0.86 , screenSize.height * 0.57));
	addChild(_trackerBox);
	trackerLabel = CCLabelTTF::create("[Player , Word , Score]","Arial",16,CCSizeZero,kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
	trackerLabel->setPosition(ccp(screenSize.width * 0.86 , screenSize.height * 0.55));
	addChild(trackerLabel);

	/*----HINT BOX---*/
	_hintBox = CCSprite::create("hint_bg.png");
	_hintBox->setPosition(ccp(screenSize.width * 0.88 , screenSize.height * 0.52));
	_hintBox->setVisible(false);
	addChild(_hintBox);
	hintLabel = CCLabelTTF::create("Top 20 possible word will be \nshown here","Arial",16);
	hintLabel->setPosition(ccp(screenSize.width * 0.88 , screenSize.height * 0.50));
	addChild(hintLabel);
	hintLabel->setVisible(false);

	/*----BLANK BOX-----*/
	_blankBox = CCSprite::create("blank_box.png");
	_blankBox->setPosition(ccp(screenSize.width * 0.87 , screenSize.height * 0.60));
	_blankBox->setVisible(false);
	addChild(_blankBox);
	tempX = screenSize.width * 0.8;
	tempY = screenSize.height * 0.8;
	for(i = 0 ; i < 26 ; i++)
	{
		_blankTile[i] = CCSprite::create(basicTiles.getLetterFile(i));
		_blankTile[i]->setPosition(ccp(tempX,tempY));
		_blankTile[i]->setScale(0.7);
		_blankTile[i]->setVisible(false);
		tempX += (_blankTile[i]->getContentSize().width * 1.25 * 0.7);
		if((i + 1) % 5 == 0 && i > 0)
		{
			tempX = screenSize.width * 0.8;
			tempY -= (_blankTile[i]->getContentSize().height * 1.25 * 0.7);
		}
		addChild(_blankTile[i]);
	}

	/*----EXCHANGE BOX---------*/
	_exchangeBox = CCSprite::create("exchange_box.png");
	_exchangeBox->setPosition(ccp(screenSize.width * 0.88 , screenSize.height * 0.7));
	_exchangeBox->setVisible(false);
	addChild(_exchangeBox);
	tempX = screenSize.width * 0.8;
	tempY = screenSize.height * 0.75;
	for(i = 0 ; i < 7 ; i++)
	{
		_exchangeTile[i] = CCSprite::create("A.png");
		_exchangeTile[i]->setPosition(ccp(tempX,tempY));
		_exchangeTile[i]->setVisible(false);
		_exchangeIndex[i] = -1;
		tempX += (_exchangeTile[i]->getContentSize().width * 1.25);
		if(i == 3)
		{
			tempX = screenSize.width * 0.8;
			tempY -= (_exchangeTile[i]->getContentSize().height * 1.25);
		}
		addChild(_exchangeTile[i]);
	}

	CCMenuItem *_exchange =  CCMenuItemImage::create("exchange_btn_box.png","exchange_hover_box.png",this,menu_selector(GameHumanScene::exchange));
	_exchange->setPosition(screenSize.width * 0.82,screenSize.height * 0.55);
	CCMenuItem *_cancel =  CCMenuItemImage::create("cancel_btn.png","cancel_hover.png",this,menu_selector(GameHumanScene::cancelExchange));
	_cancel->setPosition(screenSize.width * 0.90,screenSize.height * 0.55);
	_exchangeMenu = CCMenu::create(_exchange,_cancel,NULL);
	_exchangeMenu->setPosition(CCPointZero);
	_exchangeMenu->setVisible(false);
	addChild(_exchangeMenu);

	//endgame component
	_winnerBg = CCSprite::create("end_game.png");
	_winnerBg->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.5));
	_winnerBg->setVisible(false);
	addChild(_winnerBg);

	winnerLabel = CCLabelTTF::create("PLAYER 1 WIN !","Arial",45);
	winnerLabel->setPosition(_winnerBg->getPosition());
	winnerLabel->setVisible(false);
	addChild(winnerLabel);

	//save game to file
	time_t now;
	time (&now);
	saveFile = ctime(&now); 
	std::replace(saveFile.begin(), saveFile.end(), ' ', '_');
	std::replace(saveFile.begin(), saveFile.end(), ':', ';');
	std::replace(saveFile.begin(), saveFile.end(), '\n', '.');
	saveFile += "txt";
	//set log game file
	logFile = "LOG_COMPUTER/" + saveFile;
	saveFile = "HUMAN/"+saveFile;
	CCLOG("file %s",saveFile.c_str());
	ofstream fileGame;
	fileGame.open(saveFile.c_str());
	fileGame <<"player	 word	 durasi	 skor	tiles	method\n\n\n";
	fileGame.close();
	fileGame.open(logFile.c_str());
	fileGame.close();

	ofstream ofile (logFile.c_str(), ios::app);
	ofile<<"******HUMAN TURN******\n\n";
	ofile.close();

	holdTile = -1;
	blankTag = -1;
	playerTurn = 1;
	sec = 0;
	mTime = 0;
	isPaused = true;
	totalPass = 0;
	playerTwo.initPlayer(this,1);

	algorithm.setUnseenLetter();

	//scheduleUpdate();
	//schedule(schedule_selector(GameHumanScene::update,1),60.0);

	setTouchEnabled(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

	return true;
}

void GameHumanScene::randomRack()
{
	int i;
	int temp;
	vector<int> letter;
	srand(time(NULL));
	if(playerTurn == 1)
	{
		for(i = 0 ; i < 7;i++)
		{
			if(_arrTiles[i] == -1)
			{
				if(totalTiles > 0)
				{
					temp = rand() % letterBag.size();
					letter = letterBag.at(temp);
					_arrTiles[i] = letter.at(0);
					CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addImage(basicTiles.getLetterFile(_arrTiles[i]));
					_tiles[i]->setTexture(tex);
					updateLetterBag(temp);
					totalTiles --;
					_tiles[i]->setVisible(true);
					_tiles[i]->setScale(1.0);
				}
			}
			else
			{
				_tiles[i]->setVisible(true);
			}
		}
	}
	else
	{
		vector<char> vectorRack;
		vectorRack = playerTwo.getRackVector();
		for(i = vectorRack.size() ; i < 7 ; i++)
		{
			if(totalTiles > 0)
			{
				temp = rand() % letterBag.size();
				letter = letterBag.at(temp);
				playerTwo.setTileOnRack(basicTiles.getCharByIndex(letter.at(0)));
				updateLetterBag(temp);
				totalTiles --;
			}
		}
	}

	letterBagLabel->setString(("Left tiles : " + to_string(totalTiles)).c_str());
}



bool GameHumanScene::ccTouchBegan(CCTouch *pTouches,CCEvent *pEvent)
{

	CCPoint location = pTouches->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);

	int status,i,j;
	status = 0;
	i = 0;
	if(playerTurn == 1)
	{
		//jika yang di klik adalah tile on rack
		if(_blankBox->isVisible() == false)
		{
			while(status == 0 && i < 7)
			{
				if(_tiles[i]->boundingBox().containsPoint(location) && _tiles[i]->isVisible())
				{
					if(_exchangeBox->isVisible())
					{
						_exchangeIndex[i] = i;
						_exchangeTile[i]->setVisible(true);
						_exchangeTile[i]->setTexture(CCTextureCache::sharedTextureCache()->addImage(basicTiles.getLetterFile(_arrTiles[i])));
						_tiles[i]->setVisible(false);
					}
					else
					{
						if(holdPosition >= 0)
						{
							_tiles[holdPosition]->setScale(1.0);
						}
						_tiles[i]->setScale(0.5);
						holdTile = _arrTiles[i];
						holdPosition = i;
						status = 1;
					}
				}
				else
				{
					i++;
				}
			}
		}
		if(_exchangeBox->isVisible())
		{
			//jika yg di klik tile on exchange box
			i = 0; 
			status = 0;
			while(i < 7 && status == 0)
			{
				if(_exchangeTile[i]->boundingBox().containsPoint(location) && _exchangeTile[i]->isVisible())
				{
					_exchangeTile[i]->setVisible(false);
					_tiles[i]->setVisible(true);
					_exchangeIndex[i] = -1;
					status = 1;
				}
				else
				{
					i++;
				}

			}
		}
		if(_blankBox->isVisible())
		{
			//jika yg di klik tile on blank box
			i = 0;
			status = 0;
			while(i < 26 && status == 0)
			{
				if(_blankTile[i]->boundingBox().containsPoint(location) && _blankTile[i]->isVisible())
				{
					((TileOnBoard *)getChildByTag(this->blankTag))->setLetter(basicTiles.getCharByIndex(i));
					((TileOnBoard *)getChildByTag(this->blankTag))->changeBlankTileImage(i);
					closeBlank();
					status = 1;
				}
				else
				{
					i++;
				}

			}
		}
	}
	return true;
}

void GameHumanScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
}
void GameHumanScene::exitGame(CCObject *pSender)
{
	CCDirector::sharedDirector()->pushScene(CCTransitionSlideInL::create(1,MenuScene::scene()) );
}

int GameHumanScene::getHoldTile()
{
	return holdTile;
}

void GameHumanScene::setHoldTile(int tile)
{
	holdTile = tile;
}

int GameHumanScene::getHoldPosition()
{
	return holdPosition;
}

void GameHumanScene::setHoldPosition(int p)
{
	holdPosition = p;
}

void GameHumanScene::takeTileFromRack(int position,int tag)
{
	_tiles[position]->setVisible(false);
	tileOnBoardTag.push_back(tag);
}

void GameHumanScene::putTileToRack(int position,int tag)
{
	_tiles[position]->setVisible(true);
	_tiles[position]->setScale(1.0);
	
	vector<int>::iterator found;
	found = std::find(tileOnBoardTag.begin(), tileOnBoardTag.end(),tag) ;
	if (found!=tileOnBoardTag.end())tileOnBoardTag.erase(found);
	
}

char GameHumanScene::getLetterOnRack(int index)
{
	return basicTiles.getCharByIndex(_arrTiles[index]);
}

void GameHumanScene::update(float dt)
{
	if(isPaused == false)
	{
		sec++;
		string waktu;
		if(playerTurn == 1 && sec == 60)
		{
			timePlayer1--;
			waktu = to_string(timePlayer1 / 60) + ":" + to_string(timePlayer1 % 60);
			countdownPlayer1->setString(waktu.c_str());
			sec = 0;
		}
		if(timePlayer1 == 0 || timePlayer2 == 0)
		{
			setEndgame();
		}
	}
}

void GameHumanScene::startGame(CCObject *pSender)
{
	//SET KECERDASAN BUATAN - GADDAG
	//membuat file log game
	algorithm.getGaddag().setLogFile(logFile);
	//Membuat Graph
	algorithm.getGaddag().initGraph();
	algorithm.getGaddag().setHeuristic(false);
	//algorithm.getGaddag().startPlaying(7,7);
	setLetterBag();
	//game Awal dimainkan oleh player 1 (human)
	playerTurn = 1;
	randomRack();
	playerTurn = 2;
	randomRack();
	playerTurn = 1;
	_startButton->setVisible(false);
	_executeButton->setVisible(true);
	isPaused = false;
	/*_arrTiles[6] = 26;
	_tiles[6]->setTexture( CCTextureCache::sharedTextureCache()->addImage(basicTiles.getLetterFile(26)));*/

}

void GameHumanScene::playTile(CCObject *pSender)
{
	//scoring player 1 (human)

	algorithm.getBoard().setNewTilesOnBoard();
	string word = "";
	string hook = "";
	string newTiles = "";
	vector<char> rack;
	int startRow;
	int startCol;
	int direction = 0;
	int score = 0;
	int status = 0;
	int minTag;
	int i;
	int checkWordScore;
	int leftOnRack;
	string playerRack = "";
	for(i = 0 ; i < 7 ; i++)
	{
		if(_arrTiles[i] != -1){
			rack.push_back(basicTiles.getCharByIndex(_arrTiles[i]));
			playerRack+=basicTiles.getLetterByIndex(_arrTiles[i]);
		}
	}
	if(tileOnBoardTag.size() > 0 && playerTurn == 1)
	{
		sort(tileOnBoardTag.begin(),tileOnBoardTag.end());
		if(tileOnBoardTag.size() > 1)
		{
			//cek apakah ke samping atau menurun
			if(tileOnBoardTag.at(1) % 15 == tileOnBoardTag.at(0) % 15)
			{
				direction = 2;
			}
			else if(tileOnBoardTag.at(1) / 15 == tileOnBoardTag.at(0) / 15)
			{
				direction = 1;
			}
			if(direction != 0)
			{
				minTag = tileOnBoardTag.at(0);
				word = "";
				i = 0;
				while(i < tileOnBoardTag.size()  && status == 0)
				{
					if(minTag == tileOnBoardTag.at(i))
					{
						word += ((TileOnBoard *)getChildByTag(tileOnBoardTag.at(i)))->getLetter();
						i++;
						if(i < tileOnBoardTag.size())
						{
							if(tileOnBoardTag.at(i) / 15 != minTag / 15 && tileOnBoardTag.at(i) % 15 != minTag % 15)
							{
								//CCLOG("ini i %d",i);
								status = 1;
							}
						}
					}
					else if(((TileOnBoard *)getChildByTag(minTag))->getSetStatus() == true)
					{
						CCLOG("ambil %c di %d",((TileOnBoard *)getChildByTag(minTag))->getLetter(),minTag);
						word += ((TileOnBoard *)getChildByTag(minTag))->getLetter();
					}
					else
					{
						int stat = 1;
						if(((TileOnBoard *)getChildByTag(minTag))->getSetStatus() == false) stat = 0;
						CCLOG("ada false i ke %d tag %d status tile board %d",i,minTag,stat);
						status = 1;
					}
					if(direction == 1)
					{
						minTag++;
					}
					else
					{
						minTag += 15;
					}
				}
			}
			else
			{
				status = 1; //tidak valid
			}
		}
		else if(tileOnBoardTag.size() == 1)
		{
			word += ((TileOnBoard *)getChildByTag(tileOnBoardTag.at(0)))->getLetter();
			if(tileOnBoardTag.at(0) % 15 == 0)
			{
				if(((TileOnBoard *)getChildByTag(tileOnBoardTag.at(0) + 1))->getSetStatus() == true) direction = 1;
			}
			else if (tileOnBoardTag.at(0) % 15 == 14)
			{
				if(((TileOnBoard *)getChildByTag(tileOnBoardTag.at(0) - 1))->getSetStatus() == true) direction = 1;
			}
			else 
			{
				if(((TileOnBoard *)getChildByTag(tileOnBoardTag.at(0) - 1))->getSetStatus() == true || ((TileOnBoard *)getChildByTag(tileOnBoardTag.at(0) + 1))->getSetStatus() == true)
				{
					direction = 1;
				}
				else 
				{
					if(tileOnBoardTag.at(0) / 15 == 0)
					{
						if(((TileOnBoard *)getChildByTag(tileOnBoardTag.at(0) + 15))->getSetStatus() == true) direction = 2;
					}
					else if (tileOnBoardTag.at(0) / 15 == 14)
					{
						if(((TileOnBoard *)getChildByTag(tileOnBoardTag.at(0) - 15))->getSetStatus() == true) direction = 2;
					}
					else
					{
						if(((TileOnBoard *)getChildByTag(tileOnBoardTag.at(0) - 15))->getSetStatus() == true || ((TileOnBoard *)getChildByTag(tileOnBoardTag.at(0) + 15))->getSetStatus() == true) direction = 2;
					}
				}
			}
			//CCLOG("input word %c , tag %d, word %s",((TileOnBoard *)getChildByTag(tileOnBoardTag.at(0)))->getLetter(),tileOnBoardTag.at(0),word.c_str());
		}
		//jika valid
		if(status == 0)
		{
			startRow = tileOnBoardTag.at(0) / 15;
			startCol = tileOnBoardTag.at(0) % 15;
			/*CCLOG("row %d",startRow);
			CCLOG("col %d",startCol);
			CCLOG("direction %d",direction);*/
			algorithm.getGaddag().setActiveRack(rack);
			hook = algorithm.getGaddag().getHook(word,startRow,startCol,direction);
			word = hook + word;
			if(hook.length() > 0)
			{
				if(direction == 1)
				{
					startCol -=hook.length();
				}
				else
				{
					startRow -=hook.length();
				}
			}
			CCLOG("word %s",word.c_str());
			score = algorithm.getGaddag().wordScore(word, startRow, startCol, direction);
			if(score != -1)
			{
				CCLOG("score temp %d",score);
				if(tileOnBoardTag.size() == 7) score+=50;
				checkWordScore = algorithm.getGaddag().getScoreByAnchor(word, startRow, startCol, direction);
				if(checkWordScore != -1)
				{
					score += checkWordScore;
					algorithm.getGaddag().addWordToPapan(word,startRow,startCol,direction);
					for(i = 0 ; i < tileOnBoardTag.size() ; i++)
					{
						((TileOnBoard *)getChildByTag(tileOnBoardTag.at(i)))->setIsSetTile(true);
					}
					scorePlayer1+=score;
					scoreLabelPlayer1->setString(to_string(scorePlayer1).c_str());
					tileOnBoardTag.clear();
					for(i = 0 ; i < 7 ; i++)
					{
						if(!_tiles[i]->isVisible())
						{
							if(_arrTiles[i] != -1)newTiles += basicTiles.getCharByIndex(_arrTiles[i]);
							_arrTiles[i] = -1;
						}
					}
					//hasil word
					if(direction == 1)
					{
						startCol += word.length();
					}
					else
					{
						startRow += word.length();
					}
					minTag = (15 * startRow) + startCol;
					while(minTag < 225)
					{
						if(((TileOnBoard *)getChildByTag(minTag))->getSetStatus() == true)
						{
							word += ((TileOnBoard *)getChildByTag(minTag))->getLetter();
							if(direction == 1)
							{
								startCol++;
							}
							else
							{
								startRow++;
							}
							minTag = (15 * startRow) + startCol;
						}
						else
						{
							minTag = 225;
						}

					}
					leftOnRack = 0;
					for(i = 0 ; i < 7 ; i++)
					{
						if(_arrTiles[i] != -1) leftOnRack++;
					}

					CCLOG("new tiles on board : %s",newTiles.c_str());
					algorithm.updateUnseenLetterByWord(newTiles);
					ofstream ofile(saveFile.c_str(), ios::app);
					if ( ofile )
					{
						tracker.push_back("You               "+word+"     "+to_string(score));
						updateTracker();
						ofile<<"Player 1	"<<word<<"	"<<"--"<<"	"<<score<<"		"<<(7 - leftOnRack)<<"	HUMAN "<<"\n";
						ofile.close();
					}
					wordLabel->setString(("Word : " + word).c_str());
					scoreLabel->setString(("Score : " + to_string(score)).c_str());
					randomRack();
					totalPass = 0;
					leftOnRack = 7;
					for(i = 0 ; i < 7 ; i++)
					{
						if(_arrTiles[i] == -1) leftOnRack--;
					}	
					ofstream ofile2(logFile.c_str(), ios::app);
					ofile2<<"******HUMAN RESULT******\n";
					ofile2<<"RACK : "<<playerRack<<"\n";
					/*if(! _hintBox->isVisible())
					{
					algorithm.getGaddag().getPossibleMovement(rack);
					}*/
					ofile2<<"\n";
					ofile2<<"HUMAN MOVE : "<<word<<" , score : "<<score<<" , direction : "<<direction<<" , col : "<<startCol<<" , row : "<<startRow<<"\n\n\n";
					ofile2.close();
					if(totalTiles == 0 && leftOnRack == 0)
					{
						setEndgame();
					}
					else
					{

						/*--------COMPUTER TURN----------------*/
						
						_trackerBox->setVisible(true);
						trackerLabel->setVisible(true);
						guideLabel->setString("Computer player turn !");
						_menu->setVisible(false);
						_hintBox->setVisible(false);
						hintLabel->setVisible(false);
						scheduleOnce(schedule_selector(GameHumanScene::computerPlayTurn),1.0);
						//computerPlayTurn();
					}
					CCLOG("score %d",score);
				}
				else
				{
					guideLabel->setString("your word is not valid to be played \n Player 1 Turn!");
				}
			}
			else
			{
				guideLabel->setString("your word is not valid to be played \n Player 1 Turn!");
			}
		}
	}
}

void GameHumanScene::computerPlayTurn(float dt)
{
	
	_trackerBox->setVisible(true);
	trackerLabel->setVisible(true);
	int i;
	algorithm.getBoard().setNewTilesOnBoard();
	ofstream ofile2 (logFile.c_str(), ios::app);
	ofile2<<"******COMPUTER TURN******\n";
	ofile2<<"RACK : "<<playerTwo.getRackInString()<<"\n\n";
	ofile2.close();
	_hintBox->setVisible(false);
	hintLabel->setVisible(false);
	clock_t start;
	float durasi;
	string waktu;
	string word;
	int score;
	string method;
	//human rack
	playerTurn = 2;
	start = clock();
	if(totalTiles > 0)
	{
		algorithm.getGaddag().setHeuristic(true);
		playerTwo.setRack(algorithm.getGaddag().playGame(playerTwo.getRackVector()));
		ofstream ofile3 (logFile.c_str(), ios::app);
		ofile3<<"POSSIBLE MOVEMENT : \n";
		CCLOG("total possible movement: %d",algorithm.getGaddag().getPossibleMovement().size() );
		for(i = 0 ; i < algorithm.getGaddag().getPossibleMovement().size() ; i++)
		{
			ofile3<<algorithm.getGaddag().getPossibleMovement().at(i).word<<" ( col : "<<algorithm.getGaddag().getPossibleMovement().at(i).startCol<<" row: "<<algorithm.getGaddag().getPossibleMovement().at(i).startRow<<" ) score : "<<algorithm.getGaddag().getPossibleMovement().at(i).score<<"\n";
		}
		ofile3<<"\n\n";
		ofile3.close();
		score = algorithm.getGaddag().getScore();
		method = "GADDAG";
		word = algorithm.getGaddag().getEvalWord();
	}
	else
	{
		CCLOG("total petak kosong : %d",algorithm.getBoard().getTotalNotSetTile());
		vector<char> rack;
		for(i = 0 ; i < 7 ; i++)
		{
			if(_arrTiles[i] != -1) rack.push_back(basicTiles.getCharByIndex(_arrTiles[i]));
		}
		playerTwo.setRack(algorithm.playNegaScout(rack,playerTwo.getRackVector(),scorePlayer1,scorePlayer2,logFile));
		score = algorithm.getNegaScore();
		method = "NEGASCOUT";
		word = algorithm.getNegaWord();
	}
	durasi = ( clock() - start ) / (float) CLOCKS_PER_SEC;
	CCLOG("durasi %f",durasi);
	scorePlayer2+= score;
	if(score == 0 )
	{
		playerTwo.setPassTurn(playerTwo.getPassTurn() + 1);
		if(totalPass == 3 && playerTwo.getPassTurn() == 3)
		{
			setEndgame();
		}
	}
	else
	{
		playerTwo.setPassTurn(0);
		totalPass = 0;
	}
	scoreLabelPlayer2->setString(to_string(scorePlayer2).c_str());
	wordLabel->setString(("Word : " + word).c_str());
	scoreLabel->setString(("Score : " + to_string(score)).c_str());
	CCLOG("new tiles on board : %s",algorithm.getBoard().getNewTilesOnBoard().c_str());
	algorithm.updateUnseenLetterByWord(algorithm.getBoard().getNewTilesOnBoard());
	ofstream ofile(saveFile.c_str(), ios::app);
	if ( ofile )
	{
		tracker.push_back("Computer     "+word+"     "+to_string(score));
		updateTracker();
		ofile<<"Player 2	"<<word<<"	"<<durasi<<"	"<<score<<"		"<<(7  - playerTwo.getRackVector().size())<<"	"<<method<<"\n";
		ofile.close();
	}
	timePlayer2 = timePlayer2 - (int)durasi;
	CCLOG("time player 2 %d",timePlayer2);
	waktu = to_string(timePlayer2 / 60) + ":" + to_string(timePlayer2 % 60);
	countdownPlayer2->setString(waktu.c_str());
	randomRack();
	playerTurn = 1;
	_menu->setVisible(true);
	guideLabel->setString("Player 1 Turn !");
	if(totalTiles == 0 && playerTwo.getRackVector().size() == 0)
	{
		setEndgame();
	}

	ofile2<<"******HUMAN TURN******\n\n";
	ofile2.close();
	
	algorithm.getGaddag().setHeuristic(false);

}

void GameHumanScene::updateTracker()
{
	string game_tracker = "";
	int i = 0;
	int index = tracker.size() - 1;
	while(i < 15)
	{
		game_tracker =  tracker.at(index) +"\n"+ game_tracker;
		index--;
		i++;
		if(index == -1)
		{
			i = 15;
		}
	}
	trackerLabel->setString(game_tracker.c_str());
}

void GameHumanScene::pass(CCObject *pSender)
{
	_trackerBox->setVisible(true);
	trackerLabel->setVisible(true);
	if(playerTurn == 1)
	{
		
		_menu->setVisible(false);
		_hintBox->setVisible(false);
		hintLabel->setVisible(false);
		totalPass++;
		if(totalPass == 3 && playerTwo.getPassTurn() == 3)
		{
			setEndgame();
		}	
		else
		{
			guideLabel->setString("Computer player turn !");
			scheduleOnce(schedule_selector(GameHumanScene::computerPlayTurn),1.0);
		}
		ofstream ofile(saveFile.c_str(), ios::app);
		if ( ofile )
		{
			ofile<<"Player 1	"<<"	pass		--			0		0	\n";
			ofile.close();
		}
		ofstream ofile2 (logFile.c_str(), ios::app);
		ofile<<"--HUMAN PASS--\n\n";
		ofile.close();
	}

}


void GameHumanScene::showExchange(CCObject *pSender)
{
	if(isPaused == false)
	{

		_hintBox->setVisible(false);
		hintLabel->setVisible(false);
		
		_trackerBox->setVisible(false);
		trackerLabel->setVisible(false);
		_exchangeBox->setVisible(true);
		_exchangeMenu->setVisible(true);
		int i;
		for(i = 0 ; i < 7 ; i++)
		{
			_exchangeTile[i]->setVisible(false);
			_exchangeIndex[i] = -1;
		}
	}
}


void GameHumanScene::cancelExchange(CCObject *pSender)
{
	
	_trackerBox->setVisible(true);
	trackerLabel->setVisible(true);
	_hintBox->setVisible(false);
	hintLabel->setVisible(false);
	_exchangeBox->setVisible(false);
	_exchangeMenu->setVisible(false);
	int i;
	for(i = 0 ; i < 7 ; i++)
	{
		_exchangeTile[i]->setVisible(false);
		_exchangeIndex[i] = -1;
		_tiles[i]->setVisible(true);
	}
}

void GameHumanScene::exchange(CCObject *pSender)
{
	int i,temp,baru,changeStatus;
	vector<int> letter;
	srand(time(NULL));
	changeStatus = 0;
	string exchangeTile,newTile;
	for(i = 0 ; i < 7;i++)
	{
		if(_exchangeIndex[i] != -1)
		{
			if(totalTiles > 0)
			{
				temp = rand() % letterBag.size();
				letter = letterBag.at(temp);
				baru = letter.at(0);
				_tiles[i]->setTexture(CCTextureCache::sharedTextureCache()->addImage(basicTiles.getLetterFile(baru)));
				updateLetterBag(temp);
				_tiles[i]->setVisible(true);
				_tiles[i]->setScale(1.0);
				letter.clear();
				letter.push_back(_arrTiles[i]);
				letter.push_back(1);
				letterBag.push_back(letter);
				exchangeTile += " " + basicTiles.getLetterByIndex(_arrTiles[i]);
				newTile += " " + basicTiles.getLetterByIndex(baru);
				_arrTiles[i] = baru;
				changeStatus = 1;
			}
		}
	}
	if(changeStatus == 1) 
	{
		ofstream ofile(saveFile.c_str(), ios::app);
		if ( ofile )
		{
			ofile<<"Player 1	"<<"	pass		--			0		0	\n";
			ofile.close();
		}
		ofstream ofile2(logFile.c_str(),ios::app);
		if(ofile2)
		{
			ofile2<<"-----HUMAN EXCHANGE TILE-----\n\n";
			ofile2<<"Exchange : "<<exchangeTile<<"\n";
			ofile2<<"New Tiles : "<<newTile<<"\n\n\n";
			ofile2.close();
		}
		guideLabel->setString("Computer player turn !");
		scheduleOnce(schedule_selector(GameHumanScene::computerPlayTurn),1.0);
	}
	cancelExchange(pSender);
}

void GameHumanScene::hint(CCObject *pSender)
{
	if(((TileOnBoard*)getChildByTag((15*7)+7))->getSetStatus() == true)
	{
		vector<char> rack;
		hintResult.clear();
		string hintText = "";
		int i;
		for(i = 0;i<7;i++)
		{
			if(_arrTiles[i]!= -1)
			{
				rack.push_back(basicTiles.getCharByIndex(_arrTiles[i]));
			}
		}
		_trackerBox->setVisible(false);
		trackerLabel->setVisible(false);
		_exchangeBox->setVisible(false);
		_exchangeMenu->setVisible(false);
		for(i = 0 ; i < 7 ; i++)
		{
			_exchangeTile[i]->setVisible(false);
			_exchangeIndex[i] = -1;
		}
		_hintBox->setVisible(true);
		hintLabel->setVisible(true);
		ofstream ofile(logFile.c_str(), ios::app);
		ofile<<"-------HUMAN CLICK HINT-------\n\n";
	/*	hintResult = algorithm.getGaddag().getHint(rack);
		for(i = 0 ; i < hintResult.size() ; i++)
		{
			if(i < 20) hintText = hintText + hintResult.at(i);
		}*/
		hintLabel->setString(algorithm.getGaddag().getHint(rack).c_str());
		ofile<<"\n\n";
		ofile.close();
	}
}

void GameHumanScene::pause(CCObject *pSender)
{
	if(isPaused == false)
	{
		isPaused == true;
	}
	else
	{
		isPaused == false;
	}
}


void GameHumanScene::setLetterBag()
{
	int i;
	vector<int> letter;
	for(i = 0 ; i < 27 ; i++)
	{
		letter.push_back(i);
		letter.push_back(basicTiles.getTilesFrequentByIndex(i));
		letterBag.push_back(letter);
		letter.clear();
	}
	totalTiles = 100;

	letterBagLabel->setString(("Left tiles : " + to_string(totalTiles)).c_str());
}

void GameHumanScene::updateLetterBag(int index)
{
	int i;
	vector<int> letter = letterBag.at(index);
	if(letter.at(1) == 1)
	{
		letterBag.erase(letterBag.begin() + index);
	}
	else
	{
		letter.at(1) = letter.at(1) - 1;
		letterBag.at(index) = letter;
	}
}

void GameHumanScene::changeBlank()
{
		_trackerBox->setVisible(false);
		trackerLabel->setVisible(false);
	_blankBox->setVisible(true);
	int i;
	for(i = 0 ; i < 26 ; i++)
	{
		_blankTile[i]->setVisible(true);
	}
}
void GameHumanScene::closeBlank()
{
	if(_blankBox->isVisible())
	{
		_trackerBox->setVisible(true);
		trackerLabel->setVisible(true);
	}
	_blankBox->setVisible(false);
	int i;
	for(i = 0 ; i < 26 ; i++)
	{
		_blankTile[i]->setVisible(false);
	}
	blankTag = -1;
}

void GameHumanScene::setBlankTag(int t)
{
	blankTag = t;
}

int GameHumanScene::getBlankTag()
{
	return blankTag;
}
void GameHumanScene::setEndgame()
{
	isPaused = true;
	_winnerBg->setVisible(true);
	string winner;
	string timeP1,timeP2;
	int leftOnRack,i;
	if(scorePlayer1 > scorePlayer2)
	{
		winnerLabel->setString("PLAYER 1 WIN !");
		winner = "Player 1";
	}
	else if(scorePlayer2 > scorePlayer1)
	{
		winnerLabel->setString("PLAYER 2 WIN !");
		winner = "Player 2";
	}
	else
	{
		winnerLabel->setString("DRAW !");
		winner = "Draw";
	}
	if(winnerLabel->isVisible() == false)
	{
		leftOnRack = 0;
		for(i = 0 ; i < 7 ; i++)
		{
			if(_arrTiles[i] != -1) leftOnRack++;
		}
		ofstream ofile(saveFile.c_str(), ios::app);
		if ( ofile )
		{
			timeP1 = to_string((1500 - timePlayer1) / 60) + " menit " + to_string((1500 - timePlayer1) % 60)  + " detik ";
			timeP2 = to_string((1500 - timePlayer2) / 60) + " menit " + to_string((1500 - timePlayer2) % 60)  + " detik ";
			ofile <<"winner : "<<winner<<"\n";
			ofile <<"left tile (letter bag): "<<totalTiles<<"\n"; 
			ofile <<"player 1 durasi : "<< timeP1<<"\n";
			ofile <<"player 1 left tile : "<<leftOnRack<<"\n";
			ofile <<"player 1 total skor : "<<scorePlayer1<<"\n";
			ofile <<"player 2 durasi : "<< timeP2<<"\n";
			ofile <<"player 2 left tile : "<<playerTwo.getRackVector().size()<<"\n";
			ofile <<"player 2 total skor : "<<scorePlayer2<<"\n";
			ofile.close();
		}
	}
	winnerLabel->setVisible(true);
}