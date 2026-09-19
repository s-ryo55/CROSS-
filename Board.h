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

	// リーチ / リバース管理
	bool reach_declared = false;
	int reach_player = 0;                // リーチを宣言したプレイヤー (1..3)
	int reverse_available_for = 0;       // リバース権限があるプレイヤー (1..3)
	int reverse_remaining = 5;           // 残りリバース回数（合計で5回まで）

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
		reach_declared = false;
		reach_player = 0;
		reverse_available_for = 0;
		reverse_remaining = 5;
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

	// 勝利判定（指定プレイヤーが4つ並んでいるか）
	bool CheckWinForPlayer(int player)
	{
		// 水平
		for (int y = 0; y < 7; y++) {
			int cnt = 0;
			for (int x = 0; x < 7; x++) {
				if (board[x][y] == player) {
					cnt++;
					if (cnt >= 4) return true;
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
					if (cnt >= 4) return true;
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
						if (cnt >= 4) return true;
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
						if (cnt >= 4) return true;
					} else {
						cnt = 0;
					}
					x--; y++;
				}
			}
		}

		return false;
	}

	// 「リーチ」判定: player が1手で勝てる可能性（4連になる空きが1つ含まれる長さ4の区間を持つか）
	bool CheckReach(int player)
	{
		// 横、縦、斜めの長さ4の区間をスライドしてチェック
		// 水平区間
		for (int y = 0; y < 7; y++) {
			for (int sx = 0; sx <= 7 - 4; sx++) {
				int cntPlayer = 0, cntEmpty = 0;
				for (int k = 0; k < 4; k++) {
					int v = board[sx + k][y];
					if (v == player) cntPlayer++;
					else if (v == STATE_EMPTY) cntEmpty++;
				}
				if (cntPlayer == 3 && cntEmpty == 1) return true;
			}
		}
		// 垂直区間
		for (int x = 0; x < 7; x++) {
			for (int sy = 0; sy <= 7 - 4; sy++) {
				int cntPlayer = 0, cntEmpty = 0;
				for (int k = 0; k < 4; k++) {
					int v = board[x][sy + k];
					if (v == player) cntPlayer++;
					else if (v == STATE_EMPTY) cntEmpty++;
				}
				if (cntPlayer == 3 && cntEmpty == 1) return true;
			}
		}
		// 斜め右下区間
		for (int sx = 0; sx <= 7 - 4; sx++) {
			for (int sy = 0; sy <= 7 - 4; sy++) {
				for (int offset = 0; offset <= 0; offset++) {
					int cntPlayer = 0, cntEmpty = 0;
					for (int k = 0; k < 4; k++) {
						int v = board[sx + k][sy + k];
						if (v == player) cntPlayer++;
						else if (v == STATE_EMPTY) cntEmpty++;
					}
					if (cntPlayer == 3 && cntEmpty == 1) return true;
				}
			}
		}
		// 斜め左下区間
		for (int sx = 3; sx < 7; sx++) {
			for (int sy = 0; sy <= 7 - 4; sy++) {
				int cntPlayer = 0, cntEmpty = 0;
				for (int k = 0; k < 4; k++) {
					int v = board[sx - k][sy + k];
					if (v == player) cntPlayer++;
					else if (v == STATE_EMPTY) cntEmpty++;
				}
				if (cntPlayer == 3 && cntEmpty == 1) return true;
			}
		}
		return false;
	}

	// GetBoardStateAroundSelect は勝利判定（直前に置かれたプレイヤーの勝利）を返す既存メソッドとして維持
	int GetBoardStateAroundSelect()
	{
		int player = turn_count;
		if (CheckWinForPlayer(player)) return player;
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

	// --- リーチ / リバース関連の操作インターフェース ---
	// リーチ宣言（player がリーチになったときに呼ぶ）
	void DeclareReach(int player)
	{
		reach_declared = true;
		reach_player = player;
		// リーチから2番目の人にリバース権限を付与
		reverse_available_for = ((player + 1) % 3) + 1; // 1..3 のローテーション
	}

	bool IsReachDeclared() const { return reach_declared; }
	int GetReachPlayer() const { return reach_player; }
	int GetReverseAvailableFor() const { return reverse_available_for; }
	int GetReverseRemaining() const { return reverse_remaining; }

	// 指定プレイヤーが現在リバースを使用できるか
	bool CanUseReverseForPlayer(int player) const
	{
		return reach_declared && reverse_remaining > 0 && reverse_available_for == player;
	}

	// リバースを試行する。条件を満たさない場合は false を返す。
	// リバースによって即時勝利（flip後に actor が4連）が発生する場合は実行しない（false）。
	// 成功したら true を返し、reverse_remaining を減らし、リーチは解除する。
	bool UseReverse(int x, int y, int actorPlayer)
	{
		if (x < 0 || x >= 7 || y < 0 || y >= 7) return false;
		if (!CanUseReverseForPlayer(actorPlayer)) return false;
		int current = board[x][y];
		if (current == STATE_EMPTY) return false;          // 空セルは対象外
		if (current == actorPlayer) return false;         // 自分の石をひっくり返す意味なし
		// シミュレーションして即勝利を防ぐ
		int backup = board[x][y];
		board[x][y] = actorPlayer;
		bool wouldWin = CheckWinForPlayer(actorPlayer);
		if (wouldWin) {
			// 元に戻して失敗
			board[x][y] = backup;
			return false;
		}
		// 実行
		// board[x][y] = actorPlayer; // already set
		reverse_remaining--;
		// リーチは解除（リバース行使で局面が変わるため）
		reach_declared = false;
		reach_player = 0;
		reverse_available_for = 0;
		return true;
	}
};