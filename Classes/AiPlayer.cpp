#include<string>
#include<iostream>
#include "cocos2d.h"
#include <stdio.h>
#include <ctype.h>
#include <algorithm>

#define COCOS2D_DEBUG

USING_NS_CC;
using namespace std;
//merupakan kelas player  dalam tes AI
class AiPlayer
{
	private:
		vector<char> _rack;
		CCLayer* game;
		int gameType; //1 human, 2 AI
		int passTurn;
		float totalDurasi;
	public:
		AiPlayer()
		{
		}

		void initPlayer(CCLayer* g,int gt)
		{
			game = g;
			gameType = gt;
			passTurn = 0;
			totalDurasi = 0;
			/*_rack.push_back('n');
			_rack.push_back('t');
			_rack.push_back('*');
			_rack.push_back('m');
			_rack.push_back('*');
			_rack.push_back('u');
			_rack.push_back('u');*/
		}

		//ambil karakter di rack berdasar index
		char getTileOnRack(int index)
		{
			return _rack.at(index);
		}

		//set karakter di rack berdasar index
		void setTileOnRack(char tile)
		{
			_rack.push_back(tolower(tile));
		}

		//menghapus karakter di rack berdasar index
		void deleteTileOnRack(int index)
		{
			_rack.erase(_rack.begin() + index);
		}

		//ambil vector rack
		vector<char> getRackVector()
		{
			return _rack;
		}
		
		//durasi di satu ronde
		float getTotalDurasi()
		{
			return totalDurasi;
		}

		//menambah durasi setiap mengambil langkah
		void addDurasi(float d)
		{
			totalDurasi+=d;
		}

		//set rack keseluruhan
		void setRack(vector<char> rack)
		{
			_rack.clear();
			_rack = rack;
		}

		//mengambil rack dalam bentuk string
		string getRackInString()
		{
			
			int i;
			string rack = "";
			for (i = 0 ; i < _rack.size() ; i++)
			{
				rack+=_rack.at(i);
			}
			return rack;
		}

		int isContainBlank()
		{
			int status = 0;
			if(std::find(_rack.begin(),_rack.end(),'*')!= _rack.end())
			{
				status = 1;
			}
			return status;
		}

		//berapa kali pass
		void setPassTurn(int turn)
		{
			passTurn= turn;
		}

		int getPassTurn()
		{
			return passTurn;
		}
		
		~AiPlayer()
		{
		}
};