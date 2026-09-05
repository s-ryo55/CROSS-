#include "SceneMenu.h"

#include "DxLib.h"


void SceneMenu::Init()
{
	// 背景画像の読込
	this->bg0.Load_image("data/menu.png");
	this->game_start.Load_image("data/start.png");
	this->game_rule.Load_image("data/asobikata.png");
	this->game_exit.Load_image("data/end.png");
	this->game_start.Set_pos(50, 350);
	this->game_rule.Set_pos(50, 450);
	this->game_exit.Set_pos(50, 550);
}

/// <summary>
/// 描画処理
/// </summary>
void SceneMenu::Draw()
{
	// 背景0を描画
	this->bg0.Draw();
	// ゲームスタートスプライトを描画
	this->game_start.Draw();
	this->game_rule.Draw();
	this->game_exit.Draw();
}