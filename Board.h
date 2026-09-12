#pragma once


class Board
{
	int board[7][7] ;
	int turn = 0;

public:

	Board()
	{
		for(int x = 0; x < 7; x++){
			for(int y = 0; y < 7; y++){
				board[x][y] = 0;
			}
		}
	}

	int GetBoardState(int x, int y)
	{
		return board[x][y];
	}

	void SetBoardState(int x, int y)
	{
		board[x][y] = turn;
	}


};