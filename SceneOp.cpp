#include "SceneOp.h"
#include "DxLib.h"


void SceneOp::Init()
{
	// 背景画像の読込
	this->bg0.Load_image("data/cross3.png");
	this->title_text.Load_image("data/Messege.png");

	this->se_bgm.Load_se("data/opbgm.mp3");
	this->se_bgm.Set_playing();
}

/// <summary>
/// 入力処理
/// </summary>
void SceneOp::Input()
{
	this->mouse.Read();
	this->title_text.Set_pos(437, 400);
}

/// <summary>
/// 更新処理
/// </summary>
void SceneOp::Update()
{
	// キーが押されたかのチェック
	if (this->mouse.IsClickSprite(this->title_text) == 1)
	{
		
		// 押されていたらテストシーンへ
		this->game_ptr->ChageScene(1);
	}

	// 更新カウンターインクリメント
	this->update_cnt++;
}

/// <summary>
/// 描画処理
/// </summary>
void SceneOp::Draw()
{
	// 背景0を描画
	this->bg0.Draw();
	if((this->update_cnt / 40) % 2 == 0)
	{
		this->title_text.Draw();
	}

}
void SceneOp::Sound_play()
{
	this->se_bgm.Play_bgm();
}

