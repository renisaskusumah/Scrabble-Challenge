#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;
//merupakan kelas khusus tentang komponen yg dibutuhkan dalam permainan -> board & tiles
class BasicTiles
{
	private:
		int count;
	public:
		
		BasicTiles()
		{
		}
		char* getLetterFile(int index)
		{
			char* fileName[27] = {"assets/A.png","assets/B.png","assets/C.png","assets/D.png","assets/E.png","assets/F.png","assets/G.png","assets/H.png","assets/I.png","assets/J.png","assets/K.png","assets/L.png","assets/M.png","assets/N.png","assets/O.png","assets/P.png","assets/Q.png","assets/R.png","assets/S.png","assets/T.png","assets/U.png","assets/V.png","assets/W.png","assets/X.png","assets/Y.png","assets/Z.png","assets/blank.png"}; 
			
			return fileName[index];
		}

		char* getSmallLetterFile(int index)
		{
			char* fileName[26] = {"assets/A_small.png","assets/B_small.png","assets/C_small.png","assets/D_small.png","assets/E_small.png","assets/F_small.png","assets/G_small.png","assets/H_small.png","assets/I_small.png","assets/J_small.png","assets/K_small.png","assets/L_small.png","assets/M_small.png","assets/N_small.png","assets/O_small.png","assets/P_small.png","assets/Q_small.png","assets/R_small.png","assets/S_small.png","assets/T_small.png","assets/U_small.png","assets/V_small.png","assets/W_small.png","assets/X_small.png","assets/Y_small.png","assets/Z_small.png"}; 
			
			return fileName[index];
		}

		char* getBlankLetterFile(int index)
		{
			char* fileName[26] = {"assets/A_blank.png","assets/B_blank.png","assets/C_blank.png","assets/D_blank.png","assets/E_blank.png","assets/F_blank.png","assets/G_blank.png","assets/H_blank.png","assets/I_blank.png","assets/J_blank.png","assets/K_blank.png","assets/L_blank.png","assets/M_blank.png","assets/N_blank.png","assets/O_blank.png","assets/P_blank.png","assets/Q_blank.png","assets/R_blank.png","assets/S_blank.png","assets/T_blank.png","assets/U_blank.png","assets/V_blank.png","assets/W_blank.png","assets/X_blank.png","assets/Y_blank.png","assets/Z_blank.png"}; 
			
			return fileName[index];
		}

		int getStatusBoardByPosition(int row,int col)
		{
			//untuk mengetahui apakah statusnya biasa, 2L,3L,2W,3W, atau petak tengah
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
			return arrPapan[row][col];
		}

		

		int getTilesFrequentByIndex(int index)
		{
			//mengambil jumlah kemunculan setiap tiles dalam game
			int frequent[27] = {9,2,2,4,12,2,3,2,9,1,1,4,2,6,8,2,1,6,4,6,4,2,2,1,2,1,2};

			return frequent[index];
		}

		int getTileScoreByChar(char tile)
		{
			string letters = "abcdefghijklmnopqrstuvwxyz*";
			size_t found = letters.find(tile);
			return getTilesScoreByIndex(found);
		}

		int getTilesScoreByIndex(int index)
		{
			//mengambil score per - tile
			int score[27] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10,0};

			return score[index];
		}

		string getLetterByIndex(int index)
		{
			string alphabet[27] =  {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","*"}; 
			
			return alphabet[index];
		}

		char getCharByIndex(int index)
		{
			char alphabet[27] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','*'};  
			
			return alphabet[index];
		}

		int getIndexByLetter(char alphabet)
		{
			string letters = "abcdefghijklmnopqrstuvwxyz*";
			std::size_t found = letters.find(alphabet);
			return found;
		}

		int getLeftTilesScoreByVector(vector<char> tiles)
		{
			int score = 0;
			int i;
			for(i = 0 ; i < tiles.size();i++)
			{
				score+= getTileScoreByChar(tiles.at(i));
			}
			
			return score;
		}

		string getStringByRack(vector<char> vec)
		{
			string str = "";
			int size,i;
			size = vec.size();
			for(i = 0 ; i < size ; i++)
			{
				str+= vec.at(i);
			}
			return str;
		}

		~BasicTiles()
		{
		}
	
};