#pragma once
#include "SceneBase.h"

#include "Game.h"			// ゲームクラス
#include "Background.h"		// 背景クラス
#include "KeyReader.h"		// キーリーダー


/// <summary>
/// オープニングシーン（SceneBaseクラスの子クラス）
/// </summary>
class SceneOp : public SceneBase
{
	/// <summary>
	/// Gameインスタンスのポインター
	/// </summary>
	Game* game_ptr;

	/// <summary>
	/// キーリーダーインスタンス
	/// </summary>
	KeyReader key_state;

	// 背景クラスのインスタンス
	Background bg0;


public:
	/// <summary>
	/// コンストラクター
	/// </summary>
	/// <param name="arg_game_ptr">Gameインスタンスのポインター</param>
	SceneOp(Game* arg_game_ptr)
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
	void Sound_play() override {};

};

