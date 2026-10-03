#pragma once
#include <vector>
#include <utility>

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

	// 追加: そのリーチに紐づく「リーチ対象座標」集合（宣言時に算出）
	std::vector<std::pair<int,int>> reach_targets;

	// 追加: リーチを作った直近の置き位置（互換性で保持）
	int reach_target_x = -1;
	int reach_target_y = -1;

	// 追加: 直近 3 手の履歴を保持（SetBoardState 成功時に記録）
	struct MoveEntry { int player; int x; int y; };
	MoveEntry move_history[3];
	int history_index = 0;   // 次に書き込む位置（循環）
	int history_count = 0;   // 実際に記録されたエントリ数（<=3）

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
		reach_target_x = -1;
		reach_target_y = -1;
		reach_targets.clear();
		history_index = 0;
		history_count = 0;
		for (int i = 0; i < 3; ++i) move_history[i] = {0, -1, -1};
	}

	int Draw_judge()
	{
		for (int x = 0; x < 7; x++) {
			for (int y = 0; y < 7; y++) {
				if (board[x][y] == STATE_EMPTY) {
					return 0; // 空きがあるので引き分けではない
				}
			}
		}
	
		return 1; // 空きがないので引き分け
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

	// SetBoardState に履歴記録を追加
	bool SetBoardState(int x, int y)
	{
		if(board[x][y] == STATE_EMPTY)
		{
			board[x][y] = turn;
			select_x = x;
			select_y = y;
			// 履歴に記録 (記録は turn の値を使用)
			move_history[history_index] = { turn, x, y };
			history_index = (history_index + 1) % 3;
			if (history_count < 3) history_count++;
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
		// 互換性のため既存実装は select_x/select_y を使用
		DeclareReachWithTarget(player, select_x, select_y);
	}

	// 新規: 座標を指定してリーチ宣言（Scene 側のキュー活用時に使用）
	void DeclareReachWithTarget(int player, int x, int y)
	{
		reach_declared = true;
		reach_player = player;
		// リーチした「2ターン後の人」にリバース権限を付与する
		reverse_available_for = ((player + 1) % 3) + 1; // player=1->3,2->1,3->2 (2ターン後)
		// 指定ターゲットを保存（互換性）
		reach_target_x = x;
		reach_target_y = y;

		// reach_targets を計算して保存する（ルール2-b: 宣言時の reach_targets の座標群のみが後で反転対象になりうる）
		reach_targets = GetReachTargets(player);
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

	// ヘルパー: 過去の手を取得 (rel = 1: 直前、2: 2手前, ...)
	std::pair<int,int> GetMoveRelative(int rel) const
	{
		if (rel <= 0 || rel > history_count) return { -1, -1 };
		int idx = history_index - rel;
		while (idx < 0) idx += 3;
		const MoveEntry& m = move_history[idx % 3];
		return { m.x, m.y };
	}

	// ヘルパー: 指定座標が現在 reach_targets に含まれるか
	bool IsInReachTargets(int x, int y) const
	{
		// まず宣言時に保存した reach_target（宣言者が直前に置いたマス）を許可
		if (x == reach_target_x && y == reach_target_y) return true;

		for (const auto &p : reach_targets) {
			if (p.first == x && p.second == y) return true;
		}
		return false;
	}

	// リバースを試行する。条件を満たさない場合は false を返す。
	// ルール2-b: "宣言時に算出された reach_targets の座標のうち、現在その座標に置かれている石のみ反転可能"
	// またリバースは権限保持者のみが行える（CanUseReverseForPlayer を満たすこと）。
	// リバースによって即時勝利（flip後に actor が4連）が発生する場合は実行しない（false）。
	// 成功したら true を返し、reverse_remaining を減らし、リーチは解除する。
	bool UseReverse(int x, int y, int actorPlayer)
	{
		if (x < 0 || x >= 7 || y < 0 || y >= 7) return false;

		// 対象は宣言時に算出された reach_targets のみ（かつ宣言者直置きマスも許可）
		if (!IsInReachTargets(x, y)) return false;

		if (!CanUseReverseForPlayer(actorPlayer)) return false;

		int current = board[x][y];
		// 現在その座標に石が置かれていることが必須（空セルは不可)
		if (current == STATE_EMPTY) return false;
		// actor 自身の石は反転不可
		if (current == actorPlayer) return false;

		// シミュレーションして即勝利を防ぐ
		int backup = board[x][y];
		board[x][y] = actorPlayer;
		bool wouldWin = CheckWinForPlayer(actorPlayer);
		if (wouldWin) {
			// 元に戻して失敗
			board[x][y] = backup;
			return false;
		}

		// 実行（既に上書き済み)
		reverse_remaining--;
		// リーチは解除（リバース行使で局面が変わるため)
		reach_declared = false;
		reach_player = 0;
		reverse_available_for = 0;
		// reach_targets をクリア
		reach_targets.clear();
		reach_target_x = -1;
		reach_target_y = -1;
		return true;
	}

	// 新規: Scene から「内部の CanUseReverse 判定をバイパスして強制的に UseReverse を試みる」
	// ただし強制でも対象マスは reach_targets に限定する。
	bool UseReverseForce(int x, int y, int actorPlayer)
	{
		if (x < 0 || x >= 7 || y < 0 || y >= 7) return false;
		if (reverse_remaining <= 0) return false;
		// 強制でも対象は reach_targets のみ（かつ宣言者直置きマスも許可）
		if (!IsInReachTargets(x, y)) return false;

		int current = board[x][y];
		if (current == STATE_EMPTY) return false;
		if (current == actorPlayer) return false;
		// 仮に置き換えてみて勝利を招く場合は拒否
		int backup = board[x][y];
		board[x][y] = actorPlayer;
		bool wouldWin = CheckWinForPlayer(actorPlayer);
		if (wouldWin) {
			board[x][y] = backup;
			return false;
		}
		// 実行
		reverse_remaining--;
		// リーチ関連フラグはクリア（UseReverse と同様）
		reach_declared = false;
		reach_player = 0;
		reverse_available_for = 0;
		reach_targets.clear();
		reach_target_x = -1;
		reach_target_y = -1;
		return true;
	}

	// --- 追加: ある座標に置くと「リーチ状態」を作るか（シミュレーション判定） ---
	bool IsReachCreatingMove(int x, int y, int player)
	{
		if (x < 0 || x >= 7 || y < 0 || y >= 7) return false;
		if (board[x][y] != STATE_EMPTY) return false;
		int backup = board[x][y];
		board[x][y] = player;
		bool reach = CheckReach(player);
		board[x][y] = backup;
		return reach;
	}

	// 指定プレイヤーがリーチを作れる座標一覧を返す
	std::vector<std::pair<int,int>> GetReachTargets(int player)
	{
		// 変更: リーチ成立区間の「既に置かれている3つのマス（player の石）」を返す
		std::vector<std::pair<int,int>> res;
		auto addIfNotExist = [&](int ax, int ay) {
			for (const auto &p : res) {
				if (p.first == ax && p.second == ay) return;
			}
			res.emplace_back(ax, ay);
		};

		// 水平区間
		for (int y = 0; y < 7; y++) {
			for (int sx = 0; sx <= 7 - 4; sx++) {
				int cntPlayer = 0, cntEmpty = 0;
				for (int k = 0; k < 4; k++) {
					int v = board[sx + k][y];
					if (v == player) cntPlayer++;
					else if (v == STATE_EMPTY) cntEmpty++;
				}
				if (cntPlayer == 3 && cntEmpty == 1) {
					for (int k = 0; k < 4; k++) {
						int v = board[sx + k][y];
						if (v == player) addIfNotExist(sx + k, y);
					}
				}
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
				if (cntPlayer == 3 && cntEmpty == 1) {
					for (int k = 0; k < 4; k++) {
						int v = board[x][sy + k];
						if (v == player) addIfNotExist(x, sy + k);
					}
				}
			}
		}

		// 斜め右下区間
		for (int sx = 0; sx <= 7 - 4; sx++) {
			for (int sy = 0; sy <= 7 - 4; sy++) {
				int cntPlayer = 0, cntEmpty = 0;
				for (int k = 0; k < 4; k++) {
					int v = board[sx + k][sy + k];
					if (v == player) cntPlayer++;
					else if (v == STATE_EMPTY) cntEmpty++;
				}
				if (cntPlayer == 3 && cntEmpty == 1) {
					for (int k = 0; k < 4; k++) {
						int v = board[sx + k][sy + k];
						if (v == player) addIfNotExist(sx + k, sy + k);
					}
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
				if (cntPlayer == 3 && cntEmpty == 1) {
					for (int k = 0; k < 4; k++) {
						int v = board[sx - k][sy + k];
						if (v == player) addIfNotExist(sx - k, sy + k);
					}
				}
			}
		}
		return res;
	}

	// 追加: リーチを外部からキャンセルする（リバース以外で潰れたときに呼ぶ）
	void CancelReach()
	{
		reach_declared = false;
		reach_player = 0;
		reverse_available_for = 0;
		reach_targets.clear();
		reach_target_x = -1;
		reach_target_y = -1;
	}

	// --- 既存メソッド以下はそのまま ---
	// (以降の実装は既にあるものを維持)
};