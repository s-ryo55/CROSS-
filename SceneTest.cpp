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
			for(int state = 0; state < 7; state++) // 0..6 をロード
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
}

/// <summary>
/// 入力処理
/// </summary>
void SceneTest::Input()
{
	mouse.Read(); // マウスの状態を取得

	board_state.SetSelect(false); // 選択状態をリセット
	board_state.ResetSelect(); // 選択状態をリセット
	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 7; y++) {
			

			if (this->mouse.IsClickSprite(this->board_surface[x][y][0]) ==1)
			{
				// クリックされたときの処理
				if(board_state.SetBoardState(x, y)) { // クリックされた座標の状態を取得
					board_state.TurnTurn(); // ターンを進める

					// 勝者判定
					int winner = board_state.GetBoardStateAroundSelect();
					if (winner != 0) {
						this->game_ptr->SetWinner(winner);
						this->game_ptr->ChageScene(3);
						 board_state.Board_reset(); // 存在しない場合は削除 or 実装を追加してください
					}
				}
			}

			if (this->mouse.IsClickSprite(this->board_surface[x][y][0]) == 2 && board_state.GetSelect() == false && board_state.GetBoardState(x,y) == 0)
			{
				// 変更: 現在ターンに応じた選択状態を設定
				board_state.SetSelectAt(x, y);
				board_state.SetSelect(true);
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


	for(int i = 1; i < 4; i++) {
		if(board_state.GetTurn() == i) {
			this->text_turn[i].Draw();
		}
	}
		this->reverse_select[1].Draw();
		this->reach_select[1].Draw();
	
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



