#include "GameAiScene.h"
#include "MenuScene.h"

CCScene *GameAiScene::scene()
{
	CCScene *scene = CCScene::create();
	GameAiScene *layer = GameAiScene::create();
	layer->setTag(999);
	scene->addChild(layer);

	return scene;
}

bool GameAiScene::init()
{
	
	scorePlayer1 = 0;
	scorePlayer2 = 0;
	//25 menit
	timePlayer1 = 1500;
	timePlayer2 = 1500;
	TileOnBoard* _board[15][15];
	int i,j,k;
	screenSize = CCDirector::sharedDirector()->getWinSizeInPixels();

	
	_background = CCSprite::create("assets/bg.png");
	_background->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	addChild(_background,0);


	//inisialisasi papan
	int posX = (screenSize.width/3) + 35;
	int posY = screenSize.height - 35;
	string fileName;
	for(i = 0 ; i < 15 ; i++)
	{
		for(j = 0 ; j < 15 ; j++)
		{
			if(basicTiles.getStatusBoardByPosition(i,j) == 0)
			{
				fileName = "assets/board_tile.png";
			}
			else if(basicTiles.getStatusBoardByPosition(i,j) == 2)
			{
				fileName = "assets/board_2L.png";
			}
			else if(basicTiles.getStatusBoardByPosition(i,j) == 3)
			{
				fileName = "assets/board_3L.png";
			}
			else if(basicTiles.getStatusBoardByPosition(i,j) == 4)
			{
				fileName = "assets/board_2w.png";
			}
			else if(basicTiles.getStatusBoardByPosition(i,j) == 6)
			{
				fileName = "assets/board_3w.png";
			}
			else
			{
				fileName = "assets/board_center.png";
			}
			_board[i][j] = TileOnBoard::TileOnBoardWithTexture(CCTextureCache::sharedTextureCache()->addImage(fileName.c_str()));
			_board[i][j]->setPosition(ccp(posX,posY));
			_board[i][j]->setTag((i * 15) + j);
			_board[i][j]->setIsSetTile(false);
			_board[i][j]->setTileType(basicTiles.getStatusBoardByPosition(i,j));
			algorithm.getBoard().setTileType(basicTiles.getStatusBoardByPosition(i,j),i,j);
			addChild(_board[i][j]);
			posX+=35;
			papan[i][j] = 0;
		}
		posX = (screenSize.width/3) + 35;
		posY -= 35;
	}


	//score bg
	CCSprite *_scoreBg = CCSprite::create("assets/score.png");
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

	//random tiles rack
	setLetterBag();
	//randomRack();
	
	//membuat button
	CCMenuItem *_exitButton = CCMenuItemImage::create("assets/exit_btn.png","assets/exit_btn.png",this,menu_selector(GameAiScene::exitGame));
	_exitButton->setPosition(ccp(screenSize.width * 95 /100,screenSize.height * 95 /100));
	_startButton = CCMenuItemImage::create("assets/start_btn.png","assets/loading.png",this,menu_selector(GameAiScene::startGame));
	_startButton->setPosition(ccp(screenSize.width *10 / 100 , screenSize.height * 92/100));
	CCMenuItem *_pauseButton = CCMenuItemImage::create("assets/pause_btn.png","assets/pause_hover.png",this,menu_selector(GameAiScene::pause));
	_pauseButton->setPosition(ccp(screenSize.width *22 / 100 , screenSize.height * 92/100));
	CCMenuItem *_executeButton = CCMenuItemImage::create("assets/execute_btn.png","assets/execute_hover.png",this,menu_selector(GameAiScene::playGame));
	_executeButton->setPosition(ccp(screenSize.width *0.55 ,screenSize.height * 8/100));
	_executeButton->setVisible(false);

	//membuat menu
	CCMenu *_menu = CCMenu::create(_exitButton,_startButton,_pauseButton,_executeButton,NULL);
	_menu->setPosition(CCPointZero);
	addChild(_menu,1);

	//_loading = CCSprite::create("loading.png");
	//_loading->setPosition(ccp(screenSize.width * 0.5 , screenSize.height * 0.5));
	//_loading->setVisible(false);
	//addChild(_loading,2);
	
	CCSprite* tracker = CCSprite::create("assets/tracker.png");
	tracker->setPosition(ccp(screenSize.width * 0.88 , screenSize.height * 0.52));
	addChild(tracker);
	trackerLabel = CCLabelTTF::create("Top 20 possible word will be \nshown here","Arial",18);
	trackerLabel->setPosition(ccp(screenSize.width * 0.88 , screenSize.height * 0.50));
	addChild(trackerLabel);

	wordLabel = CCLabelTTF::create("Word : ","Arial",18);
	wordLabel->setPosition(ccp(screenSize.width * 0.2, screenSize.height * 0.5));
	addChild(wordLabel);
	scoreLabel = CCLabelTTF::create("Score : ","Arial",18);
	scoreLabel->setPosition(ccp(screenSize.width * 0.2, screenSize.height * 0.45));
	addChild(scoreLabel);
	letterBagLabel = CCLabelTTF::create("Left tiles : ","Arial",18);
	letterBagLabel->setPosition(ccp(screenSize.width * 0.2, screenSize.height * 0.55));
	addChild(letterBagLabel);
	rackLabel = CCLabelTTF::create("Player 1 rack : ","Arial",20);
	rackLabel->setPosition(ccp(screenSize.width * 0.2, screenSize.height * 0.35));
	addChild(rackLabel);

	_winnerBg = CCSprite::create("assets/end_game.png");
	_winnerBg->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.5));
	_winnerBg->setVisible(false);
	addChild(_winnerBg);

	winnerLabel = CCLabelTTF::create("PLAYER 1 WIN !","Arial",45);
	winnerLabel->setPosition(_winnerBg->getPosition());
	winnerLabel->setVisible(false);
	addChild(winnerLabel);

	playerTurn = 1;
	isPaused = true;
	sec = 0;
	mTime = 0;
	scheduleUpdate();
	
	/*INIT PLAYER*/
	playerOne.initPlayer(this,2);
	playerTwo.initPlayer(this,2);


	//save game to file
	time_t now;
	time (&now);
	saveFile = ctime(&now); 
	std::replace(saveFile.begin(), saveFile.end(), ' ', '_');
	std::replace(saveFile.begin(), saveFile.end(), ':', ';');
	std::replace(saveFile.begin(), saveFile.end(), '\n', '.');
	saveFile += "txt";
	saveFile = "GADDAG/"+saveFile;
	CCLOG("file %s",saveFile.c_str());
	ofstream fileGame;
	fileGame.open(saveFile.c_str());
	fileGame <<"player word durasi skor tiles\n\n\n";
	fileGame.close();
	
	return true;
}

void GameAiScene::randomRack()
{
	int i,temp;
	vector<int> letter;
	vector<char> vectorRack;
	if(playerTurn == 1)
	{	
		vectorRack = playerOne.getRackVector();
	}
	else
	{
		vectorRack = playerTwo.getRackVector();
	}
	srand(time(NULL));
	for(i = vectorRack.size() ; i < 7 ; i++)
	{
		if(totalTiles > 0)
		{
			temp = rand() % letterBag.size();
			letter = letterBag.at(temp);
			if(playerTurn == 1)
			{	
				playerOne.setTileOnRack(basicTiles.getCharByIndex(letter.at(0)));
			}
			else
			{
				playerTwo.setTileOnRack(basicTiles.getCharByIndex(letter.at(0)));
			}
			updateLetterBag(temp);
			totalTiles --;
		}
	}
	letterBagLabel->setString(("Left tiles : " + to_string(totalTiles)).c_str());
	if(playerTurn == 1)
	{	
		rackLabel->setString(("Player 1 rack : " + playerOne.getRackInString()).c_str());
		
	}
	else
	{	
		rackLabel->setString(("Player 2 rack : " + playerTwo.getRackInString()).c_str());
	}
	
}

void GameAiScene::ccTouchesBegan(CCSet *pTouches,CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch *) pTouches->anyObject();
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
	CCLOG("ada sebelum");
	
	
}
void GameAiScene::exitGame(CCObject *pSender)
{
	CCDirector::sharedDirector()->pushScene(CCTransitionSlideInL::create(1,MenuScene::scene()) );
}


void GameAiScene::startGame(CCObject *pSender)
{
	
	//SET KECERDASAN BUATAN - GADDAG
	//Membuat Graph
	algorithm.getGaddag().initGraph();
	//game Awal dimainkan oleh player 1
	playerTurn = 2;
	randomRack();
	playerTurn = 1;
	randomRack();
	playerTurn = 1;
	//algorithm.getGaddag().startPlaying(7,7);
	_startButton->setVisible(false);
	isPaused = false;
	schedule(schedule_selector(GameAiScene::showPlayerTurn));
		 
}

void GameAiScene::playGame(CCObject *pSender)
{
	
	//isPaused = true;
	
	/*schedule(schedule_selector(GameAiScene::scheduleGame,0.0f));*/
	showPlayerTurn(0.0f);
	
}

void GameAiScene::showPlayerTurn(float dt)
{
	clock_t start;
    float durasi;
	string waktu;
	if(isPaused == false)
	{
		if(playerTurn == 1)
		{
				
			start = clock();
			playerOne.setRack(algorithm.getGaddag().playGame(playerOne.getRackVector()));
			durasi = ( clock() - start ) / (float) CLOCKS_PER_SEC;
			playerOne.addDurasi(durasi);
			scorePlayer1+= algorithm.getGaddag().getScore();
			if(algorithm.getGaddag().getScore()  == 0 )
			{
				playerOne.setPassTurn(playerOne.getPassTurn() + 1);
			}
			else
			{
				playerOne.setPassTurn(0);
				playerTwo.setPassTurn(0);
			}
			scoreLabelPlayer1->setString(to_string(scorePlayer1).c_str());
			timePlayer1 = timePlayer1 - (int)durasi;
			waktu = to_string(timePlayer1 / 60) + ":" + to_string(timePlayer1 % 60);
			countdownPlayer1->setString(waktu.c_str());
			ofstream ofile(saveFile.c_str(), ios::app);
			if ( ofile )
			{
				ofile<<"Player 1	"<<algorithm.getGaddag().getEvalWord()<<"	"<<durasi<<"	"<<algorithm.getGaddag().getScore()<<"		"<<(7  - playerOne.getRackVector().size())<<"	"<<algorithm.getGaddag().getUseBlank()<<"\n";
				ofile.close();
			}
			randomRack();
			rackLabel->setString(("Player 2 rack : " + playerTwo.getRackInString()).c_str());
			playerTurn = 2;
		}
		else
		{
			
			start = clock();
			playerTwo.setRack(algorithm.getGaddag().playGame(playerTwo.getRackVector()));
			durasi = ( clock() - start ) / (float) CLOCKS_PER_SEC;
			playerTwo.addDurasi(durasi);
			scorePlayer2+= algorithm.getGaddag().getScore();
			if(algorithm.getGaddag().getScore()  == 0 )
			{
				playerTwo.setPassTurn(playerTwo.getPassTurn() + 1);
			}
			else
			{
				playerOne.setPassTurn(0);
				playerTwo.setPassTurn(0);
			}
			scoreLabelPlayer2->setString(to_string(scorePlayer2).c_str());
			timePlayer2 = timePlayer2 - (int)durasi;
			waktu = to_string(timePlayer2 / 60) + ":" + to_string(timePlayer2 % 60);
			countdownPlayer2->setString(waktu.c_str());
			ofstream ofile(saveFile.c_str(), ios::app);
			if ( ofile )
			{
				ofile<<"Player 2 	"<<algorithm.getGaddag().getEvalWord()<<"	"<<durasi<<"	"<<algorithm.getGaddag().getScore()<<"		"<<(7  - playerTwo.getRackVector().size())<<"	"<<algorithm.getGaddag().getUseBlank()<<"\n";
				ofile.close();
			}
			randomRack();
			rackLabel->setString(("Player 1 rack : " + playerOne.getRackInString()).c_str());
			playerTurn = 1;
		}
		ofstream fileGame;
	fileGame.open("aa.txt");
	fileGame <<"player word durasi skor tiles\n\n\n";
	fileGame.close();
		trackerLabel->setString(algorithm.getGaddag().getAllPossibleWord().c_str());
		wordLabel->setString(("Word : " + algorithm.getGaddag().getEvalWord()).c_str());
		scoreLabel->setString(("Score : " + to_string(algorithm.getGaddag().getScore())).c_str());
	}
	if(playerOne.getPassTurn() == 3 && playerTwo.getPassTurn() == 3)
	{
		isPaused = true;
		string winner;
		string timeP1,timeP2;
		int finalP1,finalP2;
		int i;
		_winnerBg->setVisible(true);
		if(scorePlayer1 > scorePlayer2)
		{
			winnerLabel->setString("PLAYER 1 WIN !");
			winner = "Player 1";
		}
		else
		{
			winnerLabel->setString("PLAYER 2 WIN !");
			winner = "Player 2";
		}
		if(winnerLabel->isVisible() == false)
		{
			ofstream ofile(saveFile.c_str(), ios::app);
			if ( ofile )
			{
				finalP1 = scorePlayer1;
				for (i = 0 ; i < playerOne.getRackVector().size() ; i++)
				{
					finalP1 -= basicTiles.getTileScoreByChar(playerOne.getRackVector().at(i));
				}
				finalP2 = scorePlayer2;
				for (i = 0 ; i < playerTwo.getRackVector().size() ; i++)
				{
					finalP2-= basicTiles.getTileScoreByChar(playerTwo.getRackVector().at(i));
				}
				//timeP1 = to_string((1500 - timePlayer1) / 60) + " menit " + to_string((1500 - timePlayer1) % 60)  + " detik ";
				//timeP2 = to_string((1500 - timePlayer2) / 60) + " menit " + to_string((1500 - timePlayer2) % 60)  + " detik ";
				ofile <<"winner : "<<winner<<"\n";
				ofile <<"left tile (letter bag): "<<totalTiles<<"\n"; 
				ofile <<"player 1 durasi : "<<playerOne.getTotalDurasi()<<"\n";
				ofile <<"player 1 left tile : "<<playerOne.getRackVector().size()<<"\n";
				ofile <<"player 1 total skor : "<<scorePlayer1<<"\n";
				ofile <<"player 1 Final skor : "<<finalP1<<"\n"; //dikurangi sisa tile
				ofile <<"player 2 durasi : "<< playerTwo.getTotalDurasi()<<"\n";
				ofile <<"player 2 left tile : "<<playerTwo.getRackVector().size()<<"\n";
				ofile <<"player 2 total skor : "<<scorePlayer2<<"\n";
				ofile <<"player 2 Final skor : "<<finalP2<<"\n"; // dikurangi sisa tile
				ofile.close();
			}
		}
		winnerLabel->setVisible(true);
		
	}
	
}

void GameAiScene::pause(CCObject *pSender)
{


}

void GameAiScene::setLetterBag()
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
}

void GameAiScene::updateLetterBag(int index)
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
BasicTiles GameAiScene::getBasicTiles()
{
	return basicTiles;
}
vector<vector<int>> GameAiScene::getLetterBag()
{
	return letterBag;
}