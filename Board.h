#pragma once
#include <vector>


class Board
{
	int board[7][7] ;
	int turn = 1;
	int turn_count = 1;

	bool reverse_now = 0;
	int reverse_count = 0;

	int select_x = 0;
	int select_y = 0;

	bool select = false;

	

public:


	Board()
	{
		Board_reset();
	}

	void Board_reset() {
		for (int x = 0; x < 7; x++) {
			for (int y = 0; y < 7; y++) {
				board[x][y] = 0;
			}
		}
		turn = 1;
		turn_count = 1;
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
		int player = turn_count;
		reverse_now = false;

		for (int board_num = 1; board_num < 3; board_num++) {
			// 水平方向
			for (int y = 0; y < 7; y++) {
				int cnt = 0;
				int board_cnt;
				for (int x = 0; x < 7; x++) {
					if (board[x][y] == player) {
						cnt++;
						if (cnt >= 4) return player;
					}
					else {
						if (cnt == 3 && board[x][y] == 0)reverse_now = true;
						cnt = 0;
					}
					if (board[x][y] == board_num) {

					}

				}
			}

			// 垂直方向
			for (int x = 0; x < 7; x++) {
				int cnt = 0;
				for (int y = 0; y < 7; y++) {
					if (board[x][y] == player) {
						cnt++;
						if (cnt >= 4) return player;
					}
					else {
						if (cnt == 3 && board[x][y] == 0)reverse_now = true;
						cnt = 0;
					}
				}
			}

			// 斜め（右下方向）
			for (int sx = 0; sx < 7; sx++) {
				for (int sy = 0; sy < 7; sy++) {
					int cnt = 0;
					int x = sx;
					int y = sy;
					while (x < 7 && y < 7) {
						if (board[x][y] == player) {
							cnt++;
							if (cnt >= 4) return player;
						}
						else {
							if (cnt == 3 && board[x][y] == 0)reverse_now = true;
							cnt = 0;
						}
						x++; y++;
					}
				}
			}

			// 斜め（左下方向）
			for (int sx = 0; sx < 7; sx++) {
				for (int sy = 0; sy < 7; sy++) {
					int cnt = 0;
					int x = sx;
					int y = sy;
					while (x >= 0 && y < 7) {
						if (board[x][y] == player) {
							cnt++;
							if (cnt >= 4) return player;
						}
						else {
							if (cnt == 3 && board[x][y] == 0)reverse_now = true;
							cnt = 0;
						}
						x--; y++;
					}
				}
			}
		}

		if (reverse_now) {
			reverse_count++;
		}
		else {
			reverse_count = 0;
		}

		return 0;
	}



	int GetBoardState(int x, int y)
	{
		return board[x][y];
	}

	bool SetBoardState(int x, int y)
	{
		if (reverse_count > 1) {
			board[x][y] = turn;
			select_x = x;
			select_y = y;
			return true;


		}
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