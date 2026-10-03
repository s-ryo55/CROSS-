#include "SceneTest.h"
#include "DxLib.h"
#include <cmath>

void SceneTest::Init()
{
	// 背景画像の読込
	this->bg0.Load_image("data/ingame.png");
	this->reverse_select[0].Load_image("data/reverse.png");
	this->reverse_select[1].Load_image("data/reverse_lock.png");
	this->reach_select[0].Load_image("data/reach.png");
	this->reach_select[1].Load_image("data/reach_lock.png");

	for(int x = 0; x <7; x++){
		for(int y = 0; y < 7; y++){
			for(int state = 0; state < 7; state++)
			{
				// スプライトの初期化
				this->board_surface[x][y][state].Load_image("data/board_" + std::to_string(state) + ".png");
				// スプライトの表示位置を設定
				this->board_surface[x][y][state].Set_pos(100 + x * 72, 200 + y * 63);
			}
		}
	}

	for(int i = 0; i < 2; i++) {
		this->reverse_select[i].Set_pos(700, 200);
		this->reach_select[i].Set_pos(700, 350);
	}

	for(int i = 1; i < 4; i++) {
		this->text_turn[i].Load_image("data/turn_" + std::to_string(i) + ".png");
		this->text_turn[i].Set_pos(67, 140);
	}

	// 初期値
	this->reverse_mode = false;
	this->reach_available_for = 0;
	this->reach_intent = false;
}

///// <summary>
///// 入力処理
///// </summary>
void SceneTest::Input()
{
	mouse.Read(); // マウスの状態を取得

	// 現在のプレイヤー（操作可能プレイヤー）を取得
	int currentPlayer = this->board_state.GetTurn();

	// まずリバースボタンの押下チェック（切替）
	// ボタンが「有効」なときだけトグル可能（Board が権限を判定）
	if (this->mouse.IsClickSprite(this->reverse_select[0]) == 1)
	{
		if (this->board_state.CanUseReverseForPlayer(currentPlayer)) {
			this->reverse_mode = !this->reverse_mode;
		} else {
			this->reverse_mode = false;
		}
	}

	// リーチボタンの押下チェック
	// - 既に「リーチ候補」がある場合は（従来どおり）宣言処理を行う
	// - そうでない場合は「事前リーチモード」のトグルに使う（押したらリーチを作る場所にしか置けない）
	if (this->mouse.IsClickSprite(this->reach_select[0]) == 1)
	{
		if (this->reach_available_for != 0) {
			// 宣言処理（候補がセットされている場合）
			this->board_state.DeclareReach(this->reach_available_for);
			// 宣言済みにする（UIは別途）
			this->reach_available_for = 0;
			// 事前モードは解除しておく
			this->reach_intent = false;
		}
		else {
			// 候補が無い場合は「事前リーチモード」のトグル
			this->reach_intent = !this->reach_intent;
		}
	}

	board_state.SetSelect(false); // 選択状態をリセット
	board_state.ResetSelect(); // 選択状態をリセット
	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 7; y++) {
			
			// リバースモード時はターゲットを選んだらリバースを試行
			if (this->reverse_mode && this->mouse.IsClickSpriteOnce(this->board_surface[x][y][0]) == 1)
			{
				int actor = this->board_state.GetTurn();
				if (this->board_state.UseReverse(x, y, actor)) {
					// 成功したらリバースモード解除・次ターンへ
					this->reverse_mode = false;
					this->board_state.TurnTurn();
					// リーチは Board::UseReverse で解除される仕様
				}
				// 失敗したら何もしない（必要ならフィードバック追加）
			}
			else
			{
				// 通常の置く操作（左クリック想定）
				if (this->mouse.IsClickSpriteOnce(this->board_surface[x][y][0]) ==1)
				{
					// クリックされたときの処理
					// 新ルール: 事前リーチモード( reach_intent ) によって配置可能な座標を制限する
					bool isReachCreating = this->board_state.IsReachCreatingMove(x, y, currentPlayer);

					// 事前リーチモードがOFFなら「リーチを作る手」は置けない
					if (!this->reach_intent && isReachCreating) {
						// 無視（必要なら効果音／メッセージ）
					}
					// 事前リーチモードがONなら「リーチを作らない手」は置けない
					else if (this->reach_intent && !isReachCreating) {
						// 無視
					}
					else
					{
						// 許可された手なので従来通り設置処理
						if(board_state.SetBoardState(x, y)) { // クリックされた座標の状態を取得
							// 置いたら事前モードは解除
							this->reach_intent = false;

							board_state.TurnTurn(); // ターンを進める

							// 勝者チェック
							int winner = board_state.GetBoardStateAroundSelect();
							if (winner != 0) {
								this->game_ptr->SetWinner(winner);
								this->game_ptr->ChageScene(3);
								board_state.Board_reset();
								// リーチ候補はクリア
								this->reach_available_for = 0;
							}
							else {
								if (this->board_state.Draw_judge()) {
									this->game_ptr->SetWinner(4); // 引き分け
									this->game_ptr->ChageScene(3);
									board_state.Board_reset();
									// リーチ候補はクリア
									this->reach_available_for = 0;
								}
								// 勝者がいなければリーチ判定（直前に打ったプレイヤー = turn_count）
								int lastPlayer = board_state.GetTurn_count();

								if (board_state.CheckReach(lastPlayer)) {
									// 自動宣言ではなく「宣言可能」にする（ボタンで宣言させる）
									this->reach_available_for = lastPlayer;
									// この時点で reach_select[0] を有効画像にして押せるようにする
								}
								else {
									// リーチでなければ候補をクリア
									this->reach_available_for = 0;
								}
							}
						}
					}
				}

				// 右クリック等の選択（既存挙動）
				if (this->mouse.IsClickSprite(this->board_surface[x][y][0]) == 2 && board_state.GetSelect() == false && board_state.GetBoardState(x,y) == 0)
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

	// reverse_select: 有効かどうかを Board に問い合わせる
	int currentPlayer = this->board_state.GetTurn();
	bool reverseEnabled = this->board_state.CanUseReverseForPlayer(currentPlayer);
	if (reverseEnabled) {
		this->reverse_select[0].Draw(); // 有効画像
	} else {
		this->reverse_select[1].Draw(); // ロック画像
	}

	// リバース残回数を表示
	{
		int reverseRemaining = this->board_state.GetReverseRemaining();
		// reverseボタン右側に表示（白色）
		int textX = this->reverse_select[0].Get_pos_x() + 80;
		int textY = this->reverse_select[0].Get_pos_y() + 10;
		DrawFormatString(textX, textY, GetColor(255, 255, 255), "x%d", reverseRemaining);
	}

	// reach_select: reach_available_for がセットされていれば有効にする
	if (this->reach_available_for != 0) {
		this->reach_select[0].Draw(); // 有効画像
	} else {
		this->reach_select[1].Draw(); // ロック画像
	}

}

/// <summary>
/// 音声再生処理
/// </summary>
void SceneTest::Sound_play()
{


}

/// <summary>
/// 敵機方向指定
/// </summary>
/// <param name="arg_dir">敵機方向</param>
void SceneTest::Select_tekki_dir(int arg_dir)
{
	
	
}









