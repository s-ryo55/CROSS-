#include "SceneTest.h"
#include "DxLib.h"

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
}

/// <summary>
/// 入力処理
/// </summary>
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
	// reach_available_for が 0 でなければ、そのプレイヤーのリーチ宣言が可能とする
	if (this->mouse.IsClickSprite(this->reach_select[0]) == 1)
	{
		if (this->reach_available_for != 0) {
			// 宣言処理
			this->board_state.DeclareReach(this->reach_available_for);
			// 宣言済みにする
			this->reach_available_for = 0;
			// 必要ならサウンドや UI をここで鳴らす
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
					if(board_state.SetBoardState(x, y)) { // クリックされた座標の状態を取得
						board_state.TurnTurn(); // ターンを進める

						// 勝者チェック
						int winner = board_state.GetBoardStateAroundSelect();
						if (winner != 0) {
							this->game_ptr->SetWinner(winner);
							this->game_ptr->ChageScene(3);
							board_state.Board_reset();
							// リーチ候補はクリア
							this->reach_available_for = 0;
						} else {
							// 勝者がいなければリーチ判定（直前に打ったプレイヤー = turn_count）
							int lastPlayer = board_state.GetTurn_count();

							if (board_state.CheckReach(lastPlayer)) {
								// 自動宣言ではなく「宣言可能」にする（ボタンで宣言させる）
								this->reach_available_for = lastPlayer;
								// この時点で reach_select[0] を有効画像にして押せるようにする
							} else {
								// リーチでなければ候補をクリア
								this->reach_available_for = 0;
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

	// ターン表示
	for(int i = 1; i < 4; i++) {
		if(board_state.GetTurn() == i) {
			this->text_turn[i].Draw();
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





