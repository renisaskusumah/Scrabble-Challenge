#include "Gaddag.h"
#include "Heuristic.cpp"

using namespace std;
class Gaddag
{
private:

	graf G;
	Papan papan; // kelas papan
	Heuristic heuristic;
	ofstream fileGraf;
	ifstream grafFile;
	int count;
	possible finalPossible;
	vector<possible> evalPossible;
	vector<string> result; //hasil generate movement(getWord)
	vector<string> evalResult; // hasil generate movement(generatemovement)
	vector<char> activeRack;//set rack yg dipakai dalam generate movement
	string maxWord; //kata yang terpilih untuk diambil jadi langkah
	int maxScore; //score dari kata yg terpilih
	string hook; //kata yang menempel pada tiles yang akan di taruh
	int startRow,startCol; // menyimpan startRow dan col dari kata yg terpilih
	int direction; //1 ke samping , 2 menurun
	CCLayer* gameScene; //scene
	string logFileName;
	string newTiles;
	string tempNewTiles;
	string column_string;
	bool useBlank; //apakah kata yg terpilih mengandung blank tile atau tidak (keperluan menandakan di file txt)
	bool useHeuristic;
	float maxHeuristic;
	vector<int> left_tiles;
	vector<possible> allPossible;
	vector<float> vector_heuristic;
	string directWord;


public:
	Gaddag()
	{
		count = 1;
		createEmpty(&G);
		papan.initPapan();
		column_string = "ABCDEFGHIJKLMNO";
		useHeuristic = false;
	}

	void setHeuristic(bool set)
	{
		useHeuristic = set;
	}

	void restartGaddag()
	{
		papan.initPapan();
		evalPossible.clear();
		result.clear(); //hasil generate movement(getWord)
		evalResult.clear(); // hasil generate movement(generatemovement)
		allPossible.clear();
		activeRack.clear();//set rack yg dipakai dalam generate movement
		maxWord = "-"; //kata yang terpilih untuk diambil jadi langkah
		maxScore = -999; //score dari kata yg terpilih
		hook = ""; //kata yang menempel pada tiles yang akan di taruh
		direction = 0; //1 ke samping , 2 menurun
		useBlank = false;
		useHeuristic = false;

	}

	void setLogFile(string logName)
	{
		logFileName = logName;
	}

	void setBoard(Papan p)
	{
		papan = p;
	}

	Papan& getBoard()
	{
		return papan;
	}

	//membuat graf yang baru
	void createEmpty(graf *G)
	{
		(*G).start = NULL;
	}

	//membuat node yang baru, node sebelum adalah node sebelumnya dari sumber yang sama
	void addSimpul(char huruf)
	{
		//huruf = toupper(huruf);
		node *baru = (node *) malloc(sizeof(node));
		baru->huruf = huruf;
		baru->next = NULL;
		baru->arc = NULL;
		baru->jmlAnak = 0;
		if((G).start == NULL)
		{
			baru->prev = NULL;
			(G).start = baru;
		}
		else
		{
			node *now = (G).start;

			while(now -> next != NULL){
				now = now -> next;
			}
			baru->prev = now;
			now->next = baru;
		}

	}

	//menghubungkan node dengan membuat jalur baru, ini digunakan jika ternyata node yang dimaksud sudah ada
	void addJalur(node *awal, node *akhir)
	{
		if(awal != NULL && akhir != NULL)
		{
			jalur *baru = (jalur *)malloc(sizeof(jalur));
			baru->nextjalur = NULL;
			baru->tujuan = akhir;
			if(awal->arc == NULL)
			{
				awal->arc = baru;
				awal->jmlAnak++;
			}
			else
			{
				//jika sudah ada jalur sebelumnya di node awal
				int status = 0; // 
				jalur *now = awal->arc;
				//cek apakah sudah ada jalur yang sama atau belum
				if(now->tujuan->huruf == akhir->huruf)
				{
					status = 1;
				}
				while(now->nextjalur != NULL && status == 0)
				{
					//cek apakah sudah ada jalur yang sama atau belum
					if(now->nextjalur->tujuan->huruf == akhir->huruf)
					{
						status = 1;
					}
					else
					{
						now = now->nextjalur;
					}
				}
				if(status == 0)
				{
					now->nextjalur = baru;
					awal->jmlAnak++;
				}
			}

		}
	}


	/*---------------GADDAG CONSTRUCTION ALGORITHM----------------*/

	node * addArc(node *sebelum, char huruf)
	{			

		//huruf = toupper(huruf);
		//cek apakah node sebelum sudah pernah memiliki jalur ke huruf tersebut 
		int status = 0;
		jalur *now = sebelum->arc;
		node *state;
		if(now!=NULL)
		{
			while(now != NULL && status == 0)
			{
				if(now->tujuan->huruf == huruf)
				{
					//std::cout<<"sudah ada : "<<huruf<<"\n";
					state = now->tujuan;
					status = 1;
				}
				else
				{
					now = now->nextjalur;
				}
			}
		}

		if(status == 0)
		{
			//std::cout<<count<<" belum ada\n";
			node *baru = (node *) malloc(sizeof(node));
			baru->huruf = huruf;
			baru->eow = 0;
			baru->next = NULL;
			baru->arc = NULL;
			baru->jmlAnak = 0;
			//baru->toFile = 0;
			sebelum->next = baru;
			addJalur(sebelum,baru);
			state = baru;
			count++;
		}

		return state;
	}

	node * addFinalArc(node *sebelum, char pertama, char kedua)
	{

		node *state;
		state = addArc(sebelum,pertama);
		state = addArc(state,kedua);
		state->eow = 1;
		return state;
	}

	node * forceArc(node *sebelum, char huruf , node *forceState)
	{
		node *state;
		state = addArc(sebelum,huruf);
		state->next = forceState;
		addJalur(state,forceState);
		return state;
	}

	/*set awal gaddag
	mengambil scene yg aktif
	menyiapkan simpul awal (G.start ->'+')
	memasukan semua kata di file ke dalam graf ->ospd.txt
	set kelas papan menginisialisasi papan jadi kosong
	*/
	void initGraph()
	{
		gameScene = (CCLayer* )(CCDirector::sharedDirector()->getRunningScene()->getChildByTag(999));
		ifstream file;
		file.open("CSW12.txt");
		string str; 
		addSimpul('+');
		while (std::getline(file, str))
		{
			//std::cout<<str<<"\n";
			addWord(str);
		}
		file.close();
		CCLOG("total node : %d",count);
	}

	//menambahkan kata ke graf 
	void addWord(string kata)
	{

		/*
		*pembuatan graf (tambah kata) berdasarkan algoritma GADDAG
		*delimiter menggunakan karakter "-"
		*/
		node *state = G.start;
		node *forceState;
		int i = 0;
		//int n = strlen(kata);
		int n = kata.length();
		int m;


		//untuk menyusun kata secara terbalik dalam graph sehingga di simpul kata[0] status eow = 1
		for(i = n-1 ; i >=2 ; i--)
		{
			state = addArc(state,kata[i]);
		}

		state = addFinalArc(state,kata[1],kata[0]);
		//memasukan kata dalam beberapa formasi termasuk delimiter
		state = G.start;
		for(i = n-2 ; i >= 0 ; i--)
		{
			state = addArc(state,kata[i]);
		}
		state = addFinalArc(state,'-',kata[n-1]);
		for(m = n-3 ; m >= 0 ; m--)
		{
			forceState = state;
			state = G.start;
			for(i = m ; i >= 0 ; i--)
			{
				state = addArc(state,kata[i]);
			}
			state = addArc(state,'-');
			state = forceArc(state,kata[m+1],forceState);
		}

	}


	/*----ALUR PERMAINAN-----*/
	//awal permainan set kotak di board yg jadi anchor square
	void startPlaying(int initRow, int initCol)
	{
		papan.setRowPlayed(initRow);
		papan.setColPlayed(initCol);
	}


	//setRack yang dipakai dalam gaddag
	void setActiveRack(vector<char> rack)
	{
		activeRack.clear();
		activeRack = rack;
	}


	//mengambil skor dari kata yang terpilih
	int getScore()
	{
		return maxScore;
	}
	//apakah menggunakan blank tile atau tidak
	string getUseBlank()
	{
		string use = "";
		if(useBlank == true) use = "blank tiles";
		return use;
	}
	//mengambil semua kata hasil generate movement
	string getAllPossibleWord()
	{
		string allWord;
		string word;
		int i,j;
		j = 0;
		if(evalResult.size() == 0)
		{
			//hasil dari void getWord
			for(i = 0 ; i < result.size() ; i++)
			{
				if(i < 20)
				{
					allWord += result.at(i) + " " + to_string(getScoreByWord(result.at(i)) * 2) + "\n";
					j++;
				}
			}
			allWord += "( "+to_string(j)+" of "+to_string(result.size())+")";	
		}
		else
		{
			//hasil dari void generation movement
			for(i = 0 ; i < evalResult.size() ; i++)
			{
				word = evalResult.at(i);
				replace(word.begin(), word.end(), '_', ' ');
				if(i < 20) 
				{
					allWord += word+ " \n";
					j++;
				}
			}
			allWord += "(" +to_string(j)+ " of "+to_string(evalResult.size())+")";	
		}


		return allWord;
	}
	//mengambil kata yg terpilih dari generate movement
	string getEvalWord()
	{
		return maxWord;
	}

	/*
	Playgame merupakan fungsi yang dipanggil setiap Ai player mendapat giliran
	jika petak tengah masih kosong maka yang dipanggil adalah void getword
	sedangkan jika sudah terisi maka dipanggil generate movement karena harus dicari mana saja anchor square
	fungsi mengembalikan rack yang baru dimana sudah diambil beberapa tile untuk ditaruh di board
	*/
	vector<char> playGame(vector<char> rack)
	{
		maxHeuristic = -999;
		maxScore = 0;
		useBlank = false;
		activeRack.clear();
		activeRack = rack;
		evalResult.clear();
		if(papan.getLetterOnPosition(7,7) == '-')
		{
			papan.setRowPlayed(7);
			papan.setColPlayed(7);
			vector<int> anchor;
			anchor.push_back((15 * 7) + 7);
			allPossible.clear();
			getPossibleMovementByAnchor(rack,anchor);
			//rack = getWord(rack);
		}
		else
		{
			vector_heuristic.clear();
			allPossible.clear();
			getAllPossible(rack);
			//rack = generateMovement(rack);
		}
		CCLOG("%d",maxScore);
		return rack;
	}

	vector<char> generateToGui(vector<char> rack)
	{
		if(papan.getLetterOnPosition(7,7) == '-')
		{
			rack = getWord(rack);
		}
		else
		{
			rack = generateMovement(rack);
		}

		return rack;
	}

	void getAllPossible(vector<char> rack)
	{
		int i;
		int j;
		vector<int> anchor;
		if(papan.getLetterOnPosition(7,7) != '-')
		{
			for(i = 0 ; i < 15 ; i++)
			{
				for(j = 0 ; j < 15 ; j++)
				{
					if(papan.getLetterOnPosition(i,j) == '-')
					{
						if(checkAnchor(i,j) == 1)
						{
							anchor.push_back((15 * i) + j);
						}
					}

				}
			}
		}
		else
		{
			anchor.push_back((15 * 7) + 7);
		}

		getPossibleMovementByAnchor(rack,anchor);
	}

	/*keperluan untuk human player, 
	generate movement dari rack pemain human lalu semuanya dibentuk dalam string
	hal ini karena hasil generate akan ditampilkan dalam bentuk label
	*/

	void getPossibleMovementByAnchor(vector<char> rack,vector<int> anchor)
	{
		allPossible.clear();
		vector_heuristic.clear();
		evalResult.clear();
		evalPossible.clear();
		possible poss;
		string word;
		//mengambil semua anchor square yg memungkinkan di papan
		int i,j,start,counter,index;
		vector<char>::iterator found;
		node *startArc;
		string dir,temp;
		int status =0 ;
		int tempTag;
		int evalDirection,evalAnchorRow,evalAnchorCol;
		string evalWord = "";
		string eval;
		evalAnchorRow = -1;
		evalAnchorCol = -1;
		int evalScore = 0;
		//direction 1 samping , 2 bawah
		/*cek mana saja yg menjadi anchor square
		setiap anchor square dilempar ke void generate movement
		*/
		for(index = 0 ; index < anchor.size() ; index++)
		{
			i = anchor.at(index) / 15;
			j = anchor.at(index) % 15;
			word = "";
			hook = "";
			counter = 1;
			status = 0;
			startArc = G.start;
			// std::cout<<"i : "<<i<<" j : "<<j<<" \n";
			if(papan.getLetterOnPosition(i,j) == '-')
			{
				if(checkAnchor(i,j) == 1)
				{
					//maka menjadi anchor square
					papan.setRowPlayed(i);
					papan.setColPlayed(j);
					maxWord = "-";
					maxScore = 0;
					maxHeuristic = -999;
					//coba direction samping dan bawah
					for(direction = 1 ; direction <=2 ; direction++)
					{
						string word = "";
						hook = "";
						counter = 1;
						status = 0;
						if(direction == 2)
						{

							if(papan.getLetterOnPosition(papan.getRowPLayed() + 1,papan.getColPlayed()) != '-' && papan.getRowPLayed() < 14)
							{
								while(status == 0)
								{
									if( papan.getLetterOnPosition(papan.getRowPLayed()+ counter,papan.getColPlayed())!= '-' && papan.getRowPLayed() + counter < 15)
									{
										hook += papan.getLetterOnPosition(papan.getRowPLayed()+ counter,papan.getColPlayed());
										counter ++;
									}
									else
									{
										status = 1;
									}
								}
								//startArc = nextArc(G.start,hook.at(0));
							}

						}
						else
						{

							if(papan.getLetterOnPosition(papan.getRowPLayed(),papan.getColPlayed() + 1) != '-' && papan.getColPlayed() < 14 )
							{
								while(status == 0)
								{
									if(papan.getLetterOnPosition(papan.getRowPLayed(),papan.getColPlayed() + counter) != '-' && papan.getColPlayed() + counter < 15)
									{
										hook += papan.getLetterOnPosition(papan.getRowPLayed(),papan.getColPlayed() + counter);
										counter ++;
									}
									else
									{
										status = 1;
									}
								}
								//startArc = nextArc(G.start,hook.at(0));
							}

						}
						result.clear();
						activeRack.clear();
						activeRack = rack;
						/*ofstream ofile(logFileName.c_str(), ios::app);
						if ( ofile)
						{
						ofile<<"\nAnchor (i :  "<<i<<" j :  "<<j<<" ) direction : "<<direction<<"\n";
						ofile.close();
						}*/
						gen(0,word,rack,startArc);
					}
				}
			}

		}

	}

	vector<float> getVectorHeuristic()
	{
		return vector_heuristic;
	}

	vector<possible> evaluateAllPossible(int limit)
	{
		vector<possible> evaluate;
		//25 nilai heuristic terbaik
		if(vector_heuristic.size() > 25)
		{
			std::sort(vector_heuristic.begin(), vector_heuristic.end());
			vector_heuristic.erase (vector_heuristic.begin(),vector_heuristic.begin()+(vector_heuristic.size() - limit));
			int i;
			for(i = 0; i < allPossible.size();i++)
			{
				if (std::find(vector_heuristic.begin(), vector_heuristic.end(), allPossible.at(i).heuristic) != vector_heuristic.end())
				{
					evaluate.push_back(allPossible.at(i));
				}
			}
		}
		else
		{
			evaluate = allPossible;
		}
		return evaluate;
	}

	vector<possible> getAllPossibleVector()
	{
		//CCLOG("all possible : %d",allPossible.size());
		return allPossible;
	}

	vector<possible> possibleMovementInVectorByAnchor(vector<char> rack,vector<int> anchor)
	{
		evalPossible.clear();
		getPossibleMovementByAnchor(rack,anchor);
		std::cout<<"possible : "<<allPossible.size()<<"\n";
		return evalPossible;
	}

	vector<possible> possibleMovementInVector(vector<char> rack)
	{
		evalPossible.clear();
		getAllPossible(rack);
		std::cout<<"possible : "<<allPossible.size()<<"\n";
		return evalPossible;
	}

	string getHint(vector<char> rack)
	{
		getAllPossible(rack);
		string allWord,word;
		string col;
		string direct;
		allWord = "";
		int i;
		vector<possible> evaluate = evaluateAllPossible(25);
		for(i = 0 ; i < evaluate.size() ; i++)
		{
			if(evaluate.at(i).direction == 1)
			{
				direct = "Right";
			}
			else
			{
				direct = "Down";
			}
			col = column_string.substr(evaluate.at(i).startCol,1);

			if(i < 25) allWord += evaluate.at(i).word+ " "  +to_string(evaluate.at(i).score) + " (" +to_string(evaluate.at(i).startRow+1)+ ","+col+" ) "+direct+" \n";


		}
		if(evaluate.size() == 0) allWord = "No hint available\n";
		std::cout<<evaluate.size()<<"\n";
		return allWord;
	}



	vector<possible> getPossibleMovement()
	{
		return evalPossible;
	}

	/**merupakan fungsi untuk generate movement jika petak tengah pada papan masih kosong
	sehingga anchor square di set berada di petak tengah
	melempar sisa tiles dalam rack
	*/
	vector<char> getWord(vector<char> rack)
	{

		int i,tempRow,tempCol,tempTag;


		vector<char> ::iterator found;
		/*vector<int> anchor;
		anchor.push_back((15 * 7) + 7);
		allPossible.clear();
		getPossibleMovementByAnchor(rack,anchor);*/
		if(allPossible.size() > 0)
		{
			maxWord = finalPossible.word;
			maxScore = finalPossible.score;
			tempRow = finalPossible.startRow;
			tempCol = finalPossible.startCol;
			direction = finalPossible.direction;
		}
		else
		{
			maxWord = "";
			maxScore = 0;
		}
		/*----EVALUASI----*/

		for(i = 0 ; i < maxWord.length() ; i++)
		{
			tempTag = (15 * tempRow) + tempCol;
			((TileOnBoard *)gameScene->getChildByTag(tempTag))->changeTileImage(getIndexByLetter(maxWord.at(i)));
			((TileOnBoard *)gameScene->getChildByTag(tempTag))->setLetter(maxWord.at(i));
			found = std::find(rack.begin(), rack.end(),maxWord.at(i)) ;
			if (found!=rack.end())
			{
				rack.erase(found);
			}
			else
			{
				found = std::find(rack.begin(), rack.end(),'*') ;
				if(found!=rack.end())
				{
					useBlank = true;
					((TileOnBoard *)gameScene->getChildByTag(tempTag))->changeBlankTileImage(getIndexByLetter(maxWord.at(i)));
					((TileOnBoard *)gameScene->getChildByTag(tempTag))->setIsBlankTile(true);
					papan.setIsBlankTile(true,tempRow,tempCol);
					rack.erase(found);
				}
			}

			papan.setCharOnPapan(tempRow,tempCol, maxWord.at(i));
			((TileOnBoard *)gameScene->getChildByTag(tempTag))->setIsSetTile(true);
			papan.setIsSetTile(true,tempRow,tempCol);
			if(direction == 1)
			{
				tempCol++;
			}
			else
			{
				tempRow++;
			}
		}
		//papan.showPapan();

		//CCLOG("%d",rack.size());

		return rack;
	}

	/*
	merupakan fungsi untuk mencari langkah langkah memungkinkan
	setiap petak dalam board di cek apakah petak tersebut anchor square atau bukan
	jika memang anchor square maka masuk ke algoritma generate movement GADDAG
	setiap kata yang memungkinkan dimasukan ke dalam vector 
	setiap memasukan diperiksa apakah kata itu memiliki skor lebih besar dari kata sebelumnya atau tidak
	*/
	vector<char> generateMovement(vector<char> rack)
	{
		int evalAnchorRow,evalAnchorCol,evalScore,evalDirection,tempTag;
		evalAnchorRow = -1;
		evalAnchorCol = -1;
		string evalWord;
		float evalHeuristic;
		int i;
		vector<char> ::iterator found;
		/*vector_heuristic.clear();
		allPossible.clear();
		getAllPossible(rack);*/
		if(allPossible.size() > 0)
		{
			evalAnchorRow = finalPossible.startRow;
			evalAnchorCol = finalPossible.startCol;
			evalScore = finalPossible.score;
			evalDirection = finalPossible.direction;
			evalWord = finalPossible.word;
			evalHeuristic = finalPossible.heuristic;
			maxWord = evalWord;
			maxScore = evalScore;
		}
		/*----EVALUASI----*/
		//jika ada langkah yang memungkinkan maka kata terpilih ditaruh ke atas papan
		if(evalAnchorRow != -1 || evalAnchorCol != -1)
		{
			std::cout<<"Hasil evaluasi : "<<evalWord<<" , score : "<<evalScore<<" , direction :"<<evalDirection<<" ,i: "<<evalAnchorRow<<" j: "<<evalAnchorCol<<"\n";
			ofstream ofile(logFileName.c_str(), ios::app);
			if ( ofile)
			{
				if(useHeuristic == false)
				{
					ofile<<"Hasil evaluasi : "<<evalWord<<" , score : "<<evalScore<<" , direction :"<<evalDirection<<" ,i: "<<evalAnchorRow<<" j: "<<evalAnchorCol<<"\n\n\n\n";
				}
				else
				{
					ofile<<"Hasil evaluasi : "<<evalWord<<" , score : "<<evalScore<<" , direction :"<<evalDirection<<" ,i: "<<evalAnchorRow<<" j: "<<evalAnchorCol<<" Heuristic: "<<evalHeuristic<<"\n\n\n\n";
				}
				ofile.close();
			}
			//CCLOG("anchor %d %d",evalAnchorRow,evalAnchorCol);
			//put tile on board
			for(i = 0 ; i < evalWord.length() ; i++)
			{
				tempTag = (15 * evalAnchorRow) + evalAnchorCol;
				CCLOG("temp tag : %d",tempTag);
				if(papan.getSetStatus(evalAnchorRow,evalAnchorCol) == false)
				{
					((TileOnBoard *)gameScene->getChildByTag(tempTag))->setLetter(evalWord.at(i));
					((TileOnBoard *)gameScene->getChildByTag(tempTag))->changeTileImage(getIndexByLetter(evalWord.at(i)));
					found = std::find(rack.begin(), rack.end(),evalWord.at(i)) ;
					if (found!=rack.end())
					{
						((TileOnBoard *)gameScene->getChildByTag(tempTag))->setIsSetTile(true);
						papan.setCharOnPapan(evalAnchorRow,evalAnchorCol,evalWord.at(i));
						papan.setIsSetTile(true,evalAnchorRow,evalAnchorCol);
						rack.erase(found);
					}
					else
					{
						CCLOG("blank keambil jadi huruf %c",evalWord.at(i));
						//berasal dari blank tile
						found = std::find(rack.begin(), rack.end(),'*') ;
						if(found!=rack.end())
						{
							useBlank = true;
							rack.erase(found);
							((TileOnBoard *)gameScene->getChildByTag(tempTag))->changeBlankTileImage(getIndexByLetter(evalWord.at(i)));
							((TileOnBoard *)gameScene->getChildByTag(tempTag))->setIsBlankTile(true);
							((TileOnBoard *)gameScene->getChildByTag(tempTag))->setIsSetTile(true);

							papan.setIsBlankTile(true,evalAnchorRow,evalAnchorCol);
							papan.setCharOnPapan(evalAnchorRow,evalAnchorCol,evalWord.at(i));
							papan.setIsSetTile(true,evalAnchorRow,evalAnchorCol);
						}
					}
				}
				if(evalDirection == 1)
				{
					evalAnchorCol++;
				}
				else
				{
					evalAnchorRow++;
				}
			}
			papan.showPapan();
			maxWord = evalWord;
			maxScore = evalScore;
		}
		return rack;
	}


	/*-----------ADDITION BEFORE MOVEMENT--------------------------*/
	vector<int> getAnchorByWord(int startRow,int startCol,string word,int direction)
	{
		vector<int> anchor;
		int i,j;
		int row = startRow;
		int col = startCol;
		/*
		xxx     x      
		xabcx   xax
		xxx    xbx 
		xcx
		x
		*/
		if(direction == 1)
		{
			if(papan.getLetterOnPosition(row,col - 1) == '-')
			{
				if(checkAnchor(row, col - 1) == 1) anchor.push_back((row * 15) + col - 1);
			}
			if(papan.getLetterOnPosition(row,col + 1) == '-')
			{
				if(checkAnchor(row, col + 1) == 1) anchor.push_back((row * 15) + col + 1);
			}
		}
		else
		{
			if(papan.getLetterOnPosition(row - 1,col) == '-')
			{
				if(checkAnchor(row - 1, col) == 1) anchor.push_back(((row - 1) * 15) + col);
			}
			if(papan.getLetterOnPosition(row + 1,col) == '-')
			{
				if(checkAnchor(row + 1,col) == 1) anchor.push_back(((row + 1) * 15) + col);
			}
		}
		for(i = 0 ; i < word.length() ; i++)
		{

			if(direction == 1)
			{
				if(papan.getLetterOnPosition(row - 1,col) == '-')
				{
					if(checkAnchor(row - 1,col) == 1) anchor.push_back(((row - 1) * 15) + col);
				}
				if(papan.getLetterOnPosition(row + 1,col) == '-')
				{
					if(checkAnchor(row + 1,col) == 1) anchor.push_back(((row + 1) * 15) + col);
				}
				col++;
			}
			else
			{
				if(papan.getLetterOnPosition(row,col - 1) == '-')
				{
					if(checkAnchor(row, col - 1) == 1) anchor.push_back((row * 15) + col - 1);
				}
				if(papan.getLetterOnPosition(row,col + 1) == '-')
				{
					if(checkAnchor(row, col + 1) == 1) anchor.push_back((row * 15) + col + 1);
				}
				row++;
			}
		}
		return anchor;
	}
	/*fungsi ini untuk mengecek apakah sebuah petak merupakan anchor square atau bukan*/
	int checkAnchor(int row, int col)
	{
		int status = 0;
		if(papan.getLetterOnPosition(7,7) == '-')
		{
			status = 1;
		}
		if(row != 0 &&  papan.getLetterOnPosition(row-1,col) != '-' && row - 1 >= 0 && status == 0)
		{
			status = 1;
		}
		if(row != 14 && papan.getLetterOnPosition(row+1,col) != '-' && row + 1 < 15 && status == 0)
		{
			status = 1;
		}
		if(col != 0 &&  papan.getLetterOnPosition(row,col - 1) != '-' && col - 1 >= 0 && status == 0)
		{
			status = 1;
		}
		if(col != 14 &&  papan.getLetterOnPosition(row,col + 1) != '-' && col + 1 < 15 && status == 0)
		{
			status = 1;
		}

		return status;
	}

	/*-----------MOVE GENERATION ALGORITHM---------------*/

	/*
	algoritma GADDAG memiliki 3 inti fungsi yaitu Gen , GoOn , dan recordPlay
	Gen untuk mengetes apakah memungkinkan untuk membentuk kata dengan mengambil satu persatu tile dari rack
	GoOn untuk mencocokan tile yg diambil dari rack untuk diposisikan
	recordplay untuk mencocokan apakah yang terbentuk dari goOn merupakan sebuah kata atau bukan lalu mengecek juga
	apakah kata yang ditaruh valid untuk dimainkan atau tidak sesuai dengan aturan permainan scrabble
	*/
	void gen(int pos, string word, vector<char> rack,node *arc)
	{	
		int row = papan.getRowPLayed();
		int col = papan.getColPlayed();

		if(direction == 1)
		{
			col = papan.getColPlayed() + pos;
		}
		else
		{
			row = papan.getRowPLayed() + pos;
		}
		if(row >= 0 && row < 15 && col >= 0 && col < 15 )
		{

			char l = papan.getLetterOnPosition(row,col);
			if(l != '-')
			{ 
				goOn(pos,l,word,rack,nextArc(arc,l),arc);
			}
			else
			{
				int i,j;
				//char temp[strlen(rack) - 1];
				vector<char> temp;
				for(i = 0 ; i < rack.size() ; i++)
				{

					temp = rack;
					temp.erase(temp.begin() + i);
					if(rack.at(i) != '*')
					{
						//jika tile bukan blank tile
						goOn(pos,rack.at(i),word,temp,nextArc(arc,rack.at(i)),arc);
					}
					else
					{
						//jika tile adalah blank tile maka dicoba setiap kemungkinan dari a - z
						for(j = 0 ; j < 26 ; j++)
						{
							goOn(pos,getLetterByIndex(j),word,temp,nextArc(arc,getLetterByIndex(j)),arc);
						}
					}
				}
			}
		}
	}

	void goOn(int pos, char l , string word , vector<char> rack, node *newArc, node *oldArc)
	{

		int row,col,tempRow,tempCol,i;
		row = papan.getRowPLayed();
		col = papan.getColPlayed();
		int startRow,startCol;
		int tipe; //atas, bawah,kiri , kanan
		startRow = row;
		startCol = col;
		tempCol = col;
		tempRow = row;
		left_tiles.clear();
		//left tiles to index
		for(i = 0 ; i < rack.size() ; i++)
		{
			left_tiles.push_back(getIndexByLetter(rack.at(i)));
		}
		if(pos <= 0)
		{
			word = l + word;
			if(direction == 1)
			{
				col = col + pos - 1;
				tipe  = 1;
				startCol = startCol + pos;
			}
			else
			{
				row = row +  pos - 1;
				tipe = 3;
				startRow = startRow + pos;
			}

			if(row >= 0 && row < 15 && col >= 0 && col < 15 )
			{

				if(newArc != NULL && papan.getLetterOnPosition(row,col) == '-')
				{
					recordplay(word,rack.size(), startRow,startCol);
				}

				if(newArc != NULL)
				{
					//apakah ada ruang kosong di kiri
					if(checkRoom(tipe,row,col) == 1)
					{

						gen(pos - 1 , word,rack,newArc);
					}
					//ganti dari kiri ke kanan
					newArc = nextArc(newArc,'-'); 
					if(direction == 1)
					{
						tempCol = papan.getColPlayed() + 1;
						tipe = 2;
					}
					else
					{
						tempRow = papan.getRowPLayed() + 1;
						tipe = 4;
					}

					if(newArc != NULL  && papan.getLetterOnPosition(row,col) == '-' && checkRoom(tipe,tempRow,tempCol) == 1)//&room to the right)
					{
						gen(1,word,rack,newArc);
					}

				}
			}
		}
		else
		{

			//pindah ke kanan
			word+=l;
			if(direction == 1)
			{
				col = col + pos + 1;
				tipe = 2;
				startCol = col  - word.length();
			}
			else
			{
				row = row +  pos + 1;
				tipe = 4;
				startRow = row - word.length();
			}
			if(row >= 0 && row < 15 && col >= 0 && col < 15 )
			{

				// jika letter pada arc lama dan tidak ada huruf tepat di sebelahnya maka recordplay
				if(newArc != NULL  && papan.getLetterOnPosition(row,col) == '-')
				{
					recordplay(word,rack.size(),  startRow,startCol);
				}
				if(direction == 1)
				{
					col = papan.getColPlayed() + pos + 1;
				}
				else
				{
					row = papan.getRowPLayed() + pos + 1;
				}
				if(newArc != NULL && checkRoom(tipe,row,col) == 1)//&room to the right?)
				{

					gen(pos + 1,word,rack,newArc);
				}
			}

		}

	}
	/*VALIDASI LANGKAH
	tahapan dari record play adalah mengecek dulu apakah kata yang dirangkai memang sudah eow atau belum
	lalu jika pada posisi tersebut menempel pada kata sebelumnya yang sudah ada (hook) 
	maka disatukan lalu dicek apakah eow dari sebuah kata valid atau bukan

	Jika kata yang dirangkai memang terdapat dalam kamus selanjutnya menghitung skor dengan memanggil wordscore
	dalam wordScore dilakukan pengecekan juga apakah ada huruf - huruf (kata) di sebelah kanan atau bawah yang berdempet
	jika ya maka di cek jika disatukan apakah kata yang valid atau tidak
	ini dikarenakan jika ditambahkan huruf paling belakang pada kata yang tertempel ada kemungkinan tidak akan jadi sebuah kata yg valid sehingga tidak terdeteksi untuk hal diatas
	misal : aba(cus) ->jadi valid
	aba(cus)z->tidak valid, karena tidak valid maka kemungkinan tidak tercek "abacus" sehingga perlu di handle oleh wordScore();

	Jika hasil dari wordScore valid maka dilanjutkan dengan getScoreByAnchor ini dilakukan untuk mengecek
	jika kata yang ditaruh apakah berdempet (dengan arah berlawanan) dengan huruf lain atau tidak sesuai dengan aturan scrabble
	jika hasil dari getScoreByAnchor adalah kata tersebut tidak valid untuk ditaruh , 
	maka kata tidak termasuk ke dalam solusi  dan begitu pula sebaliknya

	*/
	void recordplay(string word,int sisa,int row,int col)
	{
		float tempHeuristic; 
		if(hook!= "")
		{
			//cek apakah "hook" dilalui oleh gen dan goon atau tidak. Jika tidak, maka tempelkan hook ke kata yang akan di cek
			int isHooked = 0;
			if(direction == 1)
			{
				if(papan.getLetterOnPosition(startRow,startCol + word.length()) != '-')
				{
					isHooked = 1;
					//word += hook;
				}
			}
			else
			{
				if(papan.getLetterOnPosition(startRow + word.length(),startCol) != '-')
				{
					isHooked = 1;
				}
				//word += hook;
			}
			if(isHooked == 1) word += hook;
		}
		//CCLOG("%s",word);
		//if(isWord(word) == 1)
		//{
		int tempScore = wordScore(word, row, col, direction);
		if(directWord != "")
		{
			word = directWord;
		}
		/*	CCLOG("tempscore");
		CCLOG("%d",tempScore);*/
		if(tempScore != -1)
		{
			int checkWordScore;

			if(sisa == 0 && activeRack.size() == 7) 
			{
				tempScore += 50;
			}
			checkWordScore = getScoreByAnchor(word, row, col, direction);
			if(checkWordScore != -1)
			{
				// Element in vector.
				if (std::find(result.begin(), result.end(), word) == result.end())
				{
					result.push_back(word);
				}
				tempScore += checkWordScore;
				tempHeuristic = heuristic.evaluate(tempScore , left_tiles);
			}
			else
			{
				tempScore = 0;
			}
			/*Put all to allpossible vector*/
			//periksa dulu sudah ada atau belum
			if(tempScore > 0)
			{
				possible poss;
				poss.word = word;
				poss.startRow = row;
				poss.startCol = col;
				poss.direction = direction;
				poss.score = tempScore;
				poss.rowAnchor = papan.getRowPLayed();
				poss.colAnchor = papan.getColPlayed();
				poss.usedTiles = papan.getNewTilesByWord(row,col,word,direction);
				poss.heuristic = tempHeuristic;
				int j,status;
				status = 0;j = 0;
				while(j < allPossible.size() && status == 0)
				{
					if(allPossible.at(j).word == word && allPossible.at(j).startRow == row && allPossible.at(j).startCol == col && allPossible.at(j).score == tempScore && allPossible.at(j).usedTiles == poss.usedTiles && allPossible.at(j).heuristic == poss.heuristic )
					{
						status = 1;
					}
					else

					{
						j++;
					}
				}
				if(status == 0)
				{
					allPossible.push_back(poss);
					vector_heuristic.push_back(tempHeuristic);
					ofstream ofile(logFileName.c_str(), ios::app);
					if ( ofile  && tempScore > 0)
					{
						ofile<<"word : "<<word<<" anchor( col :  "<<papan.getColPlayed()<<" row : "<<papan.getRowPLayed()<<" ) score : "<<tempScore<<" new tiles : "<<poss.usedTiles<<"    Heuristic : "<<tempHeuristic<<"\n";
						ofile.close();
					}
					/*---EVALUATE untuk final possible*/
					if(allPossible.size() == 1)
					{
						finalPossible = poss;
					}
					else
					{
						//perlu ditambahkan, jika mengandung blank lebih baik ambil maksimal saja dibanding heuristic agar pada proses berikutnya tidak lama
						if(useHeuristic == false)
						{
							//greedy
							if(finalPossible.score < tempScore || (finalPossible.score == tempScore && finalPossible.usedTiles.length() > poss.usedTiles.length()))
							{
								finalPossible = poss;
							}
						}
						else
						{
							//heuristic
							if(finalPossible.heuristic < tempHeuristic || (finalPossible.heuristic == tempHeuristic && finalPossible.score < tempScore) || (finalPossible.heuristic == tempHeuristic && finalPossible.score == tempScore && finalPossible.usedTiles.length() > poss.usedTiles.length()))
							{
								finalPossible = poss;
							}
						}

					}
				}
			}
		}

		//}
	}


	//1 left, 2 right , 3 up , 4 down
	/*check room adalah fungsi untuk mengecek 
	apakah pada jalur yang dimainkan masih ada petak yang kosong atau tidak*/
	int checkRoom(int direction, int row, int col)
	{
		int status = 0;
		while(status == 0 && row < 15 && row > 0 && col > 0 && col < 15)
		{
			if(direction == 1)
			{
				col--;
			}
			else if(direction == 2)
			{
				col++;
			}
			else if(direction == 3)
			{
				row--;
			}
			else if(direction == 4)
			{
				row++;
			}
			if(row < 15 && row >= 0 && col >= 0 && col < 15)
			{
				if(papan.getLetterOnPosition(row,col) == '-' && status == 0)
				{
					status = 1;
				}
			}

		}
		return status;
	}

	/*menghitung score dari kata dan mengecek 
	jika menempel dengan kata lain yang searah apakah valid atau tidak
	Jika tidak valid maka score tidak didapat
	*/
	int wordScore(string word, int row, int col, int direction)
	{
		directWord = "";
		vector<char> tempRack = activeRack;
		vector<char>::iterator found;
		int length = word.length();
		int score,tempRow,tempCol,wordStatus;
		int wordCheck ;
		bool isBlank;
		string tempWord;
		int i,tileType,status;
		wordStatus = 0;
		score = 0;
		//score = getScoreByWord(word);
		tempRow = row;
		tempCol = col;
		//getScoreByWord
		for(i = 0 ; i < length ; i++)
		{
			isBlank = false;
			tileType = papan.getTileType(tempRow,tempCol); 
			//CCLOG("di wordscore 1");
			//CCLOG("di word score : %d",(15 * tempRow) + tempCol);
			if( papan.getBlankTileStatus(tempRow,tempCol) == false)
			{
				if(papan.getSetStatus(tempRow,tempCol) == false)
				{
					found = std::find(tempRack.begin(), tempRack.end(),word.at(i)) ;
					if(found!=tempRack.end())
					{
						isBlank = false;
						tempRack.erase(found);
					}
					else
					{
						isBlank = true;
						found = std::find(tempRack.begin(), tempRack.end(),'*') ;
						if(found!=tempRack.end())tempRack.erase(found);
					}
				}
				if(isBlank == false)
				{
					if(tileType == 2 || tileType == 3)
					{	
						score += (getTilesScoreByIndex(getIndexByLetter(word.at(i))) * tileType);

					}
					else
					{
						score += getTilesScoreByIndex(getIndexByLetter(word.at(i)));
					}
				}
			}

			if(direction == 1)
			{
				tempCol++;
			}
			else
			{
				tempRow++;
			}
		}

		if(direction == 1)
		{
			tempCol = col + word.length();
		}
		else
		{
			tempRow = row + word.length();
		}
		tempWord = word;
		//mengecek kata yang menempel searah apakah valid atau tidak
		if(tempCol >= 0 && tempCol < 15 && tempRow >= 0 && tempRow < 15)
		{
			status = 0;
			while(status == 0)
			{
				if(tempCol >= 0 && tempCol < 15 && tempRow >= 0 && tempRow < 15)
				{
					if(papan.getLetterOnPosition(tempRow,tempCol) != '-')
					{
						tempWord += papan.getLetterOnPosition(tempRow,tempCol);
						if( papan.getBlankTileStatus(tempRow,tempCol) == false)
						{
							score += getTilesScoreByIndex(getIndexByLetter(papan.getLetterOnPosition(tempRow,tempCol)));
						}
						if(direction == 1)
						{
							tempCol++;
						}
						else
						{
							tempRow++;
						}
					}
					else
					{
						status = 1;
					}
				}
				else
				{
					status = 1;
				}
			}
		}
		//CCLOG("skor disini %d",score);
		//jika hasil word nya tidak ada di dalam kamus maka jadi di set -1
		if(isWord(tempWord) != 1)
		{
			score = -1;
		}
		if(score != -1)
		{
			tileType = getScoreTypeByPosition(length,row,col,direction);
			//CCLOG("tiletype word %d position row %d col %d length %d direction %d",tileType,row,col,length,direction );
			if(tileType > 1)
			{
				score = score * tileType;
			}
			directWord = tempWord;
		}
		return score;
	}

	/*
	mengecek jika kata yang ditaruh setiap tilesnya 
	apakah menempel dengan kata lain dengan arah yang berlawanan 
	jika menempel maka dicek apakah membentuk sebuah kata yang ada di kamus atau tidak
	jika kata yang terbentuk valid maka score ditambah, jika tidak maka proses langsung diputus
	dan kata tersebut dianggap tidak valid untuk dimainkan
	*/
	int getScoreByAnchor(string word, int row, int col, int direction)
	{
		vector<char> tempRack = activeRack;
		vector<char>::iterator found;
		bool isBlank = false;
		int length = word.length();
		int i,score,start,status,tempRow,tempCol,wordStatus,tempScore;
		int wordCheck ;
		int tileType,tempType;
		tileType = getScoreTypeByPosition(length,row,col,direction);
		string tempWord;
		score = 0;
		wordStatus = 0;
		tempRow = row;
		tempCol = col;
		i = 0;
		while(wordStatus == 0 && i < length)
		{
			isBlank = false;
			tempWord = "";
			tempScore = 0;
			//jika polanya kesamping
			if(direction == 1)
			{
				//cek jika tile tersebut adalah dari rack atau sudah ada di board
				if(papan.getLetterOnPosition(row,tempCol) == '-' && tempCol < 15)
				{
					//maka cek atas dan bawahnya apakah membentuk kata baru atau tidak
					//cek bagian atas
					if(papan.getLetterOnPosition(row-1, tempCol) != '-' && row > 0)
					{
						status = 0;
						tempRow = row - 1;
						//mencari ujungnya dan membentuk kata
						while(status == 0)
						{
							if(tempRow >= 0)
							{
								tempWord = papan.getLetterOnPosition(tempRow,tempCol) + tempWord;
								if( papan.getBlankTileStatus(tempRow,tempCol) == false)
								{
									tempScore += getTilesScoreByIndex(getIndexByLetter(papan.getLetterOnPosition(tempRow,tempCol)));
								}
								tempRow --;
								if(papan.getLetterOnPosition(tempRow, tempCol) == '-' || tempRow < 0)
								{
									status = 1;
								}
							}
							else
							{
								status = 1;
							}
						}
					}

					tempWord += word.at(i);
					tempType = papan.getTileType(row,tempCol);
					if( papan.getSetStatus(row,tempCol) == false)
					{
						found = std::find(tempRack.begin(), tempRack.end(),word.at(i)) ;
						if(found!=tempRack.end())
						{
							isBlank = false;
							tempRack.erase(found);
						}
						else
						{
							isBlank = true;
							found = std::find(tempRack.begin(), tempRack.end(),'*') ;
							if(found!=tempRack.end())tempRack.erase(found);
						}
					}
					//CCLOG("di anchor direction 1");
					if(isBlank == false)
					{
						if(tempType == 2 || tempType == 3)
						{
							tempScore += (getTilesScoreByIndex(getIndexByLetter(word.at(i))) * tempType);
						}
						else
						{
							tempScore += getTilesScoreByIndex(getIndexByLetter(word.at(i)));
						}
					}
					//cek bagian bawah
					if(papan.getLetterOnPosition(row+1, tempCol) != '-' && (row + 1) < 15 && tempCol < 15)
					{
						status = 0;
						tempRow = row + 1;
						//mencari ujungnya dan membentuk kata
						while(status == 0)
						{
							if(tempRow <= 14)
							{
								tempWord = tempWord + papan.getLetterOnPosition(tempRow,tempCol);
								if( papan.getBlankTileStatus(tempRow,tempCol) == false)
								{
									tempScore += getTilesScoreByIndex(getIndexByLetter(papan.getLetterOnPosition(tempRow,tempCol)));
								}
								tempRow ++;
								if(papan.getLetterOnPosition(tempRow, tempCol) == '-' || tempRow > 14)
								{
									status = 1;
								}
							}
							else
							{
								status = 1;
							}
						}
					}
					if(tempWord.length() > 1)
					{
						wordCheck = isWord(tempWord);
						if(wordCheck == 1)
						{
							if(tempType > 3 && tempType / 2 == tileType)
							{
								tempScore = tempScore * tileType;
							}
							score += tempScore;
						}
						else
						{
							//CCLOG("gagal word %s length %d direction %d position row %d col %d",tempWord.c_str(),tempWord.length(),direction,row,col);
							score = -1;
							wordStatus = 1;
						}
					}

				}

				tempCol++;
			}
			//jika polanya kebawah
			else
			{
				//cek jika tile tersebut adalah dari rack atau sudah ada di board
				if(papan.getLetterOnPosition(tempRow,col) == '-' && tempRow < 15)
				{
					//cek bagian kiri
					if(papan.getLetterOnPosition(tempRow, col-1) != '-' && col-1 >= 0)
					{
						status = 0;
						tempCol = col - 1;
						//mencari ujungnya dan membentuk kata
						while(status == 0)
						{
							if(tempCol >= 0)
							{
								tempWord = papan.getLetterOnPosition(tempRow,tempCol) + tempWord;
								if( papan.getBlankTileStatus(tempRow,tempCol) == false)
								{
									tempScore += getTilesScoreByIndex(getIndexByLetter(papan.getLetterOnPosition(tempRow,tempCol)));
								}
								tempCol --;
								if(papan.getLetterOnPosition(tempRow, tempCol) == '-' || tempCol < 0)
								{
									status = 1;
								}
							}
							else
							{
								status = 1;
							}
						}
					}

					tempWord += word.at(i);
					tempType = papan.getTileType(tempRow,col);
					if( papan.getSetStatus(tempRow,col) == false)
					{
						found = std::find(tempRack.begin(), tempRack.end(),word.at(i)) ;
						if(found!=tempRack.end())
						{
							isBlank = false;
							tempRack.erase(found);
						}
						else
						{
							isBlank = true;
							found = std::find(tempRack.begin(), tempRack.end(),'*') ;
							if(found!=tempRack.end())tempRack.erase(found);
						}
					}
					if(isBlank == false)
					{
						if(tempType == 2 || tempType == 3)
						{
							tempScore += (getTilesScoreByIndex(getIndexByLetter(word.at(i))) * tempType);
						}
						else
						{
							tempScore += getTilesScoreByIndex(getIndexByLetter(word.at(i)));
						}
					}
					//cek bagian kanan
					if(papan.getLetterOnPosition(tempRow, col + 1) != '-' && (col + 1) < 15 )
					{
						status = 0;
						tempCol = col + 1;
						//mencari ujungnya dan membentuk kata
						while(status == 0)
						{
							if(tempCol <= 14)
							{
								tempWord =  tempWord + papan.getLetterOnPosition(tempRow,tempCol);
								if( papan.getBlankTileStatus(tempRow,tempCol) == false)
								{
									tempScore += getTilesScoreByIndex(getIndexByLetter(papan.getLetterOnPosition(tempRow,tempCol)));
								}
								tempCol ++;
								if(papan.getLetterOnPosition(tempRow, tempCol) == '-' || tempCol > 14)
								{
									status = 1;
								}
							}
							else
							{
								status = 1;
							}
						}
					}
					if(tempWord.length() > 1)
					{
						wordCheck = isWord(tempWord);
						if(wordCheck == 1)
						{
							if(tempType > 3 && tempType / 2 == tileType)
							{
								tempScore = tempScore * tileType;
							}
							score += tempScore;
						}
						else
						{

							//CCLOG("gagal word %s length %d direction %d position row %d col %d",tempWord.c_str(),tempWord.length(),direction,row,col);
							score = -1;
							wordStatus = 1;
						}
					}
				}

				tempRow++;
			}



			i++;
		}

		return score;
	}


	/*--------------ADDITION--------------------------*/

	//mengecek apakah pada jalur kata yang dimainkan mengandung 2W atau 3W
	int getScoreTypeByPosition(int length,int row, int col, int direction)
	{
		//untuk mengetahui apakah ada yg titik tengah, 2W atau 3W
		int status = 0;
		int i = 0;
		while(i < length && status <= 3 && status != -1)
		{
			if(((15 * row) + col) < 225)
			{
				status = papan.getTileType(row,col);
				//CCLOG("di score by position");
			}
			if(status <=3)
			{
				if(direction == 1)
				{
					col++;
				}
				else
				{
					row++;
				}
				i++;
			}
		}
		if(status > 3)
		{
			status  = status / 2;
		}
		else if(status == -1)
		{
			status = 2;
		}
		else
		{
			status = 1;
		}
		return status;
	}

	//mengetahui score dari sebuah kata sesuai dengan bobot tile
	int getScoreByWord(string word)
	{
		int score = 0;
		int i;
		for(i = 0 ; i < word.length() ; i++)
		{
			score += getTilesScoreByIndex(getIndexByLetter(word.at(i)));
		}
		return score;
	}

	//mengambil huruf sesuai index
	char getLetterByIndex(int index)
	{
		string letters = "abcdefghijklmnopqrstuvwxyz*";
		return letters.at(index);
	}

	//mengambil index berdasarkan huruf
	int getIndexByLetter(char alphabet)
	{
		string letters = "abcdefghijklmnopqrstuvwxyz*";
		std::size_t found = letters.find(alphabet);
		return found;
	}

	//mengambil score per - tile
	int getTilesScoreByIndex(int index)
	{
		int score[27] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10,0};

		return score[index];
	}

	//mengetahui apakah pada sebuah node memiliki cabang ke huruf (L) atau tidak
	node* nextArc(node *arc, char l)
	{
		node *now = NULL;
		jalur *jalan = arc->arc;
		int status = 0;
		while(jalan!= NULL && status == 0)
		{
			if(jalan->tujuan->huruf == l)
			{
				now = jalan->tujuan;
				status = 1;
			}
			else
			{
				jalan = jalan->nextjalur;
			}
		}
		if(now == NULL)
		{
			//std::cout<<l<<"\n";
		}
		return now;
	}

	//set kata ke kelas papan
	void addWordToPapan(string word, int row,int col,int direction)
	{
		int i;
		for(i = 0; i < word.length() ; i++)
		{
			/*if(((TileOnBoard *)gameScene->getChildByTag((15 * row) + col))->getBlankTileStatus() != false)
			{
			papan.setIsBlankTile(true,row,col);
			}*/
			papan.setCharOnPapan(row,col,word.at(i));
			papan.setIsSetTile(true,row,col);
			if(direction == 1)
			{
				col++;
			}
			else
			{
				row++;
			}
		}
	}


	void addWordToPapanGui(string word, int row,int col,int direction)
	{
		int i;
		int tempTag;
		int tempRow = row;
		int tempCol = col;
		for(i = 0; i < word.length() ; i++)
		{

			papan.setCharOnPapan(tempRow,tempCol,word.at(i));
			papan.setIsSetTile(true,tempRow,tempCol);
			tempTag = (15 * tempRow) + tempCol;
			((TileOnBoard *)gameScene->getChildByTag(tempTag))->changeTileImage(getIndexByLetter(word.at(i)));
			((TileOnBoard *)gameScene->getChildByTag(tempTag))->setLetter(word.at(i));

			((TileOnBoard *)gameScene->getChildByTag(tempTag))->setIsSetTile(true);
			if(direction == 1)
			{
				tempCol++;
			}
			else
			{
				tempRow++;
			}
			//CCLOG("masuk ke papan");
		}
	}

	//mengambil kata yang menempel (hook) pada jalur yang dimainkan (keperluan GameHumanScene)
	string getHook(string word,int row,int col,int direction)
	{
		int status,i;
		int startRow = row;
		int startCol = col;
		char letter;
		string hook = "";
		status = 1;
		if(direction == 1)
		{
			if(col > 0)
			{
				col--;
				status = 0;
			}
		}
		else
		{
			if(row > 0)
			{
				row--;
				status = 0;
			}
		}
		while(papan.getLetterOnPosition(row,col) != '-' && status == 0)
		{
			hook = papan.getLetterOnPosition(row,col) + hook;
			if(direction == 1)
			{
				col--;
				if(col < 0) status = 1;
			}
			else
			{
				row--;
				if(row < 0) status = 1;
			}
		}
		CCLOG("hook : %s start position row %d col %d",hook.c_str(),startRow,startCol);
		return hook;
	}


	//mengecek apakah sebuah kata dapat ditaruh atau ngga
	int isFitOnBoard(string word,int row,int col,int direction)
	{
		int i,status;
		i = 0;
		status = 0;
		int sama = 0;
		int fit = 1;
		while(i < word.length() && status == 0)
		{
			if(papan.getSetStatus(row,col) != false)
			{
				if(papan.getLetterOnPosition(row,col) != word.at(i))
				{
					fit = 0;
					status = 1;
				}
				else
				{
					sama++;
				}
			}
			if(fit == 1)
			{
				if(direction == 1)
				{
					col++;
				}
				else
				{
					row++;
				}
				i++;
			}
		}
		if(sama == word.length())
		{
			fit = 0;
		}
		return fit;

	}


	//memastikan apakah yang ditaruh adalah kata atau bukan
	int isWord(string kata)
	{
		int word = 0;
		node *now = G.start;
		int stat;
		int stop = 0;
		jalur *jalan;
		int i = kata.length()-1;
		while(now!=NULL && stop == 0)
		{
			if(now->arc != NULL)
			{
				jalan = now->arc;
				stat = 0;


				while(jalan != NULL && stat == 0)
				{

					if(jalan->tujuan->huruf == kata[i])
					{
						if(i == 0)
						{
							if(jalan->tujuan->eow == 1)
							{
								word = 1;
							}
							stop = 1;
							stat = 1;
						}
						else
						{
							now = jalan->tujuan;
							i--;
							stat = 1;
						}
					}
					else
					{
						jalan = jalan->nextjalur;
					}
				}
				if(stat == 0)
				{
					stop = 1;
				}
			}
			else
			{
				stop = 1;
			}
		}

		return word;
	}

	string getFirstWord(string str)
	{
		int i = 0;
		int status = 0;
		string first = "";
		while(i < str.length() && status == 0)
		{
			if(str.at(i) == ' ')
			{
				status = 1;
			}
			else
			{
				first += str.at(i);
				i++;
			}

		}

		return first;
	}

	string getWordDefinition(string word)
	{
		int status,j;
		std::ifstream file;
		string str,result;
		status = 0;
		j = 0;
		result = "";
		file.open("CSW12_definition.txt");
		status = 0;
		while (std::getline(file, str) && status == 0)
		{
			if(getFirstWord(str) == word){
				result = str;
				status = 1;
			}
		}
		file.close();
		return result;
	}



	//void clearGraph()
	//{
	//	node *now = (G).start;
	//	vector<node*> vec_node;
	//	node *hapus;
	//	int j = 0;
	//	while(now -> next != NULL){
	//		vec_node.push_back(now);
	//		now = now -> next;
	//		j++;
	//	}
	//	CCLOG("hmm %d",j);
	//	int i;
	//	for(i = (vec_node.size() - 1) ; i >= 0 ; i--)
	//	{
	//		free(vec_node.at(i));
	//	}
	//	vec_node.clear();
	//	j = 0;
	//	now = (G).start;
	//	while(now!= NULL)
	//	{
	//		CCLOG("ada");
	//		now = now->next;
	//		/*hapus  = now;
	//		now = now->prev;
	//		hapus->next = NULL;
	//		hapus->prev = NULL;
	//		free(hapus);*/
	//	}
	//}

	//mengecek jumlah anchor square pada keadaan papan
	int getTotalAnchor()
	{
		int total  = 0;
		int row , col;
		for(row = 0; row < 15 ; row++)
		{
			for(col = 0 ; col < 15 ; col++)
			{
				if(checkAnchor(row,col) == 1)
				{
					total++;
				}
			}
		}
		return total;
	}

	~Gaddag()
	{

	}
};
