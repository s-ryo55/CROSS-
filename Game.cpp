#include "Game.h"
#include "SceneOp.h"
#include "SceneTest.h"
#include "SceneEd.h"

/// <summary>
/// ゲームループ
/// </summary>
void Game::Game_loop()
{
	// オープニングシーンをシーンリストに登録
	SceneOp op_scene(this);							// シーンをローカル変数として宣言
	this->scene_arr[0] = &op_scene;					// ローカル変数のポインタを取得して登録

	// テストシーンをシーンリストに登録
	SceneTest test_scene(this);						// シーンをローカル変数として宣言
	this->scene_arr[1] = &test_scene;				// ローカル変数のポインタを取得して登録

	// エンディングシーンをシーンリストに登録
	SceneEd ed_scene(this);							// シーンをローカル変数として宣言
	this->scene_arr[2] = &ed_scene;					// ローカル変数のポインタを取得して登録


	// オープニングシーンを選択
	this->ChageScene(0);

	// ループ開始
	while (ProcessMessage() == 0)
	{
		// リフレッシュレートを設定するための処理
		clock_t check_fps = clock() + CLOCKS_PER_SEC / 60;

		// マウスカーソル表示設定
		SetMouseDispFlag(FALSE);

		// 画面上の描画を初期化（画面を消去）
		ClearDrawScreen();

		// =======================================
		//  入力処理
		// =======================================
		this->scene_arr[this->scene_no]->Input();

		// =======================================
		//  更新処理
		// =======================================
		this->scene_arr[this->scene_no]->Update();

		// =======================================
		//  描画処理
		// =======================================
		this->scene_arr[this->scene_no]->Draw();

		// =======================================
		//  音声再生処理
		// =======================================
		this->scene_arr[this->scene_no]->Sound_play();

		// リフレッシュレートが一定になるまで待つ処理
		while (clock() < check_fps) {}

		// 裏画面の描画を表に反映
		ScreenFlip();
	}
}

/// <summary>
/// シーン切り替え
/// </summary>
/// <param name="arg_scene_no">シーン番号</param>
void Game::ChageScene(int arg_scene_no)
{
	// シーン番号の保持
	this->scene_no = arg_scene_no;

	// =======================================
	//  初期化処理
	// =======================================
	this->scene_arr[this->scene_no]->Init();
}






