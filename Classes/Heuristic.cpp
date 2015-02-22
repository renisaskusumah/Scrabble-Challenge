#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include "cocos2d.h"

#define COCOS2D_DEBUG 1
USING_NS_CC;
using namespace std;

class Heuristic
{
	
	public:
		Heuristic()
		{
		
		}
		
		
		/**----WEIGHTED HEURISTIC-----**/
		//oleh Nick Ballard (heuristic left tiles on rack & keseimbangan vokal dan konsonan pada left tiles on rack)
		
		
		//parameter -> tiles dan first tiles or second tiles
		float tilesHeuristic(int tiles,int index)
		{
			float weight[27][3] = {{1,-3},{-3.5,-3},{-0.5,-3.5},{0,-2,5},{4,-2.5},{-2,-2},{-2,-2.5},
							  {0.5,-3.5},{-0.5,-4},{-3,0},{-2.5,0},{-1,-2},{-1,-2},{0.5,-2.5},
							  {-1.5,-3.5},{-1.5,-2.5},{-11.5,0},{1.5,-3.5},{7.5,-4},{0,-2.5},{-3,-3},
							  {-5.5,-3.5},{-4,-4.5},{-3.5,0},{-2,-4.5},{2,0},{24.5,-15}};
			
			
			return weight[tiles][index];
		}
		
		//evaluasi = score + tiles left + vcmix
		float evaluate(int score , vector<int> left_tiles)
		{
			float total = 0;
			int i,j;
			int status;
			total += (float)score;
			int vokal,konsonan;
			vokal = 0;
			konsonan = 0 ;
			for(i = 0 ; i < left_tiles.size() ; i++)
			{
				if(i > 0)
				{
					//cek apakah huruf tersebut pertama atau bukan di jejeran left_tiles
					status = 0;
					j = i -1;
					while(j >= 0 && status == 0)
					{
						if(left_tiles.at(j) == left_tiles.at(i))
						{
							total+= tilesHeuristic(left_tiles.at(i),1);
							status = 1;
						}
						else
						{
							j--;
						}
					}
					if(status == 0)
					{
						total+= tilesHeuristic(left_tiles.at(i),0);
					}
					
				}
				else
				{
					total+= tilesHeuristic(left_tiles.at(i),0);
				}
				if(isKonsonan(left_tiles.at(i)) == 1 && left_tiles.at(i) < 26)
				{
					konsonan++;
				}
				else if(left_tiles.at(i) < 26)
				{
					vokal++;
				}
			}
			total += (float)VCmix(konsonan,vokal);
			
			return total;
		}
		
		int isKonsonan(int index)
		{
			//a,i,u,e,o
			if(index == 0 || index == 4 || index == 8 ||index == 14 || index == 20)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		
		
		//nilai evaluasi keseimbangan konsonan dan vokal
		int VCmix(int konsonan,int vokal)
		{
			int lookup[7][7] = {{0,-1,-2,-3,-4,-5,-6},{0,1,0,-1,-2,-3,0},{-1,1,2,1,0,0,0},{-2,0,2,3,0,0,0},{-3,-1,1,0,0,0,0},{-4,-2,0,0,0,0},{-5,0,0,0,0,0,0}};
			
			return lookup[konsonan][vokal];
		}
		
		

		
		~Heuristic()
		{
		
		}
};
