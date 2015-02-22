
#include <string>
#include <stdio.h>
#include <iostream>
typedef struct
{
	bool isSet;
	bool isBlankTile;
	int tileType;
	char letter;
}tile;
using namespace std;
class Papan
{
private:
	tile papan [15][15];
	int rowPlayed;
	int colPlayed;
	string newTilesOnBoard;
public:

	Papan()
	{

	}

	void initPapan()
	{
		int i,j;
		//set papan jadi kosong
		for(i = 0 ; i < 15 ; i ++)
		{
			for(j = 0 ; j < 15 ; j++)
			{
				papan[i][j].letter = '-';
				papan[i][j].isBlankTile = false;
				papan[i][j].isSet = false;
			}
		}
	}

	void setNewTilesOnBoard()
	{	
		newTilesOnBoard = "";
	}

	string getNewTilesOnBoard()
	{
		return newTilesOnBoard;
	}

	void setTileType(int type,int row,int col)
	{
		papan[row][col].tileType = type ;
	}

	int getTileType(int row,int col)
	{
		int tiletype = papan[row][col].tileType;
		if ( papan[row][col].isSet == true)
		{
			tiletype = 1;
		}
		return tiletype ;
	}

	void setIsSetTile(bool set,int row,int col)
	{
		papan[row][col].isSet = set;
	}
	void setIsBlankTile(bool b,int row,int col)
	{
		papan[row][col].isBlankTile = b;
	}

	bool getBlankTileStatus(int row,int col)
	{
		return papan[row][col].isBlankTile;
	}

	bool getSetStatus(int row,int col)
	{
		return papan[row][col].isSet;
	}

	void setRowPlayed(int row)
	{
		rowPlayed = row;
	}

	void setColPlayed(int col)
	{
		colPlayed = col;
	}

	int getRowPLayed()
	{
		return rowPlayed;
	}

	int getColPlayed()
	{
		return colPlayed;
	}

	void setCharOnPapan(int row,int col, char huruf)
	{
		if(papan[row][col].isSet == false)
		{
			if(papan[row][col].isBlankTile == false)
			{
				newTilesOnBoard += huruf;
			}
			else
			{
				newTilesOnBoard +="*";
			}
		}
		papan[row][col].letter =  huruf;
		papan[row][col].isSet = true;
	}

	char getLetterOnPosition(int row,int column)
	{
		char l;
		//l = papan[row][column].letter ;
		if(row < 0 || row > 14 ||column < 0 || column > 14)
		{
			l = '-';
			//std::cout<<row<<" "<<column<<"\n";
		}
		else
		{
			l = papan[row][column].letter ;
		}
		return l;
	}

	void showPapan()
	{
		int i,j;
		for(i = 0 ; i < 15 ; i++)
		{
			for(j = 0 ; j < 15 ; j++)
			{	
				std::cout<<papan[i][j].letter<<" ";
			}
			std::cout<<"\n";
		}
		std::cout<<"\n";
	}

	int getTotalNotSetTile()
	{
		int i,j,total;
		total = 0;
		for(i = 0 ; i < 15 ; i++)
		{
			for(j = 0 ; j < 15 ; j++)
			{	
				if(papan[i][j].isSet == false)
				{
					total++;
				}
			}
		}
		return total;
	}

	string getNewTilesByWord(int row,int col,string word,int direction)
	{
		int i,j;
		string usedTiles = "";
		for(i = 0 ; i < word.length(); i++)
		{
			if(getSetStatus(row,col) == false)
			{
				usedTiles = usedTiles + word.at(i);
			}
			if(direction == 1)
			{
				col++;
			}
			else
			{
				row++;
			}
		}
		return usedTiles;
		
	}

	~Papan()
	{

	}	
};
