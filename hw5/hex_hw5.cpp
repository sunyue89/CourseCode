/*An initial version of Hex game for Hw4 where the program uses random strategy to move*/

/*blue is represented by X and play east to west, red is represented by O and play from north to south, blue(or X) plays first*/

/* Functions included
*  1. Draw board using ASCII symbols with size input by player
*  2. Ask the player if he/she would like to play blue(X)/first or red(O)/second
*  3. Input a move from player and print on board if legal, else keep asking for inputs
*  4. Randomly compute a legal move from program and print
*  5. Run Depth First Search on both player and program's new input to decide a winner, and exit the game if one exists
*
*  Note: Even if the first hand wins, the second hand will still place a move to end the game
*
*/


#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <vector>
#include <cmath>
#include <string>
#include <queue>

using namespace std;

enum class Stat:short{
	Empty,
	Blue,
	Red
};

enum class Win:short{
	None,
	Player,
	Prog
};

class Hex{

public:
	Hex(int i, int j){
		assert(i>0);
		player_lb_ = j;
		board_.resize(i,vector<Stat>(i));	
		cout<<i<<"x"<<i<<" hex board created"<<endl;	
		if(player_lb_ == 0)
			cout<<"player chose to play blue(X)/first by going east to west"<<endl;
		else
			cout<<"player chose to play red(O)/second by going north to south"<<endl;
		Print();
	}
	
	void Move(){
		if(player_lb_==0){
			PlayerMove();
			ProgMove();
			Print();
		}else{//swithcing calling order due to hand order switch
			ProgMove();
			Print();
			PlayerMove();
		}
	}
	void Print(){
		string s;
		for(int i=0;i<board_.size();++i){
			char c = 92;
			s+=c;
			if(board_.size()!=i+1)
				s+='/';
		} 

		for(int i=0;i<board_.size();++i){

			for(int k=0; k<2*i; ++k)
				cout<<" ";

			for(int j=0;j<board_[0].size();++j){
				PrintStat(i,j);
				if(board_[0].size() != j+1) 
					cout<<"-";
			}
			cout<<endl;

			if(board_.size()!=i+1){
				for(int k=0; k<2*i+1; ++k)
					cout<<" ";
				for(auto c:s)
					cout<<c;
				cout<<endl;
			}
		}
	}

	void Print(vector<vector<Stat>>& board){
		string s;
		for(int i=0;i<board.size();++i){
			char c = 92;
			s+=c;
			if(board.size()!=i+1)
				s+='/';
		} 

		for(int i=0;i<board.size();++i){

			for(int k=0; k<2*i; ++k)
				cout<<" ";

			for(int j=0;j<board[0].size();++j){
				PrintStat(i,j,board);
				if(board[0].size() != j+1) 
					cout<<"-";
			}
			cout<<endl;

			if(board.size()!=i+1){
				for(int k=0; k<2*i+1; ++k)
					cout<<" ";
				for(auto c:s)
					cout<<c;
				cout<<endl;
			}
		}
	}

	Win Winner(){
		if(player_lb_==0){
			if(player_lm_.size()>0){
				bool l=false, h = false;
				vector<vector<bool>> vis(board_.size(), vector<bool>(board_[0].size(),false));
				if(DFS(player_lm_[0], player_lm_[1],l, h, player_lb_, vis)){
					return Win::Player;
				}
			}
			if(prog_lm_.size()>0){
				bool l=false, h = false;
				vector<vector<bool>> vis(board_.size(), vector<bool>(board_[0].size(),false));
				if(DFS(prog_lm_[0], prog_lm_[1],l, h, 1-player_lb_, vis)){
					return Win::Prog;
				}	
			}
		}else{//switch calling order due to hand order switch
			if(prog_lm_.size()>0){
				bool l=false, h = false;
				vector<vector<bool>> vis(board_.size(), vector<bool>(board_[0].size(),false));
				if(DFS(prog_lm_[0], prog_lm_[1],l, h, 1-player_lb_, vis)){
					return Win::Prog;
				}	
			}
			if(player_lm_.size()>0){
				bool l=false, h = false;
				vector<vector<bool>> vis(board_.size(), vector<bool>(board_[0].size(),false));
				if(DFS(player_lm_[0], player_lm_[1],l, h, player_lb_, vis)){
					return Win::Player;
				}
			}
		}

		return Win::None;
	}

private:
	vector<vector<Stat>> board_;
	int player_lb_;
	vector<int> player_lm_;
	vector<int> prog_lm_;	

	void PlayerMove(){
		bool move = true;
		int i,j;

		while(move){
			cout<<"place at a hex, by entering the row and column with starting index 1"<<endl;
			cout<<"input two integers sperated by white space and press enter"<<endl;
			vector<int> arr;
			string input ="";
			getline(cin, input);
			istringstream is(input);
			int num;
			while(is>>num){
				arr.push_back(num);
			}
			if(arr.size()!=2){
				cout<<"invalid input, try again"<<endl;
				continue;
			}
			i = arr[0];
			j = arr[1];

			if(i>=1&&i<=board_.size()&&j>=1&&j<=board_[0].size()&&board_[i-1][j-1]==Stat::Empty){

				if(player_lb_==0)
					board_[i-1][j-1] = Stat::Blue;
				else
					board_[i-1][j-1] = Stat::Red;

				move = false;
				cout<<"player placed at "<<i<<" "<<j<<endl;
			}else if(i<1||i>board_.size()||j<1||j>board_[0].size()){
				cout<<"out of hex range, try again"<<endl;
			}else{
				cout<<"hex occupied, try again"<<endl;
			}
		}
		if(player_lm_.size()<=0){
			player_lm_.push_back(i-1);
			player_lm_.push_back(j-1);
		}else{
			player_lm_[0] = (i-1);
			player_lm_[1] = (j-1);

		}
		return;
	}
	
	void ProgMove(){

		vector<int> r(2);
		//deep copy, is there a better way?
		vector<vector<Stat>> board = board_;
		int t = alpha_beta_pruned(board,{0,0},r,true,2,INT_MIN,INT_MAX);
		int i=r[0]+1;
		int j=r[1]+1;

		/*
		bool move = true;
		int i,j;
		while(move){
			i=rand()%board_.size()+1;
			j=rand()%board_[0].size()+1;
			if(i>=1&&i<=board_.size()&&j>=1&&j<=board_[0].size()&&board_[i-1][j-1]==Stat::Empty){
	
				if(player_lb_==0)
					board_[i-1][j-1] = Stat::Red;
				else
					board_[i-1][j-1] = Stat::Blue;
				move = false;
				cout<<"program placed at "<<i<<" "<<j<<endl;
			}
		}*/
		if(player_lb_==0)
			board_[i-1][j-1] = Stat::Red;
		else
			board_[i-1][j-1] = Stat::Blue;
		if(prog_lm_.size()<=0){
			prog_lm_.push_back(i-1);
			prog_lm_.push_back(j-1);
		}else{
			prog_lm_[0] = (i-1);
			prog_lm_[1] = (j-1);
		}
		return;
	}

	bool check_connect(int i, int j, int k, int m, vector<vector<bool>>& vis){
		if(i+k <0 || i+k >= board_.size())
			return false;
		if(j+m <0 || j+m >= board_[0].size())
			return false;
		if(vis[i+k][j+m])
			return false;
		if(board_[i+k][j+m] != board_[i][j])
			return false;
		return true;
	}

	int neighbor_score(int i, int j, int k, int m, vector<vector<Stat>>& board){
		if(i+k <0 || i+k >= board_.size())
			return INT_MAX;
		if(j+m <0 || j+m >= board_[0].size())
			return INT_MAX;
		if(board[i+k][j+m]!=Stat::Empty && board[i+k][j+m]!=board[i][j])
			return INT_MAX;
		if(board[i+k][j+m]==Stat::Empty) 
			return 1;
		if(board[i+k][j+m]==board[i][j])
			return 0;
		return INT_MAX;
	}

	int alpha_beta_pruned(vector<vector<Stat>>& board, vector<int> move, vector<int>& dsrd_move, bool maxProg, int depth, int alpha, int beta){ 
		
		vector<vector<int>> moves;

		if (depth ==0 || scan(board,moves))
			return get_Heuris_Score(board,move);

		//debug
/*
		for(int i=0;i<moves.size();++i)
			for(int j=0;j<moves[0].size();++j)
				cout<<moves[i][j]<<" ";
		cout<<endl;
*/

		int bestValue;

		if(maxProg){

			bestValue = INT_MIN;

			for(auto m:moves){
				if(player_lb_==0)
					board[m[0]][m[1]] = Stat::Red;
				else
					board[m[0]][m[1]] = Stat::Blue;
				//cout<<"check1"<<endl;
				Print(board);
				int t = alpha_beta_pruned(board,m,dsrd_move,false,depth-1,alpha,beta);
				board[m[0]][m[1]] = Stat::Empty;

				if(t>bestValue)
					dsrd_move = m;

				cout<<t<<" "<<bestValue<<" "<<dsrd_move[0]<<" "<<dsrd_move[1]<<endl;

				bestValue = max(bestValue, t);
				alpha = max(alpha, bestValue);
				if(beta <= alpha)
					break;
			}

		}else{

			bestValue = INT_MAX;

			for(auto m:moves){
				if(player_lb_==0)
					board[m[0]][m[1]] = Stat::Blue;
				else
					board[m[0]][m[1]] = Stat::Red;
				Print(board);

				int t = alpha_beta_pruned(board,m,dsrd_move,true,depth-1,alpha,beta);
				board[m[0]][m[1]] = Stat::Empty;
				//if(t<bestValue)
					//dsrd_move = m;

				cout<<t<<" "<<bestValue<<endl;


				bestValue = min(bestValue, t);
				beta = min(beta, bestValue);
				if(beta <= alpha)
					break;
			}

		}
		return bestValue;
		
	}

	int get_Heuris_Score(vector<vector<Stat>>& board, vector<int>& move){
		int playerScore = getScoreForPath(board, move, player_lb_);
		cout<<playerScore<<" ";
		int progScore = getScoreForPath(board,move, 1-player_lb_);
		cout<<progScore<<endl;
		return playerScore-progScore;
	}

	int getScoreForPath(vector<vector<Stat>>& board, vector<int>& move, int t){

		typedef pair<int, pair<int,int>> pi;
		//apply djstra to return shortest vertexes to reach goals
		//0 blue, 1 red
		priority_queue<pi, vector<pi>, greater<pi>> pq;
		vector<vector<int>> Cost(board.size(), vector<int>(board[0].size(),INT_MAX));
		vector<vector<int>> ngb = {{-1,0},{1,0},{0,-1},{1,-1},{-1,1},{0,1}};

		Cost[move[0]][move[1]] = 0;
		pq.push({0,{move[0],move[1]}});
		bool l=false, h=false;

		while(!pq.empty()){
			pi top = pq.top();
			pq.pop();
			int sc = top.first;
			int i = top.second.first;
			int j = top.second.second;

			if(t==0){
				if(j==0)
					l = true;
				if(j==board_[0].size()-1)
					h = true;
			}else{
				if(i==0)
					l = true;
				if(i==board_.size()-1)
					h = true;
			}

			if(l&&h)
				return sc;

			for(auto n:ngb){
				int k = n[0];
				int m = n[1];
				int tmp = neighbor_score(i,j,k,m,board);
				if(tmp!=INT_MAX && tmp+sc<Cost[i+k][j+m]){
					Cost[i+k][j+m] = tmp+sc;
					pq.push({Cost[i+k][j+m],{i+k,j+m}});
				}
			}
		}
		return INT_MAX;
		
	}

	bool scan(vector<vector<Stat>>& board, vector<vector<int>>& moves){
		bool full = true;
		for(int i=0;i<board.size();++i){
			for(int j=0;j<board[0].size();++j){
				if(board[i][j] == Stat::Empty){
					moves.push_back({i,j});
					if(full == true)
						full = false;
				}
			}
		}
		return full;
	}

	bool DFS(int i, int j, bool& l, bool& h, int t, vector<vector<bool>>& vis){

		vector<vector<int>> ngb = {{-1,0},{1,0},{0,-1},{1,-1},{-1,1},{0,1}};

		if(!vis[i][j]){
			vis[i][j] = true;

			if(t==0){
				if(j==0)
					l = true;
				if(j==board_[0].size()-1)
					h = true;
			}else{
				if(i==0)
					l = true;
				if(i==board_.size()-1)
					h = true;
			}
			//debugging of DFS
			//cout<<i<<j<<l<<h<<endl;

			if(l&&h)
				return true;

			for(auto p:ngb){
				int k = p[0];
				int m = p[1];
				if( check_connect(i,j,k,m,vis)){
					if (DFS(i+k,j+m,l,h,t,vis))
						return true;
				}
			}
		}

		return false;
	}

	void PrintStat(int i, int j){
		switch (board_[i][j]){
			case (Stat::Empty):
				cout<<".";
				break;
			case (Stat::Blue):
				cout<<"X";
				break;
			case (Stat::Red):
				cout<<"O";
				break;
			default:
				break;
		}
		return;
	}

	void PrintStat(int i, int j, vector<vector<Stat>>& board){
		switch (board[i][j]){
			case (Stat::Empty):
				cout<<".";
				break;
			case (Stat::Blue):
				cout<<"X";
				break;
			case (Stat::Red):
				cout<<"O";
				break;
			default:
				break;
		}
		return;
	}

};

int main(){
	
	cout<<endl;
	cout<<"welcome to the hex game!"<<endl;
	cout<<"Blue hex is represented by X and play from east to west"<<endl;
	cout<<"Red hex is represented by O and play from north to south"<<endl;
	cout<<"Blue plays first"<<endl;
	cout<<endl;

  cout<<"please specify the size to create a size x size board"<<endl;
	cout<<"intput an integer number then press enter"<<endl;
	int i=0;
	string line = "";
	getline(cin,line);
	i = stoi(line);
	
	int j;
	while(true){
		cout<<"please select blue(X)/first or red(O)/second by entering character 'b' or 'r'"<<endl;
		string line = "";
		getline(cin,line);
		if(line == "B"||line == "b"){
			j=0;
			break;
		}else if(line == "R"||line == "r"){
			j=1;
			break;
		}else{
			cout<<"false input, try again"<<endl;
		}
	}

	Hex h(i,j);
	Win w = Win::None;

	while(w==Win::None){
		h.Move();
		w = h.Winner();		
	}
	//print one more time due to call orders for player second hand
	if(1==j)
		h.Print();

	if(w==Win::Player){
		cout<<"player win!"<<endl;
	}
	else if(w==Win::Prog){
		cout<<"program win!"<<endl;
	}
	else{
		cout<<"opps, something went wrong, replay the game!"<<endl;
	}
	return -1;
}
