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
	// 状態定義
	// 0 = empty, 1..3 = 所有プレイヤー、4..6 = 各ターンの選択表示
	static const int STATE_EMPTY = 0;
	static const int STATE_PLAYER1 = 1;
	static const int STATE_PLAYER2 = 2;
	static const int STATE_PLAYER3 = 3;
	static const int STATE_SELECT_BASE = 3; // select = STATE_SELECT_BASE + turn

	Board()
	{
		Board_reset();
	}
	void Board_reset()
	{
		for(int x = 0; x < 7; x++){
			for(int y = 0; y < 7; y++){
				board[x][y] = STATE_EMPTY;
			}
		}
		turn = 1;
		turn_count = 1;
		select = false;
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

	bool GetSelect()
	{
		return select;
	}


	void SetSelect(bool value) {
		select = value;
	}
	
	// 選択解除: 選択用の状態(STATE_SELECT_BASE+1 .. STATE_SELECT_BASE+3)をまとめて消す
	void ResetSelect() {
		for (int x = 0; x < 7; x++) {
			for (int y = 0; y < 7; y++) {
				if (board[x][y] >= STATE_SELECT_BASE + 1 && board[x][y] <= STATE_SELECT_BASE + 3)
					board[x][y] = STATE_EMPTY;
			}
		}
	}

	// 現在のターンに合わせて選択状態を設定する（例えば turn=1 -> state=4）
	void SetSelectAt(int x, int y)
	{
		if (x < 0 || x >= 7 || y < 0 || y >= 7) return;
		if (board[x][y] == STATE_EMPTY) {
			board[x][y] = STATE_SELECT_BASE + turn;
			select_x = x;
			select_y = y;
		}
	}

	// 既存の勝利判定（変更なし、返り値は勝者ID = turn_count を返す実装を想定）
	int GetBoardStateAroundSelect()
	{
		int player = turn_count;

		// 水平
		for (int y = 0; y < 7; y++) {
			int cnt = 0;
			for (int x = 0; x < 7; x++) {
				if (board[x][y] == player) {
					cnt++;
					if (cnt >= 4) return player;
				} else {
					cnt = 0;
				}
			}
		}

		// 垂直
		for (int x = 0; x < 7; x++) {
			int cnt = 0;
			for (int y = 0; y < 7; y++) {
				if (board[x][y] == player) {
					cnt++;
					if (cnt >= 4) return player;
				} else {
					cnt = 0;
				}
			}
		}

		// 斜め右下
		for (int sx = 0; sx < 7; sx++) {
			for (int sy = 0; sy < 7; sy++) {
				int cnt = 0;
				int x = sx;
				int y = sy;
				while (x < 7 && y < 7) {
					if (board[x][y] == player) {
						cnt++;
						if (cnt >= 4) return player;
					} else {
						cnt = 0;
					}
					x++; y++;
				}
			}
		}

		// 斜め左下
		for (int sx = 0; sx < 7; sx++) {
			for (int sy = 0; sy < 7; sy++) {
				int cnt = 0;
				int x = sx;
				int y = sy;
				while (x >= 0 && y < 7) {
					if (board[x][y] == player) {
						cnt++;
						if (cnt >= 4) return player;
					} else {
						cnt = 0;
					}
					x--; y++;
				}
			}
		}

		return 0;
	}



	int GetBoardState(int x, int y)
	{
		return board[x][y];
	}

	bool SetBoardState(int x, int y)
	{
		if(board[x][y] == STATE_EMPTY)
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