#pragma once

#include <ctime>			// 標準C ライブラリ ヘッダー <time.h> をインクルードし、関連する名前を std 名前空間に追加します。
#include "DxLib.h"			// DxLib
#include "SceneBase.h"

class Game
{
	/// <summary>
	/// シーン番号
	/// </summary>
	int scene_no = 0;

	/// <summary>
	/// シーンインスタンスのポインタ配列
	/// </summary>
	SceneBase* scene_arr[3] = { nullptr, nullptr, nullptr };

public:

	/// <summary>
	/// コンストラクター
	/// </summary>
	Game(){}

	/// <summary>
	/// ゲームループ
	/// </summary>
	void Game_loop();

	/// <summary>
	/// シーン切り替え
	/// </summary>
	/// <param name="arg_scene_no">シーン番号</param>
	void ChageScene(int arg_scene_no);
};

