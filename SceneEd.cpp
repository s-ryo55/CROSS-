#include "SceneEd.h"

#include "DxLib.h"


void SceneEd::Init()
{
	// Game にセットされた勝者を取得
	this->winner = this->game_ptr->GetWinner();

	// 背景画像の読込
	for (int i = 1; i < 4; i++) {
		this->bg[i].Load_image("data/WIN_" + std::to_string(i) + ".png");
	}

	this->restart[0].Load_image("data/restart.png");
	this->restart[1].Load_image("data/restart_select.png");
	this->exit[0].Load_image("data/end_button.png");
	this->exit[1].Load_image("data/end_button_select.png");
	this->title[0].Load_image("data/title_button.png");
	this->title[1].Load_image("data/title_select.png");
	for(int i = 0; i < 2; i++) {
		this->restart[i].Set_pos(1000, 200);
		this->exit[i].Set_pos(1000, 400);
		this->title[i].Set_pos(1000, 300);
	}
}

void SceneEd::Input()
{
	// マウスの状態を取得
	this->mouse.Read();

	// 押された場合の処理
	if (this->mouse.IsClickSprite(this->restart[0]) == 1)
	{
		this->game_ptr->ChageScene(2);
	}
	if (this->mouse.IsClickSprite(this->title[0]) == 1)
	{
		this->game_ptr->ChageScene(1);
	}
	if (this->mouse.IsClickSprite(this->exit[0]) == 1)
	{
		PostQuitMessage(0);
	}
}

/// <summary>
/// 更新処理
/// </summary>
void SceneEd::Update()
{
	// 必要ならここでアニメーションやタイマー更新を行う
}

/// <summary>
/// 音声再生処理
/// </summary>
void SceneEd::Sound_play()
{
	// 必要ならここでBGMやSEの再生を行う
}

/// <summary>
/// 描画処理
/// </summary>
void SceneEd::Draw()
{
	// winner に対応する背景を描画（配列は 1..3 を想定）
	if (this->winner >= 1 && this->winner <= 3) {
		this->bg[this->winner].Draw();
	}
	for (int i = 0; i < 2; i++) {
		if (this->mouse.IsClickSprite(this->restart[0]) == 2) {
			this->restart[1].Draw();
		}
		else {
			this->restart[0].Draw();
		}
		if (this->mouse.IsClickSprite(this->exit[0]) == 2) {
			this->exit[1].Draw();
		}
		else {
			this->exit[0].Draw();
		}
		if (this->mouse.IsClickSprite(this->title[0]) == 2) {
			this->title[1].Draw();
		}
		else {
			this->title[0].Draw();
		}
	}
}
