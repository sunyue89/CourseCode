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
		}
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
