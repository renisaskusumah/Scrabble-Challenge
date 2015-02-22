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
	Negascout negascout;
	int totalNode;
	int score;
	string word;
	
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

	vector<char> playNegaScout(vector<char> opponentRack,vector<char> rack2,int p1,int p2,string logName)
	{
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
		opponentRack = getRandomUnseenTiles(rack2);
		gaddag.setActiveRack(rack2);
		tempPapan = gaddag.getBoard();
		Negascout nega;
		nega.setTree(999);
		nega.setPlayerScore(p2);
		nega.setOpponentScore(p1);
		level = 5;
		totalNode = 0;
		clock_t start;
		float durasi;
		start = clock();
		generateTree(nega,opponentRack,rack2,level,nega.getTree().root,vector_movement,vector_movement2);
		nega.setTotalNode(totalNode);
		durasi = ( clock() - start ) / (float) CLOCKS_PER_SEC;
		CCLOG("durasi %f",durasi);
		//std::cout<<"Total Node : " <<totalNode<<"\n";
		CCLOG("Total Node : %d",totalNode);
		if(totalNode > 0)
		{
			nega.setDepth(level + 1);
			nega.setVisited(0);
			nega.setLogFile(logName);
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
				gaddag.addWordToPapanGui(evaluasi->word,evaluasi->row, evaluasi->col, evaluasi->direction);
				//p2+=score;
				newTiles =gaddag.getBoard().getNewTilesOnBoard();
				for(i = 0 ; i < newTiles.length() ; i++)
				{
					found = std::find(rack2.begin(), rack2.end(),newTiles.at(i)) ;
					if(found!=rack2.end())
					{
						rack2.erase(found);
					}
				}
				//gameToFile("negascout",evaluasi->word,0,score,"NEGASCOUT");
			}
		}
		else
		{
			CCLOG("heyaaaa");
			gaddag.setBoard(tempPapan);
			gaddag.getBoard().setNewTilesOnBoard();
			rack2 = gaddag.playGame(rack2);
			score = gaddag.getScore();
		}
		unseenLetter = tempUnseen;
		updateUnseenLetterByWord(gaddag.getBoard().getNewTilesOnBoard());
		return rack2;
	}

	int getNegaScore()
	{
		return score;
	}

	string getNegaWord()
	{
		return word;
	}

	void setUpNega(vector<char> r1, vector<char> r2)
	{
		papan = gaddag.getBoard();
	}

	void generateTree(Negascout nega,vector<char> opponent,vector<char> rack,int depth,nodetree *node,vector<possible> tempPossible,vector<possible> tempPossible2)
	{
		int tempDepth;
		nodetree *now;
		if(depth > 0 && rack.size() > 0 && opponent.size() > 0)
		{
			papan = gaddag.getBoard();
			tempDepth = depth;
			vector<char> temp,active;
			vector<possible> vector_movement;
			vector<possible> result;
			vector<possible> tempResult;
			if(depth %2 == 1)
			{
				active = rack;
				tempResult = tempPossible;
			}
			else
			{
				active = opponent;
				tempResult = tempPossible2;
			}
			temp = active;
			vector_movement = tempPossible;
			int i,j,row,col,k;
			vector<char>::iterator found;
			vector<int>::iterator found_integer;
			if(tempPossible.size() == 0 || tempPossible2.size() == 0)
			{
				vector_movement = gaddag.possibleMovementInVector(active);
				result = vector_movement;
			}
			else
			{
				//vector_movement hanya menampung possible baru yang terjadi jika langkah di level sebelumnya diambil
				//mengumpulkan posisi anchor yang baru
				vector<int> anchor;
				vector<int> tempAnchor;
				//pilih dulu mana saja yang harus dihapus dari tempResult , dalam kata lain anchor tersebut sudah dipakai
				for(i = 0 ; i < tempResult.size() ; i++)
				{
					tempAnchor.clear();
					//jika sudah terisi, maka dihapus
					if(papan.getSetStatus(tempResult.at(i).rowAnchor,tempResult.at(i).colAnchor) != false)
					{
						//cari anchor baru sekitar kata ini
						tempAnchor = gaddag.getAnchorByWord(tempResult.at(i).startRow,tempResult.at(i).startCol,tempResult.at(i).word,tempResult.at(i).direction);
						anchor.insert(anchor.end(), tempAnchor.begin(), tempAnchor.end());
					}
					else
					{
						//cek dulu apakah solusinya mengandung newtiles sesuai dengan sisa atau tidak
						int status = 0;
						j = 0;
						vector<char> tempActive = active;
						while(status == 0 && j < tempResult.at(i).usedTiles.length())
						{
							found = std::find(tempActive.begin(),tempActive.end(),tempResult.at(i).usedTiles.at(j)) ;
							if (found != tempActive.end())
							{
								tempActive.erase(found);
								j++;
							}
							else
							{
								status = 1;
							}
						}
						if(status == 0)
						{
							result.push_back(tempResult.at(i));
							found_integer = std::find(anchor.begin(), anchor.end(),(tempResult.at(i).rowAnchor * 15 )+tempResult.at(i).colAnchor) ;
							if (found_integer!=anchor.end())
							{
								anchor.erase(found_integer);
							}
						}
						else
						{
							
							anchor.push_back((tempResult.at(i).rowAnchor * 15 )+tempResult.at(i).colAnchor);
						}
					}
				}
				// //generate movement untuk anchor yang baru
				vector_movement = gaddag.possibleMovementInVectorByAnchor(active,anchor);
				if(vector_movement.size() > 0) result.insert(result.end(), vector_movement.begin(), vector_movement.end());
			}
			//if(depth == this->level-1)std::cout<<"node : "<<node->word<<" result : "<<result.size()<<" rack "<<getStringByRack(active)<<"\n";
			for(i = 0 ; i < result.size() ; i++)
			{
				active = temp;
				depth = tempDepth;
				gaddag.setBoard(papan);
				if(depth == this->level)
				{
					gaddag.setBoard(tempPapan);
				}
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
				now = nega.addChild(999, node,result.at(i).word,result.at(i).startRow,result.at(i).startCol,result.at(i).direction , result.at(i).score,basicTiles.getLeftTilesScoreByVector(active),result.at(i).rowAnchor,result.at(i).colAnchor,10);
				totalNode++;
				if(depth %2 == 1)
				{
					rack = active;
					generateTree(nega,opponent,rack,depth - 1,now,result,tempPossible2);
					//rack = randomRackFromUnseen(rack);
				}
				else
				{
					opponent = active;
					generateTree(nega,opponent,rack,depth - 1,now,tempPossible,result);
					//opponent = randomRackFromUnseen(opponent);
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