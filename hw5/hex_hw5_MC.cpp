/*A final version of Hex game for Hw5 where the program deploys monte carlo simulation and alpha-beta prunning to move*/

/*blue is represented by X and play east to west, red is represented by O and play from north to south, blue(or X) plays first*/

/* Functions included
*  1. Draw board using ASCII symbols with size input by player
*  2. Ask the player if he/she would like to play blue(X)/first or red(O)/second
*  3. Input a move from player and print on board if legal, else keep asking for inputs
*  4. Deploys monte carlo simulation by randomly fill up a full board and select the best move based on probability of winning
			-use recursion stacks (implemented as an unordered_set) to fill up boards and backtracking to recover
			-use DFS on a full board to determine winner
			-use alpha-beta prunning to avoid unneccsary simulations once node's probability of winning is less than max
*  5. Run Depth First Search on both player and program's new input to decide a winner, and exit the game if one exists
*  6. Program does struggle a bit in timing when board is larger than 9x9
*
*  Potential improvements
*    - Union find/Modified Djkstra algo to determine winner
*		 - Multi thread deployment for speeding up
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
#include <unordered_set>
#include <random>

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
		for(int k=0; k<i*i; ++k)
			idx_.insert(k);
		cout<<i<<"x"<<i<<" hex board created"<<endl;	
		if(player_lb_ == 0)
			cout<<"player chose to play blue(X)/first by going east to west"<<endl;
		else
			cout<<"player chose to play red(O)/second by going north to south"<<endl;
		Print();
	}
	
	Win Move(){
		Win w = Win::None;
		if(player_lb_==0){
			PlayerMove();
			Print();
			w = Winner(0);
			if(w!=Win::None){
				return w;
			}
			ProgMove();
			Print();
			w = Winner(1);
			if(w!=Win::None){
				return w;
			}
		}else{//swithcing calling order due to hand order switch
			ProgMove();
			Print();
			w = Winner(1);
			if(w!=Win::None){
				return w;
			}
			PlayerMove();
			Print();
			w = Winner(0);
			if(w!=Win::None){
				return w;
			}
		}
		return w;
	}
	void Print(){
		string s;
		for(int i=0;i<board_.size();++i){
			char c = 92;
			s+=c;
			if(board_.size()!=i+1)
				s+=" / ";
		} 

		for(int i=0;i<board_.size();++i){

			for(int k=0; k<2*i; ++k)
				cout<<" ";

			for(int j=0;j<board_[0].size();++j){
				PrintStat(i,j);
				if(board_[0].size() != j+1) 
					cout<<" - ";
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

private:
	vector<vector<Stat>> board_;
	int player_lb_;
	//deploy unordered set to track empty nodes
	//i,j
	//i*board_.size()+j
	// [0, board_.size()^2 - 1]
	unordered_set<int> idx_;
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
		idx_.erase((i-1)*board_.size()+(j-1));
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

		//deep copy to reserve class elements
		MC_Next_Move(board_,idx_,r);
		int i=r[0]+1;
		int j=r[1]+1;
		idx_.erase((i-1)*board_.size()+(j-1));

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

	Win Winner(int ite){
		if(ite==0){
			if(player_lm_.size()>0){
				bool l=false, h = false;
				vector<vector<bool>> vis(board_.size(), vector<bool>(board_[0].size(),false));
				if(DFS(player_lm_[0], player_lm_[1],l, h, player_lb_, vis)){
					return Win::Player;
				}
			}
		}else{
			if(prog_lm_.size()>0){
				bool l=false, h = false;
				vector<vector<bool>> vis(board_.size(), vector<bool>(board_[0].size(),false));
				if(DFS(prog_lm_[0], prog_lm_[1],l, h, 1-player_lb_, vis)){
					return Win::Prog;
				}	
			}
		}
		return Win::None;
	}


	void MC_Next_Move(vector<vector<Stat>> board, unordered_set<int> idx, vector<int>& m){
		cout<<"thinking..."<<endl;
		double p_l = 0;
		for(auto x:idx){
			bool end = true;
			int i = x/board.size();
			int j = x%board.size();
			vector<int> mn = {i,j};
			int w = 0;
			int T;
			if(idx.size()>=board.size()*board.size()/2)
				T = 1200;
			else
				T = 1000;
			//node exploration via random simulation
			for(int k=0;k<T;++k){
				if(MC_Rec(board,idx,mn,1))
					w++;
				board[i][j] = Stat::Empty;
				//idea of pruning
				//if winning all the rest still has a probablity less than the highest winning probility, stop
				if(static_cast<double> (T-1-k+w)/T < p_l){
					end = false;
					break;
				}
			}
			double p = static_cast<double> (w)/T;
			cout<<"potential next move and the probability of winning over " <<T<<" simulations"<<endl;
			if(end)
				cout<<i+1<<" "<<j+1<<" "<<p<<endl;
			else
				cout<<i+1<<" "<<j+1<<" >="<<p<<" <"<<p_l<<endl;
			if (p>p_l){
				p_l=p;
				m = mn;
			}
		}
	}

	bool MC_Rec(vector<vector<Stat>>& board, unordered_set<int> idx, vector<int> m, int ite){

		if(ite){
			if(player_lb_==0)
				board[m[0]][m[1]] = Stat::Red;
			else
				board[m[0]][m[1]]  = Stat::Blue;
		}else{
			if(player_lb_==0)
				board[m[0]][m[1]]  = Stat::Blue;
			else
				board[m[0]][m[1]] = Stat::Red;
		}
		//debug
		//Print(board);
		if(idx.size()>1){
			idx.erase(board.size()*m[0]+m[1]);
		}else{
			return check_prog_win(board);
		}
		int k = randomGen(0,idx.size()-1);
		//debug
		//cout<<k<<" "<<idx.size()-1<<endl;
		auto itr = idx.begin();
		advance(itr,k);
		int id = *itr;
		int i = id/board.size();
		int j = id%board.size();
		//debug
		//cout<<id<<" "<<i<<" "<<j<<endl;
		m = {i,j};
		//tree traversal via randomness
		bool r = MC_Rec(board,idx,m,1-ite);
		//debug
	 	//cout<<id<<" "<<i<<" "<<j<<endl;
		//backtracking via stack recovering
		idx.insert(id);
		board[i][j] = Stat::Empty;	
		return r;

	}

	bool check_prog_win(vector<vector<Stat>>& board){

		vector<vector<bool>> vis(board.size(), vector<bool>(board[0].size(),false));

		if(player_lb_==0){//program from north to south
			for(int j=0;j<board.size();j++){
				bool l=false, h = false;
				if(!vis[0][j] && board[0][j] == Stat::Red)
					if(DFS(0,j,l,h,1-player_lb_,vis,board))
						return true;
			}
			return false;
		}else{//program from left to right
			for(int i=0;i<board[0].size();i++){
				bool l=false, h = false;
				if(!vis[i][0] && board[i][0] == Stat::Blue)
					if(DFS(i,0,l,h,1-player_lb_,vis,board))
						return true;
			}
			return false;
		}
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
	//function overloading
	bool check_connect(int i, int j, int k, int m, vector<vector<bool>>& vis, vector<vector<Stat>>& board){
		if(i+k <0 || i+k >= board.size())
			return false;
		if(j+m <0 || j+m >= board[0].size())
			return false;
		if(vis[i+k][j+m])
			return false;
		if(board[i+k][j+m] != board[i][j])
			return false;
		return true;
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
	//function overloading
	bool DFS(int i, int j, bool& l, bool& h, int t, vector<vector<bool>>& vis, vector<vector<Stat>>& board){

		vector<vector<int>> ngb = {{-1,0},{1,0},{0,-1},{1,-1},{-1,1},{0,1}};

		if(!vis[i][j]){
			vis[i][j] = true;

			if(t==0){
				if(j==0)
					l = true;
				if(j==board[0].size()-1)
					h = true;
			}else{
				if(i==0)
					l = true;
				if(i==board.size()-1)
					h = true;
			}
			//debugging of DFS
			//cout<<i<<j<<l<<h<<endl;

			if(l&&h)
				return true;

			for(auto p:ngb){
				int k = p[0];
				int m = p[1];
				if( check_connect(i,j,k,m,vis,board)){
					if (DFS(i+k,j+m,l,h,t,vis,board))
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
	//function overloading
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

	int randomGen(int lowbound, int highbound)
	{
			std::random_device rd;
			std::default_random_engine eng(rd());
			std::uniform_int_distribution<int> distr(lowbound, highbound);
			return distr(eng);
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
		w = h.Move();
	}

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
