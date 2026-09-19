#pragma once
#include <vector>


class Board
{
	int board[7][7] ;
	int turn = 1;
	int turn_count = 0;

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

	//@select_x‚Æselect_y‚ğ’†S‚É‚V‚˜‚V‚ğæ“¾‚·‚éŠÖ”
	int GetBoardStateAroundSelect()
	{
		int board_x = select_x - 3;
		int board_x_max = select_x + 3;
		int board_y = select_y - 3;
		int board_y_max = select_y + 3;

		if (board_x < 0) board_x = 0;
		if (board_y < 0) board_y = 0;
		if (board_x_max > 6) board_x_max = 6;
		if (board_y_max > 6) board_y_max = 6;

		int x_judge[7];
		int y_judge[7];

		int line_count_y = 0;
		int line_count_x = 0;

		for (int x = board_x; x <= board_x_max; x++)
		{
			for (int y = board_y; y <= board_y_max; y++)
			{
				if(board[x][y] == turn_count)
				{
					line_count_y++;
					if (line_count_y > 3) {
						y_judge[y] = 1;
					}
				}

				for(int x = board_x; x <= board_x_max; x++)
				{
					if(board[x][y] == turn_count)
					{
						line_count_x++;
						if (line_count_x > 3) {
							x_judge[x] = 1;
						}
					}
				}
				line_count_x = 0;
			}
			line_count_y = 0;
		}

		for (int x = board_x; x <= board_x_max; x++){
			if (x_judge[x] == 1) {
				int num = 0;
				for (int y = board_y; y <= board_y_max; y++){
					if (board[x][y] == turn_count) {
						num++;
						if (num > 3) {
							return true;
						}
					}else {
						num = 0;
					}
				}
			}
		}

	
		return  0;


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