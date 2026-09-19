#pragma once
#include "SceneBase.h"

#include "Game.h"			// ゲームクラス
#include "Background.h"		// 背景クラス
#include "Jiki.h"			// 自機クラス
#include "KeyReader.h"		// キーリーダー
#include "Mouse.h"			// マウス
#include "Board.h"          // ボードクラス

/// <summary>
/// テストシーン（SceneBaseクラスの子クラス）
/// </summary>
class SceneTest : public SceneBase
{
	/// <summary>
	/// Gameインスタンスのポインター
	/// </summary>
	Game* game_ptr = nullptr;

	// 背景クラスのインスタンス
	Background bg0;

	// 変更: 状態数を拡張 (0..6) => 7 枚
	Sprite board_surface[7][7][7];

	Board board_state;

	Mouse mouse;

	Sprite reverse_select[2];
	Sprite reach_select[2];

	Sprite text_turn[4];

	// 追加: リバース操作モードフラグ（リバースボタンを押してからターゲットを選ぶ）
	bool reverse_mode = false;

	// 追加: 現在宣言可能なリーチ（石を置いたプレイヤーID）。0=なし
	int reach_available_for = 0;

public:

	/// <summary>
	/// コンストラクター
	/// </summary>
	/// <param name="arg_game_ptr">Gameインスタンスのポインター</param>
	SceneTest(Game* arg_game_ptr)
	{
		// Gameインスタンスのポインターを保持
		this->game_ptr = arg_game_ptr;
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 入力処理
	/// </summary>
	void Input() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 音声再生処理
	/// </summary>
	void Sound_play() override;

	/// <summary>
	/// 敵機方向指定
	/// </summary>
	/// <param name="arg_dir">敵機方向</param>
	void Select_tekki_dir(int arg_dir);

};
