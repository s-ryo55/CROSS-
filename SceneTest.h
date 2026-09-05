#pragma once
#include "SceneBase.h"

#include "Game.h"			// ゲームクラス
#include "Background.h"		// 背景クラス
#include "Jiki.h"			// 自機クラス
#include "KeyReader.h"		// キーリーダー

/// <summary>
/// テストシーン（SceneBaseクラスの子クラス）
/// </summary>
class SceneTest : public SceneBase
{
	/// <summary>
	/// Gameインスタンスのポインター
	/// </summary>
	Game* game_ptr = nullptr;

	/// <summary>
	/// キーリーダーインスタンス
	/// </summary>
	KeyReader key_state;

	// 背景クラスのインスタンス
	Background bg0;

	// スプライトクラスのインスタンス
	// 自機
	Sprite* jiki_ptr = nullptr;
	// 敵機
	Sprite* tekki_ptr = nullptr;

	// 敵機方向（0：上、1：右、2：下、3：左）
	int tekki_dir = 1;

public:

	/// <summary>
	/// コンストラクター
	/// </summary>
	/// <param name="arg_game_ptr">Gameインスタンスのポインター</param>
	SceneTest(Game* arg_game_ptr)
	{
		// Gameインスタンスのポインターを保持
		this->game_ptr = arg_game_ptr;

		// 自機インスタンスを生成
		this->jiki_ptr = new Jiki(this, "tama_g.png");
		// 敵機インスタンスを生成
		this->tekki_ptr = new Sprite(this, "tama_r.png");

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
