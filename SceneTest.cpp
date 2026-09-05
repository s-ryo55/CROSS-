#include "SceneTest.h"
#include "DxLib.h"


void SceneTest::Init()
{
	// 背景画像の読込
	this->bg0.Load_image("bg.png");

	// 自機の初期位置設定
	this->jiki_ptr->Set_pos(1280 / 2 - 24, 720 / 2 -24);

	// 敵機の初期位置設定
	this->tekki_ptr->Set_pos(1280 - 48, 720 / 2 -24);

}

/// <summary>
/// 入力処理
/// </summary>
void SceneTest::Input()
{
	// キー状態読込（一括）
	this->key_state.Read();
}

/// <summary>
/// 更新処理
/// </summary>
void SceneTest::Update()
{
	// スプライトを動かす
	if(this->key_state.CheckKey(KEY_INPUT_LEFT))
	{
		this->jiki_ptr->Move(-2, 0);
	}
	if(this->key_state.CheckKey(KEY_INPUT_RIGHT))
	{
		this->jiki_ptr->Move(2, 0);
	}
	if(this->key_state.CheckKey(KEY_INPUT_UP))
	{
		this->jiki_ptr->Move(0, -2);
	}
	if(this->key_state.CheckKey(KEY_INPUT_DOWN))
	{
		this->jiki_ptr->Move(0, 2);
	}

	// ESCキーで終了
	if (this->key_state.CheckKey(KEY_INPUT_ESCAPE))
	{
		this->game_ptr->ChageScene(2);
	}

}

/// <summary>
/// 描画処理
/// </summary>
void SceneTest::Draw()
{
	// 背景0を描画
	this->bg0.Draw();

	// スプライトの描画
	// 自機の描画
	this->jiki_ptr->Draw();
	// 敵機の描画
	this->tekki_ptr->Draw();

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
	// 変化がない場合はリターン
	if(this->tekki_dir == arg_dir)
	{
		return;
	}

	// 敵機方向の保持
	this->tekki_dir = arg_dir;
	// 敵機位置変更
	switch(this->tekki_dir)
	{
	case 1:
		// 右に移動（縦位置は自機と合わせる）
		this->tekki_ptr->Set_pos(1280 - 48, this->jiki_ptr->Get_pos_y());
		break;

	case 3:
		// 左に移動（縦位置は自機と合わせる）
		this->tekki_ptr->Set_pos(0, this->jiki_ptr->Get_pos_y());
		break;
	}
}


