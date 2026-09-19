#pragma once
#include <vector>


class Board
{
	int board[7][7] ;
	int turn = 1;
	int turn_count = 1;

	int select_x = 0;
	int select_y = 0;

	bool select = false;

	

public:

	Board()
	{
		for(int x = 0; x < 7; x++){
			for(int y = 0; y < 7; y++){
				board[x][y] = 0;
			}
		}
	}

	void TurnTurn()
	{
		turn_count = turn;
		turn++;
		if(turn > 3)
		{
			turn = 1;
		}
	}

	int GetTurn()
	{
		return turn;
	}

	int GetTurn_count() {
		return turn_count;
	}

	int GetSelect()
	{
		return select;
	}


	void SetSelect(bool value) {
		select = value;
	}
	
	void ResetSelect() {
		for (int x = 0; x < 7; x++) {
			for (int y = 0; y < 7; y++) {
				if(board[x][y] == 4)
				board[x][y] = 0;
			}
		}
	}

	
	int GetBoardStateAroundSelect()
	{
		for (int x = 0; x < 7; x++) {
			int num_y = 0;
			for (int y = 0; y < 7; y++) {
				if (board[x][y] == turn_count) {
					num_y++;
					if (num_y > 3) {
						return 1;
					}
				}
				else {
					num_y = 0;
				}
				int num_x = 0;
				for (int x = 0; x < 7; x++) {
					if (board[x][y] == turn_count) {
						num_x++;
						if (num_x > 3) {
							return 1;
						}
					}
					else {
						num_x=0;
					}
				}
			}
		}

		for (int x = 0; x < 4; x++) {
			int 
		}



		return 0;


	}



	int GetBoardState(int x, int y)
	{
		return board[x][y];
	}

	bool SetBoardState(int x, int y)
	{
		if(board[x][y] == 0)
		{
			board[x][y] = turn;
			select_x = x;
			select_y = y;
			return true;
		}
		return false;
	}

	void SetBoardState(int x, int y, int state)
	{
		board[x][y] = state;
	}


};