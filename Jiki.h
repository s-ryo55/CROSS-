#pragma once
#include "Sprite.h"

/// <summary>
/// 自機クラス（Spriteクラスの子クラス）
/// </summary>
class Jiki : public Sprite
{

public:
	// Spriteクラスのコンストラクタを継承する
	//（本来、コンストラクタは継承されないが、こう記述すると親クラスの全てのコンストラクタを継承できる）
	using Sprite::Sprite;

	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="arg_diff_x">横位置の差分</param>
	/// <param name="arg_diff_y">縦位置の差分</param>
	void Move(int arg_diff_x, int arg_diff_y) override;
};





