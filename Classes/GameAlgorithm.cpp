#include <string>
#include "cocos2d.h"
#include <stdio.h>
#include <ctype.h>
#include "Gaddag.cpp"
#include "NegaScout.cpp"
#include "BasicTiles.cpp"

USING_NS_CC;
using namespace std;
class GameAlgorithm
{
private:
	Gaddag gaddag;
	Papan papan;
	BasicTiles basicTiles;
	Papan tempPapan;
	int level;
	vector<vector<int> > unseenLetter;
	vector<char> opponentRack;
	int totalNode;
	int score;
	string word;
	nodetree* eval; // hasil evaluasi negascout
	int nega_stat; //apakah jadinya pakai nega atau heuristic biasa
	string fileName;
	string logFile;
public:

	GameAlgorithm()
	{
	}

	/*-----------GADDAG------------*/

	Gaddag& getGaddag()
	{
		return gaddag;
	}

	Papan& getBoard()
	{
		return gaddag.getBoard();
	}

	/*------------HEURISTIC------------*/

	/*----------NEGASCOUT---------------*/

	void setFileName(string s)
	{
		fileName = "GAME_TREE/"+s;
		logFile = s;
		ofstream fileGame;
		fileGame.open(fileName.c_str());
		fileGame.close();
		string logNega = "LOG_NEGA/"+s;
		fileGame.open(logNega.c_str());
		fileGame.close();
	}

	vector<char> playNegaScout(vector<char> rack2,int p1,int p2)
	{
		//vector<char> opponentRack;
		nega_stat = 1;
		vector<possible> vector_movement,vector_movement2;
		int tempSize;
		int i;
		score = 0;
		nodetree* evaluasi;
		vector<vector<int> > tempUnseen;
		string newTiles;
		vector<char>::iterator found;
		tempSize = rack2.size();
		tempUnseen = unseenLetter;
		if(opponentRack.size() == 0){ opponentRack = getRandomUnseenTiles(rack2); }
		gaddag.setActiveRack(rack2);
		tempPapan = gaddag.getBoard();
		Negascout nega;
		nega.setLogNega(logFile);
		nega.setTree(999);
		nega.setPlayerScore(p2);
		nega.setOpponentScore(p1);
		level = 7;
		totalNode = 0;
		clock_t start;
		float durasi;
		start = clock();
		gaddag.setHeuristic(true);
		ofstream ofile(fileName.c_str(), ios::app);
		ofile<<"\n\n";
		ofile.close();
		generateTree(nega,opponentRack,rack2,level,nega.getTree().root,vector_movement,vector_movement2);
		durasi = ( clock() - start ) / (float) CLOCKS_PER_SEC;
		ofstream ofile2(fileName.c_str(), ios::app);
		ofile2<<"durasi : "<<durasi<<"\n";
		ofile2.close();
		CCLOG("durasi %f",durasi);
		nega.setTotalNode(totalNode);
		//std::cout<<"Total Node : " <<totalNode<<"\n";
		CCLOG("Total Node : %d",totalNode);
		if(totalNode > 0)
		{
			nega.setDepth(level + 1);
			nega.setVisited(0);
			nega.setLogFile("LOG_COMPUTER/" +logFile);
			ofstream ofile(nega.getLogNega().c_str(),ios::app);
			ofile<<"\n\n";
			ofile.close();
			int result = nega.negaScout(nega.getTree().root,-999,999,1);
			CCLOG("result : %d",result);
			CCLOG("visited : %d",nega.getVisited());
			nega.ambilSolusi((result * -1) , nega.getTree().root);
			if(result != -999 && result != 999)
			{
				nega.printChild(nega.getTree().root);
				evaluasi = nega.result(result * -1,nega.getTree().root);
				gaddag.setBoard(tempPapan);
				//std::cout<<g.getHint(rack2);
				/*put word berdasar negascout*/
				gaddag.setActiveRack(rack2);
				score = 0;
				score =gaddag.wordScore(evaluasi->word,evaluasi->row, evaluasi->col, evaluasi->direction);
				score +=gaddag.getScoreByAnchor(evaluasi->word,evaluasi->row, evaluasi->col, evaluasi->direction);
				word = evaluasi->word;
				CCLOG("eval word %s",evaluasi->word.c_str());
				gaddag.getBoard().setNewTilesOnBoard();
				eval = evaluasi;
				//gaddag.addWordToPapanGui(evaluasi->word,evaluasi->row, evaluasi->col, evaluasi->direction);
				//p2+=score;
				/*newTiles =gaddag.getBoard().getNewTilesOnBoard();
				for(i = 0 ; i < newTiles.length() ; i++)
				{
					found = std::find(rack2.begin(), rack2.end(),newTiles.at(i)) ;
					if(found!=rack2.end())
					{
						rack2.erase(found);
					}
				}*/
				//gameToFile("negascout",evaluasi->word,0,score,"NEGASCOUT");
			}
		}
		else
		{
			nega_stat = 0;
			CCLOG("heyaaaa");
			gaddag.setBoard(tempPapan);
			gaddag.getBoard().setNewTilesOnBoard();
			rack2 = gaddag.playGame(rack2);
			score = gaddag.getScore();
		}
		unseenLetter = tempUnseen;
		/*updateUnseenLetterByWord(gaddag.getBoard().getNewTilesOnBoard());*/
		return rack2;
	}

	vector<char> negaResultToGui(vector<char> rack )
	{
		gaddag.getBoard().setNewTilesOnBoard();
		if(nega_stat == 1)
		{
			string newTiles;
			int i;
			vector<char>::iterator found;
			gaddag.addWordToPapanGui(eval->word,eval->row, eval->col, eval->direction);
			newTiles =gaddag.getBoard().getNewTilesOnBoard();
			for(i = 0 ; i < newTiles.length() ; i++)
			{
				found = std::find(rack.begin(), rack.end(),newTiles.at(i)) ;
				if(found!=rack.end())
				{
					rack.erase(found);
				}
			}
		}
		else
		{
			gaddag.generateToGui(rack);
		}
		CCLOG("new tiles : %s",gaddag.getBoard().getNewTilesOnBoard().c_str());
		updateUnseenLetterByWord(gaddag.getBoard().getNewTilesOnBoard());
		return rack;
	}

	int getNegaScore()
	{
		return score;
	}

	string getNegaWord()
	{
		return word;
	}

	

	void generateTree(Negascout nega,vector<char> opponent,vector<char> rack,int depth,nodetree *node,vector<possible> movement_computer,vector<possible> movement_human)
	{
		int tempDepth;
		nodetree *now;
		string stringOfRack;
		if(depth > 0 && rack.size() > 0 && opponent.size() > 0)
		{
			papan = gaddag.getBoard();
			tempDepth = depth;
			vector<char> temp,active,rack_temp;
			vector<possible> result;
			vector<possible> result_2;
			if(depth %2 == 1)
			{
				active = rack;
				result = movement_computer;
			}
			else
			{
				active = opponent;
				result = movement_human;
			}
			temp = active;
			stringOfRack = basicTiles.getStringByRack(active);
			int i,j,row,col;
			vector<char>::iterator found;
			if(result.size() == 0)
			{
				gaddag.getAllPossible(active);
				result_2 = gaddag.getAllPossibleVector();
				//result = gaddag.evaluateAllPossible(gaddag.getVectorHeuristic().size());
				result = gaddag.evaluateAllPossible(50);
				//jika mengandung blank tambahkan evaluate lagi di gaddag untuk menyederhanakan total possible
			}
			else
			{
				result_2 = result;
			}
			for(i = 0 ; i < result.size() ; i++)
			{
				active = temp;
				depth = tempDepth;
				gaddag.setBoard(papan);
				if(depth == this->level)
				{
					gaddag.setBoard(tempPapan);
				}
				//periksa dulu apakah used tilesnya ad di rack active tidak
				rack_temp = active;
				//periksa juga apakah use tiles nya ada di rack active atau tidak
				string str2 = result.at(i).usedTiles;
				string str = stringOfRack;
				std::size_t found2 = str.find_first_of(str2);
				int sama = 0;
				while (found2!=std::string::npos)
				{
					sama++;
					string::size_type loc = str2.find( str.at(found2), 0 );
					if( loc != string::npos ) {
						str2.erase(str2.begin()+loc);
					}
					str.at(found2) = '-';
					found2=str.find_first_of(str2,found2+1);
				}
				if(sama == result.at(i).usedTiles.length())
				{
					//jika ada , maka periksa apakah masih bisa ditaruh di board atau tidak (cek jajaran tersebut sudah diisi belum)
					if(gaddag.isFitOnBoard(result.at(i).word,result.at(i).startRow,result.at(i).startCol,result.at(i).direction) == 1)
					{
						row = result.at(i).startRow;
						col = result.at(i).startCol;
						for(j = 0 ; j < result.at(i).word.length() ; j++)
						{
							if(gaddag.getBoard().getLetterOnPosition(row,col) == '-')
							{
								found = std::find(active.begin(), active.end(),result.at(i).word.at(j)) ;
								if (found!=active.end())
								{
									active.erase(found);
								}
							}
							if(result.at(i).direction == 1)
							{
								col++;
							}
							else
							{
								row++;
							}
						}
						gaddag.addWordToPapan(result.at(i).word,result.at(i).startRow,result.at(i).startCol,result.at(i).direction);
						//nodetree *new_node;
						//int eval, nodetree *S,string word,int row,int col,int direction , int score,int leftTiles,int rowAnchor,int colAnchor,float heuristic,int usedTiles
						/*new_node->eval = 999;
						new_node->word = result.at(i).word;
						new_node->row = result.at(i).startRow;
						new_node->col = result.at(i).startCol;
						new_node->direction = result.at(i).direction;
						new_node->score = result.at(i).score;
						new_node->leftTiles = basicTiles.getLeftTilesScoreByVector(active);
						new_node->rowAnchor = result.at(i).rowAnchor;
						new_node->colAnchor = result.at(i).colAnchor;
						new_node->heuristic = result.at(i).heuristic;
						new_node->usedTiles = result.at(i).usedTiles.length();*/
						//now = nega.addChild(node,new_node);
						ofstream ofile(fileName.c_str(), ios::app);
						ofile<<"word : "<<result.at(i).word<<" anchor( col : "<<result.at(i).colAnchor<<" row : "<<result.at(i).rowAnchor<<" ) score : "<<result.at(i).score<<" new tiles : "<<result.at(i).usedTiles<<" heuristic : "<<result.at(i).heuristic<<" level : "<<level - depth + 1<<"\n";
						ofile.close();
						now = nega.addChild(999, node,result.at(i).word,result.at(i).startRow,result.at(i).startCol,result.at(i).direction , result.at(i).score,basicTiles.getLeftTilesScoreByVector(active),result.at(i).rowAnchor,result.at(i).colAnchor,result.at(i).heuristic,result.at(i).usedTiles.length());
						totalNode++;
						vector<int> new_anchor = gaddag.getAnchorByWord(result.at(i).startRow,result.at(i).startCol,result.at(i).word,result.at(i).direction);
						gaddag.getPossibleMovementByAnchor(active,new_anchor);
						vector<possible> hasil = gaddag.getAllPossibleVector();
						// result.insert(result.end(),g.getAllPossibleVector().begin(),g.getAllPossibleVector().end());
						if(depth %2 == 1)
						{
							rack = active;
							movement_computer = result;
							movement_computer.insert(movement_computer.end(),hasil.begin(),hasil.end());
						}
						else
						{
							opponent = active;
							movement_human = result;
							movement_human.insert(movement_human.end(),hasil.begin(),hasil.end());
						}
						generateTree(nega,opponent,rack,depth - 1,now,movement_computer,movement_human);
					}
				}
			}
		}
	}

	vector<char> randomRackFromUnseen(vector<char> rack)
	{
		srand(time(NULL));
		int i,kosong,temp;
		kosong = 7 - rack.size();
		for(i = rack.size() ; i < 7; i++)
		{
			if(unseenLetter.size() > 0)
			{
				temp = rand() % unseenLetter.size();
				rack.push_back(basicTiles.getCharByIndex(unseenLetter.at(temp).at(0)));
				updateUnseenLetter(unseenLetter.at(temp).at(0));
			}
		};
		return rack;
	}

	void setUnseenLetter()
	{
		int i;
		vector<int> letter;
		for(i = 0 ; i < 27 ; i++)
		{
			letter.push_back(i);
			letter.push_back(basicTiles.getTilesFrequentByIndex(i));
			unseenLetter.push_back(letter);
			letter.clear();
		}
	}

	void updateUnseenLetterByWord(string word)
	{
		int i;
		for(i = 0 ; i < word.length() ; i++)
		{
			updateUnseenLetter(basicTiles.getIndexByLetter(word.at(i)));
		}

	}

	void setOpponentRack(vector <char> rack)
	{
		opponentRack = getRandomUnseenTiles(rack);

	}

	vector<char> getOpponentRack(){
		return opponentRack;
	}

	void updateOpponentRack(string newTiles)
	{
		if(opponentRack.size() > 0)
		{
			int i;
			vector<char>::iterator found;
			for(i = 0 ; i < newTiles.length() ; i++)
			{
				found = std::find(opponentRack.begin(), opponentRack.end(),newTiles.at(i)) ;
				if(found!=opponentRack.end())
				{
					opponentRack.erase(found);
				}
			}
		}
	}

	//delete element jika sudah ditaruh di board
	void updateUnseenLetter(int alphabetIndex)
	{
		//find element
		int status = 0;
		int i = 0;
		while(i < unseenLetter.size() && status == 0)
		{
			if(unseenLetter.at(i).at(0) == alphabetIndex)
			{
				if(unseenLetter.at(i).at(1) == 1)
				{
					//sudah habis, hapus huruf dari unseen
					unseenLetter.erase(unseenLetter.begin() + i);
				} 
				else
				{
					unseenLetter.at(i).at(1) = unseenLetter.at(i).at(1) - 1; 
				}
				status = 1;
			}
			else
			{
				i++;
			}
		}
	}

	vector<char> getRandomUnseenTiles(vector<char> rack2)
	{
		//getSisaUnseen();
		vector<vector<int> > temp = unseenLetter;
		vector<char> unseen;
		string randomSeen = "";
		int i,index;
		//std::cout<<"size rack : "<<rack2.size()<<"\n";
		for(i = 0 ; i < rack2.size() ; i++)
		{
			updateUnseenLetter(basicTiles.getIndexByLetter(rack2.at(i)));
		}
		//getSisaUnseen();
		i = 0;
		srand(time(NULL));
		while(i < 7 && unseenLetter.size() > 0 )
		{
			index = rand() % unseenLetter.size();
			unseen.push_back(basicTiles.getCharByIndex(unseenLetter.at(index).at(0)));
			randomSeen += basicTiles.getCharByIndex(unseenLetter.at(index).at(0));
			updateUnseenLetter(unseenLetter.at(index).at(0));
			i++;
		}
		//getSisaUnseen();
		unseenLetter = temp;
		std::cout<<"random rack opponent : "<<randomSeen<<"\n";
		CCLOG("random rack : %s",randomSeen.c_str());
		return unseen;
	}

	~GameAlgorithm()
	{

	}
};