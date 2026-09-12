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
	this->game_rule_text.Load_image("data/images.jpg");
	this->game_rule_text.Set_pos(50, 100);

}
void SceneMenu::Input()
{

	if (GetMouseInput() & MOUSE_INPUT_LEFT) {
		this->game_rule_text.hover = false;
	}
	// マウスの状態を取得
	this->mouse.Read();

	/// <summary>
	/// 押された場合の処理
	/// </summary>
	// ゲームスタートボタンが押されたかのチェック
	if (this->mouse.IsClickSprite(this->game_start[0]) == 1)
	{
		// 押されていたらゲームシーンへ
		this->game_ptr->ChageScene(2);
	}
	// ゲームルールボタンが押されたかのチェック
	if (this->mouse.IsClickSprite(this->game_rule[0]) == 1)
	{
		// 押されていたらルールシーンへ
		this->game_rule_text.hover = true;
	}
	// ゲーム終了ボタンが押されたかのチェック
	if (this->mouse.IsClickSprite(this->game_exit[0]) == 1)
	{
		// 押されていたらゲーム終了
		PostQuitMessage(0);

	}


	/// <summary>
	/// ホバー時の処理
	/// </summary>
	for(int i = 0; i < 2; i++) {
		this->game_start[i].hover = false;
		this->game_rule[i].hover = false;
		this->game_exit[i].hover = false;
	}

	if (this->mouse.IsClickSprite(this->game_start[0]) == 2)
	{
		this->game_start[0].hover = true;
	}
	// ゲームルールボタンが押されたかのチェック
	if (this->mouse.IsClickSprite(this->game_rule[0]) == 2)
	{
		this->game_rule[0].hover = true;
	}
	// ゲーム終了ボタンが押されたかのチェック
	if (this->mouse.IsClickSprite(this->game_exit[0]) == 2)
	{
		this->game_exit[0].hover = true;
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
	this->game_start[this->game_start[0].hover ? 1 : 0].Draw();
	this->game_rule[this->game_rule[0].hover ? 1 : 0].Draw();
	this->game_exit[this->game_exit[0].hover ? 1 : 0].Draw();
	// ルールテキストを描画
	if(this->game_rule_text.hover)this->game_rule_text.Draw();


	
}