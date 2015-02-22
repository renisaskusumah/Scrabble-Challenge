#include "Negascout.h"

/*Betul, jika pun misal kita terpaut lumayan jauh yg tidak memungkinkan kita utk menang saat endgame, 
paling tidak kita harus memikirkan bagaimana strategi supaya selisih skor kita dgn lawan (yg biasa kita sebut 'margin') tdk terpaut jauh.

Karena semakin dibantai skor kita oleh lawan,
maka posisi kita jika mengikuti turnamen akan semakin terancam. 
Karena jika di turnamen, utk menentukan posisi peringkat menggunakan Victory Point (jmlh kemenangan) dan Accumulative Margin (total margin sepanjang turnamen).
Semakin besar minus margin kita, semakin kebawah pula peringkat kita
-Julius Wilson , Indonesian Scrabble Federation-
*/
using namespace std;
class Negascout
{
private:
	int depth;
	tree T;
	int visited;
	int playerScore;
	int opponentScore;
	int totalNode;
	string logFileName;
	string logNega;
public:
	Negascout()
	{
	}

	void setLogFile(string logName)
	{
		logFileName = logName;
	}

	void setLogNega(string s)
	{
		logNega = "LOG_NEGA/"+s;
	}

	string getLogNega()
	{
		return logNega;
	}

	/*------Tree Construction---------*/

	void setTree(int eval)
	{
		makeTree(eval,&T);
	}

	tree& getTree()
	{
		return T;
	}

	void setPlayerScore(int s)
	{
		playerScore = s;
	}


	void setOpponentScore(int s)
	{
		opponentScore = s;
	}

	void makeTree(int eval,tree *T)
	{
		nodetree * baru = new nodetree();
		baru->eval = eval;
		baru->sibling = NULL;
		baru->child = NULL;
		baru->parent = NULL;
		baru->word = "-";
		baru->direction = 0;
		baru->score = 0;
		baru->row = 0;
		baru->depth = 0;
		baru->col = 0;
		baru->depth = 0;
		baru->totalChild = 0;
		baru->rowAnchor = -1;
		baru->colAnchor = -1;
		baru->endEval = -999;
		baru->heuristic = -999;
		baru->wordLength = 0;
		(*T).root = baru;
	}

	nodetree* addChild(int eval, nodetree *S,string word,int row,int col,int direction , int score,int leftTiles,int rowAnchor,int colAnchor,float heuristic,int usedTiles)
	//nodetree* addChild(nodetree *S,nodetree *new_node)
	{
		if(S!=NULL)
		{
			nodetree * baru = new nodetree();
			baru->eval = eval;
			baru->totalChild = 0;
			baru->word = word;
			baru->direction = direction;
			baru->score = score;
			baru->row = row;
			baru->col = col;
			baru->leftTiles = leftTiles;
			baru->rowAnchor = rowAnchor;
			baru->colAnchor = colAnchor;
			baru->wordLength = word.length();
			baru->heuristic = heuristic;
			baru->usedTiles = usedTiles;
			baru->depth = (S->depth) + 1;
			//baru = new_node;
			baru->endEval = -999;
			if(S->child==NULL)
			{
				//berarti nanti ga punya sibling
				S->child = baru;
				baru->child = NULL;
				baru->sibling = NULL;
				baru->parent = S;
				S->totalChild = 1;
			}
			else if(S->child!=NULL)
			{
				if(S->child->sibling==NULL)
				{
					//berarti dia anak kedua
					S->child->sibling = baru;
					baru->sibling = S->child;
					baru->child = NULL;
					baru->parent = S;
				}
				else
				{
					//dia anak kesekian dan harus ditaruh di paling ujung 
					nodetree * now = S->child;
					while(now->sibling!=S->child)
					{
						now = now->sibling;
					}
					now->sibling = baru;
					baru->sibling = S->child;
					baru->child = NULL;
					baru->parent = S;
				}
				S->totalChild = S->totalChild + 1;
			}
			return baru;
		}
		else
		{
			return NULL;
		}

	}

	/*------------Negascout algorithm---------------*/
	void setDepth(int d)
	{
		depth = d;
	}

	void setVisited(int v)
	{
		visited = v;
	}

	int getVisited()
	{
		return visited;
	}


	int negaScout(nodetree *position, int alpha, int beta, int depth)
	{
		int i,t,m,n,max;
		nodetree *now = position->child;
		if(depth == this->depth || position->child == NULL)
		{
			//return evaluate(position);
			//return position->eval;
			int eval = evaluate(position,depth);
			if(depth % 2 == 1)
			{
				eval = eval * (-1);
			}
			ofstream ofile(logNega.c_str(),ios::app);
			ofile<<"evaluate : "<<eval<<" level : "<<depth<<" word : "<<position->word<<"\n";
			ofile.close();
			return eval;
		}
		else
		{
			m = -999;
			n = beta;
			for(i = 1 ;  i <= position->totalChild;i++)
			{

				visited++;
				//FOR t := -negascout (p.i, -n, -max(alpha,m), depth+1);
				if(alpha >= m)
				{
					max = alpha;
				}
				else
				{
					max = m;
				}
				ofstream ofile(logNega.c_str(),ios::app);
				ofile<<"word: "<<now->word<<" m: "<<m<<" n: "<<n<<" alpha: "<<alpha<<" beta: "<<beta<<" max: "<<max<<" level : "<<depth<<"\n";
				ofile.close();
				t = -1 * negaScout(now ,(-1*n),(-1*max),depth + 1);
				//std::cout<<"t "<<t<<" m " <<m<<"\n";
				if(t > m)
				{
					if(n == beta || depth >= this->depth-2)
					{
						m = t;
					}
					else
					{
						m = -1 * negaScout(now, -1 *beta,-1 *t,depth + 1);
						ofstream ofile(logNega.c_str(),ios::app);
						ofile<<"word: "<<now->word<<" m: "<<m<<" n: "<<n<<" alpha: "<<alpha<<" beta: "<<beta<<" max: "<<max<<" level : "<<depth<<"\n";
						ofile.close();
					}
					position->eval = m;
				}
				//IF m >= beta  THEN RETURN (m);
				if(m >= beta)
				{
					ofstream ofile(logNega.c_str(),ios::app);
					ofile<<"----PRUNNING----\n";
					ofile<<"word: "<<now->word<<" m: "<<m<<" n: "<<n<<" alpha: "<<alpha<<" beta: "<<beta<<" max: "<<max<<" level : "<<depth<<"\n";
					ofile<<"-----\n\n";
					ofile.close();
					return m;
				}
				//n := max (alpha,m) + 1;
				if(alpha >= m)
				{
					max = alpha;
				}
				else
				{
					max = m;
				}
				n = max + 1;
				now = now->sibling;
			}
			return m;
		}

	}

	int evaluate(nodetree *position,int depth)
	{
		//evaluate bobot kemenangan
		nodetree *now = position;
		//int turn = 2; //apakah posisi di player atau opponent
		int player = 0;
		int opponent = 0;
		int eval;
		int endEval = 0;
		int playerLeft = 0;
		int opponentLeft = 0;
		string word;
		while(now != T.root)
		{	
			if(now->depth % 2 ==  1)
			{
				player+= now->score;
				if(playerLeft == 0) playerLeft = now->leftTiles;
				//turn = 2;
			}
			else
			{
				opponent+= now->score;
				if(opponentLeft == 0) opponentLeft = now->leftTiles;
				//turn = 1;
			}
			if(now->parent == T.root)
			{
				player+=playerScore;
				opponent+=opponentScore;
				player-=playerLeft;
				opponent-=opponentLeft;
				eval = player - opponent;
				now->player = player;
				now->opponent = opponent;
				//std::cout<<"eval : "<<eval<<" akhir : "<<player<<"\n";
				if(now->endEval == -999 || now->endEval < eval) 
				{
					now->endEval = eval;
					endEval = eval;
				}
				word = now->word;
			}
			now = now->parent;
		}
		// player+=playerScore;
		// opponent+=opponentScore;
		// player-=playerLeft;
		// opponent-=opponentLeft;
		// eval = player - opponent;
		// if(eval > 0)
		// {
		// eval = eval * 2;
		// }
		eval = eval * -1;
		//std::cout<<"eval "<<eval<<"\n";
		//if(position->depth % 2 == 1)eval * -1;
		position->eval = eval;
		//benerin ini
		//std::cout<<"depth "<<depth - 1<<" eval : "<<eval<<" p1:"<<opponent<<" p2:"<<player<<" player left :  "<<playerLeft<<" word "<<word<<" end eval "<<endEval<<"\n";
		return eval;
	}

	nodetree* result(int eval,nodetree* parent)
	{
		//nodetree* hasil;
		nodetree* now = parent->child;
		nodetree* hasil;
		int max = -999;
		int maxEval = -999;
		int maxLength = -999;
		float maxHeuristic = -999;
		int status = 0;
		int i = 0;
		int ketemu = 0;
		string str = "";
		for(i = 0 ; i < T.root->totalChild; i++)
		{
			ketemu = 0;
			str = now->word;
			if(now->eval == eval)
			{
				ketemu = 0;
				if(now->heuristic >= maxHeuristic)
				{
					if(now->heuristic == maxHeuristic)
					{
						if(now->score >= max)
						{
							if(now->score == max)
							{
								if(now->usedTiles > maxLength)
								{
									std::cout<<"lebih gede\n";
									ketemu = 1;
								}
							}
							else
							{
								ketemu = 1;
							}
						}
					}
					else
					{
						ketemu = 1;
					}
				}
				if(ketemu == 1)
				{
					max = now->score;
					maxHeuristic = now->heuristic;
					maxLength = now->usedTiles;
					hasil = now;
				}
				// if(now->score > max || (now->score == max && (now->word).length() > maxEval) || (now->score == max))
				// {
				// max = now->score;
				// maxEval = now->endEval;
				// hasil = now;
				// }
				// if(now->endEval > maxEval || (now->endEval == maxEval && now->score > max))
				// {
				// maxEval = now->endEval;
				// max = now->score;
				// hasil = now;
				// }
			}
			now = now->sibling;
		}
		return hasil;
	}

	string ambilSolusi(int eval,nodetree* parent)
	{
		nodetree* hasil = result(eval,parent);
		ofstream ofile(logFileName.c_str(), ios::app);
		ofile<<"\n/*-----HASIL NEGA----*/\n"<<hasil->word<<"\n";
		ofile<<"Total Node "<<totalNode<<"\n";
		ofile<<"Visited Node "<<visited<<"\n";
        ofile<<"Evaluate "<<hasil->eval<<"\n";
		ofile<<"word "<<hasil->word<<"\n";
		ofile<<"skor "<<hasil->score<<"\n\n\n";
		ofile.close();
		std::cout<<"word "<<hasil->word<<"\n";
		std::cout<<"skor "<<hasil->score<<"\n";
		return hasil->word;
	}

	void printChild(nodetree* parent)
	{
		int i;
		ofstream ofile(logFileName.c_str(), ios::app);

		nodetree* now = parent->child;
		ofile<<"\n-------NEGA--------\n\n";
		for(i = 0 ; i < parent->totalChild; i++)
		{
			std::cout<<"eval "<<now->eval<<" word "<<now->word<<" skor "<<now->score<<" depth "<<now->depth<<" end "<<now->endEval<<" child "<<now->totalChild<<" player "<<now->player<<" opponent "<<now->opponent<<" heuristic "<<now->heuristic<<"\n";
			ofile<<"eval "<<now->eval<<" word "<<now->word<<" skor "<<now->score<<" depth "<<now->depth<<" end "<<now->endEval<<" child "<<now->totalChild<<" heuristic "<<now->heuristic<<"\n";
			now = now->sibling;
		}
		ofile<<"\n\n\n";
		ofile.close();
	}

	void setTotalNode(int n)
	{
		totalNode = n;
	}


	int getTotalNode()
	{
		return totalNode;
	}


	~Negascout()
	{

	}

};