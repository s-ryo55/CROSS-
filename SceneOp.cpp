#include "SceneOp.h"
#include "DxLib.h"


void SceneOp::Init()
{
	// 背景画像の読込
	this->bg0.Load_image("op.png");
}

/// <summary>
/// 入力処理
/// </summary>
void SceneOp::Input()
{
	// キー状態読込（一括）
	this->key_state.Read();
}

/// <summary>
/// 更新処理
/// </summary>
void SceneOp::Update()
{
	// キーが押されたかのチェック
	if (this->key_state.KeyPress())
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

	// メッセージを描画
	if ((this->update_cnt / 40) % 2 == 0)
	{
		SetFontSize(32);
		DrawString(522, 602, "Press Any Key!!", GetColor(220, 220, 220));
		DrawString(520, 600, "Press Any Key!!", GetColor(255, 0, 0));
	}
}

