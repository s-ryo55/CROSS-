#include "SceneMenu.h"

#include "DxLib.h"


void SceneMenu::Init()
{
	// 背景画像の読込
	this->bg0.Load_image("data/menu.png");
	this->game_start[0].Load_image("data/start.png");
	this->game_start[1].Load_image("data/start_hover.png");
	this->game_rule[0].Load_image("data/asobikata.png");
	this->game_rule[1].Load_image("data/asobikata_hover.png");
	this->game_exit[0].Load_image("data/end.png");
	this->game_exit[1].Load_image("data/end_hover.png");
	
	for(int i = 0; i < 2; i++) {
		this->game_start[i].Set_pos(50, 350);
		this->game_rule[i].Set_pos(50, 450);
		this->game_exit[i].Set_pos(50, 550);
	}
}
void SceneMenu::Input()
{


	// マウスの状態を取得
	this->mouse.Read();
	// ゲームスタートボタンが押されたかのチェック
	if (this->mouse.IsClickSprite(this->game_start[0]))
	{
		// 押されていたらゲームシーンへ
		this->game_ptr->ChageScene(2);
	}
	// ゲームルールボタンが押されたかのチェック
	if (this->mouse.IsClickSprite(this->game_rule[0]))
	{
		// 押されていたらルールシーンへ
		this->game_ptr->ChageScene(3);
	}
	// ゲーム終了ボタンが押されたかのチェック
	if (this->mouse.IsClickSprite(this->game_exit[0]))
	{
		// 押されていたらゲーム終了
	/*	this->game_ptr->Exit();*/
	}
	
	
}

void SceneMenu::Update()
{
	
}

/// <summary>
/// 描画処理
/// </summary>
void SceneMenu::Draw()
{
	// 背景0を描画
	this->bg0.Draw();
	// ゲームスタートスプライトを描画
	this->game_start[0].Draw();
	this->game_rule[0].Draw();
	this->game_exit[0].Draw();

	
}