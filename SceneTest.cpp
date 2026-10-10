#include "SceneTest.h"
#include "DxLib.h"
#include <cmath>
#include <algorithm> // for std::find

// 変更点（要点）
// - SceneTest::UpdatePlayerFlags を Board::CanUseReverseForPlayer の結果のみ使うよう修正（UI と実行権限を一致させる）

void SceneTest::Init()
{

	menu_se.Load_se("data/menu.mp3");
	menu_se.Set_playing();

	player_se.Load_se("data/player.mp3");
	reverse_se.Load_se("data/reverse.mp3");

	bgm.Load_se("data/playbgm.mp3");
	bgm.Set_playing();

	// 背景画像の読込
	this->bg0.Load_image("data/ingame.png");
	// 0: lock(使用不可) / 1: normal(使用可能だが未選択) / 2: selected(押している・選択中)
	this->reverse_select[0].Load_image("data/reverse_lock.png");
	this->reverse_select[1].Load_image("data/reverse.png");
	this->reverse_select[2].Load_image("data/reverse_select.png");
	this->reach_select[0].Load_image("data/reach_select.png");
	this->reach_select[1].Load_image("data/reach_lock.png");

	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 7; y++) {
			for (int state = 0; state < 7; state++)
			{
				// スプライトの初期化
				this->board_surface[x][y][state].Load_image("data/board_" + std::to_string(state) + ".png");
				// スプライトの表示位置を設定
				this->board_surface[x][y][state].Set_pos(100 + x * 72, 200 + y * 63);
			}
		}
	}

	// reverse_select は 3 枚分あるので 3 つとも位置設定する
	for (int i = 0; i < 3; i++) {
		this->reverse_select[i].Set_pos(700, 200);
	}
	// reach_select は 2 枚
	for (int i = 0; i < 2; i++) {
		this->reach_select[i].Set_pos(700, 350);
	}

	for (int i = 1; i < 4; i++) {
		this->text_turn[i].Load_image("data/turn_" + std::to_string(i) + ".png");
		this->text_turn[i].Set_pos(67, 140);
	}

	// 初期値
	this->reverse_mode = false;
	this->reach_available_for = 0;
	this->reach_queue.clear();

	// 初回フラグ更新
	UpdatePlayerFlags();

}

static void EnqueueReachIfNeeded(std::deque<int>& q, int player)
{
	// 重複を避けてキューに追加
	if (player <= 0) return;
	if (std::find(q.begin(), q.end(), player) == q.end()) {
		q.push_back(player);
	}
}

static void PopNextReach(std::deque<int>& q, int& active)
{
	if (active != 0) return; // 既に前の待ちがある
	if (!q.empty()) {
		active = q.front();
		q.pop_front();
	}
}

/// <summary>
/// 各プレイヤーのリーチ・リバース可否を更新する
/// - reach_possible[i] = board_state.CheckReach(i)
/// - reverse_allowed[i] = board_state.CanUseReverseForPlayer(i) || (現在ターンのプレイヤーは逆に許可)
///   （現在ターンのプレイヤーは、表示や操作性向上のためボタンを押せるようにしておく）
/// </summary>
void SceneTest::UpdatePlayerFlags()
{
	for (int p = 1; p <= 3; p++) {
		this->reach_possible[p] = this->board_state.CheckReach(p);
		// Board の権限に厳密に従う（現在ターン OR をしない）
		this->reverse_allowed[p] = this->board_state.CanUseReverseForPlayer(p);
	}
}

/// <summary>
/// 入力処理
/// </summary>
void SceneTest::Input()
{
	mouse.Read(); // マウスの状態を取得

	// フラグを更新（最新の盤面情報に基づく）
	UpdatePlayerFlags();

	// 現在のプレイヤー（操作可能プレイヤー）を取得
	int currentPlayer = this->board_state.GetTurn();

	// もしキューに未展開のリーチがあれば、アクティブ化する（表示待ちにする）
	PopNextReach(this->reach_queue, this->reach_available_for);

	// ---- リーチ待ち判定: reach_available_for != 0 の間の挙動
	if (this->reach_available_for != 0)
	{
		int declaringPlayer = this->reach_available_for;

		// 宣言者によるリーチ宣言処理（従来どおり）
		if (this->mouse.IsClickSprite(this->reach_select[0]) == 1)
		{
			if (this->reach_available_for == currentPlayer) {
				this->board_state.DeclareReach(this->reach_available_for);
				// 宣言済みのアクティブをクリア
				this->reach_available_for = 0;
				// 宣言したらターンを進める
				this->board_state.TurnTurn();
				// 次のキューがあればそれをアクティブにする（ターンを進めた後に待機表示）
				PopNextReach(this->reach_queue, this->reach_available_for);
				// フラグ再計算
				UpdatePlayerFlags();
			}
		}

		// "現在のターンのプレイヤー" がリバースを使えるならトグル/実行を許可
		bool reverseEnabledForCurrent = this->reverse_allowed[currentPlayer];
		if (reverseEnabledForCurrent && this->mouse.IsClickSpriteOnce(this->reverse_select[1]) == 1)
		{
			this->reverse_mode = !this->reverse_mode;
		}

		// リバースモードなら盤面クリックでリバースを試行する（宣言待ちでも実行可能）
		if (this->reverse_mode)
		{
			board_state.SetSelect(false);
			board_state.ResetSelect();
			for (int x = 0; x < 7; x++) {
				for (int y = 0; y < 7; y++) {
					if (this->mouse.IsClickSpriteOnce(this->board_surface[x][y][0]) == 1)
					{
						int actor = currentPlayer;
						bool doReverse = false;
						if (this->board_state.UseReverse(x, y, actor)) {
							doReverse = true;
						} else if (this->reverse_allowed[actor] && this->board_state.UseReverseForce(x, y, actor)) {
							// Scene が与えた権限に基づき強制実行
							doReverse = true;
						}

						if (doReverse) {
							// 成功したらリバースモード解除
							this->reverse_mode = false;

							// 置いた直後の勝利判定を actor（操作した人）で行う
							if (this->board_state.CheckWinForPlayer(actor)) {
								this->game_ptr->SetWinner(actor);
								this->game_ptr->ChageScene(3);
								board_state.Board_reset();
								this->reach_queue.clear();
								this->reach_available_for = 0;
								UpdatePlayerFlags();
								return;
							}

							// 引き分け判定
							if (this->board_state.Draw_judge()) {
								this->game_ptr->SetWinner(4); // 引き分け
								this->game_ptr->ChageScene(3);
								board_state.Board_reset();
								this->reach_queue.clear();
								this->reach_available_for = 0;
								UpdatePlayerFlags();
								return;
							}

							// リーチ判定（リバースを実行したプレイヤーが対象）
							if (board_state.CheckReach(actor)) {
								EnqueueReachIfNeeded(this->reach_queue, actor);
								// まだアクティブが空なら展開する
								PopNextReach(this->reach_queue, this->reach_available_for);
							}
							else {
								// リーチでなければ通常どおりターンを進める
								this->reach_available_for = 0;
								this->board_state.TurnTurn();
								// 展開されているキューがあれば次の待ちにセット
								PopNextReach(this->reach_queue, this->reach_available_for);
							}
							UpdatePlayerFlags();
						}
					}
				}
			}
		}

		// リーチ待ち中は他の通常操作（置くなど）は無効とする
		return;
	}

	// ---- 通常時の操作（reach 待ちでない）
	// まずリバースボタンの押下チェック（切替）
	bool reverseEnabled = this->reverse_allowed[currentPlayer];
	if (reverseEnabled && this->mouse.IsClickSpriteOnce(this->reverse_select[1]) == 1)
	{
		this->reverse_mode = !this->reverse_mode;
	}
	else if (!reverseEnabled) {
		this->reverse_mode = false;
	}

	// reach ボタン押下（通常時はアクティブがないはず）
	if (this->mouse.IsClickSprite(this->reach_select[0]) == 1)
	{
		if (this->reach_available_for != 0) {
			this->board_state.DeclareReach(this->reach_available_for);
			this->reach_available_for = 0;
			this->board_state.TurnTurn();
			PopNextReach(this->reach_queue, this->reach_available_for);
			UpdatePlayerFlags();
		}
	}

	board_state.SetSelect(false); // 選択状態をリセット
	board_state.ResetSelect(); // 選択状態をリセット
	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 7; y++) {

			// リバースモード時はターゲットを選んだらリバースを試行
			if (this->reverse_mode && this->mouse.IsClickSpriteOnce(this->board_surface[x][y][0]) == 1)
			{
				reverse_se.Set_playing();
				int actor = this->board_state.GetTurn();
				bool doReverse = false;
				if (this->board_state.UseReverse(x, y, actor)) {
					doReverse = true;
				} else if (this->reverse_allowed[actor] && this->board_state.UseReverseForce(x, y, actor)) {
					// Scene が与えた権限に基づき強制実行
					doReverse = true;
				}

				if (doReverse) {
					// 成功したらリバースモード解除（ターン進行は勝利/引き分け/リーチ判定後に行う）
					this->reverse_mode = false;

					// 置いた直後の勝利判定（リバース実行直後）
					if (this->board_state.CheckWinForPlayer(actor)) {
						this->game_ptr->SetWinner(actor);
						this->game_ptr->ChageScene(3);
						board_state.Board_reset();
						this->reach_queue.clear();
						this->reach_available_for = 0;
						UpdatePlayerFlags();
						return;
					}

					// 引き分け判定
					if (this->board_state.Draw_judge()) {
						this->game_ptr->SetWinner(4); // 引き分け
						this->game_ptr->ChageScene(3);
						board_state.Board_reset();
						this->reach_queue.clear();
						this->reach_available_for = 0;
						UpdatePlayerFlags();
						return;
					}

					// リーチ判定（リバースを実行したプレイヤーが直前プレイヤー）
					if (board_state.CheckReach(actor)) {
						EnqueueReachIfNeeded(this->reach_queue, actor);
						PopNextReach(this->reach_queue, this->reach_available_for);
					}
					else {
						this->reach_available_for = 0;
						this->board_state.TurnTurn();
						PopNextReach(this->reach_queue, this->reach_available_for);
					}
					UpdatePlayerFlags();
				}
				// 失敗したら何もしない（必要ならフィードバック追加）
			}
			else
			{
				// 通常の置く操作（左クリック想定）
				if (this->mouse.IsClickSpriteOnce(this->board_surface[x][y][0]) == 1)
				{
					
					// クリックされたときの処理
					if (board_state.SetBoardState(x, y)) { // クリックされた座標の状態を取得
						player_se.Set_playing();
						// 追加: 既に宣言されているリーチが他の手で潰れていないか検査
						if (this->board_state.IsReachDeclared()) {
							int declaredPlayer = this->board_state.GetReachPlayer();
							// 宣言が存在するが、盤面上でそのプレイヤーのリーチが成り立たなくなっていたらキャンセル
							if (!this->board_state.CheckReach(declaredPlayer)) {
								this->board_state.CancelReach();
								this->reach_available_for = 0;
								// フラグ再計算（UI 反映のため）
								UpdatePlayerFlags();
							}
						}

						// ここで「置いたとき」の勝利判定を必ず行う（TurnTurn の前）
						if (this->board_state.CheckWinForPlayer(currentPlayer)) {
							this->game_ptr->SetWinner(currentPlayer);
							this->game_ptr->ChageScene(3);
							board_state.Board_reset();
							this->reach_queue.clear();
							this->reach_available_for = 0;
							UpdatePlayerFlags();
							return;
						}

						// 引き分け判定
						if (this->board_state.Draw_judge()) {
							this->game_ptr->SetWinner(4); // 引き分け
							this->game_ptr->ChageScene(3);
							board_state.Board_reset();
							this->reach_queue.clear();
							this->reach_available_for = 0;
							UpdatePlayerFlags();
							return;
						}

						// 勝者・引き分けでなければリーチ判定（直前に打ったプレイヤー = currentPlayer）
						if (board_state.CheckReach(currentPlayer)) {
							// キューに追加して、アクティブが空なら展開する
							EnqueueReachIfNeeded(this->reach_queue, currentPlayer);
							PopNextReach(this->reach_queue, this->reach_available_for);
							UpdatePlayerFlags();
							// ターンはリーチ宣言後に進めるためここでは進めない
						}
						else {
							// リーチでなければ通常どおりターンを進める
							this->reach_available_for = 0;
							board_state.TurnTurn();
							PopNextReach(this->reach_queue, this->reach_available_for);
							UpdatePlayerFlags();
						}
					}
				}

				// 右クリック等の選択（既存挙動）
				if (this->mouse.IsClickSprite(this->board_surface[x][y][0]) == 2 && board_state.GetSelect() == false && board_state.GetBoardState(x, y) == 0)
				{
					// 選択状態を現在のターンに合わせて設定
					board_state.SetSelectAt(x, y);
					board_state.SetSelect(true);
				}
			}
		}
	}
}

/// <summary>
/// 更新処理
/// </summary>
void SceneTest::Update()
{
	// アニメーション用カウンタを進める（オーバーフロー気にせず増加）
	this->turn_anim_tick++;
	// フラグの最終更新（安全のため）
	UpdatePlayerFlags();
}

/// <summary>
/// 描画処理
/// </summary>
void SceneTest::Draw()
{
	// 背景0を描画
	this->bg0.Draw();
	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 7; y++) {
			this->board_surface[x][y][board_state.GetBoardState(x, y)].Draw();
		}
	}

	// ターン表示（アニメーション付き）
	for (int i = 1; i < 4; i++) {
		if (board_state.GetTurn() == i) {
			// サイン波で上下させる（ピクセル単位）
			const float speed = 0.12f; // 小さくするとゆっくり、値は実験して調整
			const int amplitude = 6;   // 振幅（ピクセル）
			int offset = static_cast<int>(std::sin(this->turn_anim_tick * speed) * amplitude);
			// 一時的に移動して描画し、元の位置に戻す
			this->text_turn[i].Move(0, offset);
			this->text_turn[i].Draw();
			this->text_turn[i].Move(0, -offset);
		}
	}

	// reverse_select: 配列 reverse_allowed を使って表示制御
	int currentPlayer = this->board_state.GetTurn();

	bool showReverseLock = false;
	bool showReverseSelected = false;
	bool showReverseNormal = false;

	// 有効かどうかは reverse_allowed[currentPlayer] を使う
	if (!this->reverse_allowed[currentPlayer]) {
		showReverseLock = true;
	}
	else {
		if (this->reverse_mode) showReverseSelected = true;
		else showReverseNormal = true;
	}

	if (showReverseLock) {
		this->reverse_select[0].Draw();
	}
	else if (showReverseSelected) {
		this->reverse_select[2].Draw();
	}
	else if (showReverseNormal) {
		this->reverse_select[1].Draw();
	}

	// リバース残回数を表示
	{
		int reverseRemaining = this->board_state.GetReverseRemaining();
		// reverseボタン右側に表示（白色）
		int textX = this->reverse_select[1].Get_pos_x() + 80;
		int textY = this->reverse_select[1].Get_pos_y() + 10;
		DrawFormatString(textX, textY, GetColor(255, 255, 255), "x%d", reverseRemaining);
	}

	// reach_select: アクティブがあれば有効にする
	if (this->reach_available_for != 0) {
		this->reach_select[0].Draw(); // 有効画像（宣言可能）
	}
	else {
		this->reach_select[1].Draw(); // ロック画像
	}

	// （任意）デバッグ表示: 各プレイヤーの reach_possible / reverse_allowed を画面に出すと確認しやすい
	// 例: DrawFormatString(10, 10, GetColor(255,255,255), "P1 reach:%d rev:%d", reach_possible[1], reverse_allowed[1]);
}

/// <summary>
/// 音声再生処理
/// </summary>
void SceneTest::Sound_play()
{
	menu_se.Play();
	player_se.Play();
	reverse_se.Play();
	bgm.Play_bgm();
}

/// <summary>
/// 敵機方向指定
/// </summary>
/// <param name="arg_dir">敵機方向</param>
void SceneTest::Select_tekki_dir(int arg_dir)
{


}

